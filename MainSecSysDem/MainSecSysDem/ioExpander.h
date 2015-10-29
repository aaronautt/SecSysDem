/*
 * ioExpander.h
 *
 * Created: 10/25/2015 10:13:38 PM
 *  Author: Luke
 *
 * To set a pin as an input, set the pin as a logic 1. This will enable a
 * weak pull-up resistor and allow a logic device to drive it low or high.
 */ 


#ifndef IOEXPANDER_H_
#define IOEXPANDER_H_

#include <avr/io.h>

uint8_t ioExpReadReg();
void ioExpWriteReg(uint8_t data);
uint8_t ioExpReadBit(uint8_t bitValue);
void ioExpClrBit(uint8_t bitValue);
void ioExpSetBit(uint8_t bitValue);

#endif /* IOEXPANDER_H_ */