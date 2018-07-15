#ifndef __L206_H
#define	__L206_H
#include "timer4.h"

#define L206_SINGLE_LINK_MODE 0
#define L206_MULTI_LINK_MODE 1
#define L206_SINGLE_IP_CONNECT 0
#define L206_MULTI_IP_CONNECT 1

#define L206_StartHeartBeat() TIMER4_Enable()
#define L206_StopHeartBeat() TIMER4_Disable()

uint8_t L206_EnterCmdMode(void);
uint8_t L206_SendData(char* data);
uint8_t L206_Config(void);
void L206_Reset(void);
void L206_Enable(void);
//void HeartBeat_Send(void);
#endif
