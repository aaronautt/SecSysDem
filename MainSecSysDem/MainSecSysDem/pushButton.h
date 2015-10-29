/*
 * pushButton.h
 *
 * Created: 10/28/2015 8:15:10 PM
 *  Author: Luke
 */ 


#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

// Initializes the push button by setting it as an input and setting the
// internal pullup resistors.
void pushButton_init();

// Returns a 1 is the button is pressed
uint8_t pushButtonRead();

#endif /* PUSHBUTTON_H_ */