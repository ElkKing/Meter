/***************************************
 * 文件名  ：usart1.c
 * 描述    ：配置USART1         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  

**********************************************************************************/

#include "usart1.h"
#include "usart3.h"
#include <stdarg.h>
#include <stdio.h>
#include "stm32f10x.h"
//#include "definition.h"
#include "usart.h"
//#include "delay.h"
//#include "RTC.h"
//#include "base.h"
//#include "keytouch.h"
#include "sys.h"
#include "led.h"
#include "tick.h"

//#define Rxnum  200
//#define Txnum   200
#define BUFFER_SIZE 100

//串口接收用全局变量
u8 Usart1_RxBuffer[BUFFER_SIZE];
u8 Usart1_BufferLen = 0;
//uint8_t Usart1_Recieved = 0;

void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 

	/* USART1 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 2400;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_Even ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //Enable USART3 Recieve interrupts
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE); //Enable USART3 Recieve interrupts
	USART_Cmd(USART1, ENABLE);// USART1使能
}

void USART1_BufferReset(void)
{
	memset(Usart1_RxBuffer, 0, BUFFER_SIZE);
	Usart1_BufferLen = 0;
	//Usart1_Recieved = 0;
}

/**-------------------------------------------------------
  * @函数名 USART1_IRQHandler
  * @功能   串口1接收中断
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
//void USART1_IRQHandler(void)
//{
//#if 0	
//	  char RxData;		

//    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {  
//			USART_ClearITPendingBit(USART1, USART_IT_RXNE);  
//			RxData = USART_ReceiveData(USART1);
//			USART_SendData(USART3,RxData);
//		}
//#endif	

//		LED_Switch();
//	char RxData;
//	if(Usart1_BufferLen>BUFFER_SIZE)  //接收数据的最大个数不能超过接收Buffer
//	{
//		Usart1_BufferLen=BUFFER_SIZE;
//	}
//	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清空接收标志位	
//		RxData = USART_ReceiveData(USART1);

//		if ( (Usart1_Recieved == 0)&&(Usart1_BufferLen < BUFFER_SIZE-1) )
//		{
//			Usart1_RxBuffer[Usart1_BufferLen++] = RxData;				
//		}
//		UART3_SendByte(RxData);
//	}
//	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
//	{
//		RxData = USART1->SR;
//		RxData = USART1->DR;
//		if (Usart1_Recieved == 0) 
//		{
//			Usart1_Recieved = 1;
//			//USART1_BufferReset();
//		}
//	}

//}

void USART1_IRQHandler(void)
{
	unsigned char RxData;
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
	{
		//USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清空接收标志位	
		RxData = USART_ReceiveData(USART1);

//		if ( (Usart1_Recieved == 0)&&(Usart1_BufferLen < BUFFER_SIZE-1) )
//		{
			Usart1_RxBuffer[Usart1_BufferLen++] = RxData;				
//		}
		if(Usart1_BufferLen>=90)
		{
		    Usart1_BufferLen = 0;
		}
		
		
	}

}



/*
接收电表的返回值
need check

*/
//void USART1_Rcvndata(u8 *rxbuffer)
//{	  
//  u8 i=0,a=0;
//  for(i=0;i<Usart1_BufferLen;i++)
//  {
//				if(Usart1_RxBuffer[i]==0x68 && Usart1_RxBuffer[i+7]==0x68 && Usart1_RxBuffer[Usart1_BufferLen-1]==0x16)
//				{
//						//get_Watch_data_OK=1;
//						for(a=0;a<Usart1_RxBuffer[i+9];a++)
//						{
//							rxbuffer[a]=Usart1_RxBuffer[a+i+10];
//						}
//						
//				}
//	}
//	Usart1_BufferLen=0;
//}

/*
send:16
FE FE FE FE 68 AA AA AA AA AA AA 68 13 00 DF 16
receive:22
FE FE FE FE 68 67 48 60 10 00 00 68 93 06 9A 7B 93 43 33 33 D9 16
*/
int USART1_Rcvndata(u8 *rxbuffer)
{	  
	//u8 dataLen = 0;
  
	if(Usart1_RxBuffer[4]==0x68 && Usart1_RxBuffer[4+7]==0x68 && Usart1_RxBuffer[Usart1_BufferLen-1]==0x16)
	{
		//get_Watch_data_OK=1;
		//dataLen = Usart1_RxBuffer[4+9];
		memcpy(rxbuffer,Usart1_RxBuffer,Usart1_BufferLen);
		USART1_BufferReset();
		return 1;
	}
	return 0;
}

/********************************************************
函数名称：void USART_Sendndata(u8 *p,u8 num)
功能描述:串口发送num个数据
全局变量：
参数说明： *p入口参数指针,num数据个数 
返回说明：
设计 人：
版本：1.0
说明：		  
********************************************************/
void USART1_Sendndata(u8 *p,u8 num)
{
   u8 i=0;
   TICK_DelayMs(1);
   for(i=0;i<num;i++)
   {
			USART1->DR = p[i];
      while((USART1->SR&0x40)==0); 
		  //USART_SendData(USART1,p[i]);
   }
   TICK_DelayMs(2);
}



void USART1_Send(unsigned char * data, u8 num)
{
	//unsigned char send_cnt = num;
	while(num--)
	//while(*data)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
		USART_SendData(USART1 ,*data++);				
	}
}

 /*发送一个字节数据*/
 void UART1SendByte(unsigned char SendData)
{	   
        USART_SendData(USART1,SendData);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*接收一个字节数据*/
unsigned char UART1GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {  return 0;//没有收到数据 
		}
        *GetData = USART_ReceiveData(USART1); 
        return 1;//收到数据
}
/*接收一个数据，马上返回接收到的这个数据*/
void UART1Test(void)
{
       unsigned char i = 0;

       while(1)
       {    
					while(UART1GetByte(&i))
					{
						USART_SendData(USART1,i);
					}      
       }     
}



