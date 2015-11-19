// /*
//  * MainSecSysDem.c
//  *
//  * Created: 10/25/2015 1:50:24 PM
//  *  Author: Aaron
//  */ 
// 
// //INCLUDES
// #include "secSysDefines.h"
// #include <avr/io.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <util/delay.h>
// #include "interrupt.h"
// #include "WATCH_DOG.h"
// #include "PIR_DRIVER.h"
// #include "i2c_driver.h"
// #include "uart.h"
// #include "Hall_Sensors.h"
// #include "ioExpander.h"
// #include "keypad.h"
// #include "rgbLed.h"
// #include "bell.h"
// #include "pushButton.h"
// #include "rtcDriver.h"
// #include "eeprom328p.h"
// #include <string.h>
// #include <util/delay.h>
// #include "RTC_eeprom.h"
// #include "LCD_spi.h"
// #include "Dac.h"
// #include "lcd_moving.h"
// #include <inttypes.h>
// #include <avr/eeprom.h>
// #include <math.h>
// 
// 
// //DEFINES
// #define BAUD 9600
// #define MYUBRR F_CPU/8/BAUD-1
// 
// int main(void)
// {	
// 	int i, j, scrollPosition = -10;
// 	// Initialize the UART
// 	USART_Init(MYUBRR);
// 	stdout = &uart_output;
// 	stdin  = &uart_input;
// 	I2C_Init();
// 	DAC_spi_init();
// 	LCD_init();
// 	LCD_light_init();
// 	
// 	
// 	while(1)
// 	{
// 		
// 	}
// }

