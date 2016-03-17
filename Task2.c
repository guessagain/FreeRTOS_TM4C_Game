// Updates Enemy-car position on the LCD and checks the collision between player car and enemy car. 
// This task is assigned at FreeRTOS priority of 3.


#include <stdbool.h>
#include <stdint.h>
#include "task_def.h"       		//FreeRTOS created Task function(Task1,...) declaration
#include "game_fucn.h"					//Hardware initialization function declaration and a global variable
#include "Display_objects.h"		//Stores Bit-map file data of various display images (used on LCD display)
#include "TM4C_Hardware.h"			//Board specific header file
#include "FreeRTOS_header.h"		//FreeRTOS specific header file

extern car_on_road player;
extern car_on_road enemy[2];
extern unsigned int collide_flag;
extern xSemaphoreHandle car_generation;

void Task2(void *pvParameter)
{


	while(1)
	{
		unsigned int Enemy_car_number=0,i=0,collision_pos_y=0;
    // For-loop to update two enemy car status. 
		for(i=0;i<2;i++)
		{ 
			//If enemy car life is zero than no need to update. 
			if(enemy[i].car_life ==1)
			{
			//player and enemy car collision test. 
			if((player.car_xpos <= enemy[i].car_xpos && player.car_xpos+16 >= enemy[i].car_xpos) && 
				((player.car_ypos >= enemy[i].car_ypos && player.car_ypos-10 <= enemy[i].car_ypos) ||
				(player.car_ypos <= enemy[i].car_ypos && player.car_ypos >= enemy[i].car_ypos-10)))
		
					{
						//sets collide status. 
						collide_flag=1;
						//Sets the position of "crash display" on the LCD. 
						if(player.car_ypos > enemy[i].car_ypos)
						{
							collision_pos_y = enemy[i].car_ypos+5;
						}
						else
						{
							collision_pos_y = player.car_ypos+5;
						}
					//Prints enemy car on LCD buffer. 
					Nokia5110_PrintBMP(enemy[i].car_xpos, enemy[i].car_ypos, enemy[i].carname, 0);
					//Prints player car on LCD buffer. 
					Nokia5110_PrintBMP(player.car_xpos, player.car_ypos, player.carname, 0);
					//Prints "crash display" at calculated position on LCD buffer. 
					Nokia5110_PrintBMP(enemy[i].car_xpos-8,collision_pos_y, collide, 0);
					//Prints LCD buffer on LCD. 
					Nokia5110_DisplayBuffer();
					// Turn-ff Green Led and turn-on Red Led. 
					GPIO_PORTC_DATA_R = 0x40;
					// Prints "Game over" text on LCD. 
					Nokia5110_customtext();
					// Trying to take mutex and get blocked forever. 
					xSemaphoreTake(car_generation, portMAX_DELAY);	
					
					}
		
		//Update enemy car position if it hasn't reached to the finish point. 
			if(enemy[i].car_xpos > 2)
				{
				Nokia5110_PrintBMP(enemy[i].car_xpos, enemy[i].car_ypos, Blank_car, 0);
				enemy[i].car_xpos-=1;
				
				}
				
			//If it has reached to the finish point than make enemy car life zero. 
			else
				{
				Nokia5110_PrintBMP(enemy[i].car_xpos, enemy[i].car_ypos, Blank_car, 0);
				enemy[i].car_xpos-=1;
				enemy[i].car_life = 0;
				}
			}
		}
		// Second-car generation trigger 
		if(enemy[0].car_xpos == 20)
		{
			//Gives mutex so the car generation task can unblock and generate enemy car. 
			xSemaphoreGive(car_generation);
		}
		// If first enemy car has reached to the start point than gives mutex.
		if(enemy[0].car_xpos == 1)
		{
			enemy[0].car_xpos-=1;
			//Gives mutex so the car generation task can unblock and generate enemy car. 
			xSemaphoreGive(car_generation);
		}
		// If second enemy car has reached to the start point than gives mutex.
		if(enemy[1].car_xpos == 1)
		{
			enemy[1].car_xpos-=1;
			xSemaphoreGive(car_generation);
		}
		// //Blocks this task for 30ms. This is the minimum frequency required for flicker-free display on LCD. 
		vTaskDelay(30/portTICK_RATE_MS);
	}
			
	
	
}