// It draws blank-car on the LCD buffer at the player car current pixel position. After that it updates player car 
// new pixel position according to the slider position. Drawing blank car is required to create trace-free appearance.
// Blank-car simply turn-off all the pixels on given position. 

// This task is assigned at FreeRTOS priority-2. 
#include <stdint.h>
#include "task_def.h"       		//FreeRTOS created Task function(Task1,...) declaration
#include "game_fucn.h"					//Hardware initialization function declaration and a global variable
#include "Display_objects.h"		//Stores Bit-map file data of various display images (used on LCD display)
#include "TM4C_Hardware.h"			//Board specific header file
#include "FreeRTOS_header.h"		//FreeRTOS specific header file


extern unsigned int slider_pos,collide_flag;
extern car_on_road player;

//Task 1 implementation. 
void Task1(void *pvParameter)
{
	
	player.carname = player_car;
	
	while(1)
	{
		// Player car placed at the beginning of the road. 
		player.car_xpos = 10;
		//Enable sampling on already enabled sampler of ADC. 
		ADC0_PSSI_R = 0x08;	
    //Check sampling status. 		
		while((ADC0_RIS_R&0x08)==0);
		//if no collision than draw "blank car" on the  player car current position. 
		if(collide_flag==0)
		{
		Nokia5110_PrintBMP(player.car_xpos, 15+slider_pos, Blank_car, 0);
		}
		//read ADC reading from the FIFO. 
		slider_pos = ADC0_SSFIFO3_R;
		//If slider and car position movement is in opposite direction than minus adc reading from 4096. Otherwise 
		//directly jump to the next instruction. 
		slider_pos = 4096-slider_pos;
		//Calibrate ADC reading for the car-position so car will always stays between the road. 
		slider_pos = slider_pos/157;
		//Add 15 pixels offset to avoid over-lapping of car and left-side road. car will be drawn from the 
		//left bottom-side X-Y pixel position. 
		player.car_ypos = 15+slider_pos;
		//Blocks this task for 30ms. This is the minimum frequency required for flicker-free display on LCD. 
		vTaskDelay(30/portTICK_RATE_MS);
	
	}
}