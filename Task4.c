// Draws LCD buffer data on LCD display through SSI communication. It executes at 3.3MHz. 
// This task is assigned at "1" FreeRTOS priority. 
#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include "task_def.h"       		//FreeRTOS created Task function(Task1,...) declaration
#include "game_fucn.h"					//Hardware initialization function declaration and a global variable
#include "Display_objects.h"		//Stores Bit-map file data of various display images (used on LCD display)
#include "TM4C_Hardware.h"			//Board specific header file
#include "FreeRTOS_header.h"		//FreeRTOS specific header file

extern car_on_road player;
extern car_on_road enemy[2];
//extern const unsigned char Enemy_car[],Blank_car[],road1[],road2[],road3[],road4[],road1_D[],road2_D[],road3_D[],road4_D[];
extern unsigned int collide_flag;
void Task4(void *pvParameter)
{
		unsigned int count=0,road_flag=0;
	while(1)
	{
			//It updates road to give feels of moving. 
			road_flag++;
			if(road_flag > 0 && road_flag <6)
			{
			Nokia5110_PrintBMP(1, 10, road1, 0);
			Nokia5110_PrintBMP(1, 46, road1_D, 0);
			for(count=0;count<4;count++)
			{
			Nokia5110_PrintBMP(17+count*16, 10, road1, 0);
			Nokia5110_PrintBMP(17+count*16, 46, road1_D, 0);
			}
	
			}
			else if(road_flag > 5 && road_flag <11)
			{
			Nokia5110_PrintBMP(1, 10, road2, 0);
			Nokia5110_PrintBMP(1, 46, road2_D, 0);
			for(count=0;count<4;count++)
			{
			Nokia5110_PrintBMP(17+count*16, 10, road2, 0);
	    Nokia5110_PrintBMP(17+count*16, 46, road2_D, 0);
			}
			}
			else if(road_flag > 10 && road_flag <16)
			{
			Nokia5110_PrintBMP(1, 10, road3, 0);
			Nokia5110_PrintBMP(1, 46, road3_D, 0);
			for(count=0;count<4;count++)
			{
			Nokia5110_PrintBMP(17+count*16, 10, road3, 0);
	    Nokia5110_PrintBMP(17+count*16, 46, road3_D, 0);
			}
			}
			else if(road_flag > 15 && road_flag <21)
			{
			Nokia5110_PrintBMP(1, 10, road4, 0);
			Nokia5110_PrintBMP(1, 46, road4_D, 0);
			for(count=0;count<4;count++)
			{
			Nokia5110_PrintBMP(17+count*16, 10, road4, 0);
	    Nokia5110_PrintBMP(17+count*16, 46, road4_D, 0);
			}
			}
			else
			{
			road_flag = 0;
			}
			// If collision has not happened than only it will print on LCD. 
			if(collide_flag==0)
			{
			for(count=0;count<2;count++)
			{
				if(enemy[count].car_life == 1)
				{
				//Update enemy car latest position on LCD buffer. 
				Nokia5110_PrintBMP(enemy[count].car_xpos, enemy[count].car_ypos, enemy[count].carname, 0);
				}
			}
			//Updates  player car latest position on LCD buffer. 
		  Nokia5110_PrintBMP(player.car_xpos, player.car_ypos, player.carname, 0);
			//Draws LCD buffer on the LCD screen. 
			Nokia5110_DisplayBuffer();
		  }
			//Executes this task at every 35ms. 
			vTaskDelay(35/portTICK_RATE_MS);
	}
}