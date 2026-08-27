#include "BG22AX.h"
#include "delay.h"       //延时
#include "usart.h"
#include "led.h"
#include "string.h"
#include "key.h"






/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/05/18
//版本：V1.4
//适用于HAL库
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



//IO初始化函数
void BG22AX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOA_CLK_ENABLE();

		//LINK
		GPIO_Initure.Pin=GPIO_PIN_10;	
    GPIO_Initure.Mode=GPIO_MODE_INPUT;
    GPIO_Initure.Pull=GPIO_NOPULL;	//无上拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		
		//RES
		GPIO_Initure.Pin = GPIO_PIN_11;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
		
	
		BG22AX_RES(0);//取消复位
		delay_ms(100);
		BG22AX_RES(1);//取消复位
}


//u8 BG22AX_SET_FLAG=0;
////设置检测
//void BG22AX_SET(void)
//{
//		u16 ret=0;
//		if (KEY1 == 0)//按键按下
//		{	
//				BG22AX_DEBUG("按键按下\r\n");
//				while( KEY1 == 0)
//				{
//						LED_1(0);
//						delay_ms(10);
//						ret++;
//						if ( ret > 300 )//3s
//						{
//								LED_1(1);
//								BG22AX_SET_FLAG=1;//设置蓝牙
//								BG22AX_DEBUG("按键有效\r\n");
//								while( KEY1 == 0);
//								BG22AX_DEBUG("按键松开\r\n");
//								return ;//退出函数
//						}
//				}
//				LED_1(1);
//				BG22AX_DEBUG("按键松开\r\n");
//		}
//}




//void BG22AX_Send(u8* cmd, u8 len)
//{
//	  USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) cmd, len);
//}


//发送指令，并判断指定字符串是否缓冲区中
//返回-1 没有指定的串
//0-125 字符串在数组中的位置
s16 BG22AX_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	
	  BG22AX_CLEAR();//清空缓冲区	
    USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		//BG22AX_DEBUG("rsp:%s \r\n",BG22AX_RX_BUF);		
		//BG22AX_DEBUG("SENDCMD:%s rsp:%s \r\n",cmd,BG22AX_RX_BUF);		
		res = Str_search(BG22AX_RX_BUF,BG22AX_RX_LEN,result);//查询字符串
	
		return res;
}


//进入命令模式
u8 BG22AX_Goto_CMD(void)
{
		BG22AX_DEBUG("进入命令模式\r\n");
	
		BG22AX_DEBUG("当前波特率 %d\r\n",BG22AX_INIT_BAUD);
		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化
		BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
		if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
		{
				BG22AX_DEBUG("成功进入命令模式\r\n");
				return 0;	
		}		
		
		
		while(1)//进入AT模式
		{		
				BG22AX_DEBUG("当前波特率 115200\r\n");
				BG22AX_uart_init(115200);	//串口初始化
				BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
				{
						BG22AX_DEBUG("成功进入命令模式\r\n");
						break;
				}			

				BG22AX_DEBUG("当前波特率 256000\r\n");
				BG22AX_uart_init(256000);	//串口初始化
				BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
				{
						BG22AX_DEBUG("成功进入命令模式\r\n");
						break;
				}			
				
				BG22AX_DEBUG("当前波特率 460800\r\n");
				BG22AX_uart_init(460800);	//串口初始化
				BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
				{
						BG22AX_DEBUG("成功进入命令模式\r\n");
						break;
				}			
				
				BG22AX_DEBUG("当前波特率 921600\r\n");
				BG22AX_uart_init(921600);	//串口初始化
				BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
				{
						BG22AX_DEBUG("成功进入命令模式\r\n");
						break;
				}			
		}
		
		BG22AX_DEBUG("查询固件版本和名称\r\n");
		if(BG22AX_SendCmd(BG22AX_AT_VERSION,BG22AX_AT_OK, 500) < 0)//查询固件版本
		{
				return 1;
		}
		BG22AX_DEBUG("VER:%s",BG22AX_RX_BUF);				
		BG22AX_SendCmd(BG22AX_AT_NAME,BG22AX_AT_OK, 500);
		BG22AX_DEBUG("NAME:%s",BG22AX_RX_BUF);	
		//BG22AX_SendCmd(BG22AX_AT_EXIT,BG22AX_AT_OK, 500);//退出命令模式

		return 0;					
}


u8 BG22AX_Exit_CMD(void)//退出命令模式
{
		BG22AX_SendCmd(BG22AX_AT_EXIT,BG22AX_AT_OK, 500);//退出命令模式
}


//查询连接状态
//0 已连接 1未连接
u8 BG22AX_Wait_Connect(void)
{		
		if(BG22AX_LINK == 0)//成功连接
		{
				return 0;
		}
		else
		{
				return 1;
		}
}
//蓝牙进入低功耗模式
u8 BG22AX_LowPower(void)
{	
		if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
		{
				BG22AX_SendCmd(BG22AX_AT_SLEEP,BG22AX_AT_OK, 500);	//进入超低功耗模式
		}		
}	
//蓝牙唤醒
u8 BG22AX_PowerOn(void)
{
		BG22AX_RES(1);//取消复位
		delay_ms(100);
		BG22AX_RES(0);//复位
		delay_ms(100);
		BG22AX_RES(1);//取消复位
		delay_s(2);
}


