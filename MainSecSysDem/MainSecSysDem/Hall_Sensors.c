/*
 * Hall_Sensors.c
 *
 * Created: 10/25/2015 1:50:24 PM
 *  Author: Aaron
 */ 

/* ***************************************
These functions read the output pins of a hall effect sensor. When a magnet is moved to touch the sensor it switches states.

The hall effect sensor used for this project is a bipolar latching type sensor.

To get the best effect, put the magnet in contact with the face of the sensor and slide it away from the sensor
along the face so that contact is maintained between the sensor and magnet while the thing is opening.
********************************************/

//includes
#include "secSysDefines.h"
#include <avr/io.h> // deal with port registers


//turns requisite port pins on as inputs
void HALL_init()
{
	HALL_EFF_DOOR_DDR &= ~HALL_EFF_DOOR_PIN;
	HALL_EFF_WIN_DDR &= ~HALL_EFF_WIN_PIN;
}


//checks the state of the window hall effect sensor and returns a 1 if it's high (on)
uint8_t Hall_Window_check()
{
	uint8_t response = 0;
	if((HALL_EFF_WIN_PORT_IN & (1<<HALL_EFF_WIN_PIN))) response = 1;
	else response = 0;
	return response;
}


//checks the state of the window hall effect sensor and returns a 1 if it's high (on)
uint8_t Hall_Door_check()
{
	uint8_t response = 0;
	if((HALL_EFF_DOOR_PORT_IN & (1<<HALL_EFF_DOOR_PIN))) response = 1;
	else response = 0;
	return response;
}