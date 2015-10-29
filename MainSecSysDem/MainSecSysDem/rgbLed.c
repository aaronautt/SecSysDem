/*
 * rgbLed.c
 *
 * Created: 10/28/2015 7:28:32 PM
 *  Author: Luke
 */ 

#include <avr/io.h>
#include "secSysDefines.h"



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
	RGB_DDR &= ~(RGB_RED_PIN);
	// Turn off other LEDs
	RGB_DDR |= (RGB_GRN_PIN | RGB_BLU_PIN);
}

// Turns on the RGB led to red
void rgb_green()
{
	// Turn on green LED
	RGB_DDR &= ~(RGB_GRN_PIN);
	// Turn off other LEDs
	RGB_DDR |= (RGB_RED_PIN | RGB_BLU_PIN);
}

// Turns on the RGB led to red
void rgb_blue()
{
	// Turn on blue LED
	RGB_DDR &= ~(RGB_BLU_PIN);
	// Turn off other LEDs
	RGB_DDR |= (RGB_RED_PIN | RGB_GRN_PIN);
}

// Turns off the RGB led
void rgb_off()
{
	RGB_PORT_OUT |= (RGB_RED_PIN | RGB_GRN_PIN | RGB_BLU_PIN);
}