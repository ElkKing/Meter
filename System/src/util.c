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
