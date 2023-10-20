#include "pid.h"
#include "usart.h"

/*******
* ������������ѣ���С����˳��飬
* ���Ǳ�������֣�����ٰ�΢�ּӣ�
* �����񵴺�Ƶ������������Ҫ�Ŵ�
* ����Ư���ƴ��壬����������С�⣬
* ����ƫ��ظ���������ʱ�����½���
* ���߲������ڳ�������ʱ���ټӳ���
* ������Ƶ�ʿ죬�Ȱ�΢�ֽ�������
* ���������������΢��ʱ��Ӧ�ӳ���
* ����������������ǰ�ߺ��4��1��
* һ�������������������������͡�
*/
PID pid;
void PID_Init(void)
{
		printf("PID_init begin\r\n");
    /*pid������ʼ��*/
    pid.target_val = 0.0;				//���õ������ٶ�
    pid.actual_val = 0.0;				//ʵ���ٶ�ֵ
    pid.err = 0.0;							//��ǰ��ʵ���������ƫ��
    pid.err_last = 0.0;					//��һ�ε�ƫ��
		pid.speed = 0.0;						//�����ٶ�ֵ
		pid.integral = 0.0;					//����ֵ
    pid.Kp = 0.25;
    pid.Ki = 0.015;
    pid.Kd = 0.2; 
		printf("PID_init end\r\n");
}


//λ��ʽPID�㷨ʵ��
float PID_realize(float target_val, float actual_val)
{
    /*����ʵ��ֵ*/
		pid.actual_val = actual_val;
		pid.target_val = target_val;
    /*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid.err = pid.target_val - pid.actual_val;
		printf("target_val is %f\r\n", pid.target_val);
		printf("actual_val is %f\r\n", pid.actual_val);
    /*����ۻ�*/
    pid.integral += pid.err;
	
    /*pid�㷨ʵ��*/
    pid.result = pid.Kp * pid.err + 
                     pid.Ki * pid.integral + 
                     pid.Kd * (pid.err - pid.err_last);
	//�޷�����
		if(pid.result < 1100)
			pid.result = 1100;
		else if(pid.result > 2000)
			pid.result = 2000;
    /*����*/
    pid.err_last = pid.err;
    /*���ص�ǰʵ��ֵ*/
    return pid.result;

}
