#ifndef __PID_H
#define __PID_H

#include "sys.h"

//�Զ���PID�ṹ��
typedef struct
{
   float target_val;    //Ŀ��ֵ
   float actual_val;    //ʵ��ֵ
   float err;           //ƫ��ֵ
   float err_last;      //��һ��ƫ��ֵ
   float Kp, Ki, Kd;    //���������֡�΢��ϵ��
   float integral;      //����ֵ
	 float result;			  //pid���
	 float speed;					//�����ٶ�ֵ
   
}PID;

extern PID pid;

void PID_Init(void);
float PID_realize(float target_val, float actual_val);
#endif

