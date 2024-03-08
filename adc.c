#include "adc.h"

//STM32F4XX DISCOVERY BOARD

void ADC_Init(void)
//1)Enable GPIO ClocK and ADC Clock
//2)Set the prescalar in Common Control Register (CCR)
//3)Set the scan mode and resolution in Control Register (CR1)
//4)Set the continous conversion ,EOC and Data Alignment in Control Register 2 (CR2)
//5)Set the sampling times for channels in ADC_SMPRX
//6)Set the Regular channel sequence length in ADC_SQR1
//7)Set the respective GPIO Pins in analog mode 

{
	//1)Set GPIO and ADC Clock 
	RCC->AHB1ENR|=(1U<<0); //Enable GPIOA
	RCC->APB2ENR|=(1U<<8); //Enable ADC1
	//2) set prescalar in CCR
	ADC->CCR|=(1<<16);
	//3))scan mode and resolution in CR1
	ADC1->CR1&=(uint32_t)((uint32_t)~ (1U<<24));//resolution 00 12 bit
	ADC1->CR1|=(1<<8); //scan mode enable
	//4)
	ADC1->CR2|=(1U<<1);//Continous conversion set to 1
	ADC1->CR2&=(uint32_t)((uint32_t)~(1U<<11));//right alignment
	ADC1->CR2|=(1U<<10); //EOC after each conversion
	//5)
	ADC1->SMPR2&=~(uint32_t)((7<<3)|(7<<6));//channel 1 and 2 sampling rate 3 cycle
	//6)
	ADC1->SQR1|=((8>>3)<<20); //SQRL1 =1 for 2 conversions
	//7)
	GPIOA->MODER|=(3U<<2)|(3U<<4);//pins configured as analog (PA1 and PA2)
}
void ADC_Enable(void)
{
	//STEPS TO FOLLOW
//Enable the ADC by setting ADON bit in CR2
//Wait for ADC to stabilize (approx 10us)
	
ADC1->CR2|=(1<<0);//Enables ADC
uint32_t delayy=10000;

while(delayy--);
}
void ADC_Start(int channel)
	//STEPS TO FOLLOW
//Set the channel sequence in SQR Register
//Clear the status Register
//Start the conversion by setting the SWSTRT bit in CR2
	{
	ADC1->SQR3=0;
	ADC1->SQR3|=(uint32_t)(channel<<0);	
	ADC1->SR=0;//status register cleared
	
	ADC1->CR2|=(1U<<30);//conversion start set
	
	
}

void ADC_WaitforConv(void)
	{
	//EOC Flag will be set once the conversion is complete

while(!(ADC1->SR&(1U<<1))); //wait for EOC flag to set
}
	
uint16_t ADC_GetVal(void)
{
return ADC1->DR;

}	
void Disable_ADC(void){
//Disable ADC by clearing ADON bit
	
ADC1->CR2&=(1<<0);
}	
	
	
	
	
	
	
	
