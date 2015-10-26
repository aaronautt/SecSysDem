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
#include <string.h>


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
	printf("start ");
	ioExpWriteReg(0xFF);
	
	printf("done\n");
	printf("%x",ioExpReadReg());
	
	//HALL_init();
    while(1);
    //{
		/*
         if(!(Hall_Window_check())) printf("open!\n");
		 else; //null//
		 */
    //}
}