/*
 * I2C_PRE.c
 
 Pre lab 5
 Aaron Crump
 EGR 326-903
 
 **********************************************************
 *		 CODE TAKEN FROM                                  *
 *		 Add a DS3231 RTC clock to yourAVR microcontroller*
 *		 Bruce E. Hall, W8BH                              *  **********************************************************
 *
 * Created: 9/27/2015 3:06:50 PM
 *  Author: Bruce E. Hall
	Edits made by: Aaron Crump
	EGR 326-903
	Instructor: Dr. Bossmeyer
 */ 


//includes
#define F_CPU 16000000L // CPU clock speed 16 MHz
#include <avr/io.h>
#include <util/delay.h> // used for _delay_ms function
#include <string.h> // string manipulation routines
#include <stdlib.h>#include <stdio.h>#include "uart.h"//defining standard byte sized variablestypedef uint8_t byte; 
typedef int8_t sbyte;
//defines for program
#define F_SCL 100000L // I2C clock speed 100 kHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define DS3231 0x68 // I2C bus address of DS3231 RTC
#define TW_SEND 0x84 // send data (TWINT,TWEN)
#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register
#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define I2C_Stop() TWCR = TW_STOP // inline macro for stop condition
#define TW_ACK 0xC4 // return ACK to slave
#define TW_NACK 0x84 // read data with NACK (last byte)
#define READ 1
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)//list of ds3231 registers
#define SECONDS_REGISTER 0x00
#define MINUTES_REGISTER 0x01
#define HOURS_REGISTER 0x02
#define DAYOFWK_REGISTER 0x03
#define DAYS_REGISTER 0x04
#define MONTHS_REGISTER 0x05
#define YEARS_REGISTER 0x06#define ALARM_1_SECONDS_REGISTER 0x07#define ALARM_1_MINUTES_REGISTER 0x08#define ALARM_1_HOURS_REGISTER 0x09#define ALARM_1_DAYS_REGISTER 0x0A#define ALARM_2_MINUTES_REGISTER 0x0B#define ALARM_2_HOURS_REGISTER 0x0C#define ALARM_2_DAYSDATE_REGISTER 0x0D#define CONTROL_REGISTER 0x0E#define CONTROL_STATUS_REGISTER 0x0F#define AGING_OFFSET_REGISTER 0x10#define MSB_TEMP 0x11#define LSB_TEMP 0x12//function prototypes
void I2C_Init();
byte I2C_Detect(byte addr);
byte I2C_FindDevice(byte start);
void I2C_Start (byte slaveAddr);
byte I2C_Write (byte data);
byte I2C_ReadACK ();
byte I2C_ReadNACK ();
void I2C_WriteByte(byte busAddr, byte data);
void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data);
byte I2C_ReadRegister(byte busAddr, byte deviceRegister);
void DS3231_GetTime(byte *hours, byte *minutes, byte *seconds);
void DS3231_GetDate(byte *months, byte *days, byte *years);
void SetTimeDate();
uint16_t DS3231_GetTemp();



int main(void)
{
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;

	DDRC &= _BV(4) & _BV(5);//sets SDA and SCL as inputs
	
	uint8_t hours, minutes, seconds, months, days, years, temp;
	I2C_Init();
	if(I2C_Detect(DS3231));//checks to see if the RTC is there
	else if(!I2C_Detect(DS3231)) printf("device not found!");//if it's not it spits an error
	
    while(1)
    {
		getchar();
		DS3231_GetTime(&hours, &minutes,&seconds);
		DS3231_GetDate(&months, &days, &years);
		temp = DS3231_GetTemp();
        printf("the time is %d:%d:%d\n", hours, minutes, seconds);
		printf("the date is %d:%d:%d\n", months, days, years);
		printf("the temperature is %d\n", temp);
    }
}


