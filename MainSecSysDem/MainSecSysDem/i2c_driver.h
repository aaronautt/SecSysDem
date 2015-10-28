/*
 * i2c_driver.h
 *
 * Created: 10/3/2015 6:10:21 PM
 *  Author: Luke
 */ 


#ifndef I2C_DRIVER_H_
<<<<<<< HEAD
#define I2C_DRIVER_H_// PC4 = SDA
=======
#define I2C_DRIVER_H_#include <avr/io.h>
// PC4 = SDA
>>>>>>> 35cada19e91a0bf5f91e01a25468a1d3fb118b2e
// PC5 = SCL

// Function Prototypes
void I2C_Init();
void I2C_WriteRegister(uint8_t busAddr, uint8_t deviceRegister, uint8_t data);
uint8_t I2C_ReadRegister(uint8_t busAddr, uint8_t deviceRegister);
<<<<<<< HEAD
void I2C_DoubleWriteRegister(uint8_t busAddr, uint16_t deviceRegister, uint8_t data);
uint8_t I2C_DoubleReadRegister(uint8_t busAddr, uint16_t deviceRegister);
=======
void I2C_WriteByte(uint8_t busAddr, uint8_t data);
uint8_t I2C_ReadByte(uint8_t busAddr);
>>>>>>> 35cada19e91a0bf5f91e01a25468a1d3fb118b2e

#endif /* I2C_DRIVER_H_ */