/*****************************

timers.c

Author: Aaron crump

this sets up the timers to read all the flags and run through the system

*****************************/


/**********************************

	Sets up timer 0 to check all the flags every 100msec
	
***********************************/

#include <avr/io.h>
#include "timers.h"

void timerTwo_init(void)
{
	//TCNT2 = 0;
	//TCCR2A = 0;//sets timer 0 to CTC mode
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);//sets prescaler to 1024
	//OCR2A = 255;// sets timer to 32msec
	TIMSK2 |= (1<<TOIE2);//OCR2A compare interrupt enabled
	TIFR2 |= (1<<TOV2);//clearing the interrupt flags
}

