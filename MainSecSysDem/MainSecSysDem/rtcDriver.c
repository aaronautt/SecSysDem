/*
 * rtcDriver.c
 *
 * Created: 11/4/2015 4:21:48 PM
 *  Author: Luke
 */ 

// GLOBAL DEFINES
#define F_CPU 8000000L // run CPU at 16 MHz
#define LED 5 // Boarduino LED on PB5
#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

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

#include <stdio.h>
#include <avr/io.h>
#include "i2c_driver.h"
#include "secSysDefines.h"
#include "avr/eeprom.h"
#include <util/delay.h>

uint8_t EEMEM alarmCtn;
uint8_t EEMEM armDisCtn;
uint8_t EEMEM alarmState;
const uint8_t EEMEM alarm[5][30];
const uint8_t EEMEM armDis[5][30];


// ---------------------------------------------------------------------------
//							DS3231 RTC ROUTINES
// ---------------------------------------------------------------------------
// This function returns a pointer to the current hour, minute, second,
// date, day of week, month, and year as integers.
void getTimeStampInt(uint8_t *hour, uint8_t *min, uint8_t *sec,
uint8_t *date, uint8_t *dayOfWk, uint8_t *month, uint8_t *year)
{
	*hour = I2C_ReadRegister(RTC_ADDR,HOURS_REGISTER);
	*hour = (*hour & 0xF) + 10 * ((*hour >> 4) & 0x3);
	
	*min = I2C_ReadRegister(RTC_ADDR,MINUTES_REGISTER);
	*min = (*min & 0xF) + 10 * ((*min >> 4) & 0x7);
	
	*sec = I2C_ReadRegister(RTC_ADDR,SECONDS_REGISTER);
	*sec = (*sec & 0xF) + 10 * ((*sec >> 4) & 0x7);
	
	*date = I2C_ReadRegister(RTC_ADDR,DAYS_REGISTER);
	*date = (*date & 0xF) + 10 * ((*date >> 4) & 0x3);
	
	*dayOfWk = I2C_ReadRegister(RTC_ADDR,DAYOFWK_REGISTER);
	*dayOfWk = (*dayOfWk & 0x7);
	
	*month = I2C_ReadRegister(RTC_ADDR,MONTHS_REGISTER);
	*month = (*month & 0xF) + 10 * ((*month >> 4) & 0x1);
	
	*year = I2C_ReadRegister(RTC_ADDR, YEARS_REGISTER);
	*year = (*year & 0xF) + 10 * ((*year >> 4) & 0xF);
}

// This function returns a pointer to the current hour, minute, second,
// date, day of week, month, and year as characters.
void getTimeStampChar(char hour[], char min[], char sec[],
char date[], char dayOfWk[], char month[], char year[])
{
	hour[0] = I2C_ReadRegister(RTC_ADDR,HOURS_REGISTER);
	hour[1] = 0x30 | (hour[0] & 0xF);
	hour[0] = 0x30 | ((hour[0] >> 4) & 0x3);
	
	min[0] = I2C_ReadRegister(RTC_ADDR,MINUTES_REGISTER);
	min[1] = 0x30 | (min[0] & 0xF);
	min[0] = 0x30 | ((min[0] >> 4) & 0x7);
	
	sec[0] = I2C_ReadRegister(RTC_ADDR,SECONDS_REGISTER);
	sec[1] = 0x30 | (sec[0] & 0xF);
	sec[0] = 0x30 | ((sec[0] >> 4) & 0x7);
	
	date[0] = I2C_ReadRegister(RTC_ADDR,DAYS_REGISTER);
	date[1] = 0x30 | (date[0] & 0xF);
	date[0] = 0x30 | ((date[0] >> 4) & 0x3);
	
	dayOfWk[0] = I2C_ReadRegister(RTC_ADDR,DAYOFWK_REGISTER);
	dayOfWk[0] = 0x30 | (dayOfWk[0] & 0x7);
	
	month[0] = I2C_ReadRegister(RTC_ADDR,MONTHS_REGISTER);
	month[1] = 0x30 | (month[0] & 0xF);
	month[0] = 0x30 | ((month[0] >> 4) & 0x1);
	
	year[0] = I2C_ReadRegister(RTC_ADDR, YEARS_REGISTER);
	year[1] = 0x30 | (year[0] & 0xF);
	year[0] = 0x30 | ((year[0] >> 4) & 0xF);
}

