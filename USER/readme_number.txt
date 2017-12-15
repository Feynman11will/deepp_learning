#include "ADConverter.h"
void ADC_Config(void)
{
     ADC_InitTypeDef       ADC_InitStructure;
        ADC_CommonInitTypeDef ADC_CommonInitStructure;
     GPIO_InitTypeDef      GPIO_InitStructure;
   
  //select ADC colock
        RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div6);
    //enable peripheral colock
 RCC_AHBPeriphClockCmd(ADC_PORT_CLK,ENABLE);
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12,ENABLE);

     //配置GPIO为模拟输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
       //GPIO_InitStructure其他成员初始化
        //GPIO_Init(GPIO_TypeDef * GPIOx, GPIO_InitTypeDef * GPIO_InitStruct);

         ADC_StructInit(&ADC_InitStructure);

    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
     delay_us(10);
  
  //添加Calibration

        //Common Init
      ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   //ADC_CommonInitStructure其他成员初始化
   ADC_CommonInit(ADC1,&ADC_CommonInitStructure);

         //ADC Init
 ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
     ADC_InitStructure.ADC_NbrOfRegChannel = 1;
 //ADC_InitStructure其他成员初始化
     
  ADC_Init(ADC1,&ADC_InitStructure);

     //配置ADC的通道，采样周期等
   ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_19Cycles5);
       //打开温度传感器
  ADC_TempSensorCmd(ADC1, ENABLE);
   //打开ADC
    ADC_Cmd(ADC1,ENABLE);

  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
     //开始转换
     ADC_StartConversion(ADC1); 
}


//获取温度值函数
uint16_t GetTemperature(void)
{
 u16 t,temp,Temperature;
        
  t=ADC_GetConversionValue(USING_ADC);//读取ADC数值

  temp = t*3300/0xFFF;//将读取的数值转换为电压值
 Temperature = (1430-temp)*10/43+25;//将电压值转换为温度值(℃)

     return Temperature;
}

//测试函数，每隔0.5秒读取一次测试值
//并把测试到的温度通过串口打印到PC的串口助手显示出来
void TempSensortest(void)
{
   u16 tmp=0;
     
  while(1)
           {
                  delay_ms(500);
                     tmp = GetTemperature();
                    printf("The Temperature = %d ℃\n",tmp);
            }
}

//printf()函数的重定向，使其能向串口打印数据
int fputc(int ch,FILE *f)
{
       
  USART_SendData(USART2, ch);
    
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET) ;
     
  return(ch);    
}