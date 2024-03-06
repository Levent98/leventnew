#include "stm32f446xx.h"
#include "system_stm32f4xx.h"
#include "uart.h"

void UartConfig(void)
{
//Enable Uart clock and Gpio clock
RCC->APB1ENR|=(1U<<18); //ENABLE USART 3
RCC->AHB1ENR|=(1U<<1); //ENABLE GPIOB 	
//configure uart pins for alternate functions
GPIOB->MODER|=(2U<<20)|(2U<<22);	//enable pb10 and pb11
GPIOB->OSPEEDR|=(3U<<20)|(3U<<22); //set high speed for uart
GPIOB->AFR[1]|=(7<<8)|(7<<12); //alternate function registers for pins 10 and 11 gpiob
//enable the USART by writing UE bit in USART_CR1 register 1
USART3->CR1=0x00;
USART3->CR1|=(1U<<13);
//Program the M bit in USART_CR1 to define the word length
USART3->CR1&=(uint32_t)((uint32_t)~(USART_CR1_M));//set M bit to 0
//Select the baudrate using USART_BRR Register
//USART3->BRR&=(uint32_t)~(1U<<3);
USART3->BRR|=(22U<<4)|(	0xD<<0);

//Enable Transmitter/Receiver by setting TE and RE bits in USART_CR1 registers	
USART3->CR1|=(1U<<2)|(1U<<3);

}
void Uart3sendChar(uint8_t c)
{
//Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
//for each data to be transmitted in case of single buffer.
// After writing the last data into the USART_DR register, wait until TC=1. This indicates
//that the transmission of the last frame is complete. This is required for instance when
//the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.	
USART3->DR=c;
while(!(USART3->SR&(1U<<6)));	
		
}

void UART3_SendString(char*string){
	
	
	while(*string) 
	{
		Uart3sendChar(*string++);	
	}
}
uint8_t UART3_Getchar(void)
	
	{
		//steps to be followed
		//wait for rxne bit to be set. this means data has been received thus can be read 
		//read the data from usart registers
		uint8_t temp;
		
		while(!(USART3->SR&(1U<<5))); //wait till data is received
		
		temp=USART3->DR; //this clears rxne also
		return temp;

	
	
}