/*/*
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
#define KEYPAD_1_PIN			_BV(7)
#define KEYPAD_2_PIN			_BV(6)
#define KEYPAD_3_PIN			_BV(5)
#define KEYPAD_4_PIN			_BV(4)
#define KEYPAD_5_PIN			_BV(3)
#define KEYPAD_6_PIN			_BV(2)
#define KEYPAD_7_PIN			_BV(1)

// ----- Proximity Sensor -----
#define PIR_DDR       DDRC
#define PIR_PORT_IN   PINC
#define PIR_PIN       _BV(3)

// ----- Hall Effect Sensors -----
// Window Hall Effect
#define HALL_EFF_WIN_PORT_OUT   PORTB
#define HALL_EFF_WIN_PORT_IN    PINB
#define HALL_EFF_WIN_DDR        DDRB
#define HALL_EFF_WIN_PIN        _BV(0)
// Door Hall Effect
#define HALL_EFF_DOOR_PORT_OUT  PORTB
#define HALL_EFF_DOOR_PORT_IN   PINB
#define HALL_EFF_DOOR_DDR       DDRB
#define HALL_EFF_DOOR_PIN       _BV(1)

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
#define LCD_BACKLIGHT_DDR		DDRD
#define LCD_BACKLIGHT_PORT_OUT  PORTD
#define LCD_BACKLIGHT_PIN       _BV(6)
#define LCD_BACKLIGHT_OCR		OCR0A

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
#define DOORLOCK_OCR			OCR0B

// ----- Bell Alarm -----
#define BELL_ALARM_PIN_IOEX		_BV(0)

//RTC nonsense//
#define RTC_EEP_ADDR			0b10101110 // EEPROM address on RTC Module
#define RTC_ADDR				0b11010000 // I2C bus address of DS1307 RTC

// IO Expander
#define IOEXPANDER_ADDR			0b01000000


//status/state defines
enum state
{
	UNARMED,				//1
	ARMED,					//3
	READ_MENU_ARM,			//4 
	READ_ALARM_CODE,		//5
	LAST_FIVE_ALARMS,		//6
	SET_TIME,				//7
	ALARMED_MOTION,			//8
	ALARMED_FIRE,			//9
	MENU_UNARMED,			//10
	MENU_ARMED,				//11
	READ_MENU_DISARM,		//12
	ALARMED_HALL_D,			//13
	ALARMED_HALL_W,			//14
	CHECK_CODE_AL,			//15
	CHECK_CODE_AR,			//16
	CHECK_CODE_UN,			//17
	ALARM_SOUND,			//18
	LAST_FIVE_ARM,			//19
	DISPLAY_MENU_ARMED,		//20
	DISPLAY_MENU_UNARMED,	//21
	DISPLAY_READ_MENU_DISARM, //22
	DISPLAY_READ_MENU_ARM,	//23
	DISPLAY_LAST_FIVE_ALARMS,
	DISPLAY_LAST_FIVE_ARMS,
	DISPLAY_ARM_ONE,
	DISPLAY_ARM_TWO,
	DISPLAY_ARM_THREE,
	DISPLAY_ARM_FOUR,
	DISPLAY_ARM_FIVE,
	DISPLAY_ALARM_ONE,
	DISPLAY_ALARM_TWO,
	DISPLAY_ALARM_THREE,
	DISPLAY_ALARM_FOUR,
	DISPLAY_ALARM_FIVE,
	DISPLAY_READ_ALARM_CODE,
	DISPLAY_ALARM_SOUND,
	DISPLAY_MENU_ARMED_2,
	DISPLAY_MENU_UNARMED_2,
	MENU_UNARMED_2,
	MENU_ARMED_2,
	DISPLAY_LOCK_SOLENOID,
	LOCK_SOLENOID,
	READ_LOCK_SOLENOID,
	CHECK_CODE_AR_HOLD
	};

//location of alarm defines
#define DOOR					1
#define WINDOW					2
#define MOTION					3
#define FIRE					4

#define B_ARMED					1
#define B_UNARMED				0
#define B_ALARM					2

//dac
#define DAC_PORT_OUT			PORTB
#define DAC_CLK					_BV(5)
#define DAC_IN					_BV(3)
#define DAC_CS					_BV(2)

//times
#define MINUTE					1875
#define TWENTY_SEC				625
#define NONE					1

#endif /* SECSYSDEFINES_H_ */
