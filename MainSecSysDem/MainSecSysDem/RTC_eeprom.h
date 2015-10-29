/*
 * RTC_eeprom.h
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: AARON MASTER OF THE LUNIVERSE
 */ 


#ifndef RTC_eeprom_H_
#define RTC_eeprom_H_

// Function Prototypes

uint16_t RTC_Get_Place_In_Line();
void RTC_Update_Place_In_Line(uint8_t length, uint16_t place);
void RTC_Write_Alarm_Time(uint8_t date[], uint16_t last_place);
void RTC_Write_Action_Time(uint8_t date[], uint16_t last_place, char action);
void RTC_Last_Five(char Last_five[]);
void RTC_Last_Five(char Last_five[]);

#endif /* RTC_eeprom_H_ */