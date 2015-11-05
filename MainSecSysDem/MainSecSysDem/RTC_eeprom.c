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
#include "RTC_eeprom.h"

/*   !!!!IMPORTANT!!!!

	The EEPROM needs several milliseconds to write data internally before it can be accessed
	make sure to add a delay after writing a sound
*/


/*********************************************

	This function writes a single sound file to eeprom
	
	sound = an array containing the sound file
	address = the beginning byte address where the file is to be stored
	num_bytes = length of the array to be stored
	
***********************************************/


void Write_Sound(uint8_t sound[], uint16_t address, uint16_t num_bytes)
{
	I2C_DoubleWriteRegister_S(RTC_EEP_ADD, address, num_bytes, sound);
}


/*********************************************

	This function reads a single sound file from eeprom
	
	sound = an array containing the sound file
	address = the beginning byte address where the file is stored
	num_bytes = length of the array to be read
	
***********************************************/


void Read_Sound(uint8_t sound[], uint16_t address, uint16_t num_bytes)
{
	I2C_DoubleReadRegister_S(RTC_EEP_ADD, address, num_bytes, sound);
}




