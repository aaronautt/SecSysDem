/***************************

	photo sensor.h
	
	author: Aaron Crump
	Date: 11/28/2015
	
****************************/

#ifndef photo_sensor_H_
#define photo_sensor_H_


void photo_sensor_init();
uint8_t check_photo_sensor();
uint8_t convert_adc_to_DC();


#endif /*photo_sensor_H_*/ 