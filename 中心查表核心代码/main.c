#include "stm32f10x.h"
#include "mydma1.h"
#include "usart1.h"
#include "delay.h"
#include "stdio.h"
#include "adc.h"
#include "datadeal.h"
#include "main.h"
#include "vol2ang.h"

uint16_t up_flag1 = 0;			//��ѹ�ϱ��λ
uint16_t down_flag1 = 0;		//��ѹ�±��λ
uint16_t up_flag2 = 0;			//��ѹ�ϱ��λ
uint16_t down_flag2 = 0;		//��ѹ�±��λ
/*��������������������������������������������������������������������������������������������
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 *��������������������������������������������������������������������������������������������*/
u32 ADC_DualConvertedValueTab[sample_val];		//����ȫ�ֱ�������s���鳤����8

int main(void)
{	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
//--------------------------------------------------------------
		delay_init();
		USART1_Config();										//Ĭ�ϲ�����115200
		printf("\r\n ���ڳ�ʼ���ɹ� \r\n");
//--------------------------------------------------------------
		ADC1_2_Init(ADC_DualConvertedValueTab);
		printf("\r\n adc��ʼ���ɹ� \r\n");			//˵�������ʼ���ɹ�
//--------------------------------------------------------------
	  Wait_paramater_ok();//�ȴ�����������ɹ�
	
		Adc_timer();//�򿪶�ʱ������ʹ�ò����
	
		printf("\r\n ��ʱ����ʼ���ɹ�\r\n");
//--------------------------------------------------------------
	while(1)
	{
		
	}
}
