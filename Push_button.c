#include "TM4C123GH6PM.h"
#include "stdint.h"
#include "inc/hw_nvic.h"
#include "inc/hw_ints.h"

// Button PB6 positive Logic

void pushbutton_init(void)
{
	SYSCTL_RCGC2_R |= 0x00000002;		// Enable Port-B module clock
	GPIO_PORTB_AMSEL_R &= (~0x40);  // disable analog on PB6
  GPIO_PORTB_DIR_R   &= (~0x40);  // PB6 as input
  GPIO_PORTB_AFSEL_R &= (~0x40);  // disable alt funct on PB6
  GPIO_PORTB_PDR_R   |= 0x40;     // enable pull-down on PB6
  GPIO_PORTB_DEN_R   |= 0x40;     // enable digital I/O on PB6
	GPIO_PORTB_IS_R |= 
	GPIO_PORTB_IEV_R |=0x40;
	GPIO_PORTB_IM_R |= 0x40;
	NVIC_EN0_R |= 0x010000;
	IntEnable(INT_GPIOB);
//	IntPrioritySet(INT_GPIOB,1);
//	NVIC_EnableIRQ(1);
}
