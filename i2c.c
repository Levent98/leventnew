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
//I2C1->CR1|=(1U<<10);//enable ack bit
I2C1->CR1&=~(1U<<9); //CLEAR STOP
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
	uint32_t a=I2C1->CR1;
	while(!(I2C1->SR1&(1U<<1)));
  uint8_t temp= I2C1->SR1|I2C1->SR2; //read SR1 and SR2 bit to temp 	
}

void I2C_Stop(void)
{
	I2C1->CR1|=(1U<<9); //stop I2C	
}
void WriteMulti(uint8_t* data, uint8_t size)
{
//STEPS TO BE FOLLOWED 
//Wait for TXE bit in SR register to be set . This indicates DR register is empty
//keep sending data to DR register after performing the checking that TXE bit is set
//Once the data transfer is complete wait for BTF to be set.

while(!(I2C1->SR1&(1U<<7)));	//data registeri dolu iken bekle. TXE set olmasini bekle onceki islemin bitmesi icin
while(size)
{
	while(!(I2C1->SR1&(1U<<7)));//1)dr dolu bekle . 2)dr veri yok cik
	I2C1->DR=(uint32_t)*data++; //3)dr veri yaz .txe 0 olur 
	size--;
}
while(!(I2C1->SR1&(1<<2))); //wait for TC bit to set
}

void I2C_Read(uint8_t Address,uint8_t* buffer,uint8_t size)//master receive mode
	{
	//steps to be followed
//write the slave address and wait for ADDR bit (bit 1 in SR1) to be set	
//Acknowledge disable made during EV6 (before clearing ADDR flag) and the stop condition is generated after EV6
//Wait for RXNE bit to be set 
//Read the data from DR
		//2.If Multiple bytes need to be read :
//write slave address and wait for ADDR bit to be set
//clear the ADDR by reading SR1 and SR2 registers
//wait for the RXNE bit to be set
//read the data from the DR
//generate the acknowledgement by setting the ACK bit(bit 10 in SR1)
//To generate the nonacknwledgement pulse after the last received data byte, the ACK bit must be cleared just after reading the second last byte
 //--after second last Rxne event
//In order to generate the Stop /Restart condition software must set START/RESTART bit after reading the second last data byte (after the second last RxNe event)

int remaining=size;
		
//STEP1 
  if (size==1)
		{
//step 1-a
    I2C1->DR=Address;
		while(!(I2C1->SR1&(1<<1))); //wait untill first bit in SR1 is set
		
//	Step1-b
		I2C1->CR1&=(uint32_t)~(1U<<10); //ack is disabled
		uint8_t temp=I2C1->SR1|I2C1->SR2;//clears ADDR bit
		I2C1->CR1|=(1U<<9); //stop is generated
// step 1-c
		while (!(I2C1->SR1&(1U<<6))); //wait untill RXNE bit is set
//step 1-d
		buffer[size-remaining]=I2C1->DR;
		}	
 else
	 {
	 //step 1-a
    I2C1->DR=Address;
		while(!(I2C1->SR1&(1<<1))); //wait untill first bit in SR1 ADDR is set
	//	Step1-b 
	  uint8_t temp=I2C1->SR1|I2C1->SR2;//clears ADDR bit
	 
		 while(remaining>2)
		 {
			 //Step 2 
			 while(!(I2C1->SR1&(1<<6)));//wait for rxne bit to set
			 //Step3 copy the data into buffer
			 buffer[size-remaining]=I2C1->DR;//copy the data into buffer
			 I2C1->CR1|= 1<<10;//Set the ACK bit acknowledge the data received
			 
			 remaining--;		 
			 
		 }
		//READ THE SECOND LAST BYTE
		while(!(I2C1->SR1&(1<<6))); //wait untill RXNE is set 
		buffer[size-remaining]=I2C1->DR;
		 
		///STEP2 clear ack bit
		I2C1->CR1&=(uint32_t)~(1<<10);
		//STEP3 STOP I2C
		I2C1->CR1|=(1<<9);//STOP I2C
		
		 remaining--;
		//READ LAST BYTE
		while(!(I2C1->SR1&(1<<6)));//Wait for RXNE bit to set
		buffer[size-remaining]=I2C1->DR;
		
		
	
 }
	
	}	
void master_to_device_write(uint8_t device_address,const uint8_t* write_buffer, size_t write_size)
	
{
   //I2C_Config();   
   I2C_Start();
   I2C_Adress(device_address);
	 if (write_size==1)
	 {
	 I2C_Write(*write_buffer);
	 }
	 else
	 {
	 WriteMulti(write_buffer, write_size);
	 }
	 I2C_Stop(); 
	 	
}
void master_to_device_read(uint8_t device_address, size_t write_size,uint8_t* buffer)	
{
   I2C_Config();   
   I2C_Start();
   I2C_Adress(device_address);
	 I2C_Read(device_address,buffer,write_size);	 
	 I2C_Stop(); 
	 	
}



