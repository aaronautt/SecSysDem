/*
 * i2c_driver.h
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: Luke
 */ 


#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_#include <avr/io.h>
// PC4 = SDA
// PC5 = SCL

// Function Prototypes
void I2C_Init();
void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister);
void I2C_DoubleWriteRegister(uint8_t busAddr, uint16_t deviceRegister, char data);
uint8_t I2C_DoubleReadRegister(uint8_t busAddr, uint16_t deviceRegister);
void I2C_DoubleReadRegister_S(uint8_t busAddr, uint16_t start_address, uint16_t num_bytes, uint8_t data[]);
void I2C_DoubleWriteRegister_S(uint8_t busAddr, uint16_t deviceRegister, uint16_t num_bytes, uint8_t data[]);

#endif /* I2C_DRIVER_H_ */