
#ifndef __WATCH_H
#define __WATCH_H
#include "sys.h"
#include "stdio.h"	 
#include "stdarg.h"	 	 

int Get_watch_addr(u8 * address);
void Watch_sync(void);
int Read_watch_energy(u8 *energy);
int Read_watch_energy_peak(u8 *peak);
int Read_watch_energy_normal(u8 *normal);
int Read_watch_energy_valley(u8 *valley);

//int Watch_brakeOpen(void);
//int Watch_brakeClose(void);
//int Watch_request_data(void);

//extern void RS485_Send1_Data(u8 *buf,u8 len);
//extern void RS485_Rece1_Data(u8 *buf);

#endif	
