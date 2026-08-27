#ifndef __OPENBCI_H
#define __OPENBCI_H	 
#include "sys.h"


//////////////////////////////////////////////////////////////////////////////
#define OPENBCI_USE_8_CANNLE	1
#define OPENBCI_USE_16_CANNLE	2
//////////////////////////////////////////////////////////////////////////////
#define OPENBCI_DEBUG  0				//开启串口调试？
#define OPENBCI_USE_CANNLE 			OPENBCI_USE_16_CANNLE		//通道数

#define OPENBCI_UART 						USART2										//串口号
#define OPENBCI_UART_INFO 			UART2_Info					//串口队列
#define OPENBCI_Send_STR(x) 		UART2_Send_STR(x)		//串口发送函数

//////////////////////////////////////////////////////////////////////////////
//#define OPENBCI_REC_LENGTH	256	//接收缓冲区
//#define OPENBCI_SEND_LENGTH	256	//发送缓冲区
__align(4) typedef struct {
	
		u8 Connect_Flag;
//		u8 *sendbuf;				//发送缓冲区
//		u8 *recbuf;					//接收缓冲区
//		u8 send_length; 
//		u8 rec_length; 

}_OPENBCI_Info;
extern  _OPENBCI_Info *OPENBCI_Info ; //上传数据结构
//////////////////////////////////////////////////////////////////////////////
#if OPENBCI_DEBUG == 1
#define OPENBCI_printf(...) Main_printf(__VA_ARGS__)
#else
#define OPENBCI_printf(...) 
#endif
//////////////////////////////////////////////////////////////////////////////
#define	OPENBCI_CANNEL_1_SERCH		"x1060110X"
#define	OPENBCI_CANNEL_2_SERCH		"x2060110X"
#define	OPENBCI_CANNEL_3_SERCH		"x3060110X"
#define	OPENBCI_CANNEL_4_SERCH		"x4060110X"
#define	OPENBCI_CANNEL_5_SERCH		"x5060110X"
#define	OPENBCI_CANNEL_6_SERCH		"x6060110X"
#define	OPENBCI_CANNEL_7_SERCH		"x7060110X"
#define	OPENBCI_CANNEL_8_SERCH		"x8060110X"

#define	OPENBCI_CANNEL_9_SERCH		"xQ060110X"
#define	OPENBCI_CANNEL_10_SERCH		"xW060110X"
#define	OPENBCI_CANNEL_11_SERCH		"xE060110X"
#define	OPENBCI_CANNEL_12_SERCH		"xR060110X"
#define	OPENBCI_CANNEL_13_SERCH		"xT060110X"
#define	OPENBCI_CANNEL_14_SERCH		"xY060110X"
#define	OPENBCI_CANNEL_15_SERCH		"xU060110X"
#define	OPENBCI_CANNEL_16_SERCH		"xI060110X"
//////////////////////////////////////////////////////////////////////////////


u8 OPENBCI_Connect_Init();

#endif
