/***************************************
 * File:	usart2.c
 * Description:	Api of USART2 of STM32F103C8         
 * Connection��
 *          -------------------------
 *          | PA2 - USART2(Tx)      |
 *          | PA3 - USART2(Rx)      |
 *           ------------------------
 * Vesion��	v1.0.0
 * Author:	Lori Fan

**********************************************************************************/

#include "usart2.h"
#include "usart3.h"
#include "timer2.h"
#include <stdarg.h>
#include "led.h"

/**************************************
 * USART2 Buffer
 **************************************/
#define USART2_BUFFER_SIZE 100
char GLB_Usart2Buffer[USART2_BUFFER_SIZE];
uint8_t  GLB_Usart2BufferLen = 0;
uint8_t GLB_Usart2Recieved = 0;

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART2 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 ʹ��IO�˿����� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	  
	/* USART2 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);  //��ʼ��USART2
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); //Enable USART2 Recieve interrupts
	USART2->SR;
	USART2->DR;
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART2, ENABLE);// USART2ʹ��
}

void USART2_BufferReset(void)
{
	memset(GLB_Usart2Buffer, 0, USART2_BUFFER_SIZE);
	GLB_Usart2BufferLen = 0;
	GLB_Usart2Recieved = 0;
}

void USART2_IRQHandler(void){  
	char RxData;

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{  
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		RxData=USART_ReceiveData(USART2);

		if ( (GLB_Usart2Recieved == 0)&&(GLB_Usart2BufferLen < USART2_BUFFER_SIZE-1) )
		{
			GLB_Usart2Buffer[GLB_Usart2BufferLen++] = RxData;
		}
		UART3_SendByte(RxData);
	}
	else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		RxData = USART2->SR;
		RxData = USART2->DR;
		if (GLB_Usart2Recieved == 0) 
		{
				GLB_Usart2Recieved = 1;
				USART3_Send(" | ");
		}
	}
}

void USART2_Send(char * data)
{
	while(*data)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*data++);
	}
}

/*����һ���ֽ�����*/
 void UART2_SendByte(unsigned char SendData)
{	   
	USART_SendData(USART2,SendData);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char UART2_GetByte(unsigned char* GetData)
{   	   
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{  
		return 0;//û���յ����� 
	}
	
	*GetData = USART_ReceiveData(USART2); 
	return 1;//�յ�����
}
/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
void Test_UART2_Transceive(void)
{
	unsigned char i = 0;

	while(1)
	{    
		while(UART2_GetByte(&i))	USART_SendData(USART2,i);
  }     
}




