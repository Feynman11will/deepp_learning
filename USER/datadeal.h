#ifndef __DATADEAL_
#define __DATADEAL_
#include "stm32f10x.h"

extern float temp1max,temp1min;
extern float temp2max,temp2min;

#define sample_val 1

u16  Data_deal(u16 Old_Val,u16 New_Vol,float a);

void Data_Cv(u32 ADC_Val[],int a);
void Data_Con2Ang(u32 Val_1,u32 Val_2,float t1max ,float t1min, float t2max,  float t2min);
void Get_Limit(float val1,float val2);

#endif

