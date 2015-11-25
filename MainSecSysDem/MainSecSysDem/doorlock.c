/*
 * doorlock.c
 *
 * Created: 11/22/2015 3:32:59 PM
 *  Author: Luke
 */ 

#include "secSysDefines.h"
#include <avr/io.h>

#define SOLENOID_HIGH	2
#define SOLENOID_LOW	1
#define SOLENOID_OFF	0

static uint8_t solenoidActiveFlag = 0;
static uint16_t solenoidActiveCounter = 0;

void doorlockAndLcdBacklight_init()
{
	// Set the doorlock as an output
	DOORLOCK_DDR |= DOORLOCK_PIN;
	// Set the LCD backlight as an output
	LCD_BACKLIGHT_DDR |= LCD_BACKLIGHT_PIN;
	
	// ----- Timer init -----
	// Reset timer0
	TCNT0 = 0;
	// Set the pins to clear OC0A/OC0B on compare match when up-counting. 
	// Set OC0B on compare match when down-counting.
	TCCR0A |= ((1<<COM0A1) | (1<<COM0B1));
	// Set timer 0 to PWM phase correct mode
	TCCR0A |= (1<<WGM00);
	// Set the backlight to be off initially
	LCD_BACKLIGHT_OCR = 0;
	// Turn off the doorlock solenoid initially (lock)
	DOORLOCK_OCR = 0;
	//set prescaler to 1
	TCCR0B |= (1<<CS00);
}

void LcdBacklightBrightness(uint8_t dutyCycle)
{
	LCD_BACKLIGHT_OCR = ((uint16_t) dutyCycle*255/100); 
}

void doorlockUnlock()
{
	// Set the flag that the solenoid is active and in high mode (full power)
	solenoidActiveFlag = SOLENOID_HIGH;
	// Reset the counter for the time that the solenoid has been active
	solenoidActiveCounter = 0;
	// Turn the doorlock on full power
	DOORLOCK_OCR = 255;
}

void doorlockLock()
{
	// Set the duty cycle to 0 so the doorlock gets released
	DOORLOCK_OCR = 0;
	// Set the flag that the solenoid is not active
	solenoidActiveFlag = SOLENOID_OFF;
}

void doorlockInterruptFuction()
{
	// If the solenoid is in full power mode...
	if(solenoidActiveFlag == SOLENOID_HIGH)
	{
		// If the doorlock has been active for at least than 32ms...
		if(solenoidActiveCounter >= 1)
		{
			// Lower the duty cycle to reduce the current being drawn
			DOORLOCK_OCR = 30;
			// Set the flag that the solenoid is in low mode (less current)
			solenoidActiveFlag = SOLENOID_LOW;
			
			solenoidActiveCounter=0;
		}
		// If it has not been at least 32ms...
		else
		{
			// Increment the counter so next time it will trigger.
			solenoidActiveCounter++;
		}		
	}
	else if(solenoidActiveFlag == SOLENOID_LOW)
	{
		if(solenoidActiveCounter >= 312)
		{
			doorlockLock();
			solenoidActiveFlag = SOLENOID_OFF;
		}
		else
		{
			solenoidActiveCounter++;
		}
	}
}