// This function sets the current time and date of the RTC
void setTimeStamp(uint8_t hour, uint8_t min, uint8_t sec,
uint8_t date, uint8_t dayOfWk, uint8_t month, uint8_t year)
{
	hour = 0x3F & (((hour / 10) << 4) | (hour % 10));
	I2C_WriteRegister(RTC_ADDR,HOURS_REGISTER,hour);
	
	min = ((min / 10) << 4) | (min % 10);
	I2C_WriteRegister(RTC_ADDR,MINUTES_REGISTER,min);
	
	sec = ((sec / 10) << 4) | (sec % 10);
	I2C_WriteRegister(RTC_ADDR,SECONDS_REGISTER,sec);
	
	date = ((date / 10) << 4) | (date % 10);
	I2C_WriteRegister(RTC_ADDR,DAYS_REGISTER,date);
	
	I2C_WriteRegister(RTC_ADDR,DAYOFWK_REGISTER,dayOfWk);
	
	month = ((month / 10) << 4) | (month % 10);
	I2C_WriteRegister(RTC_ADDR,MONTHS_REGISTER,month);
	
	year = ((year / 10) << 4) | (year % 10);
	I2C_WriteRegister(RTC_ADDR, YEARS_REGISTER,year);
}

// This function allows the user to set the date and time of the RTC.
void getAndSetTimeStamp()
{
	unsigned short uHour, uMin, uSec, uDate, uDayOfWk, uMonth, uYear;
	char userInput;
	
	printf("Would you like to set the current time? [Y/n] \n");
	scanf("%c",&userInput);
	
	if(userInput == 'Y')
	{
		// Request all the date and time
		printf("\nEnter Year: \n");
		fflush(stdin); // flush the buffer
		scanf("%hu", &uYear);
		uYear %= 100; // Get the last two digits of the year
		printf("\nEnter Month: \n");
		scanf("%hu", &uMonth);
		printf("\nEnter Day of Month: \n");
		scanf("%hu", &uDate);
		printf("\nEnter Day of Week: \n");
		scanf("%hu", &uDayOfWk);
		printf("\nEnter Hour: \n");
		scanf("%hu", &uHour);
		printf("\nEnter Minute: \n");
		scanf("%hu", &uMin);
		printf("\nEnter Second: \n");
		scanf("%hu", &uSec);
		
		// Set the date and time
		setTimeStamp(uHour,uMin,uSec,uDate,uDayOfWk,uMonth,(uint8_t)uYear);
	}
}

// This function returns the current temperature from the RTC in degrees C
void getTemp(uint8_t *intTemp,uint8_t *decTemp)
{
	while(I2C_ReadRegister(RTC_ADDR, CONTROL_STATUS_REGISTER)&_BV(2));

	*intTemp = I2C_ReadRegister(RTC_ADDR,MSB_TEMP);
	*decTemp = (I2C_ReadRegister(RTC_ADDR,LSB_TEMP) >> 6) * 25;
}

void getStandardTimeStampStr(char timeStamp[])
{
	char hour[2],min[2],sec[2],date[2],dayOfWk[2],month[2],year[2];
	
	// Get the time stamp from the RTC
	getTimeStampChar(hour, min, sec, date, dayOfWk, month, year);
	
	// Create string
	sprintf(timeStamp,"%c%c/%c%c/%c%c %c%c:%c%c:%c%c",month[0],month[1],date[0],
	date[1],year[0],year[1],hour[0],hour[1],min[0],min[1],sec[0],sec[1]);
}

