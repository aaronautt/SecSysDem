/*****************************

lcd_moving.c

Author: Aaron crump

These functions will control lcd output and backlight output

*****************************/


#include <inttypes.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "secSysDefines.h"
#include "LCD_spi.h"
#include "lcd_moving.h"
#include <util/delay.h>


/************************

	sets up timer for PWM with a frequency of 200Hz
	
*************************/

// void LCD_light_init(void)
// {
// 	TCNT1 = 0;
// 	TCCR1A |= (1<<COM1B1) | (1<<WGM11) | (1<<WGM10);
// 	TCCR1B |= (1<<CS12) | (1<<WGM13); //sets prescaler to 256
// 	OCR1A = 156/2;//sets period to 2*OCR1A 0.005 seconds, 200Hz
// 	OCR1B = 78; //sets duty cycle to 50% initially
// }


// Note the brightness is set in the doorlock.c file because of convenience
// with timer 2 being used by the doorlock.
/************************************

this function changes the PWM for the LCD backlight 

	brightness : a value between 1 and 10

************************************/

// void Lcd_Light_bright(uint8_t brigtness)
// {
// 				
// }

/**********************************

	scrolling text
	This function scrolls an array or something
	
***********************************/



void Scrolling_Text(char input[])
{
	int i, j, k, length;
	char message[100], swap;
	sprintf(message, "%s              ",&input[0]);
	//if(strlen(message) < 14) strcat(message, endings);
	length = strlen(message)-1;
	//printf("%d\n",length);
	for(i=0;i<19;i++)
	{
		swap = message[0];
		for(k=0;k < length;k++)
		{
			message[k] = message[k+1];
		}
		message[length] = swap;
		//message[length] = '/0';
		LCD_gotoXY(0,2);
		for(j=0;j<12;j++)
		{
			LCD_writeChar(message[j]);
			//_delay_ms(5);
			//printf("%c", message[j]);
		}
		_delay_ms(500);
		LCD_clear();
	}
}


/**********************************

	scrolling text single
	this fuction scrolls the array one character to the left
	scrolls one position to the left starting with the last shifted character
	
	position : how many shifts have already taken place, if position = 1 the array will be shifted once
	
***********************************/

void Scrolling_Text_single(char input[], uint8_t position)
{
	int i, j, k, length;
	char message[25], swap;
	sprintf(message, "%s              ",&input[0]);
	//if(strlen(message) < 14) strcat(message, endings);
	length = strlen(message)-1;
	//printf("%d\n",length);
	for(i=0;i<position;i++)
	{
		swap = message[0];
		for(k=0;k < length;k++)
		{
			message[k] = message[k+1];
		}
		message[length] = swap;
	}
		LCD_gotoXY(0,0);
		for(j=0;j<12;j++)
		{
			LCD_writeChar(message[j]);
		}
		_delay_ms(500);
		LCD_clear();
}

void display_temp(uint8_t temp)
{
	int j;
	char message[100];
	sprintf(message, "Temp : %u", temp);
	LCD_gotoXY(0,1);
	for(j=0;j<12;j++)
	{
		LCD_writeChar(message[j]);
	}
}


/*****************************************

	This function just shifts an array, it's unused but I didn't want to delete it.
	
*****************************************/
void array_shift(char message[])
{
	int j, k, length;
	char swap;
	length = strlen(message)-1;
	swap = message[0];
	for(k=0;k < length;k++)
	{
		message[k] = message[k+1];
	}
	message[length] = swap;	
}



