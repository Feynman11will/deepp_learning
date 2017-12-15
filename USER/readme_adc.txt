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

 

DMA_DeInit(DMA1_Channel1);                                                   //DMA通道1恢复到默认状态

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //设置数据来源地址

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //设置copy目标地址

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //设置copy方向

DMA_InitStructure.DMA_BufferSize = 16;                                       //设置数组大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //数据来源不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //数据保存的数组地址自增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA操作字长 32位

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA操作字长 32位

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //普通的DMA操作模式：当DMA操作转移够 数组大小                                                                                 //所限定的数据总量后，则不再进行DMA操作

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //关闭mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //使能dma

/* Enable DMA1 Channel1 */

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数2

ADC_Init(ADC1, &ADC_InitStructure);                                           //配置ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //通道配置    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数

ADC_Init(ADC2, &ADC_InitStructure);                                           //配置ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); //通道配置

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //使能ADC2的外部触发模式 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);     //                                                   //使能ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // 使能温度传感器内部参考电压通道

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                      //                             //ADC1的校准 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2的校准

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //软件启动ADC1，开始转换

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

 

/* Test on DMA1 channel1 transfer complete flag */                          //等待一组DMA存储转换完成

while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

/* Clear DMA1 channel1 transfer complete flag */

DMA_ClearFlag(DMA1_FLAG_TC1);

DMA_Done = 1;

}

两个ADC采出来的数据存在32位数组ADC_DualConvertedValueTab[ADC_BufferLength]中，其中ADC1对应低16位，ADC2对应高16位。可通过以下程序读出：

uint16_t adc1_val[1000];

uint16_t adc2_val[1000];

for(i=0;i<1000;i++)

{

adc1_val[i] = ADC_DualConvertedValueTab[i];

adc2_val[i] = ADC_DualConvertedValueTab[i]>>16;

}

最后再附上自己写的用TIM3触发采样的程序，可以自己调用来设置采样率。

程序2：

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

 

//定时器3控制采样率

TIM_TimeBaseStructure.TIM_Prescaler= 0;

TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;

TIM_TimeBaseStructure.TIM_Period= 72000000 / SampleFreq - 1;

TIM_TimeBaseStructure.TIM_ClockDivision= 0;

TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);

TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update ); 

 

TIM_ARRPreloadConfig(TIM3,ENABLE);

TIM_Cmd( TIM3, ENABLE );

 

DMA_DeInit(DMA1_Channel1);                                                   //DMA通道1恢复到默认状态

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //设置数据来源地址

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //设置copy目标地址

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //设置copy方向

DMA_InitStructure.DMA_BufferSize = ADC_BufferLength;                                       //设置数组大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //数据来源不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //数据保存的数组地址自增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA操作字长 32位

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA操作字长 32位

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //普通的DMA操作模式：当DMA操作转移够 数组大小                                                                                 //所限定的数据总量后，则不再进行DMA操作

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //关闭mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //使能dma

/* Enable DMA1 Channel1 */

 

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* DMA Transfer Complete Interrupt enable */

NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;   //通道

NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //占先级

NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应级

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //启动

NVIC_Init(&NVIC_InitStructure);                           //初始化

 

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

 

 

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数2

ADC_Init(ADC1, &ADC_InitStructure);                                           //配置ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5); //通道配置    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数

ADC_Init(ADC2, &ADC_InitStructure);                                           //配置ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5); //通道配置

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //使能ADC2的外部触发模式 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //使能ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // 使能温度传感器内部参考电压通道

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1的校准 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2的校准

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //软件启动ADC1，开始转换

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
程序1 根据官方例子写的：

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

 

DMA_DeInit(DMA1_Channel1);                                                   //DMA通道1恢复到默认状态

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //设置数据来源地址

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //设置copy目标地址

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //设置copy方向

DMA_InitStructure.DMA_BufferSize = 16;                                       //设置数组大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //数据来源不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //数据保存的数组地址自增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA操作字长 32位

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA操作字长 32位

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //普通的DMA操作模式：当DMA操作转移够 数组大小                                                                                 //所限定的数据总量后，则不再进行DMA操作

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //关闭mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //使能dma

/* Enable DMA1 Channel1 */

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数2

ADC_Init(ADC1, &ADC_InitStructure);                                           //配置ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5); //通道配置    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数

ADC_Init(ADC2, &ADC_InitStructure);                                           //配置ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5); //通道配置

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //使能ADC2的外部触发模式 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //使能ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // 使能温度传感器内部参考电压通道

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1的校准 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2的校准

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //软件启动ADC1，开始转换

ADC_SoftwareStartConvCmd(ADC1, ENABLE);

 

