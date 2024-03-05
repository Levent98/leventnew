
#include "delay.h"
#include "sysclock.h"


void TimConfig(){

//enable timer clock
RCC->APB1ENR|=(1UL<<4);
//set the presacalar and arr

TIM6->PSC|=84-1;  //1us delay
TIM6->ARR|=0xFFFF;

//ENABLE THE TIMER WAIT FOR UPDATE FLAG TO SET

TIM6->CR1|=(1<<0);

while (!(TIM6->SR&(1<<0))); //wait for this bit to set its done by hardware

}

void Delay_us(uint16_t us){

//reset the counter
//wait for the counter to reach entered value as each count will take 1us total waiting will be corresponding delay

TIM6->CNT=0;
while (TIM6->CNT<us);
}


void Delay_ms(uint16_t ms){


for (uint16_t i=0;i<ms;i++){

Delay_us(1000);

}

}