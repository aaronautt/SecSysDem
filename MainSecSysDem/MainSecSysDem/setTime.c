/*
 * setTime.c
 *
 * Created: 11/25/2015 12:14:57 PM
 *  Author: Luke
 */ 

#include <avr/io.h>
#include "LCD_spi.h"
#include "rtcDriver.h"

enum dateState{
	TO_YEAR,
	SET_YEAR,
	TO_MONTH,
	SET_MONTH,
	TO_DAY,
	SET_DAY,
	TO_DAY_OF_WEEK,
	SET_DAY_OF_WEEK,
	TO_HOUR,
	SET_HOUR,
	TO_MIN,
	SET_MIN,
	TO_SEC,
	SET_SEC,
	SAVE,
	TO_EXIT,
	EXIT
};

static uint8_t setTimeState = TO_YEAR;
static uint8_t numEntered[2] = {0}, numPointer=0;
static uint8_t keyRead = 0;
static uint8_t pushBtn = 0;

void setupNextState(char command[13], char format[13])
{			
	LCD_clear();
	LCD_writeString_F(command);
	LCD_gotoXY(0,1);
	LCD_writeString_F(format);
	LCD_gotoXY(0,2);
	LCD_writeString_F("# - Enter   ");
	LCD_gotoXY(0,3);
	LCD_writeString_F("* - Clear   ");
	numPointer=0;
}

uint8_t getAndPrintTime(uint8_t *memoryP)
{			
	if(keyRead == 10) //*
	{
		LCD_clear_row(4);
		numPointer = 0;
	}
	else if(keyRead == 12) //#
	{
		if(numPointer == 2)
		{
			*memoryP = (numEntered[0]*10+numEntered[1]);
			//setTimeState = nextState;
			return(1);
		}
	}
	else if(keyRead && numPointer<2)
	{
		// Determine the pressed key
		if(keyRead==11) numEntered[numPointer] = 0;
		else numEntered[numPointer] = keyRead;
		// print the number
		LCD_gotoXY((numPointer*7),4);
		// Convert the number to ascii and write it.
		LCD_writeChar((numEntered[numPointer] + 0x030));
		numPointer++;
	}
	
	return(0);
}

uint8_t setTimeStateMach(uint8_t keyReadPassed, uint8_t pushBtnPassed)
{
	static uint8_t hour, min, sec, date, dayOfWk=0, month, year;
	
	keyRead = keyReadPassed;
	pushBtn = pushBtnPassed;
	
	if(pushBtn)
	{
		setTimeState = EXIT;
	}
	
	
	switch(setTimeState)
	{
		case TO_YEAR:
			setupNextState("ENTER YEAR  ","(DD/MM/YY)  ");
			setTimeState = SET_YEAR;
			break;
			
		case SET_YEAR:
			if(getAndPrintTime(&year)) setTimeState = TO_MONTH;
			break;
			
		case TO_MONTH:
			setupNextState("ENTER MONTH ","(DD/MM/YY)  ");
 			setTimeState = SET_MONTH;
			break;
			
		case SET_MONTH:
			if(getAndPrintTime(&month)) setTimeState = TO_DAY;	
			break;
			
		case TO_DAY:
			setupNextState("ENTER DATE  ","(DD/MM/YY)  ");
			setTimeState = SET_DAY;
			break;
			
		case SET_DAY:
			if(getAndPrintTime(&date)) setTimeState = TO_DAY_OF_WEEK;
			break;
			
		case TO_DAY_OF_WEEK:
			LCD_clear();
			LCD_gotoXY(0,0);
			LCD_writeString_F("ENTER DAY OF");
			LCD_gotoXY(0,1);
			LCD_writeString_F("WEEK USING  ");
			LCD_gotoXY(0,2);
			LCD_writeString_F("NUMBERS 1-7 ");
			LCD_gotoXY(0,3);
			LCD_writeString_F("Ex. 1=Sunday");
			numPointer=0;
			dayOfWk = 0;
			setTimeState = SET_DAY_OF_WEEK;
			break;
			
		case SET_DAY_OF_WEEK:
			//if(getAndPrintTime(&date)) setTimeState = TO_DAY_OF_WEEK;
			if(keyRead >= 1 && keyRead <= 7)
			{
				dayOfWk = keyRead;
				LCD_clear_row(4);
				LCD_gotoXY(0,4);
				switch (keyRead)
				{
					case 1:
						LCD_writeString_F("Sunday      ");
						break;
					case 2:
						LCD_writeString_F("Monday      ");
						break;
					case 3:
						LCD_writeString_F("Tuesday     ");
						break;
					case 4:
						LCD_writeString_F("Wednesday   ");
						break;
					case 5:
						LCD_writeString_F("Thursday    ");
						break;
					case 6:
						LCD_writeString_F("Friday      ");
						break;
					case 7:
						LCD_writeString_F("Saturday    ");
						break;
					default:
						dayOfWk = 0;
						break;
				}
			}
			if(keyRead == 12 && dayOfWk)
			{
				setTimeState = TO_HOUR;
			}
			break;
			
		case TO_HOUR:
			setupNextState("ENTER HOUR  ","(hh:mm:ss)  ");
			setTimeState = SET_HOUR;
			break;
			
		case SET_HOUR:
			if(getAndPrintTime(&hour)) setTimeState = TO_MIN;
			break;
			
		case TO_MIN:
			setupNextState("ENTER MINUTE","(hh:mm:ss)  ");
			setTimeState = SET_MIN;
			break;
			
		case SET_MIN:
			if(getAndPrintTime(&min)) setTimeState = TO_SEC;
			break;
			
		case TO_SEC:
			setupNextState("ENTER SECOND","(hh:mm:ss)  ");
			setTimeState = SET_SEC;
			break;
			
		case SET_SEC:
			if(getAndPrintTime(&sec)) setTimeState = SAVE;
			break;
		case SAVE:
			LCD_clear();
			setTimeStamp(hour, min, sec, date, dayOfWk, month, year);
			LCD_writeString_F("Time Saved! ");
			LCD_gotoXY(0,2);
			LCD_writeString_F("Press button");
			LCD_gotoXY(0,3);
			LCD_writeString_F("to exit.");
			setTimeState = TO_EXIT;
			break;
		case TO_EXIT:
			break;
		case EXIT:
		default:
			LCD_clear();
			setTimeState = TO_YEAR;
			numPointer = 0;
			return(1);
			break;
	}
	return(0);
}