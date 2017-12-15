#include"adc.h"

 
__IO uint32_t ADC_DualConvertedValueTab[ADC_BufferLength];

void ADC1_2_Init(void)

{

ADC_InitTypeDef ADC_InitStructure;

DMA_InitTypeDef DMA_InitStructure;

GPIO_InitTypeDef GPIO_InitStructure;

 

RCC_ADCCLKConfig(RCC_PCLK2_Div4);

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA, ENABLE);

 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

GPIO_Init(GPIOA, &GPIO_InitStructure);

 

DMA_DeInit(DMA1_Channel1);                                                   //DMAͨ��1�ָ���Ĭ��״̬

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //����������Դ��ַ

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //����copyĿ���ַ

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //����copy����

DMA_InitStructure.DMA_BufferSize = 16;                                       //���������С

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //������Դ����

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //���ݱ���������ַ����

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //��ͨ��DMA����ģʽ����DMA����ת�ƹ� �����С                                                                                 //���޶����������������ٽ���DMA����

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA���ȼ�

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //�ر�mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //ʹ��dma

/* Enable DMA1 Channel1 */

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����2

ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //ͨ������    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����

ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); //ͨ������

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //ʹ��ADC2���ⲿ����ģʽ 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);     //                                                   //ʹ��ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // ʹ���¶ȴ������ڲ��ο���ѹͨ��

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                      //                             //ADC1��У׼ 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //�������ADC1����ʼת��

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

 

/* Test on DMA1 channel1 transfer complete flag */                          //�ȴ�һ��DMA�洢ת�����

while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

/* Clear DMA1 channel1 transfer complete flag */

DMA_ClearFlag(DMA1_FLAG_TC1);

DMA_Done = 1;

}

����ADC�ɳ��������ݴ���32λ����ADC_DualConvertedValueTab[ADC_BufferLength]�У�����ADC1��Ӧ��16λ��ADC2��Ӧ��16λ����ͨ�����³��������

uint16_t adc1_val[1000];

uint16_t adc2_val[1000];

for(i=0;i<1000;i++)

{

adc1_val[i] = ADC_DualConvertedValueTab[i];

adc2_val[i] = ADC_DualConvertedValueTab[i]>>16;

}

����ٸ����Լ�д����TIM3���������ĳ��򣬿����Լ����������ò����ʡ�

����2��

void ADC1_2_Config(u16 SampleFreq,u16 DMA_BufferSizer)

{

ADC_InitTypeDef ADC_InitStructure;

DMA_InitTypeDef DMA_InitStructure;

GPIO_InitTypeDef GPIO_InitStructure;

TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

NVIC_InitTypeDef NVIC_InitStructure;

 

RCC_ADCCLKConfig(RCC_PCLK2_Div2);

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

GPIO_Init(GPIOA, &GPIO_InitStructure);

 

//��ʱ��3���Ʋ�����

TIM_TimeBaseStructure.TIM_Prescaler= 0;

TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;

TIM_TimeBaseStructure.TIM_Period= 72000000 / SampleFreq - 1;

TIM_TimeBaseStructure.TIM_ClockDivision= 0;

TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);

TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update ); 

 

TIM_ARRPreloadConfig(TIM3,ENABLE);

TIM_Cmd( TIM3, ENABLE );

 

DMA_DeInit(DMA1_Channel1);                                                   //DMAͨ��1�ָ���Ĭ��״̬

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //����������Դ��ַ

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //����copyĿ���ַ

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //����copy����

DMA_InitStructure.DMA_BufferSize = ADC_BufferLength;                                       //���������С

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //������Դ����

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //���ݱ���������ַ����

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //��ͨ��DMA����ģʽ����DMA����ת�ƹ� �����С                                                                                 //���޶����������������ٽ���DMA����

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA���ȼ�

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //�ر�mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //ʹ��dma

/* Enable DMA1 Channel1 */

 

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* DMA Transfer Complete Interrupt enable */

NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;   //ͨ��

NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //ռ�ȼ�

NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ��

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //����

NVIC_Init(&NVIC_InitStructure);                           //��ʼ��

 

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

 

 

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����2

ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5); //ͨ������    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����

ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5); //ͨ������

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //ʹ��ADC2���ⲿ����ģʽ 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //ʹ��ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // ʹ���¶ȴ������ڲ��ο���ѹͨ��

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1��У׼ 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //�������ADC1����ʼת��

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

// while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

// /* Clear DMA1 channel1 transfer complete flag */

// DMA_ClearFlag(DMA1_FLAG_TC1);

// DMA_Done = 1;

}

void DMA1_Channel1_IRQHandler(void)

{

if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)

{

DMA_ClearITPendingBit(DMA1_IT_TC1);

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE);

ADC_Cmd(ADC1,DISABLE);

ADC_Cmd(ADC2,DISABLE);

DMA_Done =1;

}

}






/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/
����1 ���ݹٷ�����д�ģ�

#include"adc.h"

 

__IO uint32_t ADC_DualConvertedValueTab[ADC_BufferLength];

void ADC1_2_Init(void)

