/***************************************
 * File:	USART3.c
 * Description:	Api of USART3 of STM32F103C8         
 * Connection��
 *          -------------------------
 *          | PB10 - USART3(Tx)     |
 *          | PB11 - USART3(Rx)     |
 *           ------------------------
 * Vesion��	v1.0.0
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

	/* ʹ�� USART3 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 ʹ��IO�˿����� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOB
	  
	/* USART3 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);  //��ʼ��USART3
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); //Enable USART3 Recieve interrupts
	USART_Cmd(USART3, ENABLE);// USART3ʹ��
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

/*����һ���ֽ�����*/
void UART3_SendByte(unsigned char SendData)
{	   
	USART_SendData(USART3,SendData);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char UART3GetByte(unsigned char* GetData)
{   	   
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET)
	{  
		return 0;//û���յ����� 
	}
	
	*GetData = USART_ReceiveData(USART3); 
	return 1;//�յ�����
}
/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
void Test_UART3_Transceive(void)
{
	unsigned char i = 0;

	while(1)
	{    
		while(UART3GetByte(&i))	USART_SendData(USART3,i);
  }     
}




