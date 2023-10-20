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
������Ť����װ�ã���װ�����ĸ��̣߳�
ѹ���ɼ��߳�
��������߳�
�������ݷ����߳�
LED��ʾ�߳�
���Թ��̣�
2023/06/31
����Ŀ��Ʋ��֣���Ҫ�ǵ����PWM����ת��
���ñ�����ģʽ�ɼ�Ƶ��
ADC�ɼ�����ģ��
************************************************/

//�������ȼ�
#define START_TASK_PRIO		(tskIDLE_PRIORITY + 1)
#define PWM_CTL_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define LED_IO_TASK_PRIO	(tskIDLE_PRIORITY + 2)
#define ADC_TASK_PRIO			(tskIDLE_PRIORITY + 2)
#define USART_TASK_PRIO		(tskIDLE_PRIORITY + 2)

//������
void Start_Task(void *pvParameters);
void LED_Task(void *pvParameters);
void Control_Task(void *pvParameters);
void USART_Task(void *pvParameters);
void ADC_Task(void *pvParameters);
/****************ȫ�ֱ���*****************************/
__IO float Frequency;
extern __IO uint16_t comparePluse;
extern PID pid;
RCC_ClocksTypeDef RCC_CLK;/*  ʱ�ӽṹ�嶨��  */
float ShengLi = 0;
float NiuLi = 0;
float Speed = 0;
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
 

int main(void)
{
	/*Ӳ����ʼ��*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 	 
	RCC_GetClocksFreq(&RCC_CLK);                    /*��ȡϵͳʱ��*/
	if(RCC_CLK.SYSCLK_Frequency != 72000000)
	{
		while(1)
		{
			printf("sysclk is error");
		}/*���ϵͳʱ�Ӳ���ȷ�������к���ĳ���*/
	}
	delay_init();	    				//��ʱ������ʼ��	  
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	PWM_Init();		/*��ʼ��PWM*/
	IC_Init();		/*��ʼ��pwm���벶��*/	
	ADCx_Init();	/*ADC*/ 
	//������ʼ����
   xTaskCreate(Start_Task, "Start_Task", configMINIMAL_STACK_SIZE * 4, NULL, START_TASK_PRIO, NULL);  
	
	//�����������
   vTaskStartScheduler();

		while(1)
		{
		
		}
}

//��ʼ����������
void Start_Task(void *pvParameters)
{
	//�����ٽ���
  taskENTER_CRITICAL();           
  //IO�����߳�
  xTaskCreate(LED_Task, 			"LED_Task", 			256, 		NULL, 		LED_IO_TASK_PRIO, 		NULL);
	xTaskCreate(Control_Task, 	"Control_Task", 	1024, 	NULL, 		PWM_CTL_TASK_PRIO, 		NULL);   
	xTaskCreate(USART_Task, 		"USART_Task", 		1024, 	NULL, 		USART_TASK_PRIO, 			NULL);
	xTaskCreate(ADC_Task, 			"ADC_Task", 			1024, 	NULL, 		ADC_TASK_PRIO, 				NULL); 	 	
	vTaskDelete(NULL);
	//�˳��ٽ���
  taskEXIT_CRITICAL(); 
}

//LED0������ 
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
		/***ѹ������**/
		ShengLi =(float) ADC_ConvertedValue[0]/4096*3.3f;
		NiuLi =(float) ADC_ConvertedValue[1]/4096*3.3f;
//		printf("ShengLi = %.2f\r\n", ShengLi);
//		printf("NiuLi = %.2f\r\n", NiuLi);
		usart_senddata(USART1, ShengLi * 100, NiuLi * 100, NiuLi * 100);
	}
	vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/20);
}