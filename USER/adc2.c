#include "adc2.h"

void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/*使能GPIO和ADC1通道时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  
 
	/*将PA0设置为模拟输入*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*将GPIO设置为模拟输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	Adc_GPIO_Config();	
  /*72M/6=12,ADC最大时间不能超过14M*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	/*将外设 ADC1 的全部寄存器重设为默认值*/
	ADC_DeInit(ADC1); 
  /*ADC工作模式:ADC1和ADC2工作在独立模式*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/*模数转换工作在单通道模式*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	/*模数转换工作在单次转换模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  /*ADC转换由软件而不是外部触发启动*/	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	/*ADC数据右对齐*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/*顺序进行规则转换的ADC通道的数目*/
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	/*根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器*/   
	ADC_Init(ADC1, &ADC_InitStructure);	
 
  /*使能指定的ADC1*/
	ADC_Cmd(ADC1, ENABLE);	
	/*重置指定的ADC1的校准寄存器*/
	ADC_ResetCalibration(ADC1);	
	/*获取ADC1重置校准寄存器的状态,设置状态则等待*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*开始指定ADC1的校准*/
	ADC_StartCalibration(ADC1);		
  /*获取指定ADC1的校准程序,设置状态则等待*/
	while(ADC_GetCalibrationStatus(ADC1));		
  /*使能指定的ADC1的软件转换启动功能*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
}	
