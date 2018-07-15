#ifndef __GPRS_H
#define	__GPRS_H

#include "timer2.h"
#include "usart2.h"

/****************************************************************************
 * Function:
 *		void GPRS_Config(void)
 * This function will configure the GPRS module resource,like USART and Timer.
 * Parameters:
 *		None 
 * Return:
 *		None
 *****************************************************************************/
void GPRS_Config(void);

/****************************************************************************
 * Function:
 *		uint8_t GPRS_AtCommand(char * cmd, char * expect, uint16_t timeout);
 * This function will send AT command to GRPS module, and check the response.
 * Parameters:
 *		data - command content, like "AT+CIPSTATUS";
 *		expect - expect response, like "OK";
 *		timeout - set the timeout in ms for waiting response;
 * Return:
 *		0 - get expected response in time;
 *		1 - get response not expected in time;
 *		2 - it's timeout to ger response;
 *****************************************************************************/
uint8_t GPRS_AtCommand(char * cmd, char * expect, uint16_t timeout);

uint8_t GPRS_AtCommandRetry(char * cmd, char * expect, uint16_t timeout, uint8_t retry);

#define GPRS_Send(a) USART2_Send(a)
#define GPRS_SendRaw(data,len) USART2_SendRaw(data,len)
void GPRS_HandleRequest(void);
#endif
