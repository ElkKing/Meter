/***************************************
 * File:	l206.c
 * Description:	the APIs of GPRS module         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "stm32f10x.h"
#include "gprs.h"
#include "l206.h"
#include "tick.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>


uint8_t L206_EnterCmdMode(void)
{
	uint8_t ret=0;
	char cmd[] = "+++";
	char expect[] = "OK";
	uint16_t timeout = 200; //200ms

	if (GPRS_AtCommand(cmd, expect, timeout) == 0) ret = 1;
	return ret;
}
void L206_SetEcho(uint8_t echo)
{
	if (echo)	GPRS_Send("ATE1\n");
	else GPRS_Send("ATE0\n");
	TICK_DelaySecond(1);
}

uint8_t L206_SetMode(uint8_t link, uint8_t transfer)
{
	uint8_t retCode;
	char * valLink = "0";
	char * valConn = "0";
	char cmd[20] = "AT+CIPMUX?\n";
	char expect[50] = "+CIPMUX: ";
	uint16_t timeout = 200;
	
	if(link == L206_MULTI_LINK_MODE) valLink = "1";
	if(transfer == L206_MULTI_IP_CONNECT) valConn = "1";
	
	strcat(expect, valLink);
	retCode = GPRS_AtCommand(cmd, expect, timeout);
	if (retCode == 2 ) return 0;
	else if (retCode == 1 ){
		strcpy(cmd, "AT+CIPMUX=");
		strcat(cmd, valLink);
		strcat(cmd, "\n");
		strcpy(expect, "OK");
		if (GPRS_AtCommand(cmd, expect, timeout) != 0) return 0;
	}
	
	strcpy(cmd, "AT+CIPMODE?\n");
	strcpy(expect, "+CIPMODE: ");
	strcat(expect, valConn);
	retCode = GPRS_AtCommand(cmd, expect, timeout);
	if (retCode == 2 ) return 0;
	else if (retCode == 1 ){
		strcpy(cmd, "AT+CIPMODE=");
		strcat(cmd, valConn);
		strcat(cmd, "\n");
		strcpy(expect, "OK");
		if (GPRS_AtCommand(cmd, expect, timeout) != 0) return 0;
	}
	
	return 1;
}
uint8_t L206_ConnectServer(char* protocol, char* serverIp, char * port)
{
		uint8_t retCode;
		char cmd[50] = "";
		char expect[50] = "";
		uint16_t timeout = 200;
	  uint8_t retry = 3;
	
	  strcpy(cmd,"AT+CIPSHUT\n");
		strcpy(expect,"OK");
		timeout = 500;
		retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;
	
	  strcpy(cmd,"AT+CPIN?\n");
		strcpy(expect,"+CPIN: READY");
		timeout = 200;
		retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;
		
	  strcpy(cmd,"AT+CSQ\n");
		strcpy(expect,"+CSQ:");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;
		
		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CREG?\n");
		strcpy(expect,"+CREG: 0,1");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;

		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CGATT?\n");
		strcpy(expect,"+CGATT: 1");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;
		
		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CSTT=\"CMNET\"\n");
		strcpy(expect,"OK");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;

		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CIPSTATUS\n");
		strcpy(expect,"STATE: IP START");
	  retCode = GPRS_AtCommandRetry(cmd, expect, timeout, retry);
		if (retCode != 0) return 0;
		
		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CIICR\n");
		strcpy(expect,"OK");
		timeout = 60000;
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;

		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CIPSTATUS\n");
		strcpy(expect,"STATE: IP GPRSACT");
		timeout = 200;
	  retCode = GPRS_AtCommandRetry(cmd, expect, timeout, retry);
		if (retCode != 0) return 0;
		
		TICK_DelayMs(50);
	  strcpy(cmd,"AT+CIFSR\n");
		strcpy(expect,".");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;
		
		TICK_DelayMs(500);
		sprintf(cmd,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n",protocol,serverIp,port);
		USART3_Send(cmd);
		strcpy(expect,"OK");
		retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0)  return 0;

		TICK_DelaySecond(10);
	  strcpy(cmd,"AT+CIPSTATUS\n");
		strcpy(expect,"STATE: CONNECT OK");
	  retCode = GPRS_AtCommandRetry(cmd, expect, timeout, retry);
		if (retCode != 0) return 0;
		
		return 1;
}

uint8_t L206_SendData(char* data)
{
		uint8_t retCode;
		char cmd[20] = "";
		char usrData[200] = "";
		char expect[50] = "";
		char submit[2] = "\0\0";
		uint16_t timeout = 500;
		submit[0] = 0x1A;
	
		strcpy(cmd,"AT+CIPSEND\n");
		strcpy(expect,">");
	  retCode = GPRS_AtCommand(cmd, expect, timeout);
		if (retCode != 0) return 0;

		if( strlen(data) > 200) return 0;

		//memcpy(usrData,data,len(data));
		strcpy(usrData,data);
		strcat(usrData, submit);
		strcpy(expect,"SEND OK");
		timeout = 2000;
		retCode = GPRS_AtCommand(usrData, expect, timeout);
		if (retCode != 0) return 0;
		else return 1;
}
void L206_CloseConnect(void)
{
		char cmd[50] = "";
		char expect[50] = "";
		uint16_t timeout = 200;
	
		strcpy(cmd,"AT+CIPCLOSE\n");
		strcpy(expect,"CLOSE OK");
	  GPRS_AtCommand(cmd, expect, timeout);
		
		strcpy(cmd,"AT+CIPSHUT\n");
		strcpy(expect,"SHUT OK");
	  GPRS_AtCommand(cmd, expect, timeout);
}

uint8_t L206_Config(void)
{
	char * protocol = "TCP";
	char * server = "106.14.178.92";
	char * port = "16026";
	char message[50] = "\0";
		
	L206_SetEcho(0);
	sprintf(message,"Connect to %s:%s by %s",server,port,protocol); 
	DBG_Trace(message);
	//if (! L206_SetMode(L206_SINGLE_LINK_MODE,L206_SINGLE_IP_CONNECT) )return 0;
	if (! L206_ConnectServer(protocol, server, port) ) return 0;
	return 1;
}

//void HeartBeat_Send(void)
//{
//	L206_SendData(GLB_HeartBeat);
//}

