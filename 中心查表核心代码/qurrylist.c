
#include "qurrylist.h"//查表函数
#include "vol2ang.h"
 #include "math.h"
#include "main.h"
/*-----------------------------------------
	查表函数中使用的参数：
	Rl_point ：相对零值电压，为正余弦波的中间值
	Exch_Up_Point：换路阈值上电压
	Exch_Down_Point：换路阈值下电压
	Magnification：电路板总放大倍数
	Amplitude：信号电压放大倍数
-------------------------------------------*/
#define amplitude 7
#define Up_max1		temp.vol1_max //-amplitude     //3695//3696
#define Down_min1 	temp.vol1_min   //1304
#define Up_max2 	temp.vol2_max //-amplitude    //3695//3696
#define Down_min2 	temp.vol2_min  	 //1304

#define amplitude1 amp1
#define amplitude2 amp2

#define Rl_point1  center_point1 	//中心点电压
#define Rl_point2  center_point2 	//中心点电压

#define Exch_Up_Point1      (center_point1 + amp1*0.70711) //上电压切换点 
#define Exch_Down_Point1    (center_point1 - amp1*0.70711)//下电压切换点
 
#define Exch_Up_Point2       (center_point2 + amp2*0.70711)	//上电压切换点
#define Exch_Down_Point2     (center_point2 - amp2*0.70711) //下电压切换点

float angle= 0;//输出的角度存储在angle值当中
#define Vol_Err  (center_point1-center_point2)//两路电压相差
/*---------------------------------------------------
函数名称：Qurry_List
输		入：uint16_t v1,uint16_t v2
输		出: 无
函数描述：将函数传递来的数据使用查表的方法，
					1、实现反三角函数的计算，
					2、根据sin和cos的输出特性，实现0到180的float类型
						 角度值打印到电脑显示屏上。
---------------------------------------------------*/
void Qurry_List(uint16_t v1,uint16_t v2)
{
	
	if(error<=0)//以中心点小的为主进行查表v1
	{
		v2 = v2+error;
	}
	else
	{v1+=error;}
	
	angle = atan2((float)(v1-Rl_point1)/amp1,(float)(v2-Rl_point1)/amp1)/2*180/pi+90;
//	  array1;//sin(0,45)开始的首地址，大于中心点表
//    array2;//sin(0,-45)开始的首地址中心点位置处的值是0小于中心点表
//	  array3;//cos(45,0)开始的首地址小于中心点表
//    array4;//cos(45,90)开始的首地址大于中心点表
	
//	
//	if(error<=0)//以中心点小的为主进行查表v1
//	{
//		v2 = v2+error;
//	}
//	else
//	{v1+=error;}
	/*
	以下是查表函数部分代码
	*/
//		if(( v2 >=Rl_point1 )&&(v1 >= Rl_point1))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Rl_point1),array1); //sin(0,45)开始的首地址，大于中心点表
//				else
//				angle = qurry_index((v2-Rl_point1),array3); //cos(45,0)开始的首地址小于中心点表
//			}//0到45度
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point1))	//判断两路电压值的在第二区间
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((Rl_point1-v2),array4);//  array4;//cos(45,90)开始的首地址大于中心点表
//				else
//				angle = 90 - qurry_index((v1-Rl_point1) ,array1); //sin(0,45)开始的首地址，大于中心点表
//				
//			}//45到90度图形在这一个段的时候，在45切换的时候发生突变。幅度为90度
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point1)) //判断两路电压值的在第三区间
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((Rl_point1-v1) ,array2);
//				else
//				angle = 180 - qurry_index((v2-Rl_point1),array4);
//			 
//			}//90到135度
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point1))		//判断两路电压值的在第四区间
//			{
//				if(v1>=Exch_Down_Point1)
//					 
//				angle = 180+qurry_index((Rl_point1-v1),array2);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Rl_point1) ,array3);
//					//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135到180度
//			else
//if(( v2 >=Rl_point2 )&&(v1 >= Rl_point1))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Rl_point1),array1); //sin(0,45)开始的首地址，大于中心点表
//				else
//				angle = qurry_index((v2-Rl_point2),array3); //cos(45,0)开始的首地址小于中心点表
//			}//0到45度
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point2))	//判断两路电压值的在第二区间
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((Rl_point2-v2),array4);//  array4;//cos(45,90)开始的首地址大于中心点表
//				else
//				angle = 90 - qurry_index((v1-Rl_point1) ,array1); //sin(0,45)开始的首地址，大于中心点表
//				
//			}//45到90度图形在这一个段的时候，在45切换的时候发生突变。幅度为90度
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point2)) //判断两路电压值的在第三区间
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((Rl_point1-v1) ,array2);
//				else
//				angle = 180 - qurry_index((v2-Rl_point2),array4);
//			 
//			}//90到135度
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point2))		//判断两路电压值的在第四区间
//			{
//				if(v1>=Exch_Down_Point1)
//					 
//				angle = 180+qurry_index((Rl_point1-v1),array2);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Rl_point2) ,array3);
//					//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135到180度
//			else
//				printf("幅值有问题");
			printAngle(angle);
}

