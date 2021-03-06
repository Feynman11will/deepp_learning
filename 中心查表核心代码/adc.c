
#include "adc.h"
#include "stm32f10x_dma.h"
#include "datadeal.h"
#include "stm32f10x_adc.h"
#include "main.h"
#include	"stdio.h"
#include "qurrylist.h"

/*---------------------------------------------------------
通用定时器中断初始化
arr：自动重装值。
psc：时钟预分频数
定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
Ft=定时器工作频率,单位:Mhz
---------------------------------------------------------------*/
#define TIM2_CLK 100000  													// 必须可以被 72M 整除，在哪里可以看到
#define TIM2_PSC (u16)(2 * APB1_CLK / TIM2_CLK - 1)	// = 0
#define TIM2_CYCLE   0.001											// 定时周期 500us
#define TIM2_ARR (u16)(TIM2_CYCLE * TIM2_CLK - 1)		//
/*------------------------------------------------
函数名称:Adc_GPIO_Config
输入：无
输出：无
函数描述：gpio时钟打开，并将adc的引脚进行配置
-------------------------------------------------*/

void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/*使能GPIO和ADC1、ADC2 通道时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE );	  
	
	/*将PA0设置为模拟输入作为Vo1的采集引脚,将PA1设置为模拟输入作为vo2的adc采集引脚*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	/*将GPIO设置为模拟输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/*------------------------------------------------
函数名称:ADC1_2_Init
输入u32 ADC_Dual[]
输出：无
函数描述：对adc1和adc2进行同步模式配置，并将dma进行配置
					对adc1和adc2进行校准和使能，adc1的dma使能。
-------------------------------------------------*/
void ADC1_2_Init(u32 ADC_Dual[])
{
	  ADC_InitTypeDef ADC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 
		Adc_GPIO_Config();//adc1、2引脚时钟的配置
	  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		/* DMA 通道初始化 ----------------------------------------------*/
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//双模式使用的adc数据寄存器或者存储在adc1的寄存器当中
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Dual[0];  //目标数据寄存器
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
    DMA_InitStructure.DMA_BufferSize = sample_val;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//源数据寄存器的指针不变化
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//目标寄存器指针增加
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//设置字节长度
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;//设置成循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1, ENABLE); 		//使能DMA1

		/*-----开启dma的中断--------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
  
		
		/* ADC1 参数初始化------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                            //ADC1同步规则组模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 //启动扫描
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //打开外部触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数2
		ADC_Init(ADC1, &ADC_InitStructure);                                           //配置ADC1
		/* ADC1 规则通道设置---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5); //通道配置    
		
		/* ADC2 初始化 ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2同步规则组模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                  //启动扫描
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数
		ADC_Init(ADC2, &ADC_InitStructure);                                           //配置ADC2
		/* ADC2 规则通道设置---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5); //通道配置
	//ADC_ExternalTrigConvCmd(ADC1, ENABLE);				//在同步模式下，adc1为主触发模式adc2为从触发模式
	//ADC_ExternalTrigConvCmd(ADC2, ENABLE);        //使能ADC2的外部触发模式，为从触发模式 
		ADC_DMACmd(ADC1, ENABLE); //无论是否要使用dma，都要是能adc1_dma
	
  	DMA_Cmd(DMA1_Channel1, ENABLE);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
		
		ADC_Cmd(ADC1, ENABLE);  //ADC1使能   
		/*---ADC1校准----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1的校准 ，adc1和adc2必须同时被激活
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2校准----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2的校准
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
	/*---使能dma和adc和dma的中断--------------------------------------*/
		
}

/*-------------------------------------------------------------
函数名称：Adc_Calibra
输入：无
输出：无
函数描述:adc1和adc2的校准
--------------------------------------------------------------*/
void Adc_Calibra(void)
{
		ADC_Cmd(ADC1, ENABLE);  //ADC1使能   
		/*---ADC1校准----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1的校准 ，adc1和adc2必须同时被激活
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2校准----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2的校准
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
}
/*-------------------------------------------------------------
函数名称：Adc_timer
输入：无
输出：无
函数描述：对adc的触发源定时器进行初始化，进行的定时器进行初始化
					并将定时器的中断打开，在中断函数中处理数据等等操作
--------------------------------------------------------------*/
void Adc_timer(void)
{		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//打开时钟
	//---------------------------------------------------????????????????????�
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//对时钟不分频  72Mhz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = TIM2_ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM2_PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,  &TIM_TimeBaseInitStructure);//定时器信息配置
	//----------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);//定时器中断使能
	
	//----------------------------------------------------------
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器3//打开中断函数
	//定时器使能
}

void TIM2_IRQHandler(void)
{	
	//int j = 0;
//	uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//取4个数做平均值
//	{
//		Adc_v1 = (ADC_DualConvertedValueTab[j] & 0xFFFF);
//		Adc_v2 = ((ADC_DualConvertedValueTab[j] & 0xFFFF0000) >>16);
//		temp1 = temp1+Adc_v1;
//		temp2 = temp2+Adc_v2;
//	}
//	temp1 = temp1/sample_val;
//	temp2 = temp2/sample_val;
	temp1 = (ADC_DualConvertedValueTab[0] & 0xFFFF);
  temp2 = ((ADC_DualConvertedValueTab[0] & 0xFFFF0000) >>16);
	//printf("%d %d\n",temp1,temp2);
	Qurry_List(temp1, temp2);
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}
/*-----------------------------------------------------------------------
函数名称：DMA1_Channel1_IRQHandler
输入：无
输出：无
函数描述:在dma1中断函数中，实现的功能如下
				（1）在dma中断里实现数据的查表输出
				（2）对数据进行拆分将两个adc的数值分别存入adc值的暂存数组当中
				（3）清除dma软件标志位
-------------------------------------------------------------------------*/
void DMA1_Channel1_IRQHandler(void)
{	
	//int j = 0;
	//uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	//uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//取4个数做平均值
//	{
//		Adc_v1 = (ADC_DualConvertedValueTab[j] & 0xFFFF);
//		Adc_v2 = ((ADC_DualConvertedValueTab[j] & 0xFFFF0000) >>16);
//		temp1 = temp1+Adc_v1;
//		temp2 = temp2+Adc_v2;
//	}
//	temp1 = temp1/sample_val;
//	temp2 = temp2/sample_val;
	//temp1 = (ADC_DualConvertedValueTab[0] & 0xFFFF);
   //temp2 = ((ADC_DualConvertedValueTab[0] & 0xFFFF0000) >>16);
	 //printf("%d %d\n",temp1,temp2);
		DMA_ClearFlag(DMA1_FLAG_TC1); 
}