/* Test on DMA1 channel1 transfer complete flag */                          //等待一组DMA存储转换完成

while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

/* Clear DMA1 channel1 transfer complete flag */

DMA_ClearFlag(DMA1_FLAG_TC1);

DMA_Done = 1;

}

两个ADC采出来的数据存在32位数组ADC_DualConvertedValueTab[ADC_BufferLength]中，其中ADC1对应低16位，ADC2对应高16位。可通过以下程序读出：

uint16_t adc1_val[1000];

uint16_t adc2_val[1000];

for(i=0;i<1000;i++)

{

adc1_val[i] = ADC_DualConvertedValueTab[i];

adc2_val[i] = ADC_DualConvertedValueTab[i]>>16;

}

最后再附上自己写的用TIM3触发采样的程序，可以自己调用来设置采样率。

程序2：

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

 

//定时器3控制采样率

TIM_TimeBaseStructure.TIM_Prescaler= 0;

TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;

TIM_TimeBaseStructure.TIM_Period= 72000000 / SampleFreq - 1;

TIM_TimeBaseStructure.TIM_ClockDivision= 0;

TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);

TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update ); 

 

TIM_ARRPreloadConfig(TIM3,ENABLE);

TIM_Cmd( TIM3, ENABLE );

 

DMA_DeInit(DMA1_Channel1);                                                   //DMA通道1恢复到默认状态

DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;        //设置数据来源地址

DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;  //设置copy目标地址

DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           //设置copy方向

DMA_InitStructure.DMA_BufferSize = ADC_BufferLength;                                       //设置数组大小

DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             //数据来源不变

DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      //数据保存的数组地址自增

DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;      //DMA操作字长 32位

DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;              //DMA操作字长 32位

DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                //普通的DMA操作模式：当DMA操作转移够 数组大小                                                                                 //所限定的数据总量后，则不再进行DMA操作

DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          //DMA优先级

DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 //关闭mTM

DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                 //使能dma

/* Enable DMA1 Channel1 */

 

DMA_Cmd(DMA1_Channel1, ENABLE);

 

/* DMA Transfer Complete Interrupt enable */

NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;   //通道

NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //占先级

NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //响应级

NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //启动

NVIC_Init(&NVIC_InitStructure);                           //初始化

 

DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

 

 

 

/* ADC1 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC1同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数2

ADC_Init(ADC1, &ADC_InitStructure);                                           //配置ADC1

/* ADC1 regular channels configuration */ 

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5); //通道配置    

/* Enable ADC1 DMA */

ADC_DMACmd(ADC1, ENABLE); 

 

/* ADC2 configuration ------------------------------------------------------*/

ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;                              //ADC2同步规则组模式

ADC_InitStructure.ADC_ScanConvMode = ENABLE;                                  //启动扫描

ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                            //连续转换

ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;           //无外部触发

ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                        //结果右对齐

ADC_InitStructure.ADC_NbrOfChannel = 1;                                       //转换通道数

ADC_Init(ADC2, &ADC_InitStructure);                                           //配置ADC2

/* ADC2 regular channels configuration */ 

ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5); //通道配置

/* Enable ADC2 external trigger conversion */

ADC_ExternalTrigConvCmd(ADC2, ENABLE);                                        //使能ADC2的外部触发模式 

 

 

/* Enable ADC1 */

ADC_Cmd(ADC1, ENABLE);                                                        //使能ADC1

/* Enable Vrefint channel17 */

// ADC_TempSensorVrefintCmd(ENABLE);                                             // 使能温度传感器内部参考电压通道

 

/* Enable ADC1 reset calibration register */   

ADC_ResetCalibration(ADC1);                                                   //ADC1的校准 

/* Check the end of ADC1 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC1));

 

/* Start ADC1 calibration */

ADC_StartCalibration(ADC1);

/* Check the end of ADC1 calibration */

while(ADC_GetCalibrationStatus(ADC1));

 

/* Enable ADC2 */

ADC_Cmd(ADC2, ENABLE);                                                      //使能ADC2

 

/* Enable ADC2 reset calibration register */   

ADC_ResetCalibration(ADC2);                                                 //ADC2的校准

/* Check the end of ADC2 reset calibration register */

while(ADC_GetResetCalibrationStatus(ADC2));

 

/* Start ADC2 calibration */

ADC_StartCalibration(ADC2);

/* Check the end of ADC2 calibration */

while(ADC_GetCalibrationStatus(ADC2));

 

/* Start ADC1 Software Conversion */                                        //软件启动ADC1，开始转换

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

调用方式：ADC1_2_Config(5120,512); 

可以用作FFT分析前的采样。