#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
/**************************************
 * USART2 Global variables
 **************************************/
extern char GLB_Usart2Buffer[];
extern  uint8_t  GLB_Usart2BufferLen;
extern  uint8_t GLB_Usart2Recieved;
/**************************************
 * USART2 functions
 **************************************/
void USART2_Config(void);
void USART2_Send(char * data);
void USART2_BufferReset(void);
void Test_UART2_Transceive(void);
#endif /* __USART2_H */
