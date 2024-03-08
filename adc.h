#if !defined ADC_H
#define ADC_H

#include "sysclock.h"



void ADC_Init(void);
void ADC_Enable(void);
void ADC_Start(int );
void ADC_WaitforConv(void);
uint16_t ADC_GetVal(void);
void Disable_ADC(void);


#endif