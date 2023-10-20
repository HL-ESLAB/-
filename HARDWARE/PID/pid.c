#include "pid.h"
#include "usart.h"

/*******
* 参数整定找最佳，从小到大顺序查，
* 先是比例后积分，最后再把微分加，
* 曲线振荡很频繁，比例度盘要放大，
* 曲线漂浮绕大湾，比例度盘往小扳，
* 曲线偏离回复慢，积分时间往下降，
* 曲线波动周期长，积分时间再加长，
* 曲线振荡频率快，先把微分降下来，
* 动差大来波动慢，微分时间应加长，
* 理想曲线两个波，前高后低4比1，
* 一看二调多分析，调节质量不会低。
*/
PID pid;
void PID_Init(void)
{
		printf("PID_init begin\r\n");
    /*pid参数初始化*/
    pid.target_val = 0.0;				//设置的期望速度
    pid.actual_val = 0.0;				//实际速度值
    pid.err = 0.0;							//当前次实际与理想的偏差
    pid.err_last = 0.0;					//上一次的偏差
		pid.speed = 0.0;						//控制速度值
		pid.integral = 0.0;					//积分值
    pid.Kp = 0.25;
    pid.Ki = 0.015;
    pid.Kd = 0.2; 
		printf("PID_init end\r\n");
}


//位置式PID算法实现
float PID_realize(float target_val, float actual_val)
{
    /*传入实际值*/
		pid.actual_val = actual_val;
		pid.target_val = target_val;
    /*计算目标值与实际值的误差*/
    pid.err = pid.target_val - pid.actual_val;
		printf("target_val is %f\r\n", pid.target_val);
		printf("actual_val is %f\r\n", pid.actual_val);
    /*误差累积*/
    pid.integral += pid.err;
	
    /*pid算法实现*/
    pid.result = pid.Kp * pid.err + 
                     pid.Ki * pid.integral + 
                     pid.Kd * (pid.err - pid.err_last);
	//限幅处理
		if(pid.result < 1100)
			pid.result = 1100;
		else if(pid.result > 2000)
			pid.result = 2000;
    /*误差传递*/
    pid.err_last = pid.err;
    /*返回当前实际值*/
    return pid.result;

}