/*
 * EGR326_prelab09.c
 *
 * Created: 11/3/2015 1:52:23 PM
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
#define DS3231 0b11010000 // I2C bus address of DS1307 RTC
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
 #include <avr/io.h> // deal with port registers
// #include <util/delay.h> // used for _delay_ms function
// #include <string.h> // string manipulation routines
// #include <stdlib.h>
// #include <stdio.h>
// #include "uart.h"
// #include <avr/eeprom.h>
// ---------------------------------------------------------------------------
// TYPEDEFS
typedef uint8_t byte; // I just like byte & sbyte better
typedef int8_t sbyte;
// ---------------------------------------------------------------------------

uint8_t EEMEM alarm1[20];
uint8_t EEMEM alarm2[20];
uint8_t EEMEM alarm3[20];
uint8_t EEMEM alarm4[20];
uint8_t EEMEM alarm5[20];


//MISC ROUTINES
void InitAVR()
{
	DDRB = 0x3F; // 0011.1111; set B0-B5 as outputs
	DDRC = 0x00; // 0000.0000; set PORTC as inputs
}

void I2C_Init()
//at 16 MHz, the SCL frequency will be 16/(16+2(TWBR)), assuming prescalar of 0.
//so for 100KHz SCL, TWBR = ((F_CPU/F_SCL)-16)/2 = ((16/0.1)-16)/2 = 144/2 = 72.
{
	TWSR = 0; // set prescalar to zero
	TWBR = ((F_CPU/F_SCL)-16)/2; // set SCL frequency in TWI bit register
}

byte I2C_Detect(byte addr)
//look for device at specified address; return 1=found, 0=not found
{
	TWCR = TW_START; // send start condition
	while (!TW_READY); // wait
	TWDR = addr; // load device's bus address
	TWCR = TW_SEND; // and send it
	while (!TW_READY); // wait
	return (TW_STATUS==0x18); // return 1 if found; 0 otherwise
}

byte I2C_FindDevice(byte start)
//returns with address of first device found; 0=not found
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


//---------------------------------------------------------------------------
//							DS3231 RTC ROUTINES
//---------------------------------------------------------------------------
//This function returns a pointer to the current hour, minute, second,
//date, day of week, month, and year as integers.
void getTimeStampInt(uint8_t *hour, uint8_t *min, uint8_t *sec,
	uint8_t *date, uint8_t *dayOfWk, uint8_t *month, uint8_t *year)
{
	*hour = I2C_ReadRegister(DS3231,HOURS_REGISTER);
	*hour = (*hour & 0xF) + 10 * ((*hour >> 4) & 0x3);
	
	*min = I2C_ReadRegister(DS3231,MINUTES_REGISTER);
	*min = (*min & 0xF) + 10 * ((*min >> 4) & 0x7);
	
	*sec = I2C_ReadRegister(DS3231,SECONDS_REGISTER);
	*sec = (*sec & 0xF) + 10 * ((*sec >> 4) & 0x7);
	
	*date = I2C_ReadRegister(DS3231,DAYS_REGISTER);
	*date = (*date & 0xF) + 10 * ((*date >> 4) & 0x3);
	
	*dayOfWk = I2C_ReadRegister(DS3231,DAYOFWK_REGISTER);
	*dayOfWk = (*dayOfWk & 0x7);
	
	*month = I2C_ReadRegister(DS3231,MONTHS_REGISTER);
	*month = (*month & 0xF) + 10 * ((*month >> 4) & 0x1);
	
	*year = I2C_ReadRegister(DS3231, YEARS_REGISTER);
	*year = (*year & 0xF) + 10 * ((*year >> 4) & 0xF);
}

//This function returns a pointer to the current hour, minute, second,
//date, day of week, month, and year as characters.
void getTimeStampChar(char hour[], char min[], char sec[],
	char date[], char dayOfWk[], char month[], char year[])
{
	hour[0] = I2C_ReadRegister(DS3231,HOURS_REGISTER);
	hour[1] = 0x30 | (hour[0] & 0xF);
	hour[0] = 0x30 | ((hour[0] >> 4) & 0x3);
	
	min[0] = I2C_ReadRegister(DS3231,MINUTES_REGISTER);
	min[1] = 0x30 | (min[0] & 0xF);
	min[0] = 0x30 | ((min[0] >> 4) & 0x7);
	
	sec[0] = I2C_ReadRegister(DS3231,SECONDS_REGISTER);
	sec[1] = 0x30 | (sec[0] & 0xF);
	sec[0] = 0x30 | ((sec[0] >> 4) & 0x7);
	
	date[0] = I2C_ReadRegister(DS3231,DAYS_REGISTER);
	date[1] = 0x30 | (date[0] & 0xF);
	date[0] = 0x30 | ((date[0] >> 4) & 0x3);
	
	dayOfWk[0] = I2C_ReadRegister(DS3231,DAYOFWK_REGISTER);
	dayOfWk[0] = 0x30 | (dayOfWk[0] & 0x7);
	
	month[0] = I2C_ReadRegister(DS3231,MONTHS_REGISTER);
	month[1] = 0x30 | (month[0] & 0xF);
	month[0] = 0x30 | ((month[0] >> 4) & 0x1);
	
	year[0] = I2C_ReadRegister(DS3231, YEARS_REGISTER);
	year[1] = 0x30 | (year[0] & 0xF);
	year[0] = 0x30 | ((year[0] >> 4) & 0xF);
}

//This function sets the current time and date of the RTC
void setTimeStamp(uint8_t hour, uint8_t min, uint8_t sec,
uint8_t date, uint8_t dayOfWk, uint8_t month, uint8_t year)
{	
	hour = 0x3F & (((hour / 10) << 4) | (hour % 10));
	I2C_WriteRegister(DS3231,HOURS_REGISTER,hour);
	
	min = ((min / 10) << 4) | (min % 10);
	I2C_WriteRegister(DS3231,MINUTES_REGISTER,min);
	
	sec = ((sec / 10) << 4) | (sec % 10);
	I2C_WriteRegister(DS3231,SECONDS_REGISTER,sec);
	
	date = ((date / 10) << 4) | (date % 10);
	I2C_WriteRegister(DS3231,DAYS_REGISTER,date);
	
	I2C_WriteRegister(DS3231,DAYOFWK_REGISTER,dayOfWk);
	
	month = ((month / 10) << 4) | (month % 10);
	I2C_WriteRegister(DS3231,MONTHS_REGISTER,month);
	
	year = ((year / 10) << 4) | (year % 10);
	I2C_WriteRegister(DS3231, YEARS_REGISTER,year);
}

//This function allows the user to set the date and time of the RTC.
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

//This function returns the current temperature from the RTC in degrees C
void getTemp(uint8_t *intTemp,uint8_t *decTemp)
{	
	while(I2C_ReadRegister(DS3231, CONTROL_STATUS_REGISTER)&_BV(2));

	*intTemp = I2C_ReadRegister(DS3231,MSB_TEMP);
	*decTemp = (I2C_ReadRegister(DS3231,LSB_TEMP) >> 6) * 25;
}

void SetTimeDate()
//simple, hard-coded way to set the date.
{
	I2C_WriteRegister(DS3231,MONTHS_REGISTER, 0x08);
	I2C_WriteRegister(DS3231,DAYS_REGISTER, 0x31);
	I2C_WriteRegister(DS3231,YEARS_REGISTER, 0x13);
	I2C_WriteRegister(DS3231,HOURS_REGISTER, 0x08+0x40); // add 0x40 for PM
	I2C_WriteRegister(DS3231,MINUTES_REGISTER, 0x51);
	I2C_WriteRegister(DS3231,SECONDS_REGISTER, 0x00);
}

// void writeEepromTime(uint8_t* address, uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min)
// {
// 	const uint8_t iMax = 6;
// 	uint8_t i;
// 	uint8_t data[iMax];
// 	
// 	data[0]= year;
// 	data[2]= month;
// 	data[3]= day;
// 	data[4]= hour;
// 	data[5]= min;
// 	
// 	for(i=0; i<iMax; i++)
// 	{
// 		eeprom_update_byte(address+i,data[i]);
// 	}
// }
// 
// void readEepromTime(uint8_t* address, uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* min)
// {
// 	const uint8_t iMax = 6;
// 	uint8_t* data[iMax];
// 	
// // 	data[0]= (year>>8);
// // 	data[1]= (year&0x00FF);
// // 	data[2]= month;
// // 	data[3]= day;
// // 	data[4]= hour;
// // 	data[5]= min;
// 	
// 	eeprom_read_block(&data[0],address, iMax);
// //	for(i=0; i<iMax; i++)
// // 	{
// // 		eeprom_read_byte(*data[i]);
// // 	}
// }


int main(void)
{
	uint8_t* alarm = alarm1;
	uint8_t counter=0;
	char a;
	char hour[5],min[5],sec[5],date[5],dayOfWk[5],month[5],year[5];
	char timeStamp[20];
	char temp[20];
	
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	
	I2C_Init();
	
	printf("start\n");
	printf("%d\n",eeprom_read_byte(alarm1));
	printf("%d\n",eeprom_read_byte(alarm2));
	printf("%d\n",eeprom_read_byte(alarm3));
	printf("%d\n",eeprom_read_byte(alarm4));
	printf("%d\n\n-----",eeprom_read_byte(alarm5));
	
	while(1)
	{
		fflush(stdin);
		scanf("\n%c",&a);
		if(a == 's')
		{
			printf("\n\n");
			eeprom_read_block(&temp[0],alarm1,19);
			puts(temp);
			eeprom_read_block(&temp[0],alarm2,19);
			puts(temp);
			eeprom_read_block(&temp[0],alarm3,19);
			puts(temp);
			eeprom_read_block(&temp[0],alarm4,19);
			puts(temp);
			eeprom_read_block(&temp[0],alarm5,19);
			puts(temp);
		}
		else
		{
			getTimeStampChar(&hour[0], &min[0], &sec[0], &date[0], &dayOfWk[0], &month[0], &year[0]);
			printf("\n\nTime Stamp:\n");
			sprintf(timeStamp,"%c%c/%c%c/%c%c  %c%c:%c%c:%c%c",month[0],month[1],date[0],date[1],year[0],year[1],hour[0],hour[1],min[0],min[1],sec[0],sec[1]);
			puts(timeStamp);
			printf("Writing To EEPROM: %u\n",(unsigned int)alarm);

			//writeEepromTime(alarm, (uint8_t)(year[0]), month, date, hour, min);
			eeprom_update_block(&timeStamp[0],alarm,19);
			//eeprom_read_block(&temp[0],alarm1,19);
			//puts(temp);
			printf("Done.\n-----\n");
			
			switch(counter)
			{
				case 0:
				alarm = alarm2;
				counter++;
				break;
				case 1:
				alarm = alarm3;
				counter++;
				break;
				case 2:
				alarm = alarm4;
				counter++;
				break;
				case 3:
				alarm = alarm5;
				counter++;
				break;
				case 4:
				default:
				alarm = alarm1;
				counter = 0;
				break;
			}
		}
	}
}
