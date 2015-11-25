/*
 * bell.h
 *
 * Created: 10/28/2015 7:50:26 PM
 *  Author: Luke
 */ 


#ifndef BELL_H_
#define BELL_H_

// Initiates the audible sounding bell by turning it off
void bell_init();

// Turns on the audible sounding bell
//void bell_on();

// Turns off the audible sounding bell
//void bell_off();

// Enable the bell
void bell_enable();
// Disable the bell
void bell_disable();
// This function actually turns on and off the bell at the proper intervals.
// It must be placed in the while loop.
void bell_UpdateStatus();
// This function is supposed to be in the 32ms interrupt ISR
void bell_InterruptFunction();

#endif /* BELL_H_ */