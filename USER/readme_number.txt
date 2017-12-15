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

     //����GPIOΪģ������ģʽ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
       //GPIO_InitStructure������Ա��ʼ��
        //GPIO_Init(GPIO_TypeDef * GPIOx, GPIO_InitTypeDef * GPIO_InitStruct);

         ADC_StructInit(&ADC_InitStructure);

    ADC_VoltageRegulatorCmd(ADC1, ENABLE);
     delay_us(10);
  
  //���Calibration

        //Common Init
      ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   //ADC_CommonInitStructure������Ա��ʼ��
   ADC_CommonInit(ADC1,&ADC_CommonInitStructure);

         //ADC Init
 ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
     ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
     ADC_InitStructure.ADC_NbrOfRegChannel = 1;
 //ADC_InitStructure������Ա��ʼ��
     
  ADC_Init(ADC1,&ADC_InitStructure);

     //����ADC��ͨ�����������ڵ�
   ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_19Cycles5);
       //���¶ȴ�����
  ADC_TempSensorCmd(ADC1, ENABLE);
   //��ADC
    ADC_Cmd(ADC1,ENABLE);

  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
     //��ʼת��
     ADC_StartConversion(ADC1); 
}


//��ȡ�¶�ֵ����
uint16_t GetTemperature(void)
{
 u16 t,temp,Temperature;
        
  t=ADC_GetConversionValue(USING_ADC);//��ȡADC��ֵ

  temp = t*3300/0xFFF;//����ȡ����ֵת��Ϊ��ѹֵ
 Temperature = (1430-temp)*10/43+25;//����ѹֵת��Ϊ�¶�ֵ(��)

     return Temperature;
}

//���Ժ�����ÿ��0.5���ȡһ�β���ֵ
//���Ѳ��Ե����¶�ͨ�����ڴ�ӡ��PC�Ĵ���������ʾ����
void TempSensortest(void)
{
   u16 tmp=0;
     
  while(1)
           {
                  delay_ms(500);
                     tmp = GetTemperature();
                    printf("The Temperature = %d ��\n",tmp);
            }
}

//printf()�������ض���ʹ�����򴮿ڴ�ӡ����
int fputc(int ch,FILE *f)
{
       
  USART_SendData(USART2, ch);
    
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET) ;
     
  return(ch);    
}