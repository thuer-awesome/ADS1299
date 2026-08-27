#ifndef _RF4404B4_H
#define _RF4404B4_H
#include "sys.h"

#include "usart.h"       //串口 


/*RES模块复位脚， 高电平有效*/
/*SLEEP 输入脚，控制模块进入低功耗睡眠状态或进入工作状态。不管任何功耗状态，例如广播
中或者是连接后，都可以通过此脚电平切换进入低功耗状态，不影响蓝牙连接或通信。
下降沿 1→0：模块使能， 正常工作状态上升沿 0→1：模块睡眠，进入低功耗状态，
此时模块不可发送蓝牙数据，但模块可以接收蓝牙数据并从串口 TX 脚发给主机，此时
仍然会每 600ms 广播一次，可以被发现并连接。 此时若收到 AT 指令，无返回值到主机。
*/
/*LINK脚输出脚，连接状态指示，0：已连接 1：未连接*/

////////////////////////////////////////////////////////////////////////////////
#define RF4404B4_UART_DEBUG		1 				//串口调试开关
////////////////////////////////////////////////////////////////////////////////
#define RF4404B4_BCTS 			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)
#define RF4404B4_LINK				HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)

#define RF4404B4_RES(n)			(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET))
#define RF4404B4_EN(n)			(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))
#define RF4404B4_BRTS(n)		(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET))
////////////////////////////////////////////////////////////////////////////////
#define RF4404B4_UART						USART2
#define RF4404B4_USART_Handle 	UART2_Handler
#define RF4404B4_USART_IRQn 		USART2_IRQn
#define RF4404B4_uart_init(x)		uart2_init(x)
#define RF4404B4_RX_BUF					USART2_RX_BUF
#define RF4404B4_RX_LEN					USART2_RX_LEN
#define RF4404B4_CLEAR() 				USART2_Clear()
//#define RF4404B4_UART_INFO 			UART2_Info					//串口队列
////////////////////////////////////////////////////////////////////////////////
//数据包最大 248字节
//注： Android 的 MTU 为 251 字节， IOS 的为 185 字节，发送时每包大小为 MTU-3 个字节。

//连接间隔默认20ms，最大2000ms

//AT指令
#define RF4404B4_GOTO_AT	"TTM:EUP-OFF"		//进入 AT 指令模式
//#define RF4404B4_AT_ROLE  "AT+ROLE=0\r\n"	//从机


//使用蓝牙模块作为主角色的， 连接间隔可以达到 20 ms 以下（最小 8 ms）；
//一般手机由于系统问题， 连接间隔最小只能到 20 ms；		
//#if USE_NIMING_V7 ==1
//	#define RF4404B4_AT_INTERVAL  "TTM:CIT-30ms"//设置连接间隔  主从要一致
//#elif USE_USER_PROTO ==1
//	#define RF4404B4_AT_INTERVAL  "TTM:CIT-80ms"//设置连接间隔  主从要一致
//#endif
	#define RF4404B4_AT_INTERVAL  "TTM:CIT-20ms"//设置连接间隔  主从要一致


#define RF4404B4_AT_NAME	"TTM:NAM-?" //获取设备名称

#define RF4404B4_INIT_BAUD		115200	//要设置的波特率
#define RF4404B4_AT_UART  "TTM:BPS-115200"//设置波特率

#define RF4404B4_AT_MAC  "TTM:MAC-?\r\n"	//查询MAC地址 REDACTED0000012位
#define RF4404B4_AT_VERSION  "TTM:VER-?"//固件版本

#define RF4404B4_AT_RST  "TTM:RST-SYSTEMRESET"//重启
#define RF4404B4_AT_DEFAULT	"TTM:RST-RESET" //恢复出厂设置

#define RF4404B4_AT_EUP  "TTM:EUP-OFF\r\n"//关闭EN内部上拉，节能

#define RF4404B4_AT_TPL  "TTM:TPL-(2)\r\n"	//设置发射功率等级 最大为2

#define RF4404B4_AT_PHY  "TTM:PHY-(1)\r\n"//物理层速率


#define	RF4404B4_AT_OK	"TTM:OK\r\n"						//正确指令
#define	RF4404B4_AT_ERR 	"TTM:ERP\r\n"				//无效的命令格式	
////////////////////////////////////////////////////////////////////////////////
//串口DEBUG
#if RF4404B4_UART_DEBUG == 1 
		#define RF4404B4_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define RF4404B4_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////

extern u8 RF4404B4_SET_FLAG;



void RF4404B4_GPIO_Init(void);
void RF4404B4_SET(void);
u8 RF4404B4_AT_Confg(void);
void RF4404B4_Init(void);
void RF4404B4_Send(u8* cmd, u8 len);


u8 RF4404B4_Goto_CMD(void);
u8 RF4404B4_Exit_CMD(void);//退出命令模式
u8 RF4404B4_Wait_Connect(void);
u8 RF4404B4_LowPower(void);
u8 RF4404B4_PowerOn(void);

void RF4404B4_Init();

#endif




