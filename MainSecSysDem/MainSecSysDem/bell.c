/*
 * bell.c
 *
 * Created: 10/28/2015 7:50:16 PM
 *  Author: Luke
 */ 

#include <avr/io.h>
#include "secSysDefines.h"
#include "ioExpander.h"

// Initiates the audible sounding bell by turning it off
void bell_init()
{
	ioExpSetBit(BELL_ALARM_PIN_IOEX);
}

// Turns on the audible sounding bell
void bell_on()
{
	ioExpSetBit(BELL_ALARM_PIN_IOEX);
}

// Turns off the audible sounding bell
void bell_off()
{
	ioExpClrBit(BELL_ALARM_PIN_IOEX);
}