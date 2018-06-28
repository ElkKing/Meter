#ifndef __GPRS_H
#define	__GPRS_H

#include "timer2.h"
#include "usart2.h"

void GPRS_Config(void);
uint8_t GPRS_AtCommand(char * data, char * expect, uint16_t timeout);
void GPRS_HandleRequest(void);
#endif
