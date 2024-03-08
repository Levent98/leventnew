
#include "delay.h"
#include "sysclock.h"
#include "uart.h"
#include "i2c.h"
#include "adc.h"
#include "lcd.h"




int main (void){
	
SystemInitt();
//ADC_Init();
//ADC_Enable();
TimConfig();
//UartConfig();
I2C_Config();
//uint32_t a=HAL_RCC_GetSysClockFreq();
//uint32_t b=USART3->CR1;	
//app_main();
//lcd_init();	
//I2C_Start();
//I2C_Adress(0x4E);
//lcd_init();
//lcd_clear();
//lcd_put_cur(0, 0);
//lcd_send_string("selamlar");
//lcd_put_cur(1, 0);
//lcd_send_string("from ESP32");	
app_main();
while(1)
{
	/*ADC_Start(0);
	ADC_WaitforConv();
	ADC_VAL[0]=ADC_GetVal();
	
	ADC_Start(4);
	ADC_WaitforConv();
	ADC_VAL[1]=ADC_GetVal();
	*/
  //UART3_SendString((char*)data);
	//Delay_ms(1000);
	//UART3_SendString((uint8_t*)"merhaba nasilsin?");
  
	
	
	
	
}
	
	
	
	
	
	
}