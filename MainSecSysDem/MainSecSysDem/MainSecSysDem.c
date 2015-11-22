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
#include "pushButton.h"
#include <avr/interrupt.h>
#include "bell.h"
#include "pushButton.h"
#include "rtcDriver.h"
#include "eeprom328p.h"
#include <string.h>
#include <util/delay.h>
#include "RTC_eeprom.h"
#include "LCD_spi.h"
#include "Dac.h"
#include "lcd_moving.h"
#include <inttypes.h>
#include <avr/eeprom.h>
#include <math.h>


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1


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
	uint8_t i=0, j, scrollPosition = -10;
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	I2C_Init();
	DAC_spi_init();
	LCD_init();
	LCD_light_init();

	DDRD |= (_BV(5) | _BV(6));

	// ----- 5ms Timer -----
	// Reset timer0
	TCNT0 = 0;
	// set timer 0 to CTC mode
	TCCR0A |= ((1<<COM0A1) | (1<<COM0B1));
	//
	TCCR0A |= (1<<WGM00);
	// set 5 ms interval, 78 cycles
	OCR0A = 100;
	OCR0B = 255;
	
	//set prescaler to 1024
	TCCR0B |= (1<<CS00);
	//set the timer output compare 0A to on
	//TIMSK0 |= (1<<OCIE0A);
	
	_delay_ms(32);
	OCR0B = 30;
	
	while(1)
	{
		_delay_ms(5);
		i++;
		OCR0A = i;
	}
}