#include "TM4C123GH6PM.h"
#include "stdint.h"



void Slider_ADC()
{
	SYSCTL_RCGC0_R |= 0x00010000;			//ADC module clock is enabled.
	SYSCTL_RCGC2_R |= 0x08;						//GPIOD module clock is enabled.
	SYSCTL_RCGC0_R &= ~0x00000300;
	GPIO_PORTD_DIR_R |=0x04;					//Set PD2 as input. 
	GPIO_PORTD_AFSEL_R |= 0x04;				//Enable analog functionality for PD2. 
	GPIO_PORTD_DEN_R &= (~0x04);			//Disable digital functionality on PD2.
	GPIO_PORTD_AMSEL_R |= 0x04;				//Disable analog isolation circuit. 
	ADC0_ACTSS_R &= 0x00;							//Sample sequencer is disabled.
	ADC0_SSPRI_R |= 0x0123;						//Set priority of sequencer 3 to zero. 
	ADC0_EMUX_R &=(~0x0F000);					//Configure the trigger event for sequencer. 
	ADC0_SSMUX3_R =0x05;  						//Configure pin-5(PD2) as an ADC pin. 
	ADC0_SSCTL3_R =0x06;							//Analog select, end of sequence, interrupt enable
	ADC0_ACTSS_R =0x08;								//Enable sampling sequencer

}