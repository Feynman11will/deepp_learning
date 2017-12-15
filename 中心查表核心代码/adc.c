
#include "adc.h"
#include "stm32f10x_dma.h"
#include "datadeal.h"
#include "stm32f10x_adc.h"
#include "main.h"
#include	"stdio.h"
#include "qurrylist.h"

/*---------------------------------------------------------
Í¨ÓÃ¶¨Ê±Æ÷ÖÐ¶Ï³õÊ¼»¯
arr£º×Ô¶¯ÖØ×°Öµ¡£
psc£ºÊ±ÖÓÔ¤·ÖÆµÊý
¶¨Ê±Æ÷Òç³öÊ±¼ä¼ÆËã·½·¨:Tout=((arr+1)*(psc+1))/Ft us.
Ft=¶¨Ê±Æ÷¹¤×÷ÆµÂÊ,µ¥Î»:Mhz
---------------------------------------------------------------*/
#define TIM2_CLK 100000  													// ±ØÐë¿ÉÒÔ±» 72M Õû³ý£¬ÔÚÄÄÀï¿ÉÒÔ¿´µ½
#define TIM2_PSC (u16)(2 * APB1_CLK / TIM2_CLK - 1)	// = 0
#define TIM2_CYCLE   0.001											// ¶¨Ê±ÖÜÆÚ 500us
#define TIM2_ARR (u16)(TIM2_CYCLE * TIM2_CLK - 1)		//
/*------------------------------------------------
º¯ÊýÃû³Æ:Adc_GPIO_Config
ÊäÈë£ºÎÞ
Êä³ö£ºÎÞ
º¯ÊýÃèÊö£ºgpioÊ±ÖÓ´ò¿ª£¬²¢½«adcµÄÒý½Å½øÐÐÅäÖÃ
-------------------------------------------------*/

void Adc_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/*Ê¹ÄÜGPIOºÍADC1¡¢ADC2 Í¨µÀÊ±ÖÓ*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC2, ENABLE );	  
	
	/*½«PA0ÉèÖÃÎªÄ£ÄâÊäÈë×÷ÎªVo1µÄ²É¼¯Òý½Å,½«PA1ÉèÖÃÎªÄ£ÄâÊäÈë×÷Îªvo2µÄadc²É¼¯Òý½Å*/                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	/*½«GPIOÉèÖÃÎªÄ£ÄâÊäÈë*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/*------------------------------------------------
º¯ÊýÃû³Æ:ADC1_2_Init
ÊäÈë£uu32 ADC_Dual[]
Êä³ö£ºÎÞ
º¯ÊýÃèÊö£º¶Ôadc1ºÍadc2½øÐÐÍ¬²½Ä£Ê½ÅäÖÃ£¬²¢½«dma½øÐÐÅäÖÃ
					¶Ôadc1ºÍadc2½øÐÐÐ£×¼ºÍÊ¹ÄÜ£¬adc1µÄdmaÊ¹ÄÜ¡£
-------------------------------------------------*/
void ADC1_2_Init(u32 ADC_Dual[])
{
	  ADC_InitTypeDef ADC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	 
		Adc_GPIO_Config();//adc1¡¢2Òý½ÅÊ±ÖÓµÄÅäÖÃ
	  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		/* DMA Í¨µÀ³õÊ¼»¯ ----------------------------------------------*/
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//Ë«Ä£Ê½Ê¹ÓÃµÄadcÊý¾Ý¼Ä´æÆ÷»òÕß´æ´¢ÔÚadc1µÄ¼Ä´æÆ÷µ±ÖÐ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_Dual[0];  //Ä¿±êÊý¾Ý¼Ä´æÆ÷
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
    DMA_InitStructure.DMA_BufferSize = sample_val;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Ô´Êý¾Ý¼Ä´æÆ÷µÄÖ¸Õë²»±ä»¯
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		//Ä¿±ê¼Ä´æÆ÷Ö¸ÕëÔö¼Ó
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;//ÉèÖÃ×Ö½Ú³¤¶È
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;//ÉèÖÃ³ÉÑ­»·Ä£Ê½
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1, ENABLE); 		//Ê¹ÄÜDMA1

		/*-----¿ªÆôdmaµÄÖÐ¶Ï--------------------*/
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
  
		
		/* ADC1 ²ÎÊý³õÊ¼»¯------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                            //ADC1Í¬²½¹æÔò×éÄ£Ê½
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                 //Æô¶¯É¨Ãè
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //Á¬Ðø×ª»»
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //´ò¿ªÍâ²¿´¥·¢
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //½á¹ûÓÒ¶ÔÆë
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //×ª»»Í¨µÀÊý2
		ADC_Init(ADC1, &ADC_InitStructure);                                           //ÅäÖÃADC1
		/* ADC1 ¹æÔòÍ¨µÀÉèÖÃ---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5); //Í¨µÀÅäÖÃ    
		
		/* ADC2 ³õÊ¼»¯ ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2Í¬²½¹æÔò×éÄ£Ê½
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;                                  //Æô¶¯É¨Ãè
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //Á¬Ðø×ª»»
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //ÎÞÍâ²¿´¥·¢
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //½á¹ûÓÒ¶ÔÆë
		ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //×ª»»Í¨µÀÊý
		ADC_Init(ADC2, &ADC_InitStructure);                                           //ÅäÖÃADC2
		/* ADC2 ¹æÔòÍ¨µÀÉèÖÃ---------------------------------------------------- */ 
		ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5); //Í¨µÀÅäÖÃ
	//ADC_ExternalTrigConvCmd(ADC1, ENABLE);				//ÔÚÍ¬²½Ä£Ê½ÏÂ£¬adc1ÎªÖ÷´¥·¢Ä£Ê½adc2Îª´Ó´¥·¢Ä£Ê½
	//ADC_ExternalTrigConvCmd(ADC2, ENABLE);        //Ê¹ÄÜADC2µÄÍâ²¿´¥·¢Ä£Ê½£¬Îª´Ó´¥·¢Ä£Ê½ 
		ADC_DMACmd(ADC1, ENABLE); //ÎÞÂÛÊÇ·ñÒªÊ¹ÓÃdma£¬¶¼ÒªÊÇÄÜadc1_dma
	
  	DMA_Cmd(DMA1_Channel1, ENABLE);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
		
		ADC_Cmd(ADC1, ENABLE);  //ADC1Ê¹ÄÜ   
		/*---ADC1Ð£×¼----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1µÄÐ£×¼ £¬adc1ºÍadc2±ØÐëÍ¬Ê±±»¼¤»î
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2Ð£×¼----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //Ê¹ÄÜADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2µÄÐ£×¼
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
	/*---Ê¹ÄÜdmaºÍadcºÍdmaµÄÖÐ¶Ï--------------------------------------*/
		
}

