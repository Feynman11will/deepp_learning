
/**
  ******************************************************************************
  * @file    MyFlash.c
  * @author  femanwill
  * @version V1.2.0
  * @date    2017年12月6日
  * @brief   
  ******************************************************************************
  * @attention

  */
 
#include "MyFlash.h"
#include "stm32f10x_flash.h"
#include "main.h"

uint32_t EraseCounter = 0x01, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
//volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
//volatile TestStatus MemoryProgramStatus = PASSED;
#define BANK1_WRITE_START_ADDR  0x0807F000
#define FLASH_PAGE_SIZE 1024 //1024bytes
#define ProgramTimeout        ((uint32_t)0x00002000)
/**
  * @brief  write flash
  * @note   please think about dont erase code secter
  * @param  Addrass WriteAddr ,WriteAddr shuold large than x0807F000,and must 是4的倍数，是起始地址
	*	@param float num the number you want to write
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  *     @arg FLASH_Latency_2: FLASH Two Latency cycles
  * @retval None
  */
	
//在使用该函数之前先解锁stm32f103 
//  FLASH_UnlockBank1();
void Write_flash(uint32_t WriteAddr,float num)
{

	
	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址，必须是4的倍数
	
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE*EraseCounter));
  }
	
	FLASH_ProgramWord(WriteAddr, (uint32_t)num*100000);//保留小数点后5位
	
}















