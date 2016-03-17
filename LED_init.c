#include "TM4C123GH6PM.h"
#include "stdint.h"

// Status LED ---------------
// RED    PC6 (shows stop sign)
// GGREEN PC7 (Shows go sign)

void LED_init(void)
{
	SYSCTL_RCGC2_R |= 0x04;	
	GPIO_PORTC_LOCK_R = 0x4C4F434B; //Unlock the GPIO CR register
	GPIO_PORTC_CR_R |= 0xC0;        //Enables write-access to GPIOAFSEL, GPIOPUR, GPIOPDR, and GPIODEN 
	GPIO_PORTC_AMSEL_R &= (~0xC0);  //Disable analog functionality of pin
	GPIO_PORTC_AFSEL_R &= (~0xC0);  //Disable alternate function on port pins
	GPIO_PORTC_DIR_R |= 0xC0;				//
	GPIO_PORTC_DEN_R |= 0xC0;
	GPIO_PORTC_DATA_R |=0x40;
}