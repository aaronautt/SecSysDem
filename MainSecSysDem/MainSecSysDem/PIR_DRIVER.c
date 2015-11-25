/*
 * PIR.c
 *
 * Created: 10/18/2015 3:56:35 PM
 *  Author: Aaron
 
 This program consists of only two functions to initialize and read the value of the PIR motion sensor
 */ 
/*
#define F_CPU 8000000L // run CPU at 8 MHz
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1
*/

//includes
#include "secSysDefines.h"
#include <avr/io.h>


//This initialization function sets the PIR pin PC3 as an input
void PIR_init()
{
	DDRC &= ~(PIR_PIN);
}

//This checks the PIR and returns a flag if motion is detected
uint8_t PIR_check()
{
	uint8_t response = 0;
	if((PINC & (PIR_PIN))) response = 1;
	else response = 0;
	return response;
}