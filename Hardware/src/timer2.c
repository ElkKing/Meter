/***************************************
 * File:	timer2.c
 * Description:	Api of TIME2 of R2STM32F103C8         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan
 **************************************/

#include "timer2.h"
#include "usart3.h"
#include <stdarg.h>
#include "led.h"

uint8_t GLB_Timer2Timeout = 255;

void TIMER2_Config(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 10*ms - 1;    	//0.1ms
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 	//10KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	
}

void TIM2_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{ 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		//TIMER2_Disable();
		GLB_Timer2Timeout = 1;
		//LED_Switch();
		//USART3_Send("+");  
	}
}

void TIMER2_Enable(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	GLB_Timer2Timeout = 0;
}

void TIMER2_Disable(void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );
	TIM_Cmd(TIM2, DISABLE);
}
