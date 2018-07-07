#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "definition.h"
//#include "base.h"
//#include "usart.h"
#include "usart1.h"
#include "usart3.h"
//#include "RTC.h"
//#include "delay.h"
#include "sys.h"
//#include "GPRS.h"
//#include "keytouch.h"
//#include "inverter.h"
#include "watch.h"
#include "tick.h"
#include <string.h>

u8 Watch_readAddr_cmd[16]  = {0xFE,0xFE,0xFE,0xFE,0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x13,0x00,0xDF,0x16};   //读通信地址
u8 Watch_sync_cmd[16] 		 = {0xFE,0xFE,0xFE,0xFE,0x68,0x99,0x99,0x99,0x99,0x99,0x99,0x68,0x08,0x00,0x6E,0x16};
u8 Watch_energy_cmd[20];
u8 Watch_brakeOpen_buf[25];
u8 Watch_brakeClose_buf[25];
u16 Watch_checkSum(u8 *buf, u16 num);
//FE FE FE FE 68 99 99 99 99 99 99 68 08 00 6E 16

//address
u8 add[6] = {0};

/********************************************************
函数名称：void Watch_addr_get(void)
功能描述: 从电能表获取地址
全局变量：
参数说明：address 定义为数组, 入参+出参
返回说明：
unsigned char add[20] = {0};
Get_watch_addr(add);
设计 人：
版本：1.0
说明：		  Need check???
send:16
FE FE FE FE 68 AA AA AA AA AA AA 68 13 00 DF 16
receive:22
FE FE FE FE 68 67 48 60 10 00 00 68 93 06 9A 7B 93 43 33 33 D9 16
********************************************************/
//电表地址获取
int Get_watch_addr(u8 * address)
{
	//time_1ms=0;
	u8 data[22];
	USART1_BufferReset();	
	USART1_Send(Watch_readAddr_cmd,16);		//发送指令
	TICK_DelaySecond(1);
	if(!USART1_Rcvndata(data))
		return 0;
	
	if (data[12] == 0x93 && data[13] == 0x06 && data[21] == 0x16)
	{
			/* address[0] 低位*/
			address[0] = data[14] - 0x33;
			address[1] = data[15] - 0x33;
			address[2] = data[16] - 0x33;
			address[3] = data[17] - 0x33;
			address[4] = data[18] - 0x33;
			address[5] = data[19] - 0x33;
			return 1;
	}
	return 0;
}

/********************************************************
函数名称：void Watch_sync(void)
功能描述: 同步校准所有的设备
全局变量：
参数说明：
返回说明：
设计 人：L
版本：1.0
说明：		  
********************************************************/
void Watch_sync(void)
{
  //FE FE FE FE 68 99 99 99 99 99 99 68 08 00 6E 16
	//FEFEFEFE689999999999996808006E16
	
	USART1_Sendndata(Watch_sync_cmd,16);
}

/********************************************************
函数名称：void Watch_energyRead_cmd(void)
功能描述: 从电能表获取数据
全局变量：
参数说明：
返回说明：
设计 人：L
版本：1.0
说明：	Need check
send:20
FE FE FE FE 68 67 48 60 10 00 00 68 11 04 33 33 34 33 D1 16
receive:24
FE FE FE FE 68 67 48 60 10 00 00 68 91 08 33 33 34 33 33 33 33 33 21 16
********************************************************/

