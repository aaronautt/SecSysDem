/***************************

	photo sensor.c
	
	author: Aaron Crump
	Date: 11/28/2015
	
****************************/

#include <avr/io.h>

//max of 250, min of 0

void photo_sensor_init()
{
	//ADC=Vin*1024/Vref
	//ADMUX sets Vref to VCC(3.3), ADLAR sets it to 8 bit accuracy instead of 10, read ADCH for result,
	//MUX set to read ADC channel 6
	ADMUX |= _BV(REFS0) | _BV(ADLAR) | _BV(MUX2) | _BV(MUX1);
	//enables ADC, starts the first conversion and enables auto triggering 
	ADCSRA |= _BV(ADEN) | _BV(ADSC);	
}
//reads the photosensor, gets a value from 5 to 240, may vary with real circuit
uint8_t check_photo_sensor()
{
	//starts single conversion
	ADCSRA |= _BV(ADSC);
	while(ADCSRA & _BV(ADSC));// waits til the conversion is complete
	return ADCH;
}
//converts the adc reading to a duty cycle from 0 to 100
uint8_t convert_adc_to_DC()
{
	uint16_t adc_read = 0, DC_out = 0;
	uint8_t output;
	adc_read = check_photo_sensor();
	DC_out = 250-adc_read;
	output = DC_out*100/250;
	return output;
}

