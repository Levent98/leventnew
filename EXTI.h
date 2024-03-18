#if !defined EXTI_H
#define EXTI_H
#include "sysclock.h"

void GPIO_Init(void);
void InterruptConfig(void);
void delay_ (uint32_t );
void EXTI0_IRQHandler(void);




#endif


