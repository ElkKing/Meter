/***************************************
 * File:	gprs.c
 * Description:	the APIs of GPRS module         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "stm32f10x.h"
#include "gprs.h"
#include <string.h>

uint8_t SIM800C_EnterCmdMode(void)
{
	uint8_t ret=0;
	char cmd[] = "+++";
	char expect[] = "OK";
	uint16_t timeout = 500; //200ms

	if (GPRS_AtCommand(cmd, expect, timeout) == 0) ret = 1;
	return ret;
}
