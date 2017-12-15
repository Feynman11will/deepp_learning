#ifndef __VOL2ANG_
#define __VOL2ANG_
#include "main.h"


//#define array1  (uint32_t)0x0801AFFF//数组array1的首地址
#define array1  (uint32_t) 0x08010000//(0,45)sin 10k
#define array2  (uint32_t) 0x08012800//sin(-45,0)10k
#define array3  (uint32_t) 0x08015000  	//cos(90,45)10k
#define array4  (uint32_t) 0x08017800  	//cos(45,0)	10k  //间隔20k以确保擦除不对，一共擦除40k。
extern float error ;
typedef enum  
{   
  FLASH_FAILURE = 0,  
  FLASH_SUCCESS,  
} flash_status_t;  
  
flash_status_t FlashErase(uint32_t addr, uint8_t count);
Vol_limit get_amplitude(uint16_t vol1,uint16_t vol2);
void Wait_paramater_ok(void);
void array_maker(void);
	extern float amp1;
	extern float amp2;
	extern	float center_point1;
	extern float center_point2;
 extern  Vol_limit temp ;
extern float error;
#endif


