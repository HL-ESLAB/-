#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"


void PWM_Init(void);
void IC_Init(void);
uint32_t IC_GetFreq(void);
uint32_t IC_GetDuty(void);

#endif