{

ADC_InitTypeDef ADC_InitStructure;

DMA_InitTypeDef DMA_InitStructure;

GPIO_InitTypeDef GPIO_InitStructure;

 

RCC_ADCCLKConfig(RCC_PCLK2_Div4);

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOA, ENABLE);

 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

GPIO_Init(GPIOA, &GPIO_InitStructure);

 

DMA_DeInit(DMA1_Channel1);                                                   //DMAͨ��1�ָ���Ĭ��״̬

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //����������Դ��ַ

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //����copyĿ���ַ

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //����copy����

DMA_InitStructure.DMA_BufferSize = 16;                                       //���������С

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //������Դ����

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //���ݱ���������ַ����

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //��ͨ��DMA����ģʽ����DMA����ת�ƹ� �����С                                                                                 //���޶����������������ٽ���DMA����

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA���ȼ�

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //�ر�mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //ʹ��dma

/* Enable DMA1 Channel1 */

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����2

ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //ͨ������    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����

ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); //ͨ������

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //ʹ��ADC2���ⲿ����ģʽ 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //ʹ��ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // ʹ���¶ȴ������ڲ��ο���ѹͨ��

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1��У׼ 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //�������ADC1����ʼת��

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

 

/* Test on DMA1 channel1 transfer complete flag */                          //�ȴ�һ��DMA�洢ת�����

while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

/* Clear DMA1 channel1 transfer complete flag */

DMA_ClearFlag(DMA1_FLAG_TC1);

DMA_Done = 1;

}

����ADC�ɳ��������ݴ���32λ����ADC_DualConvertedValueTab[ADC_BufferLength]�У�����ADC1��Ӧ��16λ��ADC2��Ӧ��16λ����ͨ�����³��������

uint16_t adc1_val[1000];

uint16_t adc2_val[1000];

for(i=0;i<1000;i++)

{

adc1_val[i] = ADC_DualConvertedValueTab[i];

adc2_val[i] = ADC_DualConvertedValueTab[i]>>16;

}

����ٸ����Լ�д����TIM3���������ĳ��򣬿����Լ����������ò����ʡ�

����2��

void ADC1_2_Config(u16 SampleFreq,u16 DMA_BufferSizer)

{

ADC_InitTypeDef ADC_InitStructure;

DMA_InitTypeDef DMA_InitStructure;

GPIO_InitTypeDef GPIO_InitStructure;

TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

NVIC_InitTypeDef NVIC_InitStructure;

 

RCC_ADCCLKConfig(RCC_PCLK2_Div2);

RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

 

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

GPIO_Init(GPIOA, &GPIO_InitStructure);

 

//��ʱ��3���Ʋ�����

TIM_TimeBaseStructure.TIM_Prescaler= 0;

TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;

TIM_TimeBaseStructure.TIM_Period= 72000000 / SampleFreq - 1;

TIM_TimeBaseStructure.TIM_ClockDivision= 0;

TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);

TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update ); 

 

TIM_ARRPreloadConfig(TIM3,ENABLE);

TIM_Cmd( TIM3, ENABLE );

 

DMA_DeInit(DMA1_Channel1);                                                   //DMAͨ��1�ָ���Ĭ��״̬

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //����������Դ��ַ

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //����copyĿ���ַ

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //����copy����

DMA_InitStructure.DMA_BufferSize = ADC_BufferLength;                                       //���������С

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //������Դ����

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //���ݱ���������ַ����

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA�����ֳ� 32λ

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //��ͨ��DMA����ģʽ����DMA����ת�ƹ� �����С                                                                                 //���޶����������������ٽ���DMA����

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA���ȼ�

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //�ر�mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //ʹ��dma

/* Enable DMA1 Channel1 */

 

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* DMA Transfer Complete Interrupt enable */

NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;   //ͨ��

NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //ռ�ȼ�

NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ��

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //����

NVIC_Init(&NVIC_InitStructure);                           //��ʼ��

 

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

 

 

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����2

ADC_Init(ADC1, &ADC_InitStructure);                                           //����ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5); //ͨ������    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2ͬ��������ģʽ

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //����ɨ��

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //����ת��

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //���ⲿ����

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //����Ҷ���

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //ת��ͨ����

ADC_Init(ADC2, &ADC_InitStructure);                                           //����ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5); //ͨ������

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //ʹ��ADC2���ⲿ����ģʽ 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //ʹ��ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // ʹ���¶ȴ������ڲ��ο���ѹͨ��

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1��У׼ 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //ʹ��ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2��У׼

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //�������ADC1����ʼת��

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

// while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

// /* Clear DMA1 channel1 transfer complete flag */

// DMA_ClearFlag(DMA1_FLAG_TC1);

// DMA_Done = 1;

}

void DMA1_Channel1_IRQHandler(void)

{

if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)

{

DMA_ClearITPendingBit(DMA1_IT_TC1);

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE);

ADC_Cmd(ADC1,DISABLE);

ADC_Cmd(ADC2,DISABLE);

DMA_Done =1;

}

}

���÷�ʽ��ADC1_2_Config(5120,512); 

��������FFT����ǰ�Ĳ�����