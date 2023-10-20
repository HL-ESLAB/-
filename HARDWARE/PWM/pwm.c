#include "pwm.h"

__IO uint16_t comparePluse = 1000;

//初始化pwm端口
static void PWM_GPIO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道1 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//uint16_t LC_CHG(u16 ChannelPulse)   //F为需频率 C为占空比 如F=2000，C取值0-1000
//{
//	CCR1_Val = ChannelPulse;  
//	return CCR1_Val;
//}
///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void TIM3_PWM_Init(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = (20000-1);	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1);	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			
	// 初始化定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/	
	// 占空比配置
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
	// 输出比较通道 1
	TIM_OCInitStructure.TIM_Pulse = comparePluse;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
}
void IC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0xF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1 ;
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_PWMIConfig(TIM1,&TIM_ICInitStruct);
	
	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=65536-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM1,ENABLE);
}
uint32_t IC_GetFreq(void)
{
	return 1000000/TIM_GetCapture1(TIM1);
}
uint32_t IC_GetDuty(void)
{
	return TIM_GetCapture2(TIM1)*100/TIM_GetCapture1(TIM1);
}
void PWM_Init(void)
{
	PWM_GPIO_Init();
	TIM3_PWM_Init();
}



