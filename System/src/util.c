/***************************************
 * File:	util.c
 * Description:	the functions of utility         
 * Vesion£º	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/
#include "util.h"


uint8_t UTIL_SearchBuffer(char *buff, char *a)
{ 
  if(strstr(buff,a)!=NULL)
	    return 1;
	else
		return 0;
}

void UTIL_HeartBeatFormat(uint8_t address[], char * out)
{
	int i = 0;
	u8 temp;
	u8 addr[13] = "\0"; 
	
	strcpy(out, "ro0id");
	for( i=5; i >=0; i--)
	{
			temp = (address[i]>>4) + 0x30;
	    addr[(5-i)*2] = temp;
			temp = (address[i] & 0x0F) + 0x30;
		  addr[(5-i)*2 +1] = temp;
	}
	strcat(out,(char *) addr);
}

uint8_t UTIL_GetValidDataLen(char *data)
{
	uint8_t len = 0;
	while(len <101)
	{
		if (data[len++] == 0x16) return len;
	}
	return 0;
}

void UTIL_SetMeterId( u8 * out)
{
	out[0] = 0x67;
	out[1] = 0x48;
	out[2] = 0x60;
	out[3] = 0x10;
	out[4] = 0x00;
	out[5] = 0x00;
}
