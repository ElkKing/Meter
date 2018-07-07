/***************************************
 * File:	gprs.c
 * Description:	the APIs of GPRS module         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "stm32f10x.h"
#include "gprs.h"
#include "util.h"
#include "debug.h"
#include "timer4.h"
#include "tick.h"
#include <string.h>

void GPRS_Config(void)
{
	USART2_Config(); 
	TIMER2_Config(5000);
	TIMER4_Config();
	//GPRS_ModuleConfig();	
}

uint8_t GPRS_AtCommand(char * cmd, char * expect, uint16_t timeout)
{
	uint8_t retCode;
	USART2_BufferReset();
	TIMER2_Config(timeout);
	TIMER2_Enable();
	
	USART2_Send(cmd);
	while( GLB_Timer2Timeout == 0 )
	{
		while(GLB_Usart2Recieved){
			TIMER2_Disable();
			if (UTIL_SearchBuffer(GLB_Usart2Buffer, expect)) 
			{
				DBG_Trace("Matched\n");
				retCode = 0;
			}
			else 
			{
				DBG_Trace("Dismatched\n");
				retCode = 1;
			}
			USART2_BufferReset();
			return  retCode;
		}			
	}
	TIMER2_Disable();
	DBG_Trace("Timeout\n");
	return  2;	
}

uint8_t GPRS_AtCommandRetry(char * cmd, char * expect, uint16_t timeout, uint8_t retry)
{
	uint8_t retCode;
	uint8_t loop = 0;
	uint16_t delay = 200; //200ms
	
	while(loop++ < retry)
	{
		retCode = GPRS_AtCommand(cmd, expect, timeout);
		if ( retCode == 0 ) break;
		TICK_DelayMs(delay);
	}
	return retCode;
}
void GPRS_HandleRequest(void)
{
		if(GLB_Usart2Recieved)USART2_BufferReset();	
}
