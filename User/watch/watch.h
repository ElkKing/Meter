
#ifndef __WATCH_H
#define __WATCH_H
#include "sys.h"
#include "stdio.h"	 
#include "stdarg.h"	 	 

/* 外部接口*/
/* 
input:	cmd, cmd_len
output:	data, data_len
*/
int Watch_request_data(u8 *cmd, int cmd_len, u8 *data, int *data_len);


/* 内部接口*/
int Get_watch_addr(u8 * address);
void Watch_sync(void);
int Read_watch_energy(u8 *energy);
int Read_watch_energy_peak(u8 *peak);
int Read_watch_energy_normal(u8 *normal);
int Read_watch_energy_valley(u8 *valley);
int Read_watch_power(u8 *req_cmd, int cmd_len, u8 *power, int *data_len);
int Watch_SendRequest(u8 *cmd,u8 * reply);
//int Watch_brakeOpen(void);
//int Watch_brakeClose(void);
//int Watch_request_data(void);

//extern void RS485_Send1_Data(u8 *buf,u8 len);
//extern void RS485_Rece1_Data(u8 *buf);

#endif	
