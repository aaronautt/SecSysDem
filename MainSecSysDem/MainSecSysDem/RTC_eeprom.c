/*
 * RTC_eeprom.c
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: AARON MASTER OF THE LUNIVERSE
 */ 

#include "secSysDefines.h"
#include "i2c_driver.h"
#include <util/delay.h>
#include <string.h>
#include <stdio.h>


/* *********************************
	This function gets the current position in the eeprom that is the next available storage position
	
	
*********************************** */

uint16_t RTC_Get_Place_In_Line()
{
	//register position 0 and 1 are reserved for RTC eeprom current address 0 is the low byte, 1 is high byte
	uint8_t Hi, Lo;
	uint16_t place = 0;
	Lo = I2C_DoubleReadRegister(RTC_EEP_ADD, 0);
	Hi = I2C_DoubleReadRegister(RTC_EEP_ADD, 1);
	if(Lo<3) place = 2;
	else place = (Hi<<8)+Lo;
	return place;
}


/* *********************************
	This function updates the position in the eeprom that is the next available storage position
	
	length = total length of the string to be stored
	place = current register address of the first available memory block, use get place in line function
*********************************** */

void RTC_Update_Place_In_Line(uint8_t length, uint16_t place)
{
	//register position 0 and 1 are reserved for RTC eeprom current address 0 is the low byte, 1 is high byte
	uint8_t Hi, Lo;
	place = place + length + 1;//increases the position of the newest available empty storage block
	Hi = (place>>8)&0x00ff;//cuts the 16 bit register address in half for storage
	Lo = place&0x00ff;
	I2C_DoubleWriteRegister(RTC_EEP_ADD, 0, Lo);
	I2C_DoubleWriteRegister(RTC_EEP_ADD, 1, Hi);
}

/**********************************************

	This function writes the time of the alarm action in the RTC's eeprom
	
	date = the time of the action
	last_place = the first open memory block in the eeprom, found by calling RTC_Update_Place_In_Line()
	
***********************************************/

void RTC_Write_Alarm_Time(uint8_t date[], uint16_t last_place)
{
	int i;
	uint8_t Alarm_Time[50] = "alarm ";
	strcat((char*)&Alarm_Time, (char*)date);
	//printf("%s,  %s\n", Alarm_Time, date); //remove after debugging
	//printf("%s\n", Alarm_Time);
	for(i=0;i<strlen((char*)&Alarm_Time)+1;i++)
	{
		I2C_DoubleWriteRegister(RTC_EEP_ADD, i+last_place, Alarm_Time[i]);
	}
	RTC_Update_Place_In_Line(strlen((char*)Alarm_Time), last_place);
}

/**********************************************

	This function writes the time of the last arm or disarm action in the RTC's eeprom
	
	date = the time of the action
	last_place = the first open memory block in the eeprom, found by calling RTC_Update_Place_In_Line()
	action = the action taken, the options are arm = 1 or disarm = 0
	
***********************************************/

void RTC_Write_Action_Time(uint8_t date[], uint16_t last_place, uint8_t action)
{
	int i;
	uint8_t Action_Time[50];
	//changes the nature of the string stored depending on whether or not the system is being armed
	if(action == 1) 
	{
		strcpy((char*)&Action_Time, "armed ");
		strcat((char*)&Action_Time, (char*)date);
	}
	else if(action == 0)
	{
		strcpy((char*)&Action_Time, "disarmed ");
		strcat((char*)&Action_Time, (char*)date);
	}
	printf("%s\n", Action_Time);
	for(i=0;i<strlen((char*)&Action_Time)+1;i++)
	{
		I2C_DoubleWriteRegister(RTC_EEP_ADD, i+last_place, Action_Time[i]);
	}
	RTC_Update_Place_In_Line(strlen((char*)Action_Time), last_place);	
}


/*should print out everything in the RTC eeprom*/

void RTC_Last_Five(char Last_five[])
{
	int i;
	uint16_t last = RTC_Get_Place_In_Line();
	printf("place = %u\n", last);
	//I2C_DoubleReadRegister_S(RTC_EEP_ADD, 2, last, &Last_five[0]);
	for(i=0;i<last-2;i++)
	{
		Last_five[i]=(char)I2C_DoubleReadRegister(RTC_EEP_ADD, 2+i);
	}
	printf("%s\n", &Last_five[0]);
}

void readsomeshit(char out[])
{
	int i;
	for(i=0;i<57;i++)
	{
		out[i]=(char)I2C_DoubleReadRegister(RTC_EEP_ADD, i);
		printf("%s\n",out[i]);
	}
	
}