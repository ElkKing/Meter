/***************************************
 * File:	timer4.c
 * Description:	Apis for TIME4 of R2STM32F103C8.
 *							Timer4 is used as timer for Heart beat of GPRS.
 * Vesion£º	v1.0.0
 * Author:	Lori Fan
 **************************************/

#include <stdarg.h>
#include "timer4.h"
#include "debug.h"
#include "led.h"

extern char GLB_HeartBeat[];
extern uint8_t SIM800C_SendData(char* GLB_HeartBeat);
#define HeartBeat_Send() SIM800C_SendData(GLB_HeartBeat)

uint16_t HeartBeat_Interval = 30000; //30S
uint8_t HeartBeat_FailedCounter = 0;

void TIMER4_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 10*HeartBeat_Interval - 1;    	//0.1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 			//10KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);	
}

void TIM4_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
	{
		LED_Switch();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		//DBG_Trace(";");
		if (! HeartBeat_Send()) HeartBeat_FailedCounter++;
	}
}

void TIMER4_Enable(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void TIMER4_Disable(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE );
	TIM_Cmd(TIM4, DISABLE);
}
