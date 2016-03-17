// Generates Enemy-car on the road. 
// It is assigned as highest priority task. 

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include "task_def.h"       		//FreeRTOS created Task function(Task1,...) declaration
#include "game_fucn.h"					//Hardware initialization function declaration and a global variable
#include "Display_objects.h"		//Stores Bit-map file data of various display images (used on LCD display)
#include "TM4C_Hardware.h"			//Board specific header file
#include "FreeRTOS_header.h"		//FreeRTOS specific header file

extern car_on_road enemy[2];
extern xSemaphoreHandle car_generation;
//extern const unsigned char Enemy_car[];
extern unsigned volatile int First_enemy;
unsigned int ran=0;
void Task3(void *pvParameter)
{
	
	while(1)
	{
	// Execution starts from here. 
	xSemaphoreTake(car_generation, portMAX_DELAY);	
	// Calls to random number function. It decides enemy car Y-position on the LCD. 
		ran=Random(); 
		ran=ran%3;
		//At the beggining of game, it waits for two second to generate the first car.  
		if(First_enemy==0)
		{
			First_enemy=1;
			vTaskDelay(2000/portTICK_RATE_MS);
		}
		//If first enemy car is on the road and random number modulo is "1" than it generates second car. 
		//The purpose of this logic is to generate second car randomly. 
		if(enemy[1].car_life !=1 && enemy[0].car_life==1 && ran==1  )
		{
	enemy[1].car_xpos = 68;
	enemy[1].car_ypos = 15+Random();
	enemy[1].carname = Enemy_car;
	enemy[1].car_life = 1;	
		}
		
		//First car generation. If second car is present on the road than first car will not be generated. 
		if(enemy[0].car_life !=1 && enemy[1].car_life!=1)
		{
	enemy[0].car_xpos = 68;
	enemy[0].car_ypos = 15+Random();
	enemy[0].carname = Enemy_car;
	enemy[0].car_life = 1;
		}
	
	}
}
	
