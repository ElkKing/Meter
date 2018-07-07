#ifndef __TIMER4_H
#define	__TIMER4_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include <stdio.h>
#include <string.h>
/**************************************
 * TIMER4 global variable
 **************************************/
 
/**************************************
 * TIMER4 functions
 **************************************/
void TIMER4_Config(void);
void TIMER4_Enable(void);
void TIMER4_Disable(void);
#endif /* __TIMER4_H */