int Read_watch_energy(u8 *energy)
{
    u8 len1 = 0;
    u8 sum = 0;
		//FE FE FE FE 68 67 48 60 10 99 00 68 11 04 33 33 34 33 6A 16
		//FEFEFEFE68674860109900681104333334336A16
		Watch_energy_cmd[0] = 0xFE;
		Watch_energy_cmd[1] = 0xFE;
		Watch_energy_cmd[2] = 0xFE;
		Watch_energy_cmd[3] = 0xFE;
    Watch_energy_cmd[4] = 0x68;
		//address
		if(!Get_watch_addr(add))
			return 0;
    Watch_energy_cmd[5] = add[0];
    Watch_energy_cmd[6] = add[1];
    Watch_energy_cmd[7] = add[2];
    Watch_energy_cmd[8] = add[3];
    Watch_energy_cmd[9] = add[4];
    Watch_energy_cmd[10] = add[5];
		//control
    Watch_energy_cmd[11] = 0x68;
    Watch_energy_cmd[12] = 0x11;
    Watch_energy_cmd[13] = 0x04;
    Watch_energy_cmd[14] = 0x33;
    Watch_energy_cmd[15] = 0x33;
    Watch_energy_cmd[16] = 0x34;
    Watch_energy_cmd[17] = 0x33;
		//CS
		for(len1=4;len1<18;len1++)
    {
        sum=sum+Watch_energy_cmd[len1];
				sum=sum%0x100;

    }
    Watch_energy_cmd[18] = sum;
    Watch_energy_cmd[19] = 0x16;
		
		USART1_BufferReset();
		USART1_Send(Watch_energy_cmd,20);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[24];
		if(!USART1_Rcvndata(data))
			return 0;
		if (data[12] == 0x91 && data[13] == 0x08 && data[23] == 0x16)
		{
				/* data[21] 高位*/
				//*energy = (int)(((data[21]-0x33) << 24) | ((data[20] - 0x33)<< 16) | (data[19] - 0x33)<<8 | (data[18] - 0x33));
			memcpy(energy,&data[18],4);
			return 1;
		}
		
		return 0;
}

//读峰值 peak FE FE FE FE 68 66 48 60 10 00 00 68 11 04 33 35 34 33 D2 16
int Read_watch_energy_peak(u8 *peak)
{
    u8 len1 = 0;
    u8 sum = 0;

		Watch_energy_cmd[0] = 0xFE;
		Watch_energy_cmd[1] = 0xFE;
		Watch_energy_cmd[2] = 0xFE;
		Watch_energy_cmd[3] = 0xFE;
    Watch_energy_cmd[4] = 0x68;
		//address
		if(!Get_watch_addr(add))
			return 0;
    Watch_energy_cmd[5] = add[0];
    Watch_energy_cmd[6] = add[1];
    Watch_energy_cmd[7] = add[2];
    Watch_energy_cmd[8] = add[3];
    Watch_energy_cmd[9] = add[4];
    Watch_energy_cmd[10] = add[5];
		//control
    Watch_energy_cmd[11] = 0x68;
    Watch_energy_cmd[12] = 0x11;
    Watch_energy_cmd[13] = 0x04;
    Watch_energy_cmd[14] = 0x33;
    Watch_energy_cmd[15] = 0x35;
    Watch_energy_cmd[16] = 0x34;
    Watch_energy_cmd[17] = 0x33;
		//CS
		for(len1=4;len1<18;len1++)
    {
        sum=sum+Watch_energy_cmd[len1];
				sum=sum%0x100;

    }
    Watch_energy_cmd[18] = sum;
    Watch_energy_cmd[19] = 0x16;
		
		USART1_BufferReset();
		USART1_Send(Watch_energy_cmd,20);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[24];
		if(!USART1_Rcvndata(data))
			return 0;
		if (data[12] == 0x91 && data[13] == 0x08 && data[23] == 0x16)
		{
			memcpy(peak,&data[18],4);
			return 1;
		}
		
		return 0;
}

