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
#include "interrupt.h"

void timerTwo_init(void)
{
	TCNT2 = 0;
	//TCCR2A = 0;//sets timer 0 to CTC mode
	TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);//sets prescaler to 1024
	OCR2A = 255;// sets timer to 32msec
	TIMSK2 |= _BV(OCIE2A);//OCR2A compare interrupt enabled
	//ASSR = 0x00;
	//TIFR2 |= _BV(TOV2);//clearing the interrupt flags
	sei();
}

void timerOne_init(void)
{
	TCNT1 = 0;
	TCCR1A = 0;
	TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS20);//set to CTC mode with 1024 prescaler
	OCR1A = 80; //10msec
	OCR1B = 80;//10msec for ADC
	TIMSK1 |= _BV(OCIE1A) |_BV(OCIE1B);//interrupt on compare with ocr1a
	//TIFR1 |= _BV(OCF1A);//clear interrupt flag
	//sei();
}

