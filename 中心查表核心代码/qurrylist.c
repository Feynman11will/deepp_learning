
#include "qurrylist.h"//�����
#include "vol2ang.h"
 #include "math.h"
#include "main.h"
/*-----------------------------------------
	�������ʹ�õĲ�����
	Rl_point �������ֵ��ѹ��Ϊ�����Ҳ����м�ֵ
	Exch_Up_Point����·��ֵ�ϵ�ѹ
	Exch_Down_Point����·��ֵ�µ�ѹ
	Magnification����·���ܷŴ���
	Amplitude���źŵ�ѹ�Ŵ���
-------------------------------------------*/
#define amplitude 7
#define Up_max1		temp.vol1_max //-amplitude     //3695//3696
#define Down_min1 	temp.vol1_min   //1304
#define Up_max2 	temp.vol2_max //-amplitude    //3695//3696
#define Down_min2 	temp.vol2_min  	 //1304

#define amplitude1 amp1
#define amplitude2 amp2

#define Rl_point1  center_point1 	//���ĵ��ѹ
#define Rl_point2  center_point2 	//���ĵ��ѹ

#define Exch_Up_Point1      (center_point1 + amp1*0.70711) //�ϵ�ѹ�л��� 
#define Exch_Down_Point1    (center_point1 - amp1*0.70711)//�µ�ѹ�л���
 
#define Exch_Up_Point2       (center_point2 + amp2*0.70711)	//�ϵ�ѹ�л���
#define Exch_Down_Point2     (center_point2 - amp2*0.70711) //�µ�ѹ�л���

