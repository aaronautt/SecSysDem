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


// This function will set the select bits while leaving other pins unaffected
//		Ex. bitValue = 0b0010
//			Bit 1 will be set
//		Ex.	bitValue = 0b1101
//			Bits 3, 2, and 0 will be set
void ioExpSetBit(uint8_t bitValue)
{
	// Read the current state of the IO Expander
	//uint8_t portData = ioExpReadReg();
	// Change the data to set the pin
	portValues |= bitValue;
	// Write the new data back to the IO Expander
	ioExpWriteReg(portValues);
}


// This function will clear the select bits while leaving other pins unaffected
//		Ex. bitValue = 0b0010
//			Bit 1 will be cleared
//		Ex.	bitValue = 0b1101
//			Bits 3, 2, and 0 will be cleared
void ioExpClrBit(uint8_t bitValue)
{
	// Change the data to clear the select pin
	portValues &= ~(bitValue);
	// Write the new data back to the IO Expander
	ioExpWriteReg(portValues);
}


// This function will read the selected bit
//		Ex. bitValue = 0b0100
//			Bit 2 will be read and a 1 will be returned if it is high
uint8_t ioExpReadBit(uint8_t bitValue)
{
	// Read the io expander
	// If the selected bit is on, return 1, else return 0
	return (((ioExpReadReg()) & bitValue) ? 1 : 0);
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

