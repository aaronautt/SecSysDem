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
void getFiveAlarmTimes(char timeStamps[5][20]);
void saveTimeToEeprom();

#endif /* RTCDRIVER_H_ */