float angle= 0;//����ĽǶȴ洢��angleֵ����
#define Vol_Err  (center_point1-center_point2)//��·��ѹ���
/*---------------------------------------------------
�������ƣ�Qurry_List
��		�룺uint16_t v1,uint16_t v2
��		��: ��
����������������������������ʹ�ò��ķ�����
					1��ʵ�ַ����Ǻ����ļ��㣬
					2������sin��cos��������ԣ�ʵ��0��180��float����
						 �Ƕ�ֵ��ӡ��������ʾ���ϡ�
---------------------------------------------------*/
void Qurry_List(uint16_t v1,uint16_t v2)
{
	
	if(error<=0)//�����ĵ�С��Ϊ�����в��v1
	{
		v2 = v2+error;
	}
	else
	{v1+=error;}
	
	angle = atan2((float)(v1-Rl_point1)/amp1,(float)(v2-Rl_point1)/amp1)/2*180/pi+90;
//	  array1;//sin(0,45)��ʼ���׵�ַ���������ĵ��
//    array2;//sin(0,-45)��ʼ���׵�ַ���ĵ�λ�ô���ֵ��0С�����ĵ��
//	  array3;//cos(45,0)��ʼ���׵�ַС�����ĵ��
//    array4;//cos(45,90)��ʼ���׵�ַ�������ĵ��
	
//	
//	if(error<=0)//�����ĵ�С��Ϊ�����в��v1
//	{
//		v2 = v2+error;
//	}
//	else
//	{v1+=error;}
	/*
	�����ǲ�������ִ���
	*/
//		if(( v2 >=Rl_point1 )&&(v1 >= Rl_point1))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Rl_point1),array1); //sin(0,45)��ʼ���׵�ַ���������ĵ��
//				else
//				angle = qurry_index((v2-Rl_point1),array3); //cos(45,0)��ʼ���׵�ַС�����ĵ��
//			}//0��45��
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point1))	//�ж���·��ѹֵ���ڵڶ�����
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((Rl_point1-v2),array4);//  array4;//cos(45,90)��ʼ���׵�ַ�������ĵ��
//				else
//				angle = 90 - qurry_index((v1-Rl_point1) ,array1); //sin(0,45)��ʼ���׵�ַ���������ĵ��
//				
//			}//45��90��ͼ������һ���ε�ʱ����45�л���ʱ����ͻ�䡣����Ϊ90��
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point1)) //�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((Rl_point1-v1) ,array2);
//				else
//				angle = 180 - qurry_index((v2-Rl_point1),array4);
//			 
//			}//90��135��
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point1))		//�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>=Exch_Down_Point1)
//					 
//				angle = 180+qurry_index((Rl_point1-v1),array2);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Rl_point1) ,array3);
//					//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135��180��
//			else
//if(( v2 >=Rl_point2 )&&(v1 >= Rl_point1))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Rl_point1),array1); //sin(0,45)��ʼ���׵�ַ���������ĵ��
//				else
//				angle = qurry_index((v2-Rl_point2),array3); //cos(45,0)��ʼ���׵�ַС�����ĵ��
//			}//0��45��
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point2))	//�ж���·��ѹֵ���ڵڶ�����
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((Rl_point2-v2),array4);//  array4;//cos(45,90)��ʼ���׵�ַ�������ĵ��
//				else
//				angle = 90 - qurry_index((v1-Rl_point1) ,array1); //sin(0,45)��ʼ���׵�ַ���������ĵ��
//				
//			}//45��90��ͼ������һ���ε�ʱ����45�л���ʱ����ͻ�䡣����Ϊ90��
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point2)) //�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((Rl_point1-v1) ,array2);
//				else
//				angle = 180 - qurry_index((v2-Rl_point2),array4);
//			 
//			}//90��135��
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point2))		//�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>=Exch_Down_Point1)
//					 
//				angle = 180+qurry_index((Rl_point1-v1),array2);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Rl_point2) ,array3);
//					//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135��180��
//			else
//				printf("��ֵ������");
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
//			}//0��45��
//			else
//			if(v1>Rl_point1 && (v2<=Rl_point2))	//�ж���·��ѹֵ���ڵڶ�����
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((v2-Down_min2) ,array2);
//				else
//					//angle = (90-sinlist[v2-Lowest]);
//				angle = 90 - qurry_index((v1-Down_min1) ,array1);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//				
//			}//45��90��ͼ������һ���ε�ʱ����45�л���ʱ����ͻ�䡣����Ϊ90��
//			else
//			if((v1<=Rl_point1)&& (v2<=Rl_point2)) //�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((v1-Down_min1) ,array1);
//				else
//				angle = 180 - qurry_index((v2-Down_min2) ,array2);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//			}//90��135��
//			else
//				if((v1<=Rl_point1)&&(v2>Rl_point1))		//�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>=Exch_Down_Point1)
//					//angle=(180+sinlist[v2-Lowest]);
//				angle = 180+qurry_index((v1-Down_min1),array1);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Down_min1) ,array2);
//			//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135��180��
//			else
//				printf("��ֵ������");
//		
//	else//��v2Ϊ�����в��
//	{
//		v1 = v1+error;
//		if(( v2 >Rl_point2 )&&(v1 > Rl_point2))
//			{	
//				if(v1<=Exch_Up_Point1)
//				angle = qurry_index((v1-Down_min2),array1);
//				else
//				angle = qurry_index((v2-Down_min2),array2);
//			}//0��45��
//			else
//			if(v1>Rl_point2 && (v2<=Rl_point2))	//�ж���·��ѹֵ���ڵڶ�����
//			{
//				if(v1>Exch_Up_Point1)
//				angle = qurry_index((v2-Down_min2) ,array2);
//				else
//					//angle = (90-sinlist[v2-Lowest]);
//				angle = 90 - qurry_index((v1-Down_min2) ,array1);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//				
//			}//45��90��ͼ������һ���ε�ʱ����45�л���ʱ����ͻ�䡣����Ϊ90��
//			else
//			if((v1<=Rl_point2)&& (v2<=Rl_point2)) //�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>Exch_Down_Point1)
//				angle = 90 - qurry_index((v1-Down_min2) ,array1);
//				else
//				angle = 180 - qurry_index((v2-Down_min2) ,array2);
//				//printf("%f\n",(90-sinlist[v2-Lowest]));
//			}//90��135��
//			else
//				if((v1<=Rl_point2)&&(v2>Rl_point2))		//�ж���·��ѹֵ���ڵ�������
//			{
//				if(v1>=Exch_Down_Point1)
//					//angle=(180+sinlist[v2-Lowest]);
//				angle = 180+qurry_index((v1-Down_min2),array1);
//				else
//					//angle =  180- coslist[v1-Lowest];
//				angle =180 - qurry_index((v2-Down_min2) ,array2);
//			//	printf("%f\n",(180+sinlist[v2-Lowest]));     
//			}// 135��180��
//			else
//				printf("��ֵ������");
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
�������ƣ�printAngle
����������������ĽǶ�ֵAngleֵ��ӡ��ȥ
������������ͣ�float ����ang
���������void
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

