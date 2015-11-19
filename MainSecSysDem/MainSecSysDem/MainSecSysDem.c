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
#include "RTC_eeprom.h"
#include "LCD_spi.h"
#include "Dac.h"
#include "lcd_moving.h"
#include "timers.h"
#include <inttypes.h>
#include <avr/eeprom.h>
#include <math.h>


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1

//globals for interrupts
uint8_t keyRead = 22, push_press = 0, fire = 0, hall_window = 0, hall_door = 0, movement = 0; 

int main(void)
{	
	int i;
	uint8_t state = 0;
	char time[25];
	// Initialize the UART
	
	while(1)
	{
		keyRead = keypadReadPins();
		push_press = pushButtonRead();
		hall_window = Hall_Window_check();
		hall_door = Hall_Door_check();
		movement = PIR_check();
		switch(state)
		{
			case STARTUP:
			//check WDT basically just here for initializations
			USART_Init(MYUBRR);
			stdout = &uart_output;
			stdin  = &uart_input;
			I2C_Init();
			DAC_spi_init();
			LCD_init();
			LCD_light_init();
			pushButton_init();
			timerTwo_init();
			rgb_init();
			PIR_init();
			sei(); 
			state = UNARMED;
			break;
			case UNARMED:
			rgb_red();
			display_status(UNARMED, 0);
			display_temp(50);//needs temp read
			for(i=0;i<23;i++)
			{
				getStandardTimeStampStr(time);
				Scrolling_Text_single(&time[0], i);//need to figure out how to check stuff while scrolling
			}
			if(push_press) state = MENU_UNARMED;
			/*else if(fire) state = ALARMED_FIRE;        TODO          */
			break;
			case MENU_UNARMED:
			
			break;
			case MENU_ARMED:
			break;
			case ARMED:
			rgb_green();
			display_status(ARMED, 0);
			display_temp(50);// to do read temp
			if(push_press) state = MENU_ARMED;
			/*else if(fire) state = ALARMED_FIRE; TODO
			else if(motion) state = ALARMED_MOTION 
			else if(hall_effect_door) state = ALARMED_HALL_D
			else if(hall_effect_window) state = ALARMED_HALL_W*/
			break;
			case READ_MENU_UNARMED:
			break;
			case READ_MENU_ARMED:
			break;
			case READ_ALARM_CODE:
			break;
			case LAST_FIVE:
			break;
			case SET_TIME:
			break;
			case ALARMED_MOTION:
			break;
			case ALARMED_HALL_D:
			break;
			case ALARMED_HALL_W:
			break;
			case ALARMED_FIRE:
			break;
		}
		/*
		for(i=0;i<23;i++)
		{
			getStandardTimeStampStr(&time[0]);
			Scrolling_Text_single(&time[0], i);
			if(pushButtonRead())
			{
				display_main_menu();
				_delay_ms(500);
				LCD_clear();
				keyRead = keypadReadPins();
				LCD_gotoXY(0, 1);
				LCD_writeString_F((const char *)keyRead);
				
				if(keypadReadPins() == 1)
				{
					LCD_gotoXY(0, 3);
					LCD_writeString_F("arming");
				}
				else if(keypadReadPins() == 2)
				{
					LCD_gotoXY(0, 3);
					LCD_writeString_F("last five");
				}
				else if(keypadReadPins() == 3)
				{
					LCD_gotoXY(0, 3);
					LCD_writeString_F("set time");
				}
				else if(keypadReadPins() > 3);
				
			}
			
		}
		*/
	}
}

ISR(TIMER2_COMPA_vect)
{
	
}