/*-------------------------------------------------------------
º¯ÊýÃû³Æ£ºAdc_Calibra
ÊäÈë£ºÎÞ
Êä³ö£ºÎÞ
º¯ÊýÃèÊö:adc1ºÍadc2µÄÐ£×¼
--------------------------------------------------------------*/
void Adc_Calibra(void)
{
		ADC_Cmd(ADC1, ENABLE);  //ADC1Ê¹ÄÜ   
		/*---ADC1Ð£×¼----------------------------------------------------*/
		ADC_ResetCalibration(ADC1);   //ADC1µÄÐ£×¼ £¬adc1ºÍadc2±ØÐëÍ¬Ê±±»¼¤»î
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1))
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			/*---ADC2Ð£×¼----------------------------------------------------*/
		ADC_Cmd(ADC2, ENABLE);                                                      //Ê¹ÄÜADC2
		ADC_ResetCalibration(ADC2);                                                 //ADC2µÄÐ£×¼
		while(ADC_GetResetCalibrationStatus(ADC2));
		ADC_StartCalibration(ADC2);
		while(ADC_GetCalibrationStatus(ADC2)); 
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);	
}
/*-------------------------------------------------------------
º¯ÊýÃû³Æ£ºAdc_timer
ÊäÈë£ºÎÞ
Êä³ö£ºÎÞ
º¯ÊýÃèÊö£º¶ÔadcµÄ´¥·¢Ô´¶¨Ê±Æ÷½øÐÐ³õÊ¼»¯£¬½øÐÐµÄ¶¨Ê±Æ÷½øÐÐ³õÊ¼»¯
					²¢½«¶¨Ê±Æ÷µÄÖÐ¶Ï´ò¿ª£¬ÔÚÖÐ¶Ïº¯ÊýÖÐ´¦ÀíÊý¾ÝµÈµÈ²Ù×÷
--------------------------------------------------------------*/
void Adc_timer(void)
{		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//´ò¿ªÊ±ÖÓ
	//---------------------------------------------------????????????????????»
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//¶ÔÊ±ÖÓ²»·ÖÆµ  72Mhz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_Period = TIM2_ARR;
	TIM_TimeBaseInitStructure.TIM_Prescaler = TIM2_PSC;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,  &TIM_TimeBaseInitStructure);//¶¨Ê±Æ÷ÐÅÏ¢ÅäÖÃ
	//----------------------------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //¶¨Ê±Æ÷2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);//¶¨Ê±Æ÷ÖÐ¶ÏÊ¹ÄÜ
	
	//----------------------------------------------------------
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ÔÊÐí¶¨Ê±Æ÷3¸üÐÂÖÐ¶Ï
	TIM_Cmd(TIM2,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷3//´ò¿ªÖÐ¶Ïº¯Êý
	//¶¨Ê±Æ÷Ê¹ÄÜ
}

void TIM2_IRQHandler(void)
{	
	//int j = 0;
//	uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//È¡4¸öÊý×öÆ½¾ùÖµ
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
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
}
/*-----------------------------------------------------------------------
º¯ÊýÃû³Æ£ºDMA1_Channel1_IRQHandler
ÊäÈë£ºÎÞ
Êä³ö£ºÎÞ
º¯ÊýÃèÊö:ÔÚdma1ÖÐ¶Ïº¯ÊýÖÐ£¬ÊµÏÖµÄ¹¦ÄÜÈçÏÂ
				£¨1£©ÔÚdmaÖÐ¶ÏÀïÊµÏÖÊý¾ÝµÄ²é±íÊä³ö
				£¨2£©¶ÔÊý¾Ý½øÐÐ²ð·Ö½«Á½¸öadcµÄÊýÖµ·Ö±ð´æÈëadcÖµµÄÔÝ´æÊý×éµ±ÖÐ
				£¨3£©Çå³ýdmaÈí¼þ±êÖ¾Î»
-------------------------------------------------------------------------*/
void DMA1_Channel1_IRQHandler(void)
{	
	//int j = 0;
	//uint16_t Adc_convert1=0,Adc_v1 = 0,Adc_v2=0;//Adc_convert2=0;
	//uint16_t temp1=0,temp2=0;//adc1 = 0;
	
//	for( j = 0;j<sample_val;j++)//È¡4¸öÊý×öÆ½¾ùÖµ
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


