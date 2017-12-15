#include "adc2.h"

void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/*ʹ��GPIO��ADC1ͨ��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  
 
	/*��PA0����Ϊģ������*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	/*��GPIO����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void  Adc_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	Adc_GPIO_Config();	
  /*72M/6=12,ADC���ʱ�䲻�ܳ���14M*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	/*������ ADC1 ��ȫ���Ĵ�������ΪĬ��ֵ*/
	ADC_DeInit(ADC1); 
  /*ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	/*ģ��ת�������ڵ�ͨ��ģʽ*/
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	/*ģ��ת�������ڵ���ת��ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  /*ADCת��������������ⲿ��������*/	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	/*ADC�����Ҷ���*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	/*˳����й���ת����ADCͨ������Ŀ*/
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	/*����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���*/   
	ADC_Init(ADC1, &ADC_InitStructure);	
 
  /*ʹ��ָ����ADC1*/
	ADC_Cmd(ADC1, ENABLE);	
	/*����ָ����ADC1��У׼�Ĵ���*/
	ADC_ResetCalibration(ADC1);	
	/*��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�*/
	while(ADC_GetResetCalibrationStatus(ADC1));
	/*��ʼָ��ADC1��У׼*/
	ADC_StartCalibration(ADC1);		
  /*��ȡָ��ADC1��У׼����,����״̬��ȴ�*/
	while(ADC_GetCalibrationStatus(ADC1));		
  /*ʹ��ָ����ADC1�����ת����������*/
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
}	
