/***************************************
 * File:	interrupt.c
 * Description:	Configure interrupts of STM32F103C8         
 * Vesion：	v1.0.0
 * Author:	Lori Fan
 **********************************************************************************/
 
#include "interrupt.h"
 
void NVIC_Configure(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// 使能USART2中断
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 指定抢占式优先级别1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; // 指定响应优先级别0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// 使能TIMER2中断 
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	// 使能USART3中断
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 指定抢占式优先级别1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3; // 指定响应优先级别0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
