/*
 * i2c_driver.c
 *
 * Created: 10/3/2015 6:09:54 PM
 *  Author: Luke Ganzevoort & Aaron Crump
 *
 *	Base on Bruce Hall's Driver for the DS1307 RTC
 *		Author: Bruce E. Hall <bhall66@gmail.com>
 *
 * This driver is made to interface with the I2C function of the ATmega328p 
 * via the Atmel Xplained Mini development board. The I2C pins that are used
 * are PC4 for Data and PC5 for the clock.
 */ 

// GLOBAL DEFINES
//#define F_CPU 16000000L // run CPU at 16 MHz

// ----- I2C (TWI) ROUTINES -----
// PC4 = SDA
// PC5 = SCL
#define F_SCL 100000L // I2C clock speed 100 KHz
#define READ 1
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // don't return ACK to slave
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register
#define I2C_Stop() TWCR = TW_STOP // inline macro for stop condition

// ------ RTC Defines ------
#define DS3231_ADDRESS 0b11010000 // I2C bus address of DS1307 RTC
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06
#define ALARM_1_SECONDS_REGISTER 0x07
#define ALARM_1_MINUTES_REGISTER 0x08
#define ALARM_1_HOURS_REGISTER 0x09
#define ALARM_1_DAYS_REGISTER 0x0A
#define ALARM_2_MINUTES_REGISTER 0x0B
#define ALARM_2_HOURS_REGISTER 0x0C
#define ALARM_2_DAYSDATE_REGISTER 0x0D
#define CONTROL_REGISTER 0x0E
#define CONTROL_STATUS_REGISTER 0x0F
#define AGING_OFFSET_REGISTER 0x10
#define MSB_TEMP 0x11
#define LSB_TEMP 0x12

// ---------------------------------------------------------------------------
// INCLUDES
#include "secSysDefines.h"
#include <avr/io.h> // deal with port registers
// ---------------------------------------------------------------------------

void I2C_Init()
{
	TWSR = 0; // set prescalar to zero
	TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
	
	// at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming 
	// prescalar of 0. So for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 
	// = ((16/0.1)-16)/2 = 144/2 = 72.
}

uint8_t I2C_Detect(uint8_t addr)
// look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START; // send start condition
	while (!TW_READY); // wait
	TWDR = addr; // load device's bus address
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}

uint8_t I2C_FindDevice(uint8_t start)
// returns with address of first device found; 0=not found
{
	for (uint8_t addr=start;addr<0xFF;addr++) // search all 256 addresses
	{
		if (I2C_Detect(addr)) // I2C detected?
		return addr; // leave as soon as one is found
	}
	return 0; // none detected, so return 0.
}

void I2C_Start (uint8_t slaveAddr)
{
	I2C_Detect(slaveAddr);
}

uint8_t I2C_Write (uint8_t data) // sends a data byte to slave
{
	TWDR = data; // load data to be sent
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS!=0x28);
}

uint8_t I2C_ReadACK () // reads a data byte from slave
{
	TWCR = TW_ACK; // ack = will read more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}

uint8_t I2C_ReadNACK () // reads a data byte from slave
{
	TWCR = TW_NACK; // nack = not reading more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}

void I2C_WriteByte(uint8_t busAddr, uint8_t data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(data); // then send the data byte
	I2C_Stop();
}

void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(deviceRegister); // first byte = device register address
	I2C_Write(data); // second byte = data for device register
	I2C_Stop();
}

uint8_t I2C_ReadByte(uint8_t busAddr)
{
	uint8_t data = 0;
	I2C_Start(busAddr+READ); // send device address and read bit
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}

uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister)
{
	uint8_t data = 0;
	I2C_Start(busAddr); // send device address
	I2C_Write(deviceRegister); // set register pointer
	I2C_Start(busAddr+READ); // restart as a read operation
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}

void I2C_DoubleWriteRegister(uint8_t busAddr, uint16_t deviceRegister, uint8_t data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(((deviceRegister>>8)&0xff)); // set register pointer 1
	I2C_Write((deviceRegister&0xff)); // set register pointer 2
	I2C_Write(data); // second byte = data for device register
	I2C_Stop();
}

uint8_t I2C_DoubleReadRegister(uint8_t busAddr, uint16_t deviceRegister)
{
	uint8_t data = 0;
	I2C_Start(busAddr); // send device address
	I2C_Write(((deviceRegister>>8)&0xff)); // set register pointer 1
	I2C_Write((deviceRegister&0xff)); // set register pointer 2
	I2C_Start(busAddr+READ); // restart as a read operation
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}