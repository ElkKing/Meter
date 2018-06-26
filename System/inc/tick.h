#ifndef __TICK_H
#define __TICK_H
#include "stm32f10x.h"
#include "core_cm3.h"

void TICK_Config(void);
void TICK_DelayMs(uint32_t nTime);
void TICK_DelaySecond(uint32_t nTime);

#endif
