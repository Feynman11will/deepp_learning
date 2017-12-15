#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f10x.h"
#include "stdbool.h"
#include "delay.h"
#include "datadeal.h"

typedef  uint32_t Addrass; 
extern u32 ADC_DualConvertedValueTab[];
//---------------时钟-------------------------------------------------------------------------
#if 1
// 更改此设置一定要对应更改 HSE_VALUE, PLL_M,P LL_N, PLL_P 等的值
#define MAIN_PLL  72000000  
#define SYSCLK    72000000   
#define AHB_CLK   72000000
#define APB1_CLK  36000000
#define APB2_CLK  72000000 
#endif //  1
//---------------------------------------------------------------------------------------------
/*一维动态数组生成使用
电压极值存放到结构体当中
*/
typedef struct {

uint16_t vol1_max;
uint16_t vol2_max;
uint16_t vol1_min;
uint16_t vol2_min;
FlagStatus updata;
} Vol_limit;

#define Vol_Up_threshold  3600		//电压检测上域值
#define Vol_Down_threshold  1450	//下阈值

extern uint16_t up_flag1 ;			//电压上标记位
extern uint16_t down_flag1;		//电压下标记位

extern uint16_t up_flag2 ;			//电压上标记位
extern uint16_t down_flag2 ;		//电压下标记位

#define pi   3.141592653
#endif /* __MAIN_H */














