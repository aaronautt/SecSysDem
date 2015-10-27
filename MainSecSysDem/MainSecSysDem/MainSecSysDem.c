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
	ioExpWriteReg(0xF0);
	
	printf("done\n");
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpSetPin(1);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpSetPin(2);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpClrPin(7);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpClrPin(2);
	printf("%x\n",ioExpReadReg());	
	printf("%d\n",ioExpReadPin(2));
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpClrPin(2);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpSetPin(6);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	ioExpSetPin(7);
	printf("%x\n",ioExpReadReg());
	printf("%d\n\n",ioExpReadPin(7));
	
	
	//HALL_init();
    while(1);
    //{
		/*
         if(!(Hall_Window_check())) printf("open!\n");
		 else; //null//
		 */
    //}
}