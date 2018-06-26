#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>

void USART3_Config(void);
void USART3_Send(char * data);
void UART3_SendByte(unsigned char SendData);
void Test_UART3_Transceive(void);
#endif /* __USART3_H */
