/*
 * WATCH_DOG.c
 *
 * Created: 10/18/2015 9:29:34 PM
 *  Author: Aaron
 
 I basically just copied this code from the datasheet but it seems solid
 
 */ 

#include <avr/io.h>
#include "interrupt.h"
#include "secSysDefines.h"


//turns on WDT and sets it to one second
void WatchDog_on()
{
	cli();//disable interrupts
	WDTCSR |= (1<<WDCE) | (1<<WDE);// enable wdt and allow change	
	WDTCSR |= (1<<WDE) | (1<<WDP2) | (1<<WDP1);//enables WDT and sets prescaler to 128k or 1 second
	sei();//enable interrupts
}

//This Function MUST be run at initialization to ensure normal functioning
void WatchDog_init()
{
	MCUSR &= ~(1<<WDRF);//system is reset
	WDTCSR |= (1<<WDE);//reset watchdog system
}

//turns off the WDT
void WatchDog_off()
{
	cli();//disable interrupts
	MCUSR &= ~(1<<WDRF);//watchdog system reset
	WDTCSR |= (1<<WDCE) | (1<<WDE);//step 2 of reset, no prescaler change
	WDTCSR = 0x00;//turn off watch dog timer
	sei();//reenable interrupts
}