//读平值 normal FE FE FE FE 68 66 48 60 10 00 00 68 11 04 33 36 34 33 D3 16
int Read_watch_energy_normal(u8 *normal)
{
    u8 len1 = 0;
    u8 sum = 0;

		Watch_energy_cmd[0] = 0xFE;
		Watch_energy_cmd[1] = 0xFE;
		Watch_energy_cmd[2] = 0xFE;
		Watch_energy_cmd[3] = 0xFE;
    Watch_energy_cmd[4] = 0x68;
		//address
		if(!Get_watch_addr(add))
			return 0;
    Watch_energy_cmd[5] = add[0];
    Watch_energy_cmd[6] = add[1];
    Watch_energy_cmd[7] = add[2];
    Watch_energy_cmd[8] = add[3];
    Watch_energy_cmd[9] = add[4];
    Watch_energy_cmd[10] = add[5];
		//control
    Watch_energy_cmd[11] = 0x68;
    Watch_energy_cmd[12] = 0x11;
    Watch_energy_cmd[13] = 0x04;
    Watch_energy_cmd[14] = 0x33;
    Watch_energy_cmd[15] = 0x36;
    Watch_energy_cmd[16] = 0x34;
    Watch_energy_cmd[17] = 0x33;
		//CS
		for(len1=4;len1<18;len1++)
    {
        sum=sum+Watch_energy_cmd[len1];
				sum=sum%0x100;

    }
    Watch_energy_cmd[18] = sum;
    Watch_energy_cmd[19] = 0x16;
		
		USART1_BufferReset();
		USART1_Send(Watch_energy_cmd,20);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[24];
		if(!USART1_Rcvndata(data))
			return 0;
		if (data[12] == 0x91 && data[13] == 0x08 && data[23] == 0x16)
		{
			memcpy(normal,&data[18],4);
			return 1;
		}
		
		return 0;
}

//读谷值 valley FE FE FE FE 68 66 48 60 10 00 00 68 11 04 33 37 34 33 D4 16
int Read_watch_energy_valley(u8 *valley)
{
    u8 len1 = 0;
    u8 sum = 0;

		Watch_energy_cmd[0] = 0xFE;
		Watch_energy_cmd[1] = 0xFE;
		Watch_energy_cmd[2] = 0xFE;
		Watch_energy_cmd[3] = 0xFE;
    Watch_energy_cmd[4] = 0x68;
		//address
		if(!Get_watch_addr(add))
			return 0;
    Watch_energy_cmd[5] = add[0];
    Watch_energy_cmd[6] = add[1];
    Watch_energy_cmd[7] = add[2];
    Watch_energy_cmd[8] = add[3];
    Watch_energy_cmd[9] = add[4];
    Watch_energy_cmd[10] = add[5];
		//control
    Watch_energy_cmd[11] = 0x68;
    Watch_energy_cmd[12] = 0x11;
    Watch_energy_cmd[13] = 0x04;
    Watch_energy_cmd[14] = 0x33;
    Watch_energy_cmd[15] = 0x37;
    Watch_energy_cmd[16] = 0x34;
    Watch_energy_cmd[17] = 0x33;
		//CS
		for(len1=4;len1<18;len1++)
    {
        sum=sum+Watch_energy_cmd[len1];
				sum=sum%0x100;

    }
    Watch_energy_cmd[18] = sum;
    Watch_energy_cmd[19] = 0x16;
		
		USART1_BufferReset();
		USART1_Send(Watch_energy_cmd,20);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[24];
		if(!USART1_Rcvndata(data))
			return 0;
		if (data[12] == 0x91 && data[13] == 0x08 && data[23] == 0x16)
		{
			memcpy(valley,&data[18],4);
			return 1;
		}
		
		return 0;
}

#if 0
/********************************************************
函数名称：void Watch_brakeOpen_cmd(void)
功能描述: get the address and control 电表开闸关电
全局变量：
参数说明：
返回说明：
设计 人：L
版本：1.0
说明：		
********************************************************/
int Watch_brakeOpen(void)
{
    u8 len1 = 0;	
    u8 sum = 0;
		//FE FE FE FE 68 67 48 60 10 99 00 68 1C 09 35 44 44 44 87 86 74 78 4D F4 16
		Watch_brakeOpen_buf[0] = 0xFE;
		Watch_brakeOpen_buf[1] = 0xFE;
		Watch_brakeOpen_buf[2] = 0xFE;
		Watch_brakeOpen_buf[3] = 0xFE;
    Watch_brakeOpen_buf[4] = 0x68;
		//address
    Watch_brakeOpen_buf[5] = add[0];
    Watch_brakeOpen_buf[6] = add[1];
    Watch_brakeOpen_buf[7] = add[2];
    Watch_brakeOpen_buf[8] = add[3];
    Watch_brakeOpen_buf[9] = add[4];
    Watch_brakeOpen_buf[10] = add[5];
    Watch_brakeOpen_buf[11] = 0x68;
    Watch_brakeOpen_buf[12] = 0x1C;
    Watch_brakeOpen_buf[13] = 0x09;
    Watch_brakeOpen_buf[14] = 0x35;
    Watch_brakeOpen_buf[15] = 0x44;
    Watch_brakeOpen_buf[16] = 0x44;
    Watch_brakeOpen_buf[17] = 0x44;
	  Watch_brakeOpen_buf[18] = 0x87;
	  Watch_brakeOpen_buf[19] = 0x86;
	  Watch_brakeOpen_buf[20] = 0x74;
	  Watch_brakeOpen_buf[21] = 0x78;
		Watch_brakeOpen_buf[22] = 0x4D;
		for(len1=4;len1<22;len1++)
    {
      sum=sum+Watch_brakeOpen_buf[len1];
			sum=sum%0x100;
    }
    Watch_brakeOpen_buf[23] = sum;
    Watch_brakeOpen_buf[24] = 0x16;
		
		USART1_BufferReset();
		USART1_Send(Watch_brakeOpen_buf,25);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[20];
		if(!USART1_Rcvndata(data))
			return 0;
	
		if (data[12] == 0x9c && data[13] == 0)
		{
				return 1;
		}	
		else
		{
			return 0;
		}
}

