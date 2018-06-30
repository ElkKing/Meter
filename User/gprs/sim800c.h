#ifndef __SIM800C_H
#define	__SIM800C_H

#define SIM800C_SINGLE_LINK_MODE 0
#define SIM800C_MULTI_LINK_MODE 1
#define SIM800C_SINGLE_IP_CONNECT 0
#define SIM800C_MULTI_IP_CONNECT 1

uint8_t SIM800C_EnterCmdMode(void);
uint8_t SIM800C_SendData(char* data);
uint8_t SIM800C_Config(void);

#endif