//void Qurry_List(uint16_t v1,uint16_t v2)
//{
//		if(( v2 >Rl_point2 )&&(v1 > Rl_point1))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Down_min1),array1);
//				else
//				angle = qurry_index((v2-Down_min2),array2);
//			}//0到45度
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point2))	//判断两路电压值的在第二区间
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((v2-Down_min2) ,array2);
//				else
//					//angle = (90-sinlist[v2-Lowest]);
//				angle = 90 - qurry_index((v1-Down_min1) ,array1);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//				
//			}//45到90度图形在这一个段的时候，在45切换的时候发生突变。幅度为90度
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point2)) //判断两路电压值的在第三区间
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((v1-Down_min1) ,array1);
//				else
//				angle = 180 - qurry_index((v2-Down_min2) ,array2);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//			}//90到135度
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point1))		//判断两路电压值的在第四区间
//			{
//				if(v1>=Exch_Down_Point1)
//					//angle=(180+sinlist[v2-Lowest]);
//				angle = 180+qurry_index((v1-Down_min1),array1);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Down_min1) ,array2);
//			//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135到180度
//			else
//				printf("幅值有问题");
//		
//	else//以v2为主进行查表
//	{
//		v1 = v1+error;
//		if(( v2 >Rl_point2 )&&(v1 > Rl_point2))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Down_min2),array1);
//				else
//				angle = qurry_index((v2-Down_min2),array2);
//			}//0到45度
//			else
//			if(v1>Rl_point2 && (v2<=Rl_point2))	//判断两路电压值的在第二区间
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((v2-Down_min2) ,array2);
//				else
//					//angle = (90-sinlist[v2-Lowest]);
//				angle = 90 - qurry_index((v1-Down_min2) ,array1);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//				
//			}//45到90度图形在这一个段的时候，在45切换的时候发生突变。幅度为90度
//			else
//			if((v1<=Rl_point2)&& (v2<=Rl_point2)) //判断两路电压值的在第三区间
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((v1-Down_min2) ,array1);
//				else
//				angle = 180 - qurry_index((v2-Down_min2) ,array2);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//			}//90到135度
//			else
//				if((v1<=Rl_point2)&&(v2>Rl_point2))		//判断两路电压值的在第四区间
//			{
//				if(v1>=Exch_Down_Point1)
//					//angle=(180+sinlist[v2-Lowest]);
//				angle = 180+qurry_index((v1-Down_min2),array1);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Down_min2) ,array2);
//			//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135到180度
//			else
//				printf("幅值有问题");
//	}
//	
//			printAngle(angle);
//}
//if((center_point1<=v1<=Exch_Up_Point1)&&(center_point2<=v2))
//{
//	angle = qurry_index((v1-Down_min1),array1);//(0,22.5)
//}

//if((Exch_Up_Point1<=v1)&&(center_point2<=v2 ))
//{
//	angle = qurry_index((v2-Down_min2),array2);//(22.5,45)
//}

//if((Exch_Up_Point1<=v1)&&(v2<=center_point2))
//{
//	angle = qurry_index((v2-Down_min2) ,array2);//(45,60.5)
//}

//if((center_point2>=v2)&&(Exch_Down_Point1<= v1<=Exch_Up_Point1))
//{
//		angle = 90 - qurry_index((v1-Down_min1) ,array1);//(60.5,90,115.5)
//}

//if((Exch_Down_Point1>=v1)&&(v2<=center_point2))
//{
//	angle = 180 - qurry_index((v2-Down_min2) ,array2);//(115.5,135)
//}
//if((center_point2<=v2<=Exch_Up_Point2)&&((center_point1)>=v1))
//{
//	angle =180 - qurry_index((v2-Down_min2) ,array2);//(135,160.5)
//}
//if( (Exch_Up_Point2<=v2)&&(v1<=center_point2))
//{
// angle = 180+qurry_index((v1-Down_min1),array1);//(160.5,180)
//}


	//	printAngle(angle);
//wprintf("%d %d\n",v1,v2);



/*--------------------------------------------------
函数名称：printAngle
函数描述：将输出的角度值Angle值打印出去
输入参数：类型：float 名称ang
输出参数：void
-------------------------------------------------*/
void printAngle(float ang)
{

	printf("%f\n",ang); 
//	}
}

float qurry_index(uint16_t index , uint32_t arr)
{
	if((uint32_t)(arr+4*index)<=(uint32_t)0X0801A000)
	{
		if(arr == array2)
		{
			return -(float)(*(uint32_t *)(arr+ 4*index))/100000.0;
		}
		return (float)(*(uint32_t *)(arr+ 4*index))/100000.0;
	}
	else return 101010;
}

