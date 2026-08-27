#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "myQueue.h"
#include "RF4404B4.h"


//////////////////////////////////////////////////////////////////////////
#define UART_DEBUG 1 //串口使能
#define	USART_REC_LEN 		256
//////////////////////////////////////////////////////////////////////////////
//协议选择 只能二选一
#define USE_NIMING_V7					0 		//匿名V7
#define USE_USER_PROTO				1 		//自定义 协议
//////////////////////////////////////////////////////////////////////////////
#if UART_DEBUG == 1 
		#define Main_printf(...)    printf(__VA_ARGS__)		//串口1
#else
		#define Main_printf(...)		
#endif
//////////////////////////////////////////////////////////////////////////////
#if USE_NIMING_V7 ==1
	#define UART_SEND_LENGTH	210	//发送缓冲区  14*15=140   
	#define UART_QUEUE_LENGTH	90	//（队列容量）  15*6=90		15次采样	30ms
	#define UART_QUEUE_SIZE		30	//队列大小区
#elif USE_USER_PROTO ==1
	#define UART_SEND_LENGTH	250	//发送缓冲区  协议 V1.1.2
	#define UART_QUEUE_LENGTH	240	//（队列容量） 40*3=240		40次采样  80ms
	#define UART_QUEUE_SIZE		30	//队列大小区
#endif
//////////////////////////////////////////////////////////////////////////////
__align(4) typedef struct {
	
		u16 datalength; 
		u8 *sendbuf;				//发送缓冲区
		//u8 *recbuf;					//接收缓冲区
	
		u32 datanumber; 
		u8 Queue_pop_flag;	//队列数据取出标志		1BYTE
	
		QueueInfo *UART_Queue;
}_UART_Info;
extern  _UART_Info *UART_Info ; //上传数据结构
//////////////////////////////////////////////////////////////////////////////
//extern u8 USART_RX_BUF[USART_REC_LEN] ;   //接收缓冲                         
//extern u16 USART_RX_STA;         		//接收状态标记

extern u8 USART1_RX_BUF[USART_REC_LEN] ;		//接收缓冲                         
extern u16 USART1_RX_LEN;
extern u16 USART1_TIME;
extern u16 USART1_RX_EVENT;

extern u8 USART2_RX_BUF[USART_REC_LEN] ;		//接收缓冲   
extern u16 USART2_RX_LEN;
extern u16 USART2_TIME;  
extern u16 USART2_RX_EVENT;
/////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef UART1_Handler; //UART句柄
extern UART_HandleTypeDef UART2_Handler; //UART句柄
extern UART_HandleTypeDef UART3_Handler; //UART句柄
/////////////////////////////////////////////////////////////////////////////
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
/////////////////////////////////////////////////////////////////////////////
void USART1_Clear(void);
void USART2_Clear(void);
void USART3_Clear(void);
/////////////////////////////////////////////////////////////////////////////

void USARTx_Send(UART_HandleTypeDef* huart, u8 *Data, u8 Len);

s16 Str_search(u8* buf,u8 len,u8* result);//判断指定字符串是否在数组中
s16 Char_search(u8* buf,u8 len,u8 result);//判断指定字符是否在数组中
#endif