u8 BG22AX_AT_Confg(void)
{	
		u8 name[30]="BM-";
		u8 namestr[40]="AT+NAME=0,";
	
		memset(namestr+10,0,30);
/////////////////////////////////////////////////////////////////////////////////////
		BG22AX_Goto_CMD();
/////////////////////////////////////////////////////////////////////////////////////	
		BG22AX_DEBUG("恢复出厂设置\r\n");
		BG22AX_SendCmd(BG22AX_AT_DEFAULT,BG22AX_AT_OK, 1000) ;//恢复出厂设置
		BG22AX_Goto_CMD();	
/////////////////////////////////////////////////////////////////////////////////////
		//查询MAC并设置名字
		BG22AX_DEBUG("查询MAC并设置蓝牙名称\r\n");
		if(BG22AX_SendCmd(BG22AX_AT_MAC,BG22AX_AT_OK, 500) < 0)//查询MAC
		{
				return 1;
		}
		//格式化名称
		//memcpy(name+3,BG22AX_RX_BUF+7,12);
		memcpy(name+3,BG22AX_RX_BUF+7,2);
		memcpy(name+5,BG22AX_RX_BUF+10,2);
		memcpy(name+7,BG22AX_RX_BUF+13,2);
		memcpy(name+9,BG22AX_RX_BUF+16,2);
		memcpy(name+11,BG22AX_RX_BUF+19,2);
		memcpy(name+13,BG22AX_RX_BUF+22,2);
		strcat(namestr,name);//追加到指令后面
		strcat(namestr,"\r\n");//追加\r\n	
		BG22AX_DEBUG("namestr is :%s",namestr);//要设置的名字	
		if(BG22AX_SendCmd(namestr,BG22AX_AT_OK, 500) < 0)//设置名字 1~15字节 	
		{
				return 2;
		}
/////////////////////////////////////////////////////////////////////////////////////			
		BG22AX_DEBUG("设置功率等级，设置为从机模式\r\n");
		if(BG22AX_SendCmd(BG22AX_AT_ROLE,BG22AX_AT_OK, 500) < 0)//设置从机模式
		{
				return 4;
		}
		if(BG22AX_SendCmd(BG22AX_AT_TPL,BG22AX_AT_OK, 500) < 0)//设置功率等级
		{
				return 5;
		}
		if(BG22AX_SendCmd(BG22AX_AT_PACK,BG22AX_AT_OK, 500) < 0)//设置串口打包长度
		{
				return 6;
		}
//		if(BG22AX_SendCmd(BG22AX_AT_PHY,BG22AX_AT_OK, 500) < 0)//设置物理层速率
//		{
//				return 7;
//		}
		if(BG22AX_SendCmd(BG22AX_AT_WDOG,BG22AX_AT_OK, 500) < 0)//关闭看门狗
		{
				return 8;
		}
		if(BG22AX_SendCmd(BG22AX_AT_UART,BG22AX_AT_OK, 500) < 0)//设置波特率
		{
				return 3;
		}
		if(BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500) < 0)//重启
		{
				return 9;
		}	
/////////////////////////////////////////////////////////////////////////////////////
		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化
		BG22AX_DEBUG("重新进入命令模式\r\n");
		while(1) //重新进入AT模式
		{
				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
				{
						break;
				}
				else
				{
						BG22AX_DEBUG("等待\r\n");
				}
				delay_ms(500);
		}	
		BG22AX_Exit_CMD();//退出命令模式
		/////////////////////////////////////////////////////////////////////////////////////				
		//HAL_NVIC_DisableIRQ(BLE103_USART_IRQn);						//关闭USART中断通道
		//__HAL_UART_DISABLE_IT(&BLE103_USART_Handle, UART_IT_RXNE); //关闭接收中断	
		
		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化
		
		BG22AX_DEBUG("BG22AX INIT OK\r\n");	
		return 0;
}




void BG22AX_Init(void)
{			
		BG22AX_PowerOn();

		BG22AX_DEBUG("AT测试 当前波特率 %d\r\n",BG22AX_INIT_BAUD);
		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化
		BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//退出命令模式
		if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
		{
				BG22AX_SendCmd(BG22AX_AT_MAC,BG22AX_AT_OK, 500);//查询MAC
				BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500);//重启
			
				//USART_ITConfig(BG22AX_UART, USART_IT_RXNE, DISABLE);//串口接收中断	
				//__HAL_UART_DISABLE_IT(&BG22AX_USART_Handle, UART_IT_RXNE); //关闭接收中断	
			
				BG22AX_DEBUG("初始化成功\r\n");
		}		
		else
		{
				BG22AX_DEBUG("无响应，按照默认参数设置蓝牙\r\n");
				BG22AX_AT_Confg();//设置蓝牙
		}
}






