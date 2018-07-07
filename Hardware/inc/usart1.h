#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void UART1Test(void);
//void USART1_Send(unsigned char * data);
void USART1_Send(unsigned char * data, u8 num);
void UART1SendByte(unsigned char SendData);

void USART1_Sendndata(u8 *p,u8 num);
int USART1_Rcvndata(u8 *rxbuffer);
void USART1_BufferReset(void);
#endif /* __USART1_H */
