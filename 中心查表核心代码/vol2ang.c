
/*
  ******************************************************************************
  * @file    vol2ang.c
  * @author  feman_will
  * @version V1.1.0
  * @date    5-December-2017
  * @brief   this code file provide all the function that dealwith sampled voltage_value
	*	by adc3 and adc4
  ******************************************************************************
*/

#include "vol2ang.h"
#include <stdlib.h>
#include "usart1.h"
#include "main.h"
#include "math.h"
#include "MyFlash.h"
#include "stm32f10x_flash.h"
 Vol_limit temp={0,0,1500,1500};
 Vol_limit buffer={0,0,0,0};
float amp1 = 0;
float amp2  =0;
float center_point1 = 0;
float center_point2 = 0;
#define turn 5
float error = 0;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;      
/**
  * @brief  Get the Limit voltage value
  * @param  uint_16 voltage1 sampled by adc3 
	* @param  uint_16 voltage2 sampled by adc4 
  *   			This parameter can be range 0 to 4095
  * @retval maximun and minimun voltage each of voltage1 and voltage2
  */
Vol_limit get_amplitude(uint16_t vol1,uint16_t vol2)
{
	if((vol1-Vol_Up_threshold)>= 2)
	{
		if((vol1-temp.vol1_max)>=2)
		temp.vol1_max = vol1;
		temp.updata = SET;
		up_flag1 = 1;			//电压上标记位
	}
		if((Vol_Down_threshold-vol1)>=2)//else if(Vol_Down_threshold>vol1)
	{
		if((temp.vol1_min-vol1)>=2)
		temp.vol1_min = vol1;
		temp.updata = SET;
		down_flag1 = 1;		//电压下标记位
	}
	
	//else if(Vol_Up_threshold < vol2)
	if((vol2-Vol_Up_threshold)>=2)
	{
		if((vol2-temp.vol2_max)>=2)
		temp.vol2_max = vol2;
		temp.updata = SET;
		up_flag2 = 1;			//电压上标记位
	}
	  if((Vol_Down_threshold-vol2)>=2)
			//else if(Vol_Down_threshold>vol2)
	{
		if((temp.vol2_min-vol2)>=2)
		temp.vol2_min = vol2;
		temp.updata = SET;
		down_flag2 = 1;		//电压下标记位
	}
	else 
	{
		temp.updata = RESET;
	}
	return temp;
}

/**
  * @brief  Waiting for arry's paramater ok
  * @param  none
	* @param  none
  *   			
  * @retval none
*/
void Wait_paramater_ok(void)
{
	int i=0;
	static uint16_t paramater_flag = 0;
	uint16_t temp1=0,temp2=0;
	//停止adc的运行
	for(i=0; i<5;i++)
	{
		while(paramater_flag != turn)
		{
			temp1 = (ADC_DualConvertedValueTab[0] & 0xFFFF);
			temp2 = ((ADC_DualConvertedValueTab[0] & 0xFFFF0000) >>16);
		//	printf("%d %d\n",temp1,temp2);//检测的时候要用到
			get_amplitude(temp1, temp2);
			
			if((down_flag1 == 1)&& (up_flag1== 1))
			{
				paramater_flag ++;
				printf("第%d 次,还有%d 次\n",paramater_flag,turn-paramater_flag);
				down_flag1 = 0;
				up_flag1 = 0;
			}
		}
		paramater_flag = 0;
		buffer.vol1_max += temp.vol1_max;//回归初始值。
		buffer.vol2_max += temp.vol2_max;
		buffer.vol1_min += temp.vol1_min;
		buffer.vol2_min += temp.vol2_min;
		
		temp.vol1_max=0;//回归初始值。
		temp.vol2_max=0;
		temp.vol1_min = 1500;
		temp.vol2_min=1500;

	}
		buffer.vol1_max /= 5;//求的平均值
		buffer.vol2_max /= 5;
		buffer.vol1_min /= 5;
		buffer.vol2_min /= 5;
		temp = buffer;//最后将平均值赋值给temp
	 
	
	array_maker();//利用参数生成动态数组
}

