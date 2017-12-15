
#include "adc.h"
#include "stm32f10x_dma.h"
#include "datadeal.h"
#include "stm32f10x_adc.h"
#include "main.h"
#include	"stdio.h"
#include "qurrylist.h"

/*---------------------------------------------------------
ͨ�ö�ʱ���жϳ�ʼ��
arr���Զ���װֵ��
psc��ʱ��Ԥ��Ƶ��
��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
Ft=��ʱ������Ƶ��,��λ:Mhz
---------------------------------------------------------------*/
#define TIM2_CLK 100000  													// ������Ա� 72M ��������������Կ���
#define TIM2_PSC (u16)(2 * APB1_CLK / TIM2_CLK - 1)	// = 0
#define TIM2_CYCLE   0.001											// ��ʱ���� 500us
#define TIM2_ARR (u16)(TIM2_CYCLE * TIM2_CLK - 1)		//
/*------------------------------------------------
��������:Adc_GPIO_Config
���룺��
�������
����������gpioʱ�Ӵ򿪣�����adc�����Ž�������
-------------------------------------------------*/

void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/*ʹ��GPIO��ADC1��ADC2 ͨ��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE );	  
	
	/*��PA0����Ϊģ��������ΪVo1�Ĳɼ�����,��PA1����Ϊģ��������Ϊvo2��adc�ɼ�����*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	/*��GPIO����Ϊģ������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/*------------------------------------------------
��������:ADC1_2_Init
����uu32 ADC_Dual[]
�������
������������adc1��adc2����ͬ��ģʽ���ã�����dma��������
					��adc1��adc2����У׼��ʹ�ܣ�adc1��dmaʹ�ܡ�
-------------------------------------------------*/
void ADC1_2_Init(u32 ADC_Dual[])
{
	  ADC_InitTypeDef ADC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 
		Adc_GPIO_Config();//adc1��2����ʱ�ӵ�����
	  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		/* DMA ͨ����ʼ�� ----------------------------------------------*/
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//˫ģʽʹ�õ�adc���ݼĴ������ߴ洢��adc1�ļĴ�������
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Dual[0];  //Ŀ�����ݼĴ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
    DMA_InitStructure.DMA_BufferSize = sample_val;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Դ���ݼĴ�����ָ�벻�仯
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//Ŀ��Ĵ���ָ������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//�����ֽڳ���
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;//���ó�ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1, ENABLE); 		//ʹ��DMA1

		/*-----����dma���ж�--------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
  
		
		/* ADC1 ������ʼ��------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                            //ADC1ͬ��������ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 //����ɨ��
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����2
		ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1
		/* ADC1 ����ͨ������---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5); //ͨ������    
		
		/* ADC2 ��ʼ�� ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                  //����ɨ��
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����
		ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2
		/* ADC2 ����ͨ������---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5); //ͨ������
	//ADC_ExternalTrigConvCmd(ADC1, ENABLE);				//��ͬ��ģʽ�£�adc1Ϊ������ģʽadc2Ϊ�Ӵ���ģʽ
	//ADC_ExternalTrigConvCmd(ADC2, ENABLE);        //ʹ��ADC2���ⲿ����ģʽ��Ϊ�Ӵ���ģʽ 
		ADC_DMACmd(ADC1, ENABLE); //�����Ƿ�Ҫʹ��dma����Ҫ����adc1_dma
	
  	DMA_Cmd(DMA1_Channel1, ENABLE);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
		
		ADC_Cmd(ADC1, ENABLE);  //ADC1ʹ��   
		/*---ADC1У׼----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1��У׼ ��adc1��adc2����ͬʱ������
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2У׼----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
	/*---ʹ��dma��adc��dma���ж�--------------------------------------*/
		
}

/*-------------------------------------------------------------
�������ƣ�Adc_Calibra
���룺��
�������
��������:adc1��adc2��У׼
--------------------------------------------------------------*/
void Adc_Calibra(void)
{
		ADC_Cmd(ADC1, ENABLE);  //ADC1ʹ��   
		/*---ADC1У׼----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1��У׼ ��adc1��adc2����ͬʱ������
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2У׼----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
}
/*-------------------------------------------------------------
�������ƣ�Adc_timer
���룺��
�������
������������adc�Ĵ���Դ��ʱ�����г�ʼ�������еĶ�ʱ�����г�ʼ��
					������ʱ�����жϴ򿪣����жϺ����д������ݵȵȲ���
--------------------------------------------------------------*/
void Adc_timer(void)
{		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//��ʱ��
	//---------------------------------------------------????????????????????�
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//��ʱ�Ӳ���Ƶ  72Mhz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = TIM2_ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM2_PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,  &TIM_TimeBaseInitStructure);//��ʱ����Ϣ����
	//----------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);//��ʱ���ж�ʹ��
	
	//----------------------------------------------------------
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��3//���жϺ���
	//��ʱ��ʹ��
}

void TIM2_IRQHandler(void)
{	
	//int j = 0;
//	uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//ȡ4������ƽ��ֵ
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
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
/*-----------------------------------------------------------------------
�������ƣ�DMA1_Channel1_IRQHandler
���룺��
�������
��������:��dma1�жϺ����У�ʵ�ֵĹ�������
				��1����dma�ж���ʵ�����ݵĲ�����
				��2�������ݽ��в�ֽ�����adc����ֵ�ֱ����adcֵ���ݴ����鵱��
				��3�����dma�����־λ
-------------------------------------------------------------------------*/
void DMA1_Channel1_IRQHandler(void)
{	
	//int j = 0;
	//uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	//uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//ȡ4������ƽ��ֵ
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


