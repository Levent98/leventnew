#include "SPI.h"

void SPI_Config(void)
{
// Enable SPI clock
// Configure the Control Register 1
// Configure the CR2
RCC->APB1ENR|=(1<<14); //enable SPI2 
SPI2->CR1&=(uint32_t)~(1<<0);	
SPI2->CR1&=(uint32_t)~(1<<1);
	
	
	
	
}