
#include <stdint.h>
#include "task_def.h"       		//FreeRTOS created Task function(Task1,...) declaration
#include "game_fucn.h"					//Hardware initialization function declaration and a global variable
#include "Display_objects.h"		//Stores Bit-map file data of various display images (used on LCD display)
#include "TM4C_Hardware.h"			//Board specific header file
#include "FreeRTOS_header.h"		//FreeRTOS specific header file

				

//******************Hardware connection****************************************
// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground
//
// Push-button ---------------
// Button PB6 positive Logic
//
// Position slider(Rsistive pot slider)---------------
// slider PD2 (connected to ADC pin, voltage varies from 0 to 3.3V)
//
// Status LED ---------------
// RED    PC6 (shows stop sign)
// GGREEN PC7 (Shows go sign)
//

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


// All Global variables and semaphores used in this project. 
unsigned volatile int slider_pos,collide_flag=0,game_start=0,while_logic=0,First_enemy=0;
unsigned volatile int Random_number;
// car struct(defined in game_fucn.h) which represents X-pos, Y-pos, carname(for display on LCD) and life status. 
car_on_road player;


//There could be two enemy-cars on the road at the same time. 
car_on_road enemy[2];

// Mutex semaphore which will be given only at the creation of enemy car. (Unlocks enemy car generation task)
xSemaphoreHandle car_generation;

//User-switch interrupt routine. It sets binary semaphore(game_start) which starts the car race. 
void GPIOB_Handler(void)
{
	IntDisable(INT_GPIOF);								//Disable interrupt 
	GPIO_PORTB_ICR_R &= (0x40);    				//clears interrupt flag
	GPIO_PORTB_IM_R &= (~0x40);						//clears gpio interrupt enable bit
	game_start = 1;												//Gives Binary semaphore
	
}
int main(void)
{

    // Set the clocking to run at 80 MHz from the PLL.
		// RTOS kernel runs at the frequency of 1000Hz which means it will devide 1ms between tasks equally
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
		LED_init();													//Hardware LED initialization routine
		Slider_ADC();												//Resistive slider initialization routine
		pushbutton_init();									//Push-button initialization routine
		Nokia5110_Init();										//LCD initialization routine
		Nokia5110_Clear();									//clears LCD						
		Nokia5110_ClearBuffer();						//Clear LCD image display buffer
		Nokia5110_DrawFullImage(logo1);			//Draws Game logo on the LCD. 
		
		//Create mutex to control Enemy car generation task. 
		car_generation = xSemaphoreCreateMutex();
		//Gives mutex to the enemy car generation task. 
		xSemaphoreGive(car_generation);
		//Waits for the binary semaphore(game_start), it will set when user press the switch. 
		while(!game_start);
		//Turn-off Red LED and turn-on Green LED 
		GPIO_PORTC_DATA_R = 0x80;
		//check the mutex condition. If successfully created than tasks will be created. (used for debug)
		if(car_generation!=NULL && game_start==1)
		{
			//Player car position update task. It updates player car position on the LCD buffer according to slider position. 
			if(xTaskCreate(Task1, "Player car update", 500, NULL,tskIDLE_PRIORITY + 2, NULL)!=pdTRUE)
			{
			while(1);
			}
			//Enemy car position update task. It updates enemy car position on the LCD buffer. 
			if(xTaskCreate(Task2, "Enemy car update", 500, NULL,tskIDLE_PRIORITY + 3, NULL)!=pdTRUE)
			{
			while(1);
			}
			//Enemy car generation Task. It creates enemy car on the road. 
			if(xTaskCreate(Task3, "Enemy car generate", 500, NULL,tskIDLE_PRIORITY + 4, NULL)!=pdTRUE)
			{
			while(1);
			}
			//LCD buffer print task. It prints road, player car and enemy car. 
			if(xTaskCreate(Task4, "LCD update", 500, NULL,tskIDLE_PRIORITY + 1, NULL)!=pdTRUE)
			{
			while(1);
			}
	  }
		//start the scheduler. 
    vTaskStartScheduler();


    while(1)
    {
    }
}
