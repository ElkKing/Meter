#ifndef __TIMER2_H
#define	__TIMER2_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include <stdio.h>
#include <string.h>
/**************************************
 * TIMER2 global variable
 **************************************/
 extern uint8_t GLB_Timer2Timeout;
/**************************************
 * TIMER2 functions
 **************************************/
void TIMER2_Config(uint16_t ms); //ms <= 6535
void TIMER2_Enable(void);
void TIMER2_Disable(void);
#endif /* __TIMER2_H */
