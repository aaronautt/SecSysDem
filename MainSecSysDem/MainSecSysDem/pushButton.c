/*
 * pushButton.c
 *
 * Created: 10/28/2015 8:15:22 PM
 *  Author: Luke
 */ 

#include "avr/io.h"
#include "secSysDefines.h"

// Initializes the push button by setting it as an input and setting the
// internal pullup resistors.
void pushButton_init()
{
	// Set input as input
	PUSH_BTN_DDR &= ~PUSH_BTN_PIN;
	// Set pull up resistor
	PUSH_BTN_PORT_OUT |= PUSH_BTN_PIN;
}

// Returns a 1 is the button is pressed
uint8_t pushButtonRead()
{
	// Read the pin and return a logic 1 if its pressed, and zero if its not
	return(!(PUSH_BTN_PORT_IN & PUSH_BTN_PIN));
}