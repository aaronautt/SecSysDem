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
#include "RTC_eeprom.h"
#include <inttypes.h>

//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

int main(void)
{
	
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	I2C_Init();
	
	uint16_t i;
	uint8_t date[] = "05 230 45 55 2015", action[]= "03 25 01 56 2222";
	char  wank[1000] = "10", blue[60];

	RTC_Write_Alarm_Time(&date[0], RTC_Get_Place_In_Line());
	RTC_Write_Action_Time(&action[0], RTC_Get_Place_In_Line(), 1);
	printf("wank 1 = %s\n", &wank[0]);
	RTC_Last_Five(&wank[0]);
	printf("wank 2 = %s\n", &wank[0]);
	readsomeshit(&blue[0]);
while(1)
{
}
    
}