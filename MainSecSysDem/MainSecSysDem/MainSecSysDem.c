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
#include "Dac.h"
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
	
	// ----- Init Functions -----
	rgb_init();
	pushButton_init();
	//I2C_Init();
	
	rgb_blue();
	DAC_spi_init();
	DAC_write_byte(100);	
	rgb_green();
	//DAC_spi_init();
	//DAC_write_byte(100);
	
	
    while(1)
	{
		_delay_ms(1500);
		DAC_write_byte(0);
		_delay_ms(1500);
		DAC_write_byte(100);
		_delay_ms(1500);
		DAC_write_byte(200);
		_delay_ms(1500);
		DAC_write_byte(255);
	}
}