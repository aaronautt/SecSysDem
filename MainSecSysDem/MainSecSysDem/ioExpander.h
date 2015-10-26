/*
 * ioExpander.h
 *
 * Created: 10/25/2015 10:13:38 PM
 *  Author: Luke
 */ 


#ifndef IOEXPANDER_H_
#define IOEXPANDER_H_

#define IOEXPANDER_ADDR	0b01001110

uint8_t ioExpReadPin(uint8_t pinNumber);
void ioExpClrPin(uint8_t pinNumber);
void ioExpSetPin(uint8_t pinNumber);
uint8_t ioExpReadReg();
void ioExpWriteReg(uint8_t data);

#endif /* IOEXPANDER_H_ */