/*
 * ioExpander.c
 *
 * Created: 10/25/2015 10:14:26 PM
 *  Author: Luke
 */ 

#include "i2c_driver.h"
#include "ioExpander.h"
#include <avr/io.h>

void ioExpWriteReg(uint8_t data)
{
	I2C_WriteByte(IOEXPANDER_ADDR, data);
}

uint8_t ioExpReadReg()
{
	return(I2C_ReadByte(IOEXPANDER_ADDR));
}

void ioExpSetPin(uint8_t pinNumber)
{
	// Read the current state of the IO Expander
	uint8_t portData = ioExpReadReg();
	// Change the data to set the pin
	portData |= (1 << pinNumber);
	// Write the new data back to the IO Expander
	ioExpWriteReg(portData);	
}

void ioExpClrPin(uint8_t pinNumber)
{
	// Read the current state of the IO Expander
	uint8_t portData = ioExpReadReg();
	// Change the data to set the pin
	portData &= ~(1 << pinNumber);
	// Write the new data back to the IO Expander
	ioExpWriteReg(portData);
}

uint8_t ioExpReadPin(uint8_t pinNumber)
{
	// Read the current state of the IO Expander
	uint8_t portData = ioExpReadReg();
	// Move the wanted bit to the LSB position and remove the other numbers
	portData = ((portData >> pinNumber) & 0b1);
	// Return a 1 for high and a 0 for low
	return portData;
}