/**
  * @brief  maker 2 dynamic array1 and array2
  * @param  none
	* @param  none
  *   			Vsin1 = (asind((V11-2500)/amp1))/2;
						Vcos1 = (acosd((V12-2512)/amp1))/2;
  * @retval none
*/
#define ProgramTimeout        ((uint32_t)0x00002000)
flash_status_t status_my;


void array_maker(void)
{
	int i;
	float   array1i = 0;
	uint32_t addrass_temp1 = array1;//sin(0,45)开始的首地址，大于中心点表
  uint32_t addrass_temp2 = array2;//sin(0,-45)开始的首地址中心点位置处的值是0小于中心点表
	uint32_t addrass_temp3 = array3;//cos(45,0)开始的首地址小于中心点表
  uint32_t addrass_temp4 = array4;//cos(45,90)开始的首地址大于中心点表
	
	FLASH_Status status = FLASH_COMPLETE;
	amp1 = (temp.vol1_max - temp.vol1_min)/2;//第一路放大倍数
  amp2 = (temp.vol2_max - temp.vol2_min)/2;//第二路放大倍数
  center_point1 = (temp.vol1_max + temp.vol1_min)/2;		//中心点1
  center_point2 = (temp.vol2_max + temp.vol2_min)/2;		//中心点2
	
	error = center_point1 - center_point2;	//两个中心点之间的误差
	FLASH_Unlock();		//解锁flash，要不然不可以访问falsh的寄存器

	status = FLASH_WaitForLastOperation(ProgramTimeout);//检查寄存器被操作了没有
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   //清楚存储器相关标志位
	
	status_my = FlashErase(array1+1, (uint8_t)40);//擦除40个区域
	
	
  if((FLASHStatus == FLASH_COMPLETE)&&(status_my ==FLASH_SUCCESS))
	 {
//		 	if(error<=0)//如果error小于零就以中心点小的为主，中心点小是v1
//		{
				for(i=0;i<amp1;i++)//生成sin(0,45)度数
				{
					array1i =((float)(asin((float)(i)/amp1))/2.0*180.0/pi);
					delay_us(10);				//必须得等到没有数组访问的时候再来对其进行访问
					FLASH_ProgramWord(addrass_temp1 , (uint32_t)(array1i*100000.0));//保留小数点后5位
					addrass_temp1 += 0x04; 
				}
				array1i=0;
				for(i=0;i<amp1;i++)//sin(0,-45)度数
				{
					array1i =((float)(asin((float)(-i)/amp1))/2.0*180.0/pi);
					delay_us(10);				//必须得等到没有数组访问的时候再来对其进行访问
					FLASH_ProgramWord(addrass_temp2 , (uint32_t)(array1i*100000.0));//保留小数点后5位
					addrass_temp2 += 0x04; 
				}
				array1i=0;
				
				for(i=0;i<amp2;i++)//cos(45,0)开始的首地址小于中心点表
				{ 
				array1i = ((float)((float)acos((i)/amp2))/2.0*180.0/pi);
				delay_us(10);
				FLASH_ProgramWord(addrass_temp3, (uint32_t)(array1i*100000.0));//保留小数点后5位
				addrass_temp3 +=  0x04; 				
				}
				array1i=0;
				for(i=0;i<amp2;i++)//cos(45,90)开始的首地址小于中心点表
				{ 
				array1i = ((float)((float)acos((-i)/amp2))/2.0*180.0/pi);
				delay_us(10);
				FLASH_ProgramWord(addrass_temp4, (uint32_t)(array1i*100000.0));//保留小数点后5位
				addrass_temp4 +=  0x04; 				
				}
		}
//		else
//		{
//				if(error<=0)//如果error小于零就以中心点小的为主，中心点小是v1
//		{
//				for(i=0;i<amp2;i++)//生成sin(0,45)度数
//				{
//					array1i =((float)(asin((float)(i)/amp2))/2.0*180.0/pi);
//					delay_us(10);				//必须得等到没有数组访问的时候再来对其进行访问
//					FLASH_ProgramWord(addrass_temp1 , (uint32_t)(array1i*100000.0));//保留小数点后5位
//					addrass_temp1 += 0x04; 
//				}
//				array1i=0;
//				for(i=0;i<amp2;i++)//sin(0,-45)度数
//				{
//					array1i =((float)(asin((float)(-i)/amp2))/2.0*180.0/pi);
//					delay_us(10);				//必须得等到没有数组访问的时候再来对其进行访问
//					FLASH_ProgramWord(addrass_temp2 , (uint32_t)(array1i*100000.0));//保留小数点后5位
//					addrass_temp2 += 0x04; 
//				}
//				array1i=0;
//				
//				for(i=0;i<amp2;i++)//cos(45,0)开始的首地址小于中心点表
//				{ 
//				array1i = ((float)((float)acos((i)/amp2))/2.0*180.0/pi);
//				delay_us(10);
//				FLASH_ProgramWord(addrass_temp3, (uint32_t)(array1i*100000.0));//保留小数点后5位
//				addrass_temp3 +=  0x04; 				
//				}
//				array1i=0;
//					for(i=0;i<amp2;i++)//cos(45,90)开始的首地址小于中心点表
//				{ 
//				array1i = ((float)((float)acos((-i)/amp2))/2.0*180.0/pi);
//				delay_us(10);
//				FLASH_ProgramWord(addrass_temp4, (uint32_t)(array1i*100000.0));//保留小数点后5位
//				addrass_temp4 +=  0x04; 				
//				}
//		}
//}
	FLASH_Lock();//上锁

//	 sin(0,45)开始的首地址，大于中心点表
//   array2;//sin(0,-45)开始的首地址中心点位置处的值是0小于中心点表
//	 array3;//cos(45,0)开始的首地址小于中心点表
//   array4;//cos(45,90)开始的首地址大于中心点表
	for(i=0;i<amp1;i++)//读取第一个表sin(0,45)开始的首地址，大于中心点表
	{
		printf("%f\n",(float)(*(uint32_t *)(array1+ i*4))/100000 );
	}
	printf(" sin(0,45) 的结束了\n");
	for(i=0;i<amp1;i++)//sin(0,-45)开始的首地址中心点位置处的值是0小于中心点表
	{
		printf("%f\n",(float)(*(uint32_t *)(array2+ i*4 ))/100000 );
	}
		printf("sin(0,-45)的结束了\n");
	for(i=0;i<amp2;i++)//array3;//cos(45,0)开始的首地址小于中心点表
	{
		printf("%f\n",(float)(*(uint32_t *)(array3+ i*4 ))/100000 );
	}
 	printf("cos(45,0)的结束了\n");
	for(i=0;i<amp2;i++)//   array4;//cos(45,90)开始的首地址大于中心点表
	{
		printf("%f\n",(float)(*(uint32_t *)(array4+ i*4 ))/100000 );
	}
		printf("cos(45,90)的结束了\n");

//}
}
#define FLASH_START_ADDR   array1
#define FLASH_END_ADDR     0x0801 FFFF
#define FLASH_SECTOR_NUM    128    
#define FLASH_SECTOR_SIZE   1024 

flash_status_t FlashErase(uint32_t addr, uint8_t count)  
{  
  uint8_t i;  
  
  for(i = 0; i < count; ++i)  
  {  
    if(FLASH_ErasePage(addr + i * FLASH_SECTOR_SIZE) != FLASH_COMPLETE)  
    {  
      return FLASH_FAILURE;  
    }  
  }  

  return FLASH_SUCCESS;  
} 







