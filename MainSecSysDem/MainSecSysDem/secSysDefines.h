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
// Keypad Pins on the IO Expander
#define KEYPAD_1_PIN			_BV(1)
#define KEYPAD_2_PIN			_BV(2)
#define KEYPAD_3_PIN			_BV(3)
#define KEYPAD_4_PIN			_BV(4)
#define KEYPAD_5_PIN			_BV(5)
#define KEYPAD_6_PIN			_BV(6)
#define KEYPAD_7_PIN			_BV(7)

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
// LCD Data/Command
#define LCD_DC_DDR				DDRD
#define LCD_DC_PORT_OUT			PORTD
#define LCD_DC_PIN_IO			_BV(2)
// LCD Reset
#define LCD_RST_DDR				DDRD
#define LCD_RST_PORT_OUT		PORTD
#define LCD_RST_PIN_IO			_BV(3)
// LCD Slave Select
#define LCD_SS_DDR				DDRD
#define LCD_SS_PORT_OUT			PORTD
#define LCD_SS_PIN				_BV(4)
// LCD Backlight
#define LCD_DC_DDR				DDRD
#define LCD_BACKLIGHT_PORT_OUT  PORTD
#define LCD_BACKLIGHT_PIN       _BV(6)

// ----- PUSH BUTTON -----
#define PUSH_BTN_DDR			DDRD
#define PUSH_BTN_PORT_IN        PIND
#define PUSH_BTN_PORT_OUT		PORTD
#define PUSH_BTN_PIN            _BV(7)

// ----- RGB LED -----
#define RGB_DDR					DDRC
#define RGB_PORT_OUT			PORTC
#define RGB_RED_PIN				_BV(0)
#define RGB_GRN_PIN				_BV(1)
#define RGB_BLU_PIN				_BV(2)

// ----- Doorlocking Solenoid -----
#define DOORLOCK_PORT			PORTD
#define DOORLOCK_DDR			DDRD
#define DOORLOCK_PIN			_BV(5)

// ----- Bell Alarm -----
#define BELL_ALARM_PIN_IOEX		_BV(0)

//---RTC EEPROM---
#define RTC_EEP_ADD					0b10101110

// IO Expander
#define IOEXPANDER_ADDR	0b01000000

#endif /* SECSYSDEFINES_H_ */
