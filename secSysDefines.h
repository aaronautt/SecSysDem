/*
 * secSysDefines.h
 *
 * Created: 10/18/2015 10:16:40 PM
 *  Author: Luke
 */ 


#ifndef SECSYSDEFINES_H_
#define SECSYSDEFINES_H_


#define F_CPU	8000000L

// ----- Keypad -----
// Keypad Column Pins
#define KEYPAD_COL_PORT_OUT   PORTC
#define KEYPAD_COL_PORT_IN    PINC
#define KEYPAD_COL_DDR        DDRC
#define KEYPAD_COL_1          _BV(0)
#define KEYPAD_COL_2          _BV(1)
#define KEYPAD_COL_3          _BV(2)
// Keypad Row Pins
#define KEYPAD_ROW_PORT_OUT   PORTD
#define KEYPAD_ROW_PORT_IN    PIND
#define KEYPAD_ROW_DDR        DDRD
#define KEYPAD_ROW_1          _BV(5)
#define KEYPAD_ROW_2          _BV(4)
#define KEYPAD_ROW_3          _BV(3)
#define KEYPAD_ROW_4          _BV(2)

// ----- Proximity Sensor -----
#define PIR_DDR       DDRC
#define PIR_PORT_IN   PINC
#define PIR_PIN       _BV(3)
#define PIR_READ      ((PIR_PORT_IN & PIR_PIN) >> PIR_PIN)

// ----- Hall Effect Sensors -----



#endif /* SECSYSDEFINES_H_ */
