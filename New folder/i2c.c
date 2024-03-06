#include "sysclock.h"
#include "delay.h"
#include "i2c.h"

/*STM32FXX DISCOVERY BOARD*/
void I2C_Config(void)
/*STEPS TO BE FOLLOWED
//Enable I2C clock and GPIO clock
//2.Configure I2C pins for alternate functions
  a)Select alternate function in moder register
  b)Select open drain output
  c)select high speed for pins
   d)select pull up for both pins
  e)configure alternate function in AFR registers
3.Reset I2C Register
4.Program the peripheral input clock in I2C_CR2 Register in order to generate correct timing
5.Configure clock control registers
6.Configure rise time register
7.Program I2C_CR1 Register to enable peripheral
*/
{

RCC->APB1ENR|=(1U<<21); //Enable I2C clock
RCC->AHB1ENR|=(1U<<1);//enable GPIOB clock
//select pins and configure them
GPIOB->MODER|=(2U<<16)|(2U<<18);//ALTERNATE FUNCTION CONFIG
GPIOB->OTYPER|=(1U<<8)|(1U<<9);//OPEN DRAIN CONFIG
GPIOB->OSPEEDR|=(3U<<16)|(3U<<18);//SELECT HIGH SPEED
GPIOB->PUPDR|=(1U<<16)|(1U<<18);//SELECT PULL UP PINS
GPIOB->AFR[1]=(4U<<0)|(4U<<4);
// pin configuration ended PB8 and PB9 for i2c

//reset i2c

I2C1->CR1|=(1U<<15); //software reset for i2c
I2C1->CR1&=~(1U<<15); //reset is stopped cleared
//propgram peripheral clock in i2c register to generate timing
I2C1->CR2|=(42U<<0); //it runs on 42 MHZ PCLK FREQ IN MHZ
	//CONFIGURE CLOCK CONTROL REGISTERS:
I2C1->CCR|=(210U<<0); //calculated ccr value Tr/Pclk1
//CONFIGURE RISE TIME REGISTER
I2C1->TRISE|=(43U<<0); //calculated  from pdf 1000 max value/Tpclk1
//PROGRAM 12C REGISTER PERIPHER ENABLE
I2C1->CR1|=(1U<<0);
}


void I2C_Start(void)
{
/*STEPS FOLLOWED 
1)Send the start condition 
2)Wait for the start bit (Bit 0 in SR1) to set.This indicates that start condition is generated */
//1)This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
//2)Set when a Start condition generated.
//Cleared by software by reading the SR1 register followed by writing the DR register, or by
I2C1->CR1|=(1U<<8);
while(!(I2C1->SR1&(uint32_t)(1U<<0)));
}
void I2C_Write(uint8_t data)
//wait for the TXE (bit 7 in SR1) to be set. This indicates DR is empty --> when data is written to/in DR register TXE will be set to 0.it is the first condition to be satisfied
//send the data to dr register
//wait for BTR (bit transmission complete bit in SR2 ) to be set.This indicates last data transmission.
{
	while(!(I2C1->SR1&(1U<<7))); //1st step
	I2C1->DR=data;//step2
	while(!(I2C1->SR1&(1U<<2)));//step3
	
}
void I2C_Adress(uint8_t Address)
//Send the address to DR register
//wait for the ADDR (bit 1 in SR1) to set.This indicates the end of address transmission
//clear the ADDR by reading SR1 and SR2
{
	I2C1->DR=Address;
	while(!(I2C1->SR1&(1U<<1)));
  uint8_t temp= I2C1->SR1|I2C1->SR2; //read SR1 and SR2 bit to temp 	
}

void I2C_Stop(void)
{
	I2C1->CR1|=(1U<<9); //stop I2C	
}
void WriteMulti(uint8_t* data, uint8_t size)
//STEPS TO BE FOLLOWED 
//Wait for TXE bit in SR register to be set . This indicates DR register is empty
//keep sending data to DR register after performing the checking that TXE bit is set
//Once the data transfer is complete wait for BTF to be set.
{
while(!(I2C1->SR1&(1U<<7)));	//data registeri dolu iken bekle. TXE set olmasini bekle onceki islemin bitmesi icin
while(size)
{
	while(!(I2C1->SR1&(1U<<7)));//1)dr dolu bekle . 2)dr veri yok cik
	I2C1->DR=(volatile uint32_t)*data++; //3)dr veri yaz .txe 0 olur 
	size--;
}
while(!(I2C1->SR1&(1<<2))); //wait for TC bit to set
}






