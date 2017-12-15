#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f10x.h"
#include "stdbool.h"
#include "delay.h"
#include "datadeal.h"

typedef  uint32_t Addrass; 
extern u32 ADC_DualConvertedValueTab[];
//---------------ʱ��-------------------------------------------------------------------------
#if 1
// ���Ĵ�����һ��Ҫ��Ӧ���� HSE_VALUE, PLL_M,P LL_N, PLL_P �ȵ�ֵ
#define MAIN_PLL  72000000  
#define SYSCLK    72000000   
#define AHB_CLK   72000000
#define APB1_CLK  36000000
#define APB2_CLK  72000000 
#endif //  1
//---------------------------------------------------------------------------------------------
/*һά��̬��������ʹ��
��ѹ��ֵ��ŵ��ṹ�嵱��
*/
typedef struct {

uint16_t vol1_max;
uint16_t vol2_max;
uint16_t vol1_min;
uint16_t vol2_min;
FlagStatus updata;
} Vol_limit;

#define Vol_Up_threshold  3600		//��ѹ�������ֵ
#define Vol_Down_threshold  1450	//����ֵ

extern uint16_t up_flag1 ;			//��ѹ�ϱ��λ
extern uint16_t down_flag1;		//��ѹ�±��λ

extern uint16_t up_flag2 ;			//��ѹ�ϱ��λ
extern uint16_t down_flag2 ;		//��ѹ�±��λ

#define pi   3.141592653
#endif /* __MAIN_H */














