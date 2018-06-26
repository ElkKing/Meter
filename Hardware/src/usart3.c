/***************************************
 * File:	USART3.c
 * Description:	Api of USART3 of STM32F103C8         
 * Connection：
 *          -------------------------
 *          | PB10 - USART3(Tx)     |
 *          | PB11 - USART3(Rx)     |
 *           ------------------------
 * Vesion：	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/

#include "usart3.h"
#include "usart1.h"
#include "usart2.h"
#include <stdarg.h>
#include "led.h"


void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART3 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOB
	  
	/* USART3 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);  //初始化USART3
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); //Enable USART3 Recieve interrupts
	USART_Cmd(USART3, ENABLE);// USART3使能
}

USART_TypeDef * USARTx_Debug = USART2;

void USART3_IRQHandler(void){  
    char RxData;		

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {  
			USART_ClearITPendingBit(USART3, USART_IT_RXNE);  
			RxData = USART_ReceiveData(USART3);
			USART_SendData(USARTx_Debug,RxData);
		}
}

void USART3_Send(char * data)
{
	while(*data)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET); 
		USART_SendData(USART3 ,*data++);				
	}
}

/*发送一个字节数据*/
void UART3_SendByte(unsigned char SendData)
{	   
	USART_SendData(USART3,SendData);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
}  

/*接收一个字节数据*/
unsigned char UART3GetByte(unsigned char* GetData)
{   	   
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
	{  
		return 0;//没有收到数据 
	}
	
	*GetData = USART_ReceiveData(USART3); 
	return 1;//收到数据
}
/*接收一个数据，马上返回接收到的这个数据*/
void Test_UART3_Transceive(void)
{
	unsigned char i = 0;

	while(1)
	{    
		while(UART3GetByte(&i))	USART_SendData(USART3,i);
  }     
}




