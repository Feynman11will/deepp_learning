#ifndef __ADC_H
#define __ADC_H	
#include "stm32f10x.h"

#define sam_val 8
extern int Save_flag;

void Calculate(void);
void Adc_Calibra(void);
void Adc_GPIO_Config(void); 
void Adc_timer(void);
void ADC1_2_Init(u32 ADC_Dual[]);
void MYDMA_Enable(u16 ndtr);
void Wait_Adma_Done(void);


#endif 
