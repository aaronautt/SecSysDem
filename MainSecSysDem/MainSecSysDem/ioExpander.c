/*
 * ioExpander.c
 *
 * Created: 10/25/2015 10:14:26 PM
 *  Author: Luke
 */ 

#include "i2c_driver.h"
#include "ioExpander.h"
#include "secSysDefines.h"
#include <avr/io.h>

static uint8_t portValues = 0xFF;

void ioExpWriteReg(uint8_t data)
{
	I2C_WriteByte(IOEXPANDER_ADDR, data);
}

uint8_t ioExpReadReg()
{
	return(I2C_ReadByte(IOEXPANDER_ADDR));
}

void ioExpSetBit(uint8_t bitValue)
{
	// Read the current state of the IO Expander
	//uint8_t portData = ioExpReadReg();
	// Change the data to set the pin
	portValues |= bitValue;
	// Write the new data back to the IO Expander
	ioExpWriteReg(portValues);
}

void ioExpClrBit(uint8_t bitValue)
{
	// Read the current state of the IO Expander
	//uint8_t portData = ioExpReadReg();
	// Change the data to set the pin
	portValues &= ~(bitValue);
	// Write the new data back to the IO Expander
	ioExpWriteReg(portValues);
}

uint8_t ioExpReadBit(uint8_t bitValue)
{
	// Read the io expander 
	uint8_t portData = ((ioExpReadReg()) & bitValue) ? 1 : 0;
	// Return a 1 for high and a 0 for low
	return portData;
}

// void ioExpSetPin(uint8_t pinNumber)
// {
// 	// Read the current state of the IO Expander
// 	//uint8_t portData = ioExpReadReg();
// 	// Change the data to set the pin
// 	portValues |= (1 << pinNumber);
// 	// Write the new data back to the IO Expander
// 	ioExpWriteReg(portValues);
// }
//
// void ioExpClrPin(uint8_t pinNumber)
// {
// 	// Read the current state of the IO Expander
// 	//uint8_t portData = ioExpReadReg();
// 	// Change the data to set the pin
// 	portValues &= ~(1 << pinNumber);
// 	// Write the new data back to the IO Expander
// 	ioExpWriteReg(portValues);
// }
//
// uint8_t ioExpReadPin(uint8_t pinNumber)
// {
// 	// Read the current state of the IO Expander
// 	//uint8_t portData = ioExpReadReg();
// 	// Move the wanted bit to the LSB position and remove the other numbers
// 	uint8_t logicVal = (((ioExpReadReg()) >> pinNumber) & 0b1);
// 	// Return a 1 for high and a 0 for low
// 	return logicVal;
// }

