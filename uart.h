#if !defined UART_H
#define UART_H

#include "delay.h"

void UartConfig(void);
void Uart3sendChar(uint8_t);
void UART3_SendString(char*);
uint8_t UART3_Getchar(void);

#endif
