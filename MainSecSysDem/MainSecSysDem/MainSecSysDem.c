/*
 * MainSecSysDem.c
 *
 * Created: 10/25/2015 1:50:24 PM
 *  Author: Aaron
 */ 

//INCLUDES
#include "secSysDefines.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "interrupt.h"
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
//#include "RTC_eeprom.h"
//#include "LCD_spi.h"
//#include "Dac.h"
//#include "lcd_moving.h"
#include <inttypes.h>
#include <avr/eeprom.h>
#include <math.h>


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1
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


int main(void)
{

	char a;
	char times[5][20];
	
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	
	I2C_Init();
	
	while(1)
	{
		fflush(stdin);
		scanf("%c",&a);
		if(a == 's')
		{
			getFiveAlarmTimes(&times[0]);
			printf("%s\n%s\n%s\n%s\n%s\n\n", times[0], times[1], times[2], times[3], times[4]);
		}
		else
		{
			saveTimeToEeprom();
		}
	}
}
