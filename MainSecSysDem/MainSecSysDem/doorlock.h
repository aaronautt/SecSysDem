/*
 * doorlock.h
 *
 * Created: 11/22/2015 3:33:18 PM
 *  Author: Luke
 */ 


#ifndef DOORLOCK_H_
#define DOORLOCK_H_

void doorlockAndLcdBacklight_init();
void LcdBacklightBrightness(uint8_t dutyCycle);
void doorlockUnlock();
void doorlockLock();

// This function will automatically reduce the duty cycle of the solenoid
// to save power after at least 32ms have passed since the solenoid has
// been active. This function MUST BE CALLED IN THE ISR ROUTINE THAT OCCURS
// EVERY 32ms. 
void doorlockInterruptFuction();



#endif /* DOORLOCK_H_ */