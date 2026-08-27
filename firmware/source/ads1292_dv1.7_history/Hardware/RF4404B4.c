#include "RF4404B4.h"
#include "delay.h"       //延时
#include "usart.h"
#include "string.h"






/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/05/26
//版本：V1.4
//适用于HAL库
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



//IO初始化函数
void RF4404B4_GPIO_Init(void)
{
	
		GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		//RES
		GPIO_Initure.Pin = GPIO_PIN_1;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);
		
		//EN
		GPIO_Initure.Pin = GPIO_PIN_0;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);
			
		//BRTS
		GPIO_Initure.Pin = GPIO_PIN_5;
		GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Initure.Pull = GPIO_NOPULL;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);
		
		//BCTS
		GPIO_Initure.Pin=GPIO_PIN_4;	
		GPIO_Initure.Mode=GPIO_MODE_INPUT;
		GPIO_Initure.Pull=GPIO_NOPULL;	//无上拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			
		//LINK
		GPIO_Initure.Pin=GPIO_PIN_7;	
		GPIO_Initure.Mode=GPIO_MODE_INPUT;
		GPIO_Initure.Pull=GPIO_NOPULL;	//无上拉
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			
		
		RF4404B4_RES(1); 	//不复位 	复位低电平有效
		RF4404B4_EN(1); 	//失能		使能低电平有效
		RF4404B4_BRTS(1);	//不唤醒模块 低电平唤醒，才可以发送数据
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
//					LED_1=LED_ON;
//						delay_ms(10);
//						ret++;
//						if ( ret > 300 )//3s
//						{
//								LED_1=LED_OFF;
//								BG22AX_SET_FLAG=1;//设置蓝牙
//								BG22AX_DEBUG("按键有效\r\n");
//								while( KEY1 == 0);
//								BG22AX_DEBUG("按键松开\r\n");
//								return ;//退出函数
//						}
//				}
//				LED_1=LED_OFF;
//				BG22AX_DEBUG("按键松开\r\n");
//		}
//}


//发送指令，并判断指定字符串是否缓冲区中
//返回-1 没有指定的串
//0-125 字符串在数组中的位置
s16 RF4404B4_SendCmd(u8* cmd, u8* result, u16 timeOut)
{		
		s16 res;
	
	  RF4404B4_CLEAR();//清空缓冲区	
    USARTx_Send(&RF4404B4_USART_Handle,(uint8_t*) cmd, strlen(cmd));
		delay_ms(timeOut);
		//RF4404B4_DEBUG("rsp:%s \r\n",RF4404B4_RX_BUF);		
		RF4404B4_DEBUG("SENDCMD:%s rsp:%s \r\n",cmd,RF4404B4_RX_BUF);		
		res = Str_search(RF4404B4_RX_BUF,RF4404B4_RX_LEN,result);//查询字符串
	
		return res;
}



//进入命令模式
u8 RF4404B4_Goto_CMD(void)
{
		RF4404B4_DEBUG("进入命令模式\r\n");
	
		RF4404B4_DEBUG("当前波特率 %d\r\n",RF4404B4_INIT_BAUD);
		RF4404B4_uart_init(RF4404B4_INIT_BAUD);	//串口初始化
		RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
		if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
		{
				RF4404B4_DEBUG("成功进入命令模式\r\n");
				return 0;	
		}		
				
		while(1)//进入AT模式
		{		
				RF4404B4_DEBUG("当前波特率 115200\r\n");
				RF4404B4_uart_init(115200);	//串口初始化
				RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
				if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
				{
						RF4404B4_DEBUG("成功进入命令模式\r\n");
						break;
				}			

				RF4404B4_DEBUG("当前波特率 256000\r\n");
				RF4404B4_uart_init(256000);	//串口初始化
				RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
				if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
				{
						RF4404B4_DEBUG("成功进入命令模式\r\n");
						break;
				}	

				RF4404B4_DEBUG("当前波特率 460800\r\n");
				RF4404B4_uart_init(460800);	//串口初始化
				RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
				if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
				{
						RF4404B4_DEBUG("成功进入命令模式\r\n");
						break;
				}			
				
				RF4404B4_DEBUG("当前波特率 921600\r\n");
				RF4404B4_uart_init(921600);	//串口初始化
				RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
				if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
				{
						RF4404B4_DEBUG("成功进入命令模式\r\n");
						break;
				}			
		}	
//		RF4404B4_DEBUG("查询固件版本和名称\r\n");
//		if(RF4404B4_SendCmd(RF4404B4_AT_VERSION,RF4404B4_AT_OK, 500) < 0)//查询固件版本
//		{
//				return 1;
//		}
//		RF4404B4_DEBUG("VER:%s",RF4404B4_RX_BUF);				
//		RF4404B4_SendCmd(RF4404B4_AT_NAME,RF4404B4_AT_OK, 500);
//		RF4404B4_DEBUG("NAME:%s",RF4404B4_RX_BUF);	
		
		return 0;					
}


