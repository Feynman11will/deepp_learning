#include "datadeal.h"
#include "math.h"
#include "stdio.h"



float temp1max = 0,temp1min = 3000;
float temp2max = 0,temp2min = 3000;
/*——————————————————————————————
函数名称：Data_deal
输入：上次采集的电压u16 Old_Val,这一次输出的电压值u16 New_Vol
	a=调节参数（0<a<1）
输出：这一次的输出值的电压值u16 New_Out
函数说明：将上一次返回的结果和此次的结果带入到差分方程当中，
		实现一阶惯性环节的滤波
——————————————————————————————*/
uint16_t  Data_deal(uint16_t  Old_Val,uint16_t  New_Vol,float a)
{
	uint16_t New_Out = 9;

	New_Out = a*New_Vol + (1-a)*Old_Val;
	return New_Out;	
}

/*———————————————————————————————————————————————————————————————
函数名称：Data_Con2Ang
输入：两路滤波的结果u16 Val_1 u16 Val_2，float temp1max  temp1min  temp2ax  temp2min
输出：
函数说明:
————————————————————————————————————————————————————————————————*/
void Data_Con2Ang(uint32_t Val_1,uint32_t Val_2,float t1max ,float t1min, float t2max,  float t2min)
{
	//输入波形的函数t2为使用绿色波形作为t2，为正弦波形Asin(2W)，蓝色波形为相位之后45度Asin(2w+45)。
	//先求幅值
		float A1 = 0,A2 = 0;
		float a11 = 0, a22 = 0;
		double deg1=0,deg2=0;
		A1 = (t1max - t1min)/2.0;
		a11= (t1max + t1min)/2.0;
		A2 = (t2max - t2min)/2.0;
		a22= (t2max + t2min)/2.0;
		deg1 = (180.0/3.1416*asin((double)(Val_1 - a11)/A1)-45)/2.0;
		deg2 = (180.0/3.1416*asin((double)(Val_2 - a22)/A2))/2.0;	
		printf("%lf %lf\r",deg1,deg2);
}

/*——————————————————————————————
函数名称:Get_Gain
输入：t1和t2的值float val1,float val2
输出：增益数组float Gain[2]，其中Gain[0]是t1的总的放大倍数，
	Gain[1]是t2总的放大倍数。
函数说明：获取一次总的放大倍数，并将总的放大倍数返回一个float值
		然后使用40作为放大前
		
——————————————————————————————*/
/*————————————————————————————————————————
函数名称：Data_Cv
输入：要处理的数组
输出：无
描述：将数据批处理，将一组数值进行冒泡排序处理，求数组的平均值，最后将结果打印出去
————————————————————————————————————————*/


