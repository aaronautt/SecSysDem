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
	cli();
	// Clear the reset flag, the WDRF bit (bit 3) of MCUSR.
	MCUSR = MCUSR & 0B11110111;
	
	// Set the WDCE bit (bit 4) and the WDE bit (bit 3)
	// of WDTCSR. The WDCE bit must be set in order to
	// change WDE or the watchdog prescalers. Setting the
	// WDCE bit will allow updtaes to the prescalers and
	// WDE for 4 clock cycles then it will be reset by
	// hardware.
	WDTCSR = WDTCSR | 0B00011000;

	// Set the watchdog timeout prescaler value to 1024 K
	// which will yeild a time-out interval of about 8.0 s.
	WDTCSR = 0B00100001;

	// Enable the watchdog timer interupt.
	WDTCSR = WDTCSR | 0B01000000;
	MCUSR = MCUSR & 0B11110111;
	sei();
	/*
	cli();//disable interrupts
	WDTCSR |= (1<<WDCE) | (1<<WDE);// enable wdt and allow change	
	WDTCSR |= (1<<WDE) | (1<<WDP2) | (1<<WDP1);//enables WDT and sets prescaler to 128k or 1 second
	sei();//enable interrupts
	*/
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