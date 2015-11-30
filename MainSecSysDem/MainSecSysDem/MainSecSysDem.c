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
#include "setTime.h"
#include "photo_sensor.h"

//DEFINES
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1



//globals for interrupts

volatile uint8_t next_scroll = 0, timer = 0, idle = 0, ambient = 0, press_flag = 0, press_flag_count = 0;
static uint16_t idle_timer = 0; 


int main(void)
{
	uint8_t keyRead = 22,keyReadPrev = 0, push_press = 0, hall_window = 0, hall_door = 0, movement = 0;
	uint8_t i, /*scroll_postion = 0,*/ dec_temp = 0, int_temp = 0, location = 10, button_press = 0;
	uint8_t prev_position = 0;
	uint8_t state = 1, new_code = 0, code_position = 0, armed_state = 0;
	char time[25], time_array[5][30];
	uint8_t  code[4] = {0 ,0 ,0, 0}, master_code[4] = {1, 2, 3, 4};
	// Initialize the UART
	USART_Init(MYUBRR);
	stdout = &uart_output;
	stdin  = &uart_input;
	I2C_Init();
	DAC_spi_init();
	LCD_init();
	pushButton_init();
	timerTwo_init();
	timerOne_init();
	rgb_init();
	PIR_init();
	HALL_init();
	doorlockAndLcdBacklight_init();
	bell_init();
	photo_sensor_init();
	if(get_alarm_state() == 1) state = ARMED;
	else if(get_alarm_state() == 0) state = UNARMED;
	else state = UNARMED;
	sei();
	while(1)
	{
		//this start block checks all the sensors and updates their flags
		keyRead = keypadReadPins();
		button_press = pushButtonRead();
		if(button_press)
		{
			press_flag_count = 0;
			push_press = 1;
		}
		else if(press_flag) 
		{
			press_flag = 0;
			push_press = 0;
		}
		//movement = PIR_check();
		bell_UpdateStatus();
		// Reset the keyRead if it was the same as lastTime
		if(keyRead == keyReadPrev) keyRead = 0;
		else keyReadPrev = keyRead;
		//hall_window = Hall_Window_check();
		//hall_door = Hall_Door_check();
		if(keyRead != 0) new_code = 1;
		else new_code = 0;
		if(keyRead != 0 || push_press) idle_timer = 0;
		if(next_scroll > 23)
		{
			next_scroll = 0;
		}
		
		if(ambient)
		{
			LcdBacklightBrightness(convert_adc_to_DC());
			ambient = 0;
		}
		switch(state)
		{
			//basic unarmed state, displays status, scrolling time and temp, exits to menu_unarmed when button is pressed or alarm if fire is detected 
			case UNARMED:
				armed_state = 0;
				save_alarm_state(0);
				idle_timer = 0;//keep idle timer at 0 unless accessing a menu
				if(push_press==1) state = DISPLAY_MENU_UNARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;//43 C is 110F
				rgb_green();
				display_status(B_UNARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(int_temp, dec_temp);
				
					getStandardTimeStampStr(time);
					Scrolling_Text_single(&time[0], next_scroll);//need to figure out how to check stuff while scrolling
			break;
			
			//just displays the menu
			case DISPLAY_MENU_UNARMED:
			rgb_blue();
			display_main_menu();
			state = MENU_UNARMED;
			break;
			
			//unarmed menu, displays the menu and exits to the three menu options on keypad press, returns to unarmed if one minute idle counter is reached
			//	1. arm
			//	2. last five alarms
			//	3. last five dis/arm
			//	4. set time
			//	5. more
			case MENU_UNARMED:
				rgb_blue();
				if(keyRead == 1) state = DISPLAY_READ_MENU_ARM;
				else if(keyRead == 2) state = DISPLAY_LAST_FIVE_ALARMS;
				else if(keyRead == 3) state = DISPLAY_LAST_FIVE_ARMS;
				else if(keyRead == 4) state = SET_TIME;
				else if(keyRead == 5) state = DISPLAY_MENU_UNARMED_2;
				if(idle)
					{
						state = UNARMED;
						idle = 0;
						idle_timer = 0;
					}
			break;
			
			case DISPLAY_MENU_UNARMED_2:
			rgb_blue();
			display_main_menu_two();
			state = MENU_UNARMED_2;
			break;
			
			case MENU_UNARMED_2:
			rgb_blue();
			if(keyRead == 6) state = DISPLAY_LOCK_SOLENOID;
			else if(keyRead == 7) state = UNARMED;
			if(idle)
			{
				state = UNARMED;
				idle = 0;
				idle_timer = 0;
			}
			break;
			//armed state displays status, temp. and scrolling time. exits to menu on button press.
			//exits to any of the four alarm states on sensor trip, fire, motion, door, window
			case ARMED:
				armed_state = 1;
				save_alarm_state(1);
				idle_timer = 0; //keep idle timer at 0 unless accessing a menu
				if(push_press==1) state = DISPLAY_MENU_ARMED;
				rgb_red();
				display_status(B_ARMED, 0);
				getTemp(&int_temp, &dec_temp);
				display_temp(int_temp, dec_temp);// to do read temp
				
					getStandardTimeStampStr(time);
					Scrolling_Text_single(&time[0], next_scroll);
				if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				if(movement) state = ALARMED_MOTION; 
				/*
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
				*/
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
			//	5. Exit
			case MENU_ARMED:
				rgb_blue();
				if(keyRead == 1) state = DISPLAY_READ_MENU_DISARM;
				else if(keyRead == 2) state = DISPLAY_LAST_FIVE_ALARMS;
				else if(keyRead == 3) state = DISPLAY_LAST_FIVE_ARMS;
				else if(keyRead == 4) state = SET_TIME;
				else if(keyRead == 5) state = DISPLAY_MENU_ARMED_2;
				if(idle)
				{
					state = ARMED;
					idle = 0;
					idle_timer = 0;
				}
				//else state = MENU_ARMED;
			break;
				
			case DISPLAY_MENU_ARMED_2:
			rgb_blue();
			display_main_menu_two();
			state = MENU_ARMED_2;
			break;
			
			case MENU_ARMED_2:
			rgb_blue();
			if(keyRead == 6) state = DISPLAY_LOCK_SOLENOID;
			else if(keyRead == 7) state = ARMED;
			if(idle)
			{
				state = ARMED;
				idle = 0;
				idle_timer = 0;
			}
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
				if(new_code && code_position<4)
				{
					code[code_position] = keyRead;
					LCD_gotoXY((code_position*7),4);
					LCD_writeChar('*');
					code_position++;
				}
				if(keyRead == 12 && code_position==4)
				{
					state = CHECK_CODE_UN;
					code_position = 0;
				}
				if(idle)
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
				if(new_code && code_position<4)
				{
					code[code_position] = keyRead;
					LCD_gotoXY((code_position*7),4);
					LCD_writeChar('*');
					code_position++;
				}
				if(keyRead == 12 && code_position==4)
				{
					state = CHECK_CODE_AR;
					code_position = 0;
				}
				if(idle)
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
			
			case DISPLAY_READ_ALARM_CODE:
				//rgb_alarm();
				LCD_clear();
				display_get_armcode();
				state = READ_ALARM_CODE;
			break;
			
			case READ_ALARM_CODE:
				rgb_flash_check();
				//bell();
				if(new_code && code_position<4)
				{
					code[code_position] = keyRead;
					LCD_gotoXY((code_position*7),4);
					LCD_writeChar('*');
					code_position++;
				}
				if(keyRead == 12 && code_position == 4)
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
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				 && code[3] == master_code[3])
				{
					state = ARMED;
					rgb_flash_stop();
					bell_disable();
				}
				else
				{
					state = ALARM_SOUND;
				}
			break;
			//checks code[] input from read_menu_disarm, if it matches it returns to unarmed state, if it doesn't it stays in armed state
			case CHECK_CODE_UN:
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				&& code[3] == master_code[3])
				{
					state = UNARMED;
					saveArmDisarmTimeToEeprom(B_UNARMED);
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
					saveArmDisarmTimeToEeprom(B_ARMED);
				}
				else
				{
					state = UNARMED;
				}
			break;
			//entered from armed menu or unarmed menu, exited if any alarm is tripped or the push button is pressed to enter the menu again
			//if the idle flag is set 
			
			case DISPLAY_LAST_FIVE_ALARMS:
				display_last_five_alarms();
				state = LAST_FIVE_ALARMS;
			break;
			case LAST_FIVE_ALARMS:
				if(keyRead == 1) state = DISPLAY_ALARM_ONE;
				else if(keyRead == 2) state = DISPLAY_ALARM_TWO;
				else if(keyRead == 3) state = DISPLAY_ALARM_THREE;
				else if(keyRead == 4) state = DISPLAY_ALARM_FOUR;
				else if(keyRead == 5) state = DISPLAY_ALARM_FIVE;
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
				if(push_press && armed_state == 0) state = DISPLAY_MENU_UNARMED;
				if(push_press && armed_state == 1) state = DISPLAY_MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				else if(movement) state = ALARMED_MOTION; 
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
				//else state = LAST_FIVE_ALARMS;
			break;
			
			case DISPLAY_LAST_FIVE_ARMS:
				display_last_five_arm();
				state = LAST_FIVE_ARM;
			break;
			
			//entered from armed menu or unarmed menu, exited if any alarm is tripped or the push button is pressed to enter the menu again
			//if the idle flag is set 
			case LAST_FIVE_ARM:
				if(keyRead == 1) state = DISPLAY_ARM_ONE;
				else if(keyRead == 2) state = DISPLAY_ARM_TWO;
				else if(keyRead == 3) state = DISPLAY_ARM_THREE;
				else if(keyRead == 4) state = DISPLAY_ARM_FOUR;
				else if(keyRead == 5) state = DISPLAY_ARM_FIVE;
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
				if(push_press && armed_state == 0) state = DISPLAY_MENU_UNARMED;
				if(push_press && armed_state == 1) state = DISPLAY_MENU_ARMED;
				else if(int_temp > 43) state = ALARMED_FIRE;// 43 C is 110F TODO
				else if(movement) state = ALARMED_MOTION;
				else if(hall_door) state = ALARMED_HALL_D;
				else if(hall_window) state = ALARMED_HALL_W;
			break;
			
			case DISPLAY_ARM_ONE:
				getFiveArmDisarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<7;i++) LCD_writeChar(time_array[0][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<9;i++) LCD_writeChar(time_array[0][i+7]);
				LCD_gotoXY(0,2);
				for(i=0;i<5;i++) LCD_writeChar(time_array[0][i+16]);
				state = LAST_FIVE_ARM;
			break;
			
			case DISPLAY_ARM_TWO:
				getFiveArmDisarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<7;i++) LCD_writeChar(time_array[1][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<9;i++) LCD_writeChar(time_array[1][i+7]);
				LCD_gotoXY(0,2);
				for(i=0;i<5;i++) LCD_writeChar(time_array[1][i+16]);
				state = LAST_FIVE_ARM;
			break;
			
			case DISPLAY_ARM_THREE:
				getFiveArmDisarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<7;i++) LCD_writeChar(time_array[2][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<9;i++) LCD_writeChar(time_array[2][i+7]);
				LCD_gotoXY(0,2);
				for(i=0;i<5;i++) LCD_writeChar(time_array[2][i+16]);
				state = LAST_FIVE_ARM;
			break;
			
			case DISPLAY_ARM_FOUR:
				getFiveArmDisarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<7;i++) LCD_writeChar(time_array[3][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<9;i++) LCD_writeChar(time_array[3][i+7]);
				LCD_gotoXY(0,2);
				for(i=0;i<5;i++) LCD_writeChar(time_array[3][i+16]);
				state = LAST_FIVE_ARM;
			break;
			
			case DISPLAY_ARM_FIVE:
				getFiveArmDisarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<7;i++) LCD_writeChar(time_array[4][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<9;i++) LCD_writeChar(time_array[4][i+7]);
				LCD_gotoXY(0,2);
				for(i=0;i<5;i++) LCD_writeChar(time_array[4][i+16]);
				state = LAST_FIVE_ARM;
			break;
			
			case DISPLAY_ALARM_ONE:
				getFiveAlarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<9;i++) LCD_writeChar(time_array[0][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<8;i++) LCD_writeChar(time_array[0][i+9]);
				state = LAST_FIVE_ALARMS;
			break;
			
			case DISPLAY_ALARM_TWO:
				getFiveAlarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<9;i++) LCD_writeChar(time_array[1][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<8;i++) LCD_writeChar(time_array[1][i+9]);
				state = LAST_FIVE_ALARMS;
			break;
			
			case DISPLAY_ALARM_THREE:
				getFiveAlarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<9;i++) LCD_writeChar(time_array[2][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<8;i++) LCD_writeChar(time_array[2][i+9]);
				state = LAST_FIVE_ALARMS;
			break;
			
			case DISPLAY_ALARM_FOUR:
				getFiveAlarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<9;i++) LCD_writeChar(time_array[3][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<8;i++) LCD_writeChar(time_array[3][i+9]);
				state = LAST_FIVE_ALARMS;
			break;
			
			case DISPLAY_ALARM_FIVE:
				getFiveAlarmTimes(time_array);
				LCD_clear();
				LCD_gotoXY(0,0);
				for(i=0;i<9;i++) LCD_writeChar(time_array[4][i]);
				LCD_gotoXY(0,1);
				for(i=0;i<8;i++) LCD_writeChar(time_array[4][i+9]);
				state = LAST_FIVE_ALARMS;
			break;
			//this sets the time, Luke's job to write this state
			case SET_TIME:
				 if(setTimeStateMach(keyRead, push_press))
				 {
					 state = (armed_state) ? DISPLAY_MENU_ARMED : DISPLAY_MENU_UNARMED;
				 }
			break;
			
			case DISPLAY_LOCK_SOLENOID:
				LCD_clear();
				display_get_armcode();
				state = READ_LOCK_SOLENOID;
			break;
			
			case READ_LOCK_SOLENOID:
				if(new_code && code_position<4)
				{
					code[code_position] = keyRead;
					LCD_gotoXY((code_position*7),4);
					LCD_writeChar('*');
					code_position++;
				}
				if(keyRead == 12 && code_position == 4)
				{
					state = LOCK_SOLENOID;
					code_position = 0;
				}
				if(idle && armed_state == 0)
				{
					state = UNARMED;
					idle = 0;
					idle_timer = 0;
					code_position = 0;
				}
				else if(idle && armed_state == 1) state = ARMED;
			break;
			
			case LOCK_SOLENOID:
				if(code[0] == master_code[0] && code[1] == master_code[1] && code[2] == master_code[2]
				&& code[3] == master_code[3])
				{
					doorlockUnlock();
					
					if(armed_state == 0) state = UNARMED;
					else state = ARMED;
				}
				else
				{
					LCD_clear();
					LCD_writeString_F("Wrong Code");
					if(armed_state == 0) state = UNARMED;
					else state = ARMED;
				}
			break;
			//alarmed_motion is entered when the PIR is tripped, exits to alarm_sound after setting location
			case ALARMED_MOTION:
				location = MOTION;
				state = DISPLAY_ALARM_SOUND;
			break;
			//alarmed_hall_d is entered when the door hall effect sensor is tripped, exits to alarm_sound after setting location
			case ALARMED_HALL_D:
				location = DOOR;
				state = DISPLAY_ALARM_SOUND;
			break;
			//alarmed_hall_w is entered when the window hall effect sensor is tripped, exits to alarm_sound after setting location
			case ALARMED_HALL_W:
				location = WINDOW;
				state = DISPLAY_ALARM_SOUND;
			break;
			//alarmed_fire is entered when the fire alarm is tripped, it exits to alarm_sound after location is set for status function
			case ALARMED_FIRE:
				location = FIRE;
				state = DISPLAY_ALARM_SOUND;
			break;
			//alarm sound state sounds the bell and the flashing LED, it exits only upon push button press to read_alarm state
			case DISPLAY_ALARM_SOUND:
				display_status(B_ALARM, location);
				rgb_flash_start();
				bell_enable();
				state = ALARM_SOUND;
			break;
			
			case ALARM_SOUND:
				idle_timer = 0; //keep idle timer at 0 if not in a menu
				rgb_flash_check();
				if(push_press) state = DISPLAY_READ_ALARM_CODE;
			break;
			
		}
		
	}
	
}

ISR(TIMER1_COMPA_vect)
{	
	press_flag_count++;
	bell_InterruptFunction();
	if(press_flag_count >= 3)
	{
		press_flag_count = 0;
		press_flag = 1;
	}
}


ISR(TIMER2_OVF_vect)
{
	timer = timer + 1;
	idle_timer = idle_timer + 1;
	rgb_flash_32msInterrupt();
	doorlockInterruptFuction();
	//once the timer counts up to 12 32 ms interrupts it scrolls the text once (400msec)
	if(timer > 14)
	{
		next_scroll = next_scroll + 1;
		timer = 0;
		ambient = 1;
	}
	if(idle_timer > TWENTY_SEC)
	{
		idle = 1;
		idle_timer = 0;
	}
}
