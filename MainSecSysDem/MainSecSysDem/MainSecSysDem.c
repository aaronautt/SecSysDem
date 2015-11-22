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
#include "doorlock.h"


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
	pushButton_init();
	doorlockAndLcdBacklight_init();
	rgb_init();

	// ----- 5ms Timer -----
	// Reset timer0
	TCNT2 = 0;
	// set timer 0 to CTC mode
	TCCR2A |= (1<<WGM21);
	// set 5 ms interval, 78 cycles
	OCR2A = 255;
	//set prescaler to 1024
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);
	//set the timer output compare 0A to on
	TIMSK2 |= (1<<OCIE2A);
	// Enable global interrupts
	sei();
	
	while(1)
	{	
		i = OCR0B;
		if(i == 255)
		{
			rgb_red();
		}
		else if(i == 30)
		{
			rgb_blue();
		}
		else if(i == 0)
		{
			rgb_green();
		}
		else
		{
			rgb_off();
		}
		

		if(pushButtonRead())
		{
			doorlockUnlock();
		}
	}
}

ISR(TIMER2_COMPA_vect)
{
	doorlockInterruptFuction();
}