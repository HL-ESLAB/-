#include "led.h"
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"
#include "pid.h"
#include "adc.h"
#include "My_Packet.h"


/************************************************
升力反扭测量装置，该装置有四个线程：
压力采集线程
电机控制线程
串口数据发送线程
LED显示线程
调试过程：
2023/06/31
电机的控制部分，主要是电机的PWM控制转速
采用编码器模式采集频率
ADC采集数据模块
************************************************/

//任务优先级
#define START_TASK_PRIO		(tskIDLE_PRIORITY + 1)
#define PWM_CTL_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define LED_IO_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define ADC_TASK_PRIO			(tskIDLE_PRIORITY + 2)
#define USART_TASK_PRIO		(tskIDLE_PRIORITY + 2)

//任务函数
void Start_Task(void *pvParameters);
void LED_Task(void *pvParameters);
void Control_Task(void *pvParameters);
void USART_Task(void *pvParameters);
void ADC_Task(void *pvParameters);
/****************全局变量*****************************/
__IO float Frequency;
extern __IO uint16_t comparePluse;
extern PID pid;
RCC_ClocksTypeDef RCC_CLK;/*  时钟结构体定义  */
float ShengLi = 0;
float NiuLi = 0;
float Speed = 0;
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
 

int main(void)
{
	/*硬件初始化*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 	 
	RCC_GetClocksFreq(&RCC_CLK);                    /*获取系统时钟*/
	if(RCC_CLK.SYSCLK_Frequency != 72000000)
	{
		while(1)
		{
			printf("sysclk is error");
		}/*如果系统时钟不正确，则不运行后面的程序*/
	}
	delay_init();	    				//延时函数初始化	  
	uart_init(115200);					//初始化串口
	LED_Init();		  					//初始化LED
	PWM_Init();		/*初始化PWM*/
	IC_Init();		/*初始化pwm输入捕获*/	
	ADCx_Init();	/*ADC*/ 
	//创建开始任务
   xTaskCreate(Start_Task, "Start_Task", configMINIMAL_STACK_SIZE * 4, NULL, START_TASK_PRIO, NULL);  
	
	//开启任务调度
   vTaskStartScheduler();

		while(1)
		{
		
		}
}

//开始任务任务函数
void Start_Task(void *pvParameters)
{
	//进入临界区
  taskENTER_CRITICAL();           
  //IO控制线程
  xTaskCreate(LED_Task, 			"LED_Task", 			256, 		NULL, 		LED_IO_TASK_PRIO, 		NULL);
	xTaskCreate(Control_Task, 	"Control_Task", 	1024, 	NULL, 		PWM_CTL_TASK_PRIO, 		NULL);   
	xTaskCreate(USART_Task, 		"USART_Task", 		1024, 	NULL, 		USART_TASK_PRIO, 			NULL);
	xTaskCreate(ADC_Task, 			"ADC_Task", 			1024, 	NULL, 		ADC_TASK_PRIO, 				NULL); 	 	
	vTaskDelete(NULL);
	//退出临界区
  taskEXIT_CRITICAL(); 
}

//LED0任务函数 
void LED_Task(void *pvParameters)
{
	TickType_t 	PreviousWakeTime = xTaskGetTickCount();
    while(1)
    {
			PreviousWakeTime = xTaskGetTickCount();
      LED1=~LED1;
      vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ / 10);
    }
}   

void Control_Task(void *pvParameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
		comparePluse++;
		if(comparePluse > 2000)
		{
			comparePluse =2000;
		}else if(comparePluse < 1000)
		{
			comparePluse = 1000;
		}
		TIM_SetCompare1(TIM3, comparePluse);
		Frequency = IC_GetFreq() / 12;
//		printf("comparePluse = %d\r\n", comparePluse);
//		printf("Frequency = %.3f\r\n", Frequency);
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ / 20);
	}		
}

void USART_Task(void *pvParameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
		//usart_senddata(USART1, );
	}	
	vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/20);
}	

void ADC_Task(void *pvParameters)
{
	TickType_t PreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
		/***压力计算**/
		ShengLi =(float) ADC_ConvertedValue[0]/4096*3.3f;
		NiuLi =(float) ADC_ConvertedValue[1]/4096*3.3f;
//		printf("ShengLi = %.2f\r\n", ShengLi);
//		printf("NiuLi = %.2f\r\n", NiuLi);
		usart_senddata(USART1, ShengLi * 100, NiuLi * 100, NiuLi * 100);
	}
	vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/20);
}