#include "delay.h"

void I2C_Config(void);
void I2C_Start(void);
void I2C_Write(uint8_t data);
void I2C_Adress(uint8_t Address);
void I2C_Stop(void);
void WriteMulti(uint8_t* data, uint8_t size);
