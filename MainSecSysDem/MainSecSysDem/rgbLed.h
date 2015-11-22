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

void rgb_flash_start();

void rgb_flash_stop();

void rgb_flash_check();

void rgb_flash_32msInterrupt();

#endif /* RGBLED_H_ */