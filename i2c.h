#ifndef I2C_H
#define I2C_H
#include <stdio.h>
#include <stdint.h>

void I2C_Config(void);
void I2C_Start(void);
void I2C_Write(uint8_t );
void WriteMulti(uint8_t* , uint8_t );
void I2C_Adress(uint8_t );
void I2C_Stop(void);
void WriteMulti(uint8_t* data, uint8_t size);
void I2C_Read(uint8_t ,uint8_t* ,uint8_t );
void master_to_device_write(uint8_t ,const uint8_t* , size_t );//for lcd
void master_to_device_read(uint8_t , size_t ,uint8_t* );//for lcd

#endif