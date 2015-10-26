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
#include <string.h>
#include <util/delay.h>


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
	
	int i, out=0;
	char word[6] = "hello";
	
	
	I2C_WriteRegister(RTC_EEP_ADDR, 0, 1);
	
	out=I2C_ReadRegister(RTC_EEP_ADDR, 0);
	printf("hello\n");
	_delay_ms(1000);
	
	//HALL_init();
    while(1)
    {
		
		/*
         if(!(Hall_Window_check())) printf("open!\n");
		 else; //null//
		 */
    }
}