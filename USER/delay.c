#include "delay.h"
//#include "sys.h"

static u8  fac_us=0;							//us?????			   
static u16 fac_ms=0;							//ms?????,?os?,???????ms?
	

void delay_init(void)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=72000000/8000000;						//??????OS,fac_us?????
	fac_ms=(u16)fac_us*1000;				//?OS?,????ms???systick???   
}								    

//nus?????us?.	
//??:nus??,????798915us(????2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//????	  		 
	SysTick->VAL=0x00;        				//?????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //???? 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//??????   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //?????
	SysTick->VAL =0X00;       				//????? 
}
//??nms
//??nms???
//SysTick->LOAD?24????,??,?????:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK???Hz,nms???ms
//?168M???,nms<=798ms 
static void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//????(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;           			//?????
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //???? 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//??????   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //?????
	SysTick->VAL =0X00;     		  		//?????	  	    
} 
//??nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//???540,??????????????,
											//?????248M???,delay_xms??????541ms???
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 

			 