u8 RF4404B4_Exit_CMD(void)//退出命令模式
{
		RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
}
//查询连接状态
//0 已连接 1未连接
u8 RF4404B4_Wait_Connect(void)
{		
		if(RF4404B4_LINK == 0)//成功连接
		{
				return 0;
		}
		else
		{
				return 1;
		}
}
//蓝牙进入低功耗模式
u8 RF4404B4_LowPower(void)
{	
	
		RF4404B4_EN(1); 	//失能		使能低电平有效
		RF4404B4_BRTS(1);	//不唤醒模块 低电平唤醒，可以发送数据
//		if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
//		{
//				RF4404B4_SendCmd(RF4404B4_AT_SLEEP,RF4404B4_AT_OK, 500);	//进入超低功耗模式
//		}		
}	
//蓝牙唤醒
u8 RF4404B4_PowerOn(void)
{	
		RF4404B4_EN(0); 		//使能	低电平有效
		RF4404B4_BRTS(0);	//唤醒模块，准备发送数据

		RF4404B4_RES(0); 	//复位 低电平有效
		delay_ms(100);
		RF4404B4_RES(1);
		delay_s(2);
}


u8 RF4404B4_AT_Confg(void)
{	
//		u8 name[30]="BM-";
//		u8 namestr[40]="AT+NAME=0,";
//	
//		memset(namestr+10,0,30);
///////////////////////////////////////////////////////////////////////////////////////
		RF4404B4_Goto_CMD();
///////////////////////////////////////////////////////////////////////////////////////	
		RF4404B4_DEBUG("恢复出厂设置\r\n");
		RF4404B4_SendCmd(RF4404B4_AT_DEFAULT,RF4404B4_AT_OK, 1000) ;//恢复出厂设置
		RF4404B4_Goto_CMD();	
///////////////////////////////////////////////////////////////////////////////////////
//		//查询MAC并设置名字
//		BG22AX_DEBUG("查询MAC并设置蓝牙名称\r\n");
//		if(BG22AX_SendCmd(BG22AX_AT_MAC,BG22AX_AT_OK, 500) < 0)//查询MAC
//		{
//				return 1;
//		}
//		//格式化名称
//		//memcpy(name+3,BG22AX_RX_BUF+7,12);
//		memcpy(name+3,BG22AX_RX_BUF+7,2);
//		memcpy(name+5,BG22AX_RX_BUF+10,2);
//		memcpy(name+7,BG22AX_RX_BUF+13,2);
//		memcpy(name+9,BG22AX_RX_BUF+16,2);
//		memcpy(name+11,BG22AX_RX_BUF+19,2);
//		memcpy(name+13,BG22AX_RX_BUF+22,2);
//		strcat(namestr,name);//追加到指令后面
//		strcat(namestr,"\r\n");//追加\r\n	
//		BG22AX_DEBUG("namestr is :%s",namestr);//要设置的名字	
//		if(BG22AX_SendCmd(namestr,BG22AX_AT_OK, 500) < 0)//设置名字 1~15字节 	
//		{
//				return 2;
//		}
///////////////////////////////////////////////////////////////////////////////////////			
		RF4404B4_DEBUG("设置蓝牙关键参数\r\n");
		if(RF4404B4_SendCmd(RF4404B4_AT_UART,RF4404B4_AT_OK, 500) < 0)//波特率
		{
				return 4;
		}
		if(RF4404B4_SendCmd(RF4404B4_AT_INTERVAL,RF4404B4_AT_OK, 500) < 0)//连接间隔
		{
				return 5;
		}

//		BG22AX_DEBUG("设置功率等级，设置为从机模式\r\n");
//		if(BG22AX_SendCmd(BG22AX_AT_ROLE,BG22AX_AT_OK, 500) < 0)//设置从机模式
//		{
//				return 4;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_TPL,BG22AX_AT_OK, 500) < 0)//设置功率等级
//		{
//				return 5;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_PACK,BG22AX_AT_OK, 500) < 0)//设置串口打包长度
//		{
//				return 6;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_PHY,BG22AX_AT_OK, 500) < 0)//设置物理层速率
//		{
//				return 7;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_WDOG,BG22AX_AT_OK, 500) < 0)//关闭看门狗
//		{
//				return 8;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_INTERVAL,BG22AX_AT_OK, 500) < 0)//设置连接间隔 10ms
//		{
//				return 9;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_UART,BG22AX_AT_OK, 500) < 0)//设置波特率
//		{
//				return 3;
//		}
//		if(BG22AX_SendCmd(BG22AX_AT_RST,BG22AX_AT_OK, 500) < 0)//重启
//		{
//				return 10;
//		}	
///////////////////////////////////////////////////////////////////////////////////////
//		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化
//		BG22AX_DEBUG("重新进入命令模式\r\n");
//		while(1) //重新进入AT模式
//		{
//				if(BG22AX_SendCmd(BG22AX_GOTO_AT,BG22AX_AT_OK, 500) >= 0)//进入AT模式
//				{
//						break;
//				}
//				else
//				{
//						BG22AX_DEBUG("等待\r\n");
//				}
//				delay_ms(500);
//		}	
//		BG22AX_Exit_CMD();//退出命令模式
//		/////////////////////////////////////////////////////////////////////////////////////				
//		//HAL_NVIC_DisableIRQ(BLE103_USART_IRQn);						//关闭USART中断通道
//		//__HAL_UART_DISABLE_IT(&BLE103_USART_Handle, UART_IT_RXNE); //关闭接收中断	
//		
//		BG22AX_uart_init(BG22AX_INIT_BAUD);	//串口初始化

//		BG22AX_DEBUG("BG22AX INIT OK\r\n");	
		return 0;
}




void RF4404B4_Init(void)
{			
		RF4404B4_PowerOn();

		RF4404B4_DEBUG("AT测试 默认波特率 %d\r\n",RF4404B4_INIT_BAUD);
		RF4404B4_uart_init(RF4404B4_INIT_BAUD);	//串口初始化
		RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//退出命令模式
		if(RF4404B4_SendCmd(RF4404B4_GOTO_AT,RF4404B4_AT_OK, 500) >= 0)//进入AT模式
		{
				//RF4404B4_SendCmd(RF4404B4_AT_MAC,RF4404B4_AT_OK, 500);//查询MAC
				RF4404B4_SendCmd(RF4404B4_AT_RST,RF4404B4_AT_OK, 500);//重启
			
				//USART_ITConfig(BG22AX_UART, USART_IT_RXNE, DISABLE);//串口接收中断	
				//__HAL_UART_DISABLE_IT(&BG22AX_USART_Handle, UART_IT_RXNE); //关闭接收中断	
			
				RF4404B4_DEBUG("初始化成功\r\n");
		}		
		else
		{
				
				RF4404B4_DEBUG("无响应，按照默认参数设置蓝牙\r\n");
				RF4404B4_AT_Confg();//设置蓝牙
		}
}






