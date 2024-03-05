
#include "delay.h"
#include "sysclock.h"
#include "uart.h"


static uint8_t data[15]="hello world!";



int main (void){
	
SystemInitt();
TimConfig();
UartConfig();
uint32_t a=HAL_RCC_GetSysClockFreq();
uint32_t b=USART3->CR1;	
	
	
while(1)
{
	
  UART3_SendString((char*)data);
	Delay_ms(1000);
	//UART3_SendString((uint8_t*)"merhaba nasilsin?");
	
	
	
	
	
}
	
	
	
	
	
	
}