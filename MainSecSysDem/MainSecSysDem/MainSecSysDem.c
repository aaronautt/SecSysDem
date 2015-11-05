/*
 * MainSecSysDem.c
 *
 * Created: 10/25/2015 1:50:24 PM
 *  Author: Aaron
 */ 

//INCLUDES
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "secSysDefines.h"
#include "WATCH_DOG.h"
#include "PIR_DRIVER.h"
#include "i2c_driver.h"
#include "uart.h"
#include "Hall_Sensors.h"
#include "ioExpander.h"
#include "keypad.h"
#include "rgbLed.h"
#include "bell.h"
#include "pushButton.h"
#include "rtcDriver.h"
#include "eeprom328p.h"
#include <string.h>
#include <util/delay.h>


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

#include <avr/eeprom.h>
// uint8_t EEMEM alarm1[20];
// uint8_t EEMEM alarm2[20];
// uint8_t EEMEM alarm3[20];
// uint8_t EEMEM alarm4[20];
// uint8_t EEMEM alarm5[20];

// int main(void)
// {
// 	// Initialize the UART
// 	USART_Init(MYUBRR);
// 	stdout = &uart_output;
// 	stdin  = &uart_input;
// 	
// 	I2C_Init();
// 
//     while(1)
// 	{
// 
// 	}
// }

int main(void)
{
	//uint8_t* alarm = alarm1;
	//uint8_t counter=0, 
	uint8_t i;
	char a;
	//char hour[5],min[5],sec[5],date[5],dayOfWk[5],month[5],year[5];
	//char timeStamp[20];
	//char temp[20];
	char timeStamps[5][20];
	
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	
	I2C_Init();
	

// 	printf("start\n");
// 	printf("%d\n",eeprom_read_byte(alarm1));
// 	printf("%d\n",eeprom_read_byte(alarm2));
// 	printf("%d\n",eeprom_read_byte(alarm3));
// 	printf("%d\n",eeprom_read_byte(alarm4));
// 	printf("%d\n\n-----",eeprom_read_byte(alarm5));
	
	while(1)
	{
		printf("Enter a command: ");
		fflush(stdin);
		scanf("%c",&a);
		if(a == 's')
		{
			printf("\ns was entered\n");
			getFiveAlarmTimes(timeStamps);
			printf("\nI got them!\n");
			for(i=0; i<5; i++)
			{
				puts(&timeStamps[i][0]);				
			}
			printf("\nprinted!\n");
		}
		else
		{
			saveTimeToEeprom();
		}
	}
}
