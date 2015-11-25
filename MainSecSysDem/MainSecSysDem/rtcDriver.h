/*
 * rtcDriver.h
 *
 * Created: 11/4/2015 4:21:59 PM
 *  Author: Luke
 */ 


#ifndef RTCDRIVER_H_
#define RTCDRIVER_H_

void getTimeStampChar(char hour[], char min[], char sec[],
char date[], char dayOfWk[], char month[], char year[]);
void getTimeStampInt(uint8_t *hour, uint8_t *min, uint8_t *sec,
uint8_t *date, uint8_t *dayOfWk, uint8_t *month, uint8_t *year);
void setTimeStamp(uint8_t hour, uint8_t min, uint8_t sec,
uint8_t date, uint8_t dayOfWk, uint8_t month, uint8_t year);
void SetTimeDate();
void getStandardTimeStampStr(char timeStamp[]);
void getTemp(uint8_t *intTemp,uint8_t *decTemp);
void getAndSetTimeStamp();

//------------- getFiveAlarmTimes --------------
// Example code:
//		char times[5][20];
//		//Getting the last 5 times that were saved to the EEPROM
//		getFiveAlarmTimes(&times[0]);
void getFiveAlarmTimes(char timeStamps[5][20]);
void getFiveArmDisarmTimes(char timeStamps[5][20]);


//------------- saveTimeToEeprom ---------------
// Example code:
//		// Saving the current time to the oldest time stamp spot in EEPROM
//		saveAlarmTimeToEeeprom();
void saveAlarmTimeToEeprom();
void saveArmDisarmTimeToEeprom();


#endif /* RTCDRIVER_H_ */