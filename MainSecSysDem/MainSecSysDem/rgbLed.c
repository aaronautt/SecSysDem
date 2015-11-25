/*
 * rgbLed.c
 *
 * Created: 10/28/2015 7:28:32 PM
 *  Author: Luke
 */ 

#include <avr/io.h>
#include "secSysDefines.h"

static uint8_t rgbFlashFlag = 0;
static uint8_t rgbFlashTimer = 0;

void rgb_init()
{
	// Set the pins as outputs
	RGB_DDR |= (RGB_RED_PIN | RGB_GRN_PIN | RGB_BLU_PIN);
	//Turn off the leds
	RGB_PORT_OUT |= (RGB_RED_PIN | RGB_GRN_PIN | RGB_BLU_PIN);
}

// Turns on the RGB led to red
void rgb_red()
{
	// Turn on red LED
	RGB_PORT_OUT &= ~(RGB_RED_PIN);
	// Turn off other LEDs
	RGB_PORT_OUT |= (RGB_GRN_PIN | RGB_BLU_PIN);
}

// Turns on the RGB led to green
void rgb_green()
{
	// Turn on green LED
	RGB_PORT_OUT &= ~(RGB_GRN_PIN);
	// Turn off other LEDs
	RGB_PORT_OUT |= (RGB_RED_PIN | RGB_BLU_PIN);
}

// Turns on the RGB led to blue
void rgb_blue()
{
	// Turn on blue LED
	RGB_PORT_OUT &= ~(RGB_BLU_PIN);
	// Turn off other LEDs
	RGB_PORT_OUT |= (RGB_RED_PIN | RGB_GRN_PIN);
}

// Turns on the RGB led to white
void rgb_white()
{
	// Turn on all LEDs
	RGB_PORT_OUT &= ~(RGB_BLU_PIN | RGB_RED_PIN | RGB_GRN_PIN);
}



// Turns off the RGB led
void rgb_off()
{
	RGB_PORT_OUT |= (RGB_RED_PIN | RGB_GRN_PIN | RGB_BLU_PIN);
}

void rgb_flash_start()
{
	// Set the flag to flash the LED
	rgbFlashFlag = 1;
	// Clear the flash timer
	rgbFlashTimer = 0;
	// Turn the red LED on
	rgb_red();
}

void rgb_flash_stop()
{
	// Clear the flashing flag
	rgbFlashFlag = 0;
	rgb_off();
}

void rgb_flash_check()
{
	const uint8_t numberOf32secIntervals = 30;
	// If the flash flag is set...
	if(rgbFlashFlag)
	{
		// If the interval is over...
		if(rgbFlashTimer >= (numberOf32secIntervals))
		{
			// Clear the flashing timer
			rgbFlashTimer = 0;
			// Turn on the red LED
			rgb_red();
		}
		// If the timer is half over...
		else if(rgbFlashTimer >= (numberOf32secIntervals / 2))
		{
			// Turn off the red LED
			rgb_off();
		}
	}
}

void rgb_flash_32msInterrupt()
{
	if(rgbFlashFlag)
	{
		rgbFlashTimer++;
	}
}


