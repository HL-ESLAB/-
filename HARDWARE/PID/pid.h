#ifndef __PID_H
#define __PID_H

#include "sys.h"

//自定义PID结构体
typedef struct
{
   float target_val;    //目标值
   float actual_val;    //实际值
   float err;           //偏差值
   float err_last;      //上一个偏差值
   float Kp, Ki, Kd;    //比例、积分、微分系数
   float integral;      //积分值
	 float result;			  //pid结果
	 float speed;					//定义速度值
   
}PID;

extern PID pid;

void PID_Init(void);
float PID_realize(float target_val, float actual_val);
#endif

