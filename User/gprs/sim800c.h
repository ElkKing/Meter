#ifndef __SIM800C_H
#define	__SIM800C_H
#include "timer4.h"

#define SIM800C_SINGLE_LINK_MODE 0
#define SIM800C_MULTI_LINK_MODE 1
#define SIM800C_SINGLE_IP_CONNECT 0
#define SIM800C_MULTI_IP_CONNECT 1

#define SIM800C_StartHeartBeat() TIMER4_Enable()
#define SIM800C_StopHeartBeat() TIMER4_Disable()

uint8_t SIM800C_EnterCmdMode(void);
uint8_t SIM800C_SendData(char* data);
uint8_t SIM800C_Config(void);
//void HeartBeat_Send(void);
#endif
