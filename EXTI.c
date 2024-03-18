#include "EXTI.h"

//STM32F4XX DISCOVERY BOARD EXTI CONFIG AND APPLICATION
void GPIO_Init(void)
//Enable GPIO Clock
//Set the required pin in input mode
//Configure PULL UP/PULL DOWN according to your requirements
{
	RCC->AHB1ENR|=(1<<0); //Enable GPIO Clock
	GPIOA->MODER&=(uint32_t)~(3<<0);//pin in input mode "00" 
	GPIOA->PUPDR&=(uint32_t)~(1<<0); //user push button no pull-up or pulldown 
	
}
void InterruptConfig(void)
{
	//Enable the SYSCNFG bit in RCC Register
	//Configure the EXTI Configuration in Register in SYSCNFG
	//Enable EXTI using Interrupt Mask Register (IMR)
	//Configure the rising edge falling edge trigger
	//Set the interrupt priority
	//Enable the interrupt
RCC->APB2ENR|=(1<<14);//ENABLE syscnfg
SYSCFG->EXTICR[0]&=(uint32_t)~(0xF<<0);//configure EXTI0 for PA0
EXTI->IMR|=(1<<0);// Disable mask on EXTI0
EXTI->RTSR|=(1<<0);//rising edge trigger enabled
EXTI->FTSR&=(uint32_t)~(1<<0);//falling edge interrupt disabled

NVIC_SetPriority(EXTI0_IRQn,1);
NVIC_EnableIRQ(EXTI0_IRQn);
	
}
extern int flag;

void delay_ (uint32_t time )
{
	while (time--){}
}
int flag=0;
int count=0;
void EXTI0_IRQHandler(void)
{
	//steps:
	//check the pin which triggered the Interrupt
	//clear the interrupt pending bit 
	if (EXTI->PR & (1<<0))//check if interrupt is triggered 
		{
		EXTI->PR|=(1<<0);//clears interrupt pending bit
		flag=1;
		
}
}
/*int main (void){
	
SystemInitt();//initializes clock
GPIO_Init();//Initializes GPIOA 
InterruptConfig(); //Interrupt configuration 

while(1)
{
	
 if (flag){
	 delay(100000);
	 count++;
	 flag=0;	
	
}
	
	
		
}
*/