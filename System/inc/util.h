/***************************************
 * File:	util.h
 * Description:	the functions of utility         
 * Vesion：	v1.0.0
 * Author:	Lori Fan
 **************************************/
#ifndef __UTIL_H
#define	__UTIL_H

#include "stm32f10x.h"
#include <string.h>
uint8_t UTIL_SearchBuffer(char *buff, char *a);
void UTIL_HeartBeatFormat(u8 * address, char * out);
void UTIL_SetMeterId( u8 * out);
uint8_t UTIL_GetValidDataLen(char *data);
#endif
