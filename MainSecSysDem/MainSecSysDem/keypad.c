/*
 * keypad.c
 *
 * Created: 10/27/2015 2:46:09 PM
 *  Author: Luke
 */ 

#include "ioExpander.h"
#include "secSysDefines.h"
#include "stdio.h"

#define ROW_PINS	(KEYPAD_4_PIN | KEYPAD_5_PIN | KEYPAD_6_PIN | KEYPAD_7_PIN)
#define COL_PINS	(KEYPAD_1_PIN | KEYPAD_2_PIN |KEYPAD_3_PIN)	
#define COL_1		KEYPAD_3_PIN
#define COL_2		KEYPAD_2_PIN
#define COL_3		KEYPAD_1_PIN
#define ROW_1		KEYPAD_7_PIN
#define ROW_2		KEYPAD_6_PIN
#define ROW_3		KEYPAD_5_PIN
#define ROW_4		KEYPAD_4_PIN

void keypadReset()
{
	uint8_t regData = ioExpReadReg();
	
	regData |= ROW_PINS;
	regData &= ~COL_PINS;
	
	ioExpWriteReg(regData);
}


// Keypad button numbers that are returned from this function
// Returns 0 if no button is pressed
//////////////////
//	1	2	3	//
//	4	5	6	//
//	7	8	9	//
//	10	11	12	//
//////////////////
uint8_t keypadReadPins()
{
	uint8_t regData;
	uint8_t col;
	keypadReset();
	
	regData = ioExpReadReg();
	
	// If a keypad button is pressed...
	if((regData & ROW_PINS) != ROW_PINS)
	{
		for(col=0; col<3; col++)
		{
			// Set the appropriate column pin low
			switch(col)
			{
				case 0:{
					ioExpSetBit(COL_2);
					ioExpSetBit(COL_3);
					ioExpClrBit(COL_1);
					break;				
				}
				case 1:{
					ioExpSetBit(COL_1);
					ioExpClrBit(COL_2);
					//ioExpWriteReg(~COL_2);
					break;
				}
				case 2:{
					ioExpSetBit(COL_2);
					ioExpClrBit(COL_3);
					//ioExpWriteReg(~COL_3);
					break;
				}
			}
			
			// Read the need pin values with the updated columns
			regData = ioExpReadReg();
			
			// If any of the row pins are not high... find which row is low
			if((ROW_PINS & regData) != ROW_PINS)
			{
				// If row 1 is low...
				if(!(ROW_1 & regData))
				{
					return(col + 1);
				}
				// If row 2 is low...
				else if(!(ROW_2 & regData))
				{
					return(col + 4);
				}
				// If row 3 is low...
				else if(!(ROW_3 & regData))
				{
					return(col + 7);
				}
				// If row 4 is low...
				else if(!(ROW_4 & regData))
				{
					return(col + 10);
				}
			}			
		}
	}
	return(0);
}

