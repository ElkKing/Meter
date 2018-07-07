
/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "led.h"
#include "tick.h"
#include "sim800c.h"
#include "gprs.h"
#include "watch.h"
#include "debug.h"
#include "usart1.h"
#include "timer2.h"
#include "util.h"
#include "interrupt.h"

uint8_t GLB_MeterId[20] = "\0";
char GLB_HeartBeat[30];
extern uint8_t HeartBeat_FailedCounter;

int main(void)
{ 
	/**************************************
	 * FSM of system:
	 *     0 - Init Hardware*
	 
	***************************************/
	static uint8_t FSM = 0;
	//uint8_t loop = 0;
	
  SystemInit();	//配置系统时钟为 72M
	NVIC_Configure();

	DBG_Config();
	DBG_Info("Program starts!\n");

	GPRS_Config();	
	TICK_Config();
	
	USART1_Config();
	
	LED_Config();
	LED_Switch();
	
//	while(1)
//	{
//		SIM800C_StartHeartBeat();
//		TICK_DelaySecond(30);
//		SIM800C_StopHeartBeat();
//		TICK_DelaySecond(30);
//	};
	TICK_DelaySecond(30);
	DBG_Info("Enter the loop!\n");
  while (1)
  {
		switch (FSM)
		{			
			case 0:
				if ( SIM800C_Config() )
				{
					FSM = 1;
				}
				break;
			case 1:
//				FSM=2;
//				UTIL_SetMeterId(GLB_MeterId);
//				UTIL_HeartBeatFormat(GLB_MeterId,GLB_HeartBeat);
//				DBG_Trace(GLB_HeartBeat);
//				SIM800C_StartHeartBeat();

				if (Get_watch_addr(GLB_MeterId))
				{
					//DBG_Error((char *) GLB_MeterId);
					UTIL_HeartBeatFormat(GLB_MeterId,GLB_HeartBeat);
					DBG_Trace(GLB_HeartBeat);
					SIM800C_StartHeartBeat();
					FSM = 2;
				}
				break;
			case 2:
			{
				//HeartBeat_Send();
			  u8 energy[5] = "\0";
				GPRS_HandleRequest();
				
				SIM800C_StopHeartBeat();
				if (Read_watch_energy(energy))
				{
					SIM800C_SendData((char *)energy);
				}
				TICK_DelaySecond(1);
				memset(energy,0x0,5);
				if (Read_watch_energy_peak(energy))
				{
					SIM800C_SendData((char *)energy);
				}
				TICK_DelaySecond(1);
				memset(energy,0x0,5);
				if (Read_watch_energy_valley(energy))
				{
					SIM800C_SendData((char *)energy);
				}
				TICK_DelaySecond(1);
				memset(energy,0x0,5);
				if (Read_watch_energy_normal(energy))
				{
					SIM800C_SendData((char *)energy);
				}
				TICK_DelaySecond(1);
				Watch_sync();
				
				SIM800C_StartHeartBeat();
				if (HeartBeat_FailedCounter >=5) 
				{
					FSM=0;
					HeartBeat_FailedCounter = 0;
					SIM800C_StopHeartBeat();
				}
				break;
			}
			default:
				break;
		}
		
		TICK_DelaySecond(10);
		
		//HeartBeat_Send();
		//LED_Switch();		
		//GPRS_HandleRequest();
		//SIM800C_SendData("Hello!");
  }
}

