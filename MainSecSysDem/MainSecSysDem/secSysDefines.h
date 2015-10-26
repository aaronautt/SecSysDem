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

// ----- Hall Effect Sensors -----
// Window Hall Effect
#define HALL_EFF_WIN_PORT_OUT   PORTB
#define HALL_EFF_WIN_PORT_IN    PINB
#define HALL_EFF_WIN_DDR        DDRB
#define HALL_EFF_WIN_PIN        PINB0
// Door Hall Effect
#define HALL_EFF_DOOR_PORT_OUT  PORTB
#define HALL_EFF_DOOR_PORT_IN   PINB
#define HALL_EFF_DOOR_DDR       DDRB
#define HALL_EFF_DOOR_PIN       PINB1

// ----- LCD ------
// LCD Backlight
#define LCD_BACKLIGHT_PORT_OUT  PORTD
#define LCD_BACKLIGHT_PIN       _BV(6)
// LCD Data/Command
#define LCD_DC_PIN_IOEX         _BV(3)
// LCD Reset
#define LCD_RST_PIN_IOEX        _BV(4)
// LCD Slave Select
#define LCD_SS_PIN_IOEX         _BV(5)

// ----- PUSH BUTTON -----
#define PUSH_BTN_PORT_OUT       PORTD
#define PUSH_BTN_PORT_IN        PIND
#define PUSH_BTN_PIN            _BV(7)

// ----- RGB LED -----
#define RGB_RED_IOEX                 _BV(0)
#define RGB_GRN_IOEX                 _BV(1)
#define RGB_BLU_IOEX                 _BV(2)

// ----- Doorlocking Solenoid -----
#define DOORLOCK_PIN_IOEX            _BV(6)

// ----- Bell Alarm -----
#define BELL_ALARM_PIN_IOEX          _BV(7)

#endif /* SECSYSDEFINES_H_ */
