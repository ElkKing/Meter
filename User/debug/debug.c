#include "debug.h"

/********************************************************
 * Debug level:
 *	0 - none
 *  1 - error
 *  2 - warning
 *  3 - information
 *  4 - trace information
 *  5 - all
 *******************************************************/
#define DBG_OFF_LEVEL		0
#define DBG_ERROR_LEVEL		1
#define DBG_WARN_LEVEL		2
#define DBG_INFO_LEVEL		3
#define DBG_TRACE_LEVEL		4
#define DBG_ALL_LEVEL		5
static uint8_t GLB_DebugLevel = 5;


/********************************************************
 *
 * 	Functions
 *
 *******************************************************/
void DBG_SetLevel(uint8_t level)
{
	GLB_DebugLevel = level;
}

void DBG_Error(char * message)
{
	if( GLB_DebugLevel >= DBG_ERROR_LEVEL){
		USART3_Send("[ERROR]");
		USART3_Send( message );
		USART3_Send("\n");
	}
}

void DBG_Warn(char * message)
{
	if( GLB_DebugLevel >= DBG_WARN_LEVEL){
		USART3_Send("[WARNING]");
		USART3_Send( message );
		USART3_Send("\n");
	}
}

void DBG_Info(char * message)
{
	if( GLB_DebugLevel >= DBG_INFO_LEVEL){
		USART3_Send( message );
		USART3_Send("\n");
	}
}
void DBG_Trace(char * message)
{
	if( GLB_DebugLevel >= DBG_TRACE_LEVEL){
		USART3_Send( message );
		USART3_Send("\n");
	}
}

void DBG_TraceByte(char  message)
{
	if( GLB_DebugLevel >= DBG_TRACE_LEVEL){
		UART3_SendByte( message );
	}
}