/********************************************************
函数名称：void Watch_brakeclose_cmd(void)
功能描述: 电表合闸送电
全局变量：
参数说明：
返回说明：
设计 人：M
版本：1.0
说明：		 
********************************************************/
int Watch_brakeClose(void)
{
    u8 len1 = 0;	
    u8 sum = 0;	
		//FE FE FE FE 68 67 48 60 10 99 00 68 1C 09 35 44 44 44 87 86 74 78 4F F6 16
	//68674860109900681C0935444444878674784FF616

		Watch_brakeClose_buf[0] = 0xFE;
		Watch_brakeClose_buf[1] = 0xFE;
		Watch_brakeClose_buf[2] = 0xFE;
		Watch_brakeClose_buf[3] = 0xFE;
	
    Watch_brakeClose_buf[4] = 0x68;
		//address
    Watch_brakeClose_buf[5] = add[0];
    Watch_brakeClose_buf[6] = add[1];
    Watch_brakeClose_buf[7] = add[2];
    Watch_brakeClose_buf[8] = add[3];
    Watch_brakeClose_buf[9] = add[4];
    Watch_brakeClose_buf[10] = add[5];
    Watch_brakeClose_buf[11] = 0x68;
    Watch_brakeClose_buf[12] = 0x1C;
    Watch_brakeClose_buf[13] = 0x09;
    Watch_brakeClose_buf[14] = 0x35;
    Watch_brakeClose_buf[15] = 0x44;
    Watch_brakeClose_buf[16] = 0x44;
    Watch_brakeClose_buf[17] = 0x44;
	  Watch_brakeClose_buf[18] = 0x87;
	  Watch_brakeClose_buf[19] = 0x86;
	  Watch_brakeClose_buf[20] = 0x74;
	  Watch_brakeClose_buf[21] = 0x78;
		Watch_brakeClose_buf[22] = 0x4F;
		for(len1=4;len1<22;len1++)
    {
      sum=sum+Watch_brakeClose_buf[len1];
			sum=sum%0x100;
    }
    Watch_brakeClose_buf[23] = sum;
    Watch_brakeClose_buf[24] = 0x16;

		USART1_BufferReset();
		USART1_Send(Watch_brakeClose_buf,25);		//发送指令
		TICK_DelaySecond(1);
		unsigned char data[20];
		if(!USART1_Rcvndata(data))
			return 0;
		
		if (data[12] == 0x9c && data[13] == 0)
		{
				return 1;
		}	
		else
		{
			return 0;
		}		
}
#endif
//int Watch_request_data(void)
//{
//		if(!Get_watch_addr(add))
//			return 0;
//		Watch_sync();
//		int energy;
//		if(!Read_watch_energy(&energy))
//			return 0;
////		if(!Watch_brakeOpen())
////			return 0;
////		if(!Watch_brakeClose())
////			return 0;
//		
//		return 1;
//}
