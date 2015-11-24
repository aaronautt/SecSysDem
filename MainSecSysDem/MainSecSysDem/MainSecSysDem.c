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
#include <string.h>
#include "RTC_eeprom.h"
#include "LCD_spi.h"
#include "Dac.h"
#include "lcd_moving.h"
#include "timers.h"
#include <inttypes.h>
#include <avr/eeprom.h>
#include "doorlock.h"


//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1



//globals for interrupts

static uint8_t next_scroll = 0, timer = 0, idle = 0;
static uint16_t idle_timer = 0; 

int main(void)

{
	uint8_t keyRead = 22, push_press = 0, fire = 0, hall_window = 0, hall_door = 0, movement = 0;
	uint8_t i, scroll_postion = 0, dec_temp = 0, int_temp = 0, location = 10, color = 0;
	uint8_t state = 1, new_code = 0, code_position = 0, armed_state = 0;
	char time[25];
	uint8_t  code[4] = {0 ,0 ,0, 0}, master_code[4] = {1, 2, 3, 4};
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	I2C_Init();
	DAC_spi_init();
	LCD_init();
	//LCD_light_init();
	pushButton_init();
	//timerTwo_init();
	timerOne_init();
	rgb_init();
	PIR_init();
	HALL_init();
	sei();
	while(1)
	{
		//this start block checks all the sensors and updates their flags
		keyRead = keypadReadPins();
		push_press = pushButtonRead();
		//hall_window = Hall_Window_check();
		//hall_door = Hall_Door_check();
		//movement = PIR_check();
		scroll_postion = scroll_postion + next_scroll;
		if(keyRead != 0) new_code = 1;
		else if(keyRead == 0) new_code = 0;
		if(keyRead != 0 || push_press) idle_timer = 0;
		//next_scroll = 0;
		if(scroll_postion > 23)
		{
			 scroll_postion = 0;
			 next_scroll = 0;
		}
		switch(state)
		{
			/*
			case STARTUP:
				//check WDT, basically just here for initializations
				USART_Init(MYUBRR);
				stdout = &uart_output;
				stdin  = &uart_input;
				I2C_Init();
				DAC_spi_init();
				LCD_init();
				//LCD_light_init();
				pushButton_init();
				timerTwo_init();
				timerOne_init();
				rgb_init();
				PIR_init();
				HALL_init();
				sei(); 
				state = UNARMED;
			break;
			*/
			//basic unarmed state, displays status, scrolling time and temp, exits to menu_unarmed when button is pressed or alarm if fire is detected 
			case UNARMED:
				armed_state = 0;
				idle_timer = 0;//keep idle timer at 0 unless accessing a menu
				LCD_clear();
				rgb_red();
				display_status(UNARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(int_temp, dec_temp);
				getStandardTimeStampStr(time);
				Scrolling_Text_single(&time[0], scroll_postion);//need to figure out how to check stuff while scrolling
				if(push_press) 
				{
					state = DISPLAY_MENU_UNARMED;
				}
				else if(int_temp > 43) state = ALARMED_FIRE;//43 C is 110F
				//else state = UNARMED;
			break;
			
			//just displays the menu
			case DISPLAY_MENU_UNARMED:
			rgb_blue();
			LCD_clear();
			LCD_gotoXY(0,0);
			display_main_menu();
			state = MENU_UNARMED;
			break;
			//unarmed menu, displays the menu and exits to the three menu options on keypad press, returns to unarmed if one minute idle counter is reached
			//	1. arm
			//	2. last five alarms
			//	3. last five dis/arm
			//	4. set time
			//	5. speak time
			case MENU_UNARMED:
				rgb_blue();
				if(keyRead == 1) state = DISPLAY_READ_MENU_ARM;
				else if(keyRead == 2) state = LAST_FIVE_ALARMS;
				else if(keyRead == 3) state = LAST_FIVE_ARM;
				else if(keyRead == 4) state = SET_TIME;
				else if(keyRead == 5) state = SPEAK_TIME;
				if(idle_timer > 1875)
					{
						state = UNARMED;
						idle = 0;
						idle_timer = 0;
					}
					
				//else state = MENU_UNARMED;
			break;
			//armed state displays status, temp. and scrolling time. exits to menu on button press.
			//exits to any of the four alarm states on sensor trip, fire, motion, door, window
			case ARMED:
				armed_state = 1;
				idle_timer = 0; //keep idle timer at 0 unless accessing a menu
				LCD_clear();
				rgb_green();
				display_status(ARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(dec_temp, int_temp);// to do read temp
				getStandardTimeStampStr(time);
				Scrolling_Text_single(&time[0], scroll_postion);
				if(push_press) state = DISPLAY_MENU_ARMED;
				/*
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				else if(movement) state = ALARMED_MOTION; 
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
				*/
				else state = ARMED;
			break;
			case DISPLAY_MENU_ARMED:
			rgb_blue();
			LCD_clear();
			display_main_menu();
			state = MENU_ARMED;
			break;	
			//armed menu, displays the menu and exits to the three menu options on keypad press, returns to unarmed if one minute idle counter is reached
			//	1. disarm
			//	2. last five alarms
			//	3. last five dis/arm
			//	4. set time
			//	5. speak time
			case MENU_ARMED:
				rgb_blue();
				if(keyRead == 1) state = DISPLAY_READ_MENU_DISARM;
				
				else if(keyRead == 2) state = LAST_FIVE_ALARMS;
				else if(keyRead == 3) state = LAST_FIVE_ARM;
				else if(keyRead == 4) state = SET_TIME;
				else if(keyRead == 5) state = SPEAK_TIME;
				
				if(idle_timer > 1875)
				{
					state = ARMED;
					idle = 0;
					idle_timer = 0;
				}
				//else state = MENU_ARMED;
			break;
				
			case DISPLAY_READ_MENU_DISARM:
				rgb_green();
				LCD_clear();
				display_get_armcode();
				state = READ_MENU_DISARM;
			break;
				
			
			
			//to disarm system, reads the keypad presses, stores them in the array code[], exits to check_code_un
			//to check the inputted code to master after four digits have been collected
			case READ_MENU_DISARM:
				rgb_green();
				if(new_code) 
				{
					code[code_position] = keyRead;	
					code_position++;
					//display_armcode(&code[0]);
					//state = READ_MENU_DISARM;
				}
				if(keyRead == 12)
				{
					state = CHECK_CODE_UN;
					code_position = 0;
				}
				if(idle_timer>1875)
				{
					state = ARMED;
					idle = 0;
					idle_timer = 0;
					code_position = 0;
				}
				//else state = READ_MENU_DISARM;
			break;
			
			case DISPLAY_READ_MENU_ARM:
			rgb_green();
			display_get_armcode();
			state = READ_MENU_ARM;
			break;
			
			//to arm system, reads the keypad presses, stores them in the array code[], exits to check_code_ar
			//to check the inputted code to master after four digits have been collected
			case READ_MENU_ARM:
				rgb_green();
				if(new_code)
				{
					code[code_position] = keyRead;
					code_position++;
					//display_armcode(&code[0]);
					//state = READ_MENU_ARM;
				}
				if(keyRead == 12)
				{
					state = CHECK_CODE_AR;
					code_position = 0;
				}
				if(idle_timer>1875)
				{
					state = UNARMED;
					idle = 0;
					idle_timer = 0;
					code_position = 0;
				}
				//else state = READ_MENU_ARM;
			break;
			//to disarm system when in alarm state, reads the keypad presses, stores them in the array code[], exits to check_code_al
			//to check the inputted code to master after four digits have been collected
			case READ_ALARM_CODE:
				//rgb_alarm();
				//bell();
				LCD_clear();
				display_get_armcode();
				if(new_code)
				{
					code[code_position] = keyRead;
					code_position++;
					display_armcode(&code[0]);
				}
				if(keyRead == 12)
				{
					state = CHECK_CODE_AL;
					code_position = 0;
				}
				if(idle)
				{
					state = ALARM_SOUND;
					idle = 0;
					idle_timer = 0;
					code_position = 0;
				}
				//else state = READ_ALARM_CODE;
			break;
			//checks code[] input from read_alarm_code, if it matches it returns to armed state, if it doesn't it stays in alarm_sound state
			case CHECK_CODE_AL:
				LCD_clear();
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
			//checks code[] input from read_menu_disarm, if it matches it returns to unarmed state, if it doesn't it stays in armed state
			case CHECK_CODE_UN:
				LCD_clear();
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
			//checks code[] input from read_menu_arm, if it matches it returns to armed state, if it doesn't it stays in unarmed state
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
			//entered from armed menu or unarmed menu, exited if any alarm is tripped or the push button is pressed to enter the menu again
			//if the idle flag is set 
			case LAST_FIVE_ALARMS:
				//display_last_five_disarms(); complete this function
				if(idle && armed_state == 0)
				{
					state = UNARMED;
					idle_timer = 0;
					idle = 0;
				}
				if(idle && armed_state == 1)
				{
					state = ARMED;
					idle_timer = 0;
					idle = 0;
				}
				if(push_press && armed_state == 0) state = MENU_UNARMED;
				if(push_press && armed_state == 1) state = MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				else if(movement) state = ALARMED_MOTION; 
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
				else state = LAST_FIVE_ALARMS;
			break;
			
			//entered from armed menu or unarmed menu, exited if any alarm is tripped or the push button is pressed to enter the menu again
			//if the idle flag is set 
			case LAST_FIVE_ARM:
				//display_last_five_arm(); complete this function
				if(idle && armed_state == 0)
				{
					state = UNARMED;
					idle_timer = 0;
					idle = 0;
				}
				if(idle && armed_state == 1)
				{
					state = ARMED;
					idle_timer = 0;
					idle = 0;
				}
				if(push_press && armed_state == 0) state = MENU_UNARMED;
				else if(push_press && armed_state == 1) state = MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				else if(movement) state = ALARMED_MOTION; 
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
				else state = LAST_FIVE_ARM;
			break;
			//this sets the time, Luke's job to write this state
			case SET_TIME:
				state = UNARMED;
			break;
			//alarmed_motion is entered when the PIR is tripped, exits to alarm_sound after setting location
			case ALARMED_MOTION:
				location = MOTION;
				state = ALARM_SOUND;
			break;
			//alarmed_hall_d is entered when the door hall effect sensor is tripped, exits to alarm_sound after setting location
			case ALARMED_HALL_D:
				location = DOOR;
				state = ALARM_SOUND;
			break;
			//alarmed_hall_w is entered when the window hall effect sensor is tripped, exits to alarm_sound after setting location
			case ALARMED_HALL_W:
				location = WINDOW;
				state = ALARM_SOUND;
			break;
			//alarmed_fire is entered when the fire alarm is tripped, it exits to alarm_sound after location is set for status function
			case ALARMED_FIRE:
				location = FIRE;
				state = ALARM_SOUND;
			break;
			//alarm sound state sounds the bell and the flashing LED, it exits only upon push button press to read_alarm state
			case ALARM_SOUND:
				idle_timer = 0; //keep idle timer at 0 if not in a menu
				//rgb_alarm();
				//bell();
				display_status(2, location);
				if(push_press) state = READ_ALARM_CODE;
				else state = ALARM_SOUND;
			break;
			//this state speaks the current time when the corresponding menu selection is made, exits to the previous menu state
			case SPEAK_TIME:
				getStandardTimeStampStr(&time[0]);
				//speak_time_stamp();    !!!! TODO write this function
				if((armed_state = 1))
				{
					state = ARMED;
				}
				else state = UNARMED;
			break;
			
		}
		
	}
	
}

ISR(TIMER1_COMPA_vect)
{	
	
	next_scroll = 0;
	timer = timer + 1;
	idle_timer = idle_timer + 1;
	//once the timer counts up to 12 32 ms interrupts it scrolls the text once (400msec)
	if(timer > 2)
	{
		next_scroll = 1;
		timer = 0;
	}
	//once the idle time reaches a minute it sets the idle flag and resets the menuing state
	/*
	if(idle_timer > 1875)
	{
		idle = 10;
		idle_timer = 0;
	}
	*/
}


ISR(TIMER2_OVF_vect)
{
	/*
	next_scroll = 0;
	timer = timer + 1;
	idle_timer = idle_timer + 1;
	//once the timer counts up to 12 32 ms interrupts it scrolls the text once (400msec)
	if(timer > 2)
	{
		next_scroll = 1;
		timer = 0;
	}
	//once the idle time reaches a minute it sets the idle flag and resets the menuing state
	if(idle_timer > 1875)
	{
		idle = 1;
		idle_timer = 0;
	}
	*/
}
