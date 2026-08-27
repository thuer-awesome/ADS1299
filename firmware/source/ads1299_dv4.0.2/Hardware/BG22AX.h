#ifndef _BG22AX_H
#define _BG22AX_H
#include "sys.h"



/*RES模块复位脚， 高电平有效*/
/*SLEEP 输入脚，控制模块进入低功耗睡眠状态或进入工作状态。不管任何功耗状态，例如广播
中或者是连接后，都可以通过此脚电平切换进入低功耗状态，不影响蓝牙连接或通信。
下降沿 1→0：模块使能， 正常工作状态上升沿 0→1：模块睡眠，进入低功耗状态，
此时模块不可发送蓝牙数据，但模块可以接收蓝牙数据并从串口 TX 脚发给主机，此时
仍然会每 600ms 广播一次，可以被发现并连接。 此时若收到 AT 指令，无返回值到主机。
*/
/*LINK脚输出脚，连接状态指示，0：已连接 1：未连接*/


#define BG22AX_UART_DEBUG		1 				//串口调试开关
////////////////////////////////////////////////////////////////////////////////
//上位机选择 只能二选一
#define  USE_NIMING 	1
#define  USE_OPENBCI 	0

#define BG22AX_LINK				PBin(7)
#define BG22AX_RES				PAout(1)
#define BG22AX_RTS				PBin(4)

////////////////////////////////////////////////////////////////////////////////
#define BG22AX_UART						USART2
#define BG22AX_uart_init(x)		uart2_init(x)
#define BG22AX_RX_LEN					USART2_RX_LEN
#define BG22AX_RX_BUF					USART2_RX_BUF
#define BG22AX_CLEAR() 				USART2_Clear()
#define BG22AX_UART_INFO 			UART2_Info					//串口队列
////////////////////////////////////////////////////////////////////////////////
//AT指令
#define BG22AX_GOTO_AT	"+++"				//进入 AT 指令模式
#define BG22AX_AT_MAC  "AT+MAC?\r\n"	//查询MAC地址 REDACTED0000012位
#define BG22AX_AT_ROLE  "AT+ROLE=0\r\n"	//从机
#define BG22AX_AT_TPL  "AT+POWER=0\r\n"	//设置发射功率等级 最大为0

#if USE_NIMING==1
	#define BG22AX_AT_PACK  "AT+PACK=195,1\r\n"//设置串口打包长度和打包时间 
#endif
#if USE_OPENBCI==1
	#define BG22AX_AT_PACK  "AT+PACK=66,1\r\n"//设置串口打包长度和打包时间  
#endif


#define BG22AX_AT_PHY  "AT+PHY=2\r\n"//物理层速率
#define BG22AX_AT_WDOG  "AT+WDOG=0\r\n"//关闭开门狗
#define BG22AX_AT_RST  "AT+RESTART\r\n"//重启
#define BG22AX_AT_NAME	"AT+NAME?\r\n"
#define BG22AX_AT_DEFAULT	"AT+RESET\r\n" //恢复出厂设置

//#define BG22AX_INIT_BAUD		115200	//要设置的波特率
//#define BG22AX_AT_UART  "AT+UART=115200\r\n"//设置波特率
//#define BG22AX_INIT_BAUD		256000	//要设置的波特率
//#define BG22AX_AT_UART  "AT+UART=256000\r\n"//设置波特率
#define BG22AX_INIT_BAUD		460800	//要设置的波特率
#define BG22AX_AT_UART  "AT+UART=460800\r\n"//设置波特率
//#define BG22AX_INIT_BAUD		921600	//要设置的波特率
//#define BG22AX_AT_UART  "AT+UART=921600\r\n"//设置波特率

#define BG22AX_AT_VERSION  "AT+VERSION\r\n"//固件版本
#define BG22AX_AT_EXIT  "AT+EXIT\r\n"//退出命令模式

#define BG22AX_AT_SLEEP  "AT+SLEEP=0,0\r\n"//休眠模式，串口功能关闭后， 可通过先把 CTS 引脚置高再置低来唤醒串口

//返回值
#define	BG22AX_AT_OK	"OK"						//正确指令
#define	BG22AX_AT_ERR1 	"ERR=1"				//无效的命令格式	
#define	BG22AX_AT_ERR2 	"ERR=2"				//无效的参数
#define	BG22AX_AT_NOCMD		"NO ATCMD"	//无效的命令
#define	BG22AX_AT_ONLINE		"OnLine"	//成功连接
#define	BG22AX_AT_OFFLINE		"OffLine"	//没有连接
////////////////////////////////////////////////////////////////////////////////
//串口DEBUG
#if BG22AX_UART_DEBUG == 1 
		#define BG22AX_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define BG22AX_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////

extern u8 BG22AX_SET_FLAG;


void BG22AX_GPIO_Init(void);
void BG22AX_SET(void);
u8 BG22AX_AT_Confg(void);
void BG22AX_Init(void);


u8 BG22AX_Goto_CMD(void);
u8 BG22AX_Exit_CMD(void);//退出命令模式
u8 BG22AX_Wait_Connect(void);
u8 BG22AX_LowPower(void);
u8 BG22AX_PowerOn(void);

void BG22AX_Init();

#endif




