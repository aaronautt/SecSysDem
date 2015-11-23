/*
 * rgbLed.h
 *
 * Created: 10/28/2015 7:28:44 PM
 *  Author: Luke
 */ 


#ifndef RGBLED_H_
#define RGBLED_H_

// Initializes the rgb ddr and turns it off
void rgb_init();

// Turns on the RGB led to red
void rgb_red();

// Turns on the RGB led to red
void rgb_green();

// Turns on the RGB led to red
void rgb_blue();

// Turns off the RGB led
void rgb_off();


// ----- FLASHING RED LED FUNCTIONS -----
// This function is called to start the LED flashing
//		note: rgb_init() must be called first
void rgb_flash_start();
// This function is called to stop the LED flashing
void rgb_flash_stop();
// This function must be called in the main while loop. This is the 
// function that actually turns the LED on or off.
void rgb_flash_check();
// This function must be called in the interrupt that is triggered
// every 32ms. This function simply increments a variable.
void rgb_flash_32msInterrupt();

#endif /* RGBLED_H_ */