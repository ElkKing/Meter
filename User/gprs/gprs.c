/***************************************
 * File:	gprs.c
 * Description:	the APIs of GPRS module         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "stm32f10x.h"
#include "gprs.h"
#include "util.h"
#include "usart3.h"
#include <string.h>

void GPRS_Config(void)
{
	USART2_Config();
	TIMER2_Config(5000);
	//GPRS_ModuleConfig();	
}

uint8_t GPRS_AtCommand(char * data, char * expect, uint16_t timeout)
{
	uint8_t retCode;
	USART2_BufferReset();
	TIMER2_Config(timeout);
	TIMER2_Enable();
	
	USART2_Send(data);
	while( GLB_Timer2Timeout == 0 )
	{
		while(GLB_Usart2Recieved){
			TIMER2_Disable();
			if (UTIL_SearchBuffer(GLB_Usart2Buffer, expect)) 
			{
				USART3_Send("Matched\n");
				retCode = 0;
			}
			else 
			{
				USART3_Send("Dismatched\n");
				retCode = 1;
			}
			USART2_BufferReset();
			return  retCode;
		}			
	}
	USART3_Send("Timeout\n");
	return  2;	
}

void GPRS_HandleRequest(void)
{
		if(GLB_Usart2Recieved)USART2_BufferReset();	
}
