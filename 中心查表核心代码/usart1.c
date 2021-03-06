/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/
#include "usart1.h"
#include <stdarg.h>

/*————————————————————————————————————————————————————
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1，同时打开kma，将数据通过dma传输到io口去
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 ————————————————————————————————————————————————————*/
void USART1_Config(void)
{
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
		/* 配置串口1 （USART1） 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	/*串口GPIO端口配置*/
  /* 配置串口1 （USART1 Tx (PA.09)）*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* 配置串口1 USART1 Rx (PA.10)*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
		/* 串口1工作模式（USART1 mode）配置 */
	USART_InitStructure.USART_BaudRate = 256000;//一般设置为9600;最高是460800
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);//使能串口
	USART_DMACmd(USART1, USART_DMAReq_Tx  ,ENABLE);
	
}

void MY_usDMA_Enable(u16 ndtr)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);                      //关闭DMA传输   
		
	DMA_SetCurrDataCounter(DMA1_Channel1,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA1_Channel1, ENABLE);                      //开启DMA传输 
}	  
/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
//int fputc(int ch, FILE *f)
//{
//	/* 将Printf内容发往串口 */
//	USART_SendData(USART1, (unsigned char) ch);
//	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
//	return (ch);
//}

int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
