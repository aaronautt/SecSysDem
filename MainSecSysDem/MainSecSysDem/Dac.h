/*
 * Dac.c
 *
 * Created: 10/25/2015 1:50:24 PM
 *  Author: Aaron
 */ 

#ifndef Dac_H_
#define Dac_H_

#include <inttypes.h>

//function prototypes
void DAC_spi_init(void);
void DAC_write_byte(uint8_t data);
void sirenInterruptFunction();
void sirenOff();
void sirenOn();
void sirenInit();

#endif /* Dac_H_ */