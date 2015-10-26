/*
 * i2c_driver.h
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: Luke
 */ 


#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_#include <avr/io.h>
// PC4 = SDA
// PC5 = SCL

// Function Prototypes
void I2C_Init();
void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister);
void I2C_WriteByte(uint8_t busAddr, uint8_t data);
uint8_t I2C_ReadByte(uint8_t busAddr);

#endif /* I2C_DRIVER_H_ */