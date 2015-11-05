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

void getFiveAlarmTimes(char timeStamps[5][20]);
void saveTimeToEeprom();

#endif /* RTCDRIVER_H_ */