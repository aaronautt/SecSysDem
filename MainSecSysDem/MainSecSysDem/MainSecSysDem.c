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
#include "pushButton.h"
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1


// int main(void)
// {
// 	// Initialize the UART
// 	USART_Init(MYUBRR);
// 	stdout = &uart_output;
// 	stdin  = &uart_input;
// 	
// 	I2C_Init();
// 
// 
//     while(1)
// 	{
// 
// 	}
// }

int main()
{
	// ----- 5ms Timer -----
	// Reset timer0
	TCNT0 = 0;
	// set timer 0 to CTC mode
	TCCR0A |= (1<<WGM01);
	// set 5 ms interval, 78 cycles
	OCR0A = 255;
	//set prescaler to 1024
	TCCR0B |= (1<<CS02) | (1<<CS00);
	//set the timer output compare 0A to on
	TIMSK0 |= (1<<OCIE0A);
	// Enable global interrupts
	sei();	
	
	rgb_init();
	pushButton_init();
	//rgb_blue();
	rgb_flash_start();

	while(1)
	{
		rgb_flash_check();
		
		if(pushButtonRead())
		{
			rgb_flash_stop();
		}
		_delay_ms(32);
	}
	
	return(0);
}

ISR(TIMER0_COMPA_vect)
{
	rgb_flash_32msInterrupt();
}