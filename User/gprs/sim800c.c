/***************************************
 * File:	gprs.c
 * Description:	the APIs of GPRS module         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "stm32f10x.h"
#include "gprs.h"
#include "sim800c.h"
#include <string.h>

uint8_t SIM800C_EnterCmdMode(void)
{
	uint8_t ret=0;
	char cmd[] = "+++";
	char expect[] = "OK";
	uint16_t timeout = 200; //200ms

	if (GPRS_AtCommand(cmd, expect, timeout) == 0) ret = 1;
	return ret;
}

uint8_t SIM800C_SetMode(uint8_t link, uint8_t transfer)
{
	char * valLink = "0";
	char * valConn = "0";
	char cmd[20] = "AT+CIPMUX=";
	char expect[] = "OK";
	uint16_t timeout = 200;
	
	if(link == SIM800C_MULTI_LINK_MODE) valLink = "1";
	if(transfer == SIM800C_MULTI_IP_CONNECT) valConn = "1";
	
	strcat(cmd, valLink);
	if (GPRS_AtCommand(cmd, expect, timeout) != 0) return 0;
	
	strcpy(cmd, "AT+CIPMODE=");
	strcat(cmd, valConn);
	if (GPRS_AtCommand(cmd, expect, timeout) != 0) return 0;
	else return 1;
}

uint8_t SIM800C_Config(void)
{
	SIM800C_SetMode(SIM800C_SINGLE_LINK_MODE,SIM800C_SINGLE_IP_CONNECT);
	return 1;
}
