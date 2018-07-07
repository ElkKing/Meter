#ifndef __DBG_H
#define	__DBG_H

#include "stm32f10x.h"
#include "usart3.h"

#define DBG_Config() USART3_Config()

void DBG_SetLevel(uint8_t level);
void DBG_Error(char * message);
void DBG_Warn(char * message);
void DBG_Info(char * message);
void DBG_Trace(char * message);
void DBG_TraceByte(char  message);
#endif /* __DGB_H */