void SetTimeDate()
// simple, hard-coded way to set the date.
{
	I2C_WriteRegister(RTC_ADDR, MONTHS_REGISTER, 0x08);
	I2C_WriteRegister(RTC_ADDR, DAYS_REGISTER, 0x31);
	I2C_WriteRegister(RTC_ADDR, YEARS_REGISTER, 0x13);
	I2C_WriteRegister(RTC_ADDR, HOURS_REGISTER, 0x08+0x40); // add 0x40 for PM
	I2C_WriteRegister(RTC_ADDR, MINUTES_REGISTER, 0x51);
	I2C_WriteRegister(RTC_ADDR, SECONDS_REGISTER, 0x00);
}

void saveAlarmTimeToEeprom()
{
	char stampStr[20];
	uint8_t stampNum;
	
	// Figure out which time was set last
	eeprom_read_block(&stampNum,&alarmCtn,1);
	
	// Increment the counter
	stampNum = (stampNum >= 4) ? 0 : stampNum+1;
	
	//Get the current time stamp
	getStandardTimeStampStr(stampStr);
	
	// Write the new time to the EEPROM
	eeprom_update_block(stampStr,(void*)&alarm[stampNum][0],30);
	
	// Write the new time stamp to the eeprom
	eeprom_write_byte(&alarmCtn, stampNum);
}


//saves the arm/disarm times to eeprom takes status as 0 for disarm or 1 for arm
void saveArmDisarmTimeToEeprom(uint8_t status)
{
	char stampStr[20], stampOut[30];
	uint8_t stampNum;
	
	// Figure out which time was set last
	eeprom_read_block(&stampNum,&armDisCtn,1);
	
	// Increment the counter
	stampNum = (stampNum >= 4) ? 0 : stampNum+1;
	
	//Get the current time stamp
	getStandardTimeStampStr(stampStr);
	
	if(status == 0)
	{
		sprintf(stampOut, "Disarm %s", stampStr);
	}
	else if(status == 1)
	{
		sprintf(stampOut, "Armed  %s", stampStr);
	}
	
	// Write the new time to the EEPROM
	eeprom_update_block(stampOut,(void*)&armDis[stampNum][0],30);
	
	
	// Write the new time stamp to the eeprom
	eeprom_write_byte(&armDisCtn, stampNum);
}

void getFiveAlarmTimes(char timeStamps[5][20])
{
	uint8_t stampNum, rowNum;
	
	// Figure out which time was set last
	uint8_t lastStampNum = eeprom_read_byte(&alarmCtn);

	for(stampNum=lastStampNum+1,rowNum=4; rowNum<5;rowNum--, stampNum++)
	{
		// If stampNum is greater than 5, reset it to read the 1st time stamp
		if( stampNum>= 5)
		{
			stampNum = 0;
		}
		
		// Read the time stamp and store it in the next row.
		// Since rowNum is decreasing, the temeStamps maxtrix will read
		// the arrays from newest to oldest.
		eeprom_read_block(&timeStamps[rowNum][0],&alarm[stampNum][0],20);
	}	
}

void getFiveArmDisarmTimes(char timeStamps[5][30])
{
	uint8_t stampNum, rowNum;
	
	// Figure out which time was set last
	uint8_t lastStampNum = eeprom_read_byte(&armDisCtn);

	for(stampNum=lastStampNum+1,rowNum=4; rowNum<5;rowNum--, stampNum++)
	{
		// If stampNum is greater than 5, reset it to read the 1st time stamp
		if( stampNum>= 5)
		{
			stampNum = 0;
		}
		
		// Read the time stamp and store it in the next row.
		// Since rowNum is decreasing, the temeStamps maxtrix will read
		// the arrays from newest to oldest.
		eeprom_read_block(&timeStamps[rowNum][0],&armDis[stampNum][0],30);
	}
}

//saves the alarm state so that upon power cycle the state is saved
void save_alarm_state(uint8_t alarm_state)
{		
	eeprom_update_byte(&alarmState, alarm_state);
}

//gets the alarm state so that upon power cycle the state is saved
uint8_t get_alarm_state()
{
	uint8_t out;
	out = eeprom_read_byte(&alarmState);
	return out;
}