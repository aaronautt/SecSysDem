/*
 * bell.c
 *
 * Created: 10/28/2015 7:50:16 PM
 *  Author: Luke
 */ 

#include <avr/io.h>
#include "secSysDefines.h"
#include "ioExpander.h"

enum bellStatus{
	NO_CHANGE,
	TURN_OFF,
	TURN_ON
};

static uint8_t changeBell=NO_CHANGE;
static uint8_t bellEnabled=0;

// Initiates the audible sounding bell by turning it off
void bell_init()
{
	ioExpSetBit(BELL_ALARM_PIN_IOEX);
}

// Turns on the audible sounding bell
void bell_on()
{
	ioExpClrBit(BELL_ALARM_PIN_IOEX);
}

// Turns off the audible sounding bell
void bell_off()
{
	ioExpSetBit(BELL_ALARM_PIN_IOEX);
}

void bell_enable()
{
	bellEnabled = 1;
	changeBell = NO_CHANGE;
	bell_off();
}

void bell_disable()
{
	bellEnabled = 0;
	bell_off();
}

void bell_UpdateStatus()
{
	if(bellEnabled)
	{
		if(changeBell == TURN_ON)
		{
			changeBell = NO_CHANGE;
			bell_on();
		}
		else if(changeBell == TURN_OFF)
		{
			changeBell = NO_CHANGE;
			bell_off();
		}	
	}
}

void bell_InterruptFunction()
{
	static uint8_t counter=0;
	if(bellEnabled)
	{
		counter++;
		if(counter == 1)
		{
			changeBell = TURN_ON;
		}
		else if(counter == 2)
		{
			changeBell = TURN_OFF;
		}
		else if(counter > 16)
		{
			counter = 0;
		}	
	}
}