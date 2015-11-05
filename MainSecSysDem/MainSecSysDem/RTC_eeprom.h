/*
 * RTC_eeprom.h
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: AARON MASTER OF THE LUNIVERSE
 */ 


#ifndef RTC_eeprom_H_
#define RTC_eeprom_H_#define RTC_EEP_ADD					0b10100000

//these address values need to contain the beginning point of each sound file, add more as needed

#define RTC_EEP_ADD_ONE				0
#define RTC_EEP_ADD_TWO				20
#define RTC_EEP_ADD_THREE			40
#define RTC_EEP_ADD_FOUR			60
#define RTC_EEP_ADD_FIVE			80

// Function Prototypes

void Write_Sound(uint8_t sound[], uint16_t address, uint16_t num_bytes);
void Read_Sound(uint8_t sound[], uint16_t address, uint16_t num_bytes);

#endif /* RTC_eeprom_H_ */