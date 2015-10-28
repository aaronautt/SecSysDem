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

#define IOEXPANDER_ADDR	0b01001110

uint8_t ioExpReadPin(uint8_t pinNumber);
void ioExpClrPin(uint8_t pinNumber);
void ioExpSetPin(uint8_t pinNumber);
uint8_t ioExpReadReg();
void ioExpWriteReg(uint8_t data);

#endif /* IOEXPANDER_H_ */