void I2C_Init()
// at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming prescalar of 0.
// so for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72.
{
	TWSR = 0; // set prescalar to zero
	TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
}
byte I2C_Detect(byte addr)
// look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START; // send start condition
	while (!TW_READY); // wait
	TWDR = addr; // load device's bus address
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}
byte I2C_FindDevice(byte start)
// returns with address of first device found; 0=not found
{
	for (byte addr=start;addr<0xFF;addr++) // search all 256 addresses
	{
		if (I2C_Detect(addr)) // I2C detected?
		return addr; // leave as soon as one is found
	}
	return 0; // none detected, so return 0.
}
void I2C_Start (byte slaveAddr)
{
	I2C_Detect(slaveAddr);
}
byte I2C_Write (byte data) // sends a data byte to slave
{
	TWDR = data; // load data to be sent
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS!=0x28);
}
byte I2C_ReadACK () // reads a data byte from slave
{
	TWCR = TW_ACK; // ack = will read more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}
byte I2C_ReadNACK () // reads a data byte from slave
{
	TWCR = TW_NACK; // nack = not reading more data
	while (!TW_READY); // wait
	return TWDR;
	//return (TW_STATUS!=0x28);
}
void I2C_WriteByte(byte busAddr, byte data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(data); // then send the data byte
	I2C_Stop();
}
void I2C_WriteRegister(byte busAddr, byte deviceRegister, byte data)
{
	I2C_Start(busAddr); // send bus address
	I2C_Write(deviceRegister); // first byte = device register address
	I2C_Write(data); // second byte = data for device register
	I2C_Stop();
}
byte I2C_ReadRegister(byte busAddr, byte deviceRegister)
{
	byte data = 0;
	I2C_Start(busAddr); // send device address
	I2C_Write(deviceRegister); // set register pointer
	I2C_Start(busAddr+READ); // restart as a read operation
	data = I2C_ReadNACK(); // read the register data
	I2C_Stop(); // stop
	return data;
}

void DS3231_GetTime(byte *hours, byte *minutes, byte *seconds)
// returns hours, minutes, and seconds in BCD format
{
	*hours = I2C_ReadRegister(DS3231,HOURS_REGISTER);
	*minutes = I2C_ReadRegister(DS3231,MINUTES_REGISTER);
	*seconds = I2C_ReadRegister(DS3231,SECONDS_REGISTER);
	if (*hours & 0x40) // 12hr mode:
	*hours &= 0x1F; // use bottom 5 bits (pm bit = temp & 0x20)
	else *hours &= 0x3F; // 24hr mode: use bottom 6 bits
}
void DS3231_GetDate(byte *months, byte *days, byte *years)
// returns months, days, and years in BCD format
{
	*months = I2C_ReadRegister(DS3231,MONTHS_REGISTER);
	*days = I2C_ReadRegister(DS3231,DAYS_REGISTER);
	*years = I2C_ReadRegister(DS3231,YEARS_REGISTER);
}
void SetTimeDate()
// simple, hard-coded way to set the date 09/28/2105
{
	//time and date must be set before using
	I2C_WriteRegister(DS3231,MONTHS_REGISTER, 0x08);
	I2C_WriteRegister(DS3231,DAYS_REGISTER, 0x31);
	I2C_WriteRegister(DS3231,YEARS_REGISTER, 0x13);
	I2C_WriteRegister(DS3231,HOURS_REGISTER, 0x08+0x40); // add 0x40 for PM
	I2C_WriteRegister(DS3231,MINUTES_REGISTER, 0x51);
	I2C_WriteRegister(DS3231,SECONDS_REGISTER, 0x00);
}
uint16_t DS3231_GetTemp()
// returns temp
{
	uint8_t high_temp, low_temp;
	uint16_t temperature = 0;
	high_temp = I2C_ReadRegister(DS3231,MSB_TEMP);
	low_temp = I2C_ReadRegister(DS3231,LSB_TEMP);
	temperature |= (high_temp<<2) | (low_temp>>6);//puts the temp data in the bottom 10 bits of a 16 bit number
	return temperature;
}