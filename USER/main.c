#include "stm32f10x.h"
#include "mydma1.h"
#include "usart1.h"
#include "delay.h"
#include "stdio.h"
#include "adc.h"
#include "datadeal.h"
#include "main.h"
#include "vol2ang.h"

uint16_t up_flag1 = 0;			//电压上标记位
uint16_t down_flag1 = 0;		//电压下标记位
uint16_t up_flag2 = 0;			//电压上标记位
uint16_t down_flag2 = 0;		//电压下标记位
/*——————————————————————————————————————————————
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 *——————————————————————————————————————————————*/
u32 ADC_DualConvertedValueTab[sample_val];		//声明全局变量数组数组长度是8

int main(void)
{	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组
//--------------------------------------------------------------
		delay_init();
		USART1_Config();										//默认波特率115200
		printf("\r\n 串口初始化成功 \r\n");
//--------------------------------------------------------------
		ADC1_2_Init(ADC_DualConvertedValueTab);
		printf("\r\n adc初始化成功 \r\n");			//说明程序初始化成功
//--------------------------------------------------------------
	  Wait_paramater_ok();//等待数组表制作成功
	
		Adc_timer();//打开定时器，并使用查表功能
	
		printf("\r\n 定时器初始化成功\r\n");
//--------------------------------------------------------------
	while(1)
	{
		
	}
}
