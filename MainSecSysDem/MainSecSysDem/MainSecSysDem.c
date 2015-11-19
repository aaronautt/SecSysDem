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

uint8_t next_scroll = 0, timer = 0, idle = 0;
uint16_t idle_timer = 0; 

int main(void)
{	
	uint8_t keyRead = 22, push_press = 0, fire = 0, hall_window = 0, hall_door = 0, movement = 0;
	uint8_t i, scroll_postion = 0, dec_temp = 0, int_temp = 0, location = 10;
	uint8_t state = 0, new_code = 0, code_position = 0, armed_state = 0;
	char time[25], code[4] = "$$$$", master_code[4] = "1234";
	// Initialize the UART
	
	while(1)
	{
		keyRead = keypadReadPins();
		if(keyRead != 0) new_code = 1;
		else if(keyRead == 0) new_code = 0;
		else if(keyRead != 0 || push_press) idle_timer = 0;
		push_press = pushButtonRead();
		hall_window = Hall_Window_check();
		hall_door = Hall_Door_check();
		movement = PIR_check();
		scroll_postion = scroll_postion + next_scroll;
		//next_scroll = 0;
		if(scroll_postion > 23)
		{
			 scroll_postion = 0;
			 next_scroll = 0;
		}
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
				armed_state = 0;
				rgb_red();
				display_status(UNARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(int_temp, dec_temp);//needs temp read
				getStandardTimeStampStr(time);
				Scrolling_Text_single(&time[0], scroll_postion);//need to figure out how to check stuff while scrolling
				if(push_press) state = MENU_UNARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;//43 C is 110F
				else state = UNARMED;
			break;
			
			case MENU_UNARMED:
				rgb_red();
				display_main_menu();
				if(keyRead == 1) state = READ_MENU_ARM;
				else if(keyRead == 2) state = LAST_FIVE;
				else if(keyRead == 3) state = SET_TIME;
				else if(idle)
					{
						state = UNARMED;
						idle = 0;
					}
			break;
			
			case MENU_ARMED:
				rgb_green();
				display_main_menu();
				if(keyRead == 1) state = READ_MENU_DISARM;
				else if(keyRead == 2) state = LAST_FIVE;
				else if(keyRead == 3) state = SET_TIME;
				else if(idle)
				{
					state = ARMED;
					idle = 0;
				}
			break;
			
			case ARMED:
				armed_state = 1;
				rgb_green();
				display_status(ARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(dec_temp, int_temp);// to do read temp
				getStandardTimeStampStr(time);
				Scrolling_Text_single(&time[0], scroll_postion);
				if(push_press) state = MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				/*else if(motion) state = ALARMED_MOTION 
				else if(hall_effect_door) state = ALARMED_HALL_D
				else if(hall_effect_window) state = ALARMED_HALL_W*/
				else state = ARMED;
			break;
			
			case READ_MENU_DISARM:
				rgb_green();
				display_get_armcode(&code[0]);
				if(new_code) 
				{
					code[code_position] = keyRead;	
					code_position++;
				}
				else if(code_position >= 4)
				{
					state = CHECK_CODE_UN;
					code_position = 0;
				}
				else if(idle)
				{
					state = ARMED;
					idle = 0;
				}
			break;
			
			case READ_MENU_ARM:
				rgb_red();
				display_get_armcode(&code[0]);
				if(new_code)
				{
					code[code_position] = keyRead;
					code_position++;
				}
				else if(code_position >= 4)
				{
					state = CHECK_CODE_AR;
					code_position = 0;
				}
				else if(idle)
				{
					state = UNARMED;
					idle = 0;
				}
			break;
			
			case READ_ALARM_CODE:
				//rgb_alarm();
				//bell();
				display_get_armcode(&code[0]);
				if(new_code)
				{
					code[code_position] = keyRead;
					code_position++;
				}
				else if(code_position >= 4)
				{
					state = CHECK_CODE_AL;
					code_position = 0;
				}
				else if(idle)
				{
					state = ALARM_SOUND;
					idle = 0;
				}
			break;
			
			case CHECK_CODE_AL:
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				 && code[3] == master_code[3])
				{
					state = ARMED;
				}
				else
				{
					state = ALARM_SOUND;
				}
			break;
			
			case CHECK_CODE_UN:
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				&& code[3] == master_code[3])
				{
					state = UNARMED;
				}
				else
				{
					state = ARMED;
				}
			break;
			
			case CHECK_CODE_AR:
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				&& code[3] == master_code[3])
				{
					state = ARMED;
				}
				else
				{
					state = UNARMED;
				}
			break;
			
			case LAST_FIVE:
				//display_last_five(); complete this function
				if(idle && armed_state == 0)
				{
					state = UNARMED;
					idle = 0;
				}
				else if(idle && armed_state == 1)
				{
					state == ARMED;
					idle = 0;
				}
				else if(push_press && armed_state == 0) state = MENU_UNARMED;
				else if(push_press && armed_state == 1) state = MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				/*else if(motion) state = ALARMED_MOTION 
				else if(hall_effect_door) state = ALARMED_HALL_D
				else if(hall_effect_window) state = ALARMED_HALL_W*/
			break;
			
			case SET_TIME:
				
			break;
			
			case ALARMED_MOTION:
				location = MOTION;
				state = ALARM_SOUND;
			break;
			
			case ALARMED_HALL_D:
				location = DOOR;
				state = ALARM_SOUND;
			break;
			
			case ALARMED_HALL_W:
				location = WINDOW;
				state = ALARM_SOUND;
			break;
			
			case ALARMED_FIRE:
				location = FIRE;
				state = ALARM_SOUND;
			break;
			
			case ALARM_SOUND:
				//rgb_alarm();
				//bell();
				display_status(2, location);
				if(push_press) state = READ_ALARM_CODE;
			break;
		}
		
	}
}

ISR(TIMER2_COMPA_vect)
{
	next_scroll = 0;
	timer++;
	idle_timer++;
	//once the timer counts up to 12 32 ms interrupts it scrolls the text once (400msec)
	if(timer > 12)
	{
		next_scroll = 1;
		timer = 0;
	}
	else if(idle_timer > 1875)
	{
		idle = 1;
	}
}