
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
#include "usart3.h"
#include "timer2.h"
#include "interrupt.h"

uint16_t METER_Id[12];

int main(void)
{  
	uint8_t FSM = 0;
	uint8_t loop = 0;
	
  SystemInit();	//配置系统时钟为 72M
		
	NVIC_Configure();

	USART3_Config();
	USART3_Send("Program starts!\n");

	GPRS_Config();	
	TICK_Config();
	
	LED_Config();
	LED_Switch();
	TICK_DelaySecond(120);
	
	while (FSM == 0)
	{
		if ( SIM800C_Config() ) 
		{
			FSM = 1;
			break;
		}
		TICK_DelaySecond(5);
		if ( ++loop == 5) break;
	}
	
	USART3_Send("Enter the loop!\n");
  while (1)
  {
		TICK_DelaySecond(30);
		
		LED_Switch();		
		GPRS_HandleRequest();
		SIM800C_SendData("Hello!");
		//USART3_Send("Program is running!\n");
		//delay_ms(10000);
		//delay_ms(10000);
		//LED_Switch();
		//if ( SIM800C_EnterCmdMode()) USART3_Send("Command mode succed!\n");
		//else USART3_Send("Command mode failed!\n");
  }
}

