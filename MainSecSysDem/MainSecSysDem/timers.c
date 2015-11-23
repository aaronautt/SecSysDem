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
	TCNT2 = 0;
	TCCR2A = 0x02;//sets timer 0 to CTC mode
	TCCR2B = 0x07;//sets prescaler to 1024
	OCR2A = 255;// sets timer to 32msec
	TIMSK2 = 0x02;//OCR2A compare interrupt enabled
	TIFR2 = 0x07;//clearing the interrupt flags
}

