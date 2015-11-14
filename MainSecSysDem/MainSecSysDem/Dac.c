/*
 * Dac.c
 *
 * Created: 10/25/2015 1:50:24 PM
 *  Author: Aaron
 */ 

#include "secSysDefines.h"
#include "Dac.h"
#include <inttypes.h>
#include <avr/io.h>

/********************************************
This initializes the SPI for use with the DAC at it's highest speed
********************************************/

void DAC_spi_init(void)
{
	//Enable spi
	//Set as master
	//Set prescaler of 2 or 4MHz clock
	DDRB |= DAC_CS | DAC_IN | DAC_CLK;//sets data direction for DAC spi bits
	SPCR = 0x50; //setup SPI
	SPSR =0x01;//set SPI to double speed
}

/******************************************
This function takes an 8 bit number as data and transmits it to the DAC for sound generation
*******************************************/

void DAC_write_byte(uint8_t data)
{
	uint8_t command_bits = 0x30;//command bits setting the dac to unbuffered mode with gain of 1
	uint8_t output;
	command_bits = (data>>4) + command_bits;//adds the command bits to the data register and prepares it
	output = (data<<4);
	DAC_PORT_OUT &= ~DAC_CS;//set CS low for data sending
	SPDR = command_bits;
	while ( !(SPSR & 0x80) );//wait for data
	SPDR = output;
	while ( !(SPSR & 0x80) );//wait for data
	DAC_PORT_OUT |= DAC_CS;//set CS high to deactivate SPI
}