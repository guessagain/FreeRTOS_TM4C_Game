#include "TM4C123GH6PM.h"
#include "stdint.h"
extern unsigned int Random_number;
extern unsigned int slider_pos;
int Random(void)
{
	
	Random_number = NVIC_ST_CURRENT_R+(slider_pos*10000)+Random_number;
//	number = number % 28;
	return (Random_number %26);
}