#ifndef __USART_H
#define __USART_H
#include "sys.h" 
#include "myQueue.h"


#define USART1_QUEUE_SIZE			10	//队列长度，根据MCU内存调整
#define USART1_QUEUE_LENGTH		256 	//队列大小，一次采样数据
#define USART2_QUEUE_SIZE			10	//队列长度，根据MCU内存调整
#define USART2_QUEUE_LENGTH		256 	//队列大小，一次采样数据


#define UART1_DEBUG  1
#define USART_REC_LEN  			255 //串口接收缓冲区大小
//////////////////////////////////////////////////////////////////////////////
#if UART1_DEBUG == 1
#define Main_printf(...) printf(__VA_ARGS__)
#else
#define Main_printf(...) 
#endif
//////////////////////////////////////////////////////////////////////////////
#define UART_REC_LENGTH		256	//接收缓冲区
#define UART_SEND_LENGTH	256	//发送缓冲区
__align(4) typedef struct {
		u16 sendlength; 
		u16 reclength; 	
		u8 *sendbuf;				//发送缓冲区
		u8 *recbuf;					//接收缓冲区
		QueueInfo *USART_Queue;
}_UART_Info;
extern  _UART_Info *UART_Info ; //上传数据结构
//////////////////////////////////////////////////////////////////////////////
extern u8 USART1_RX_BUF[USART_REC_LEN] ;		//接收缓冲                         
extern u16 USART1_RX_LEN;
extern u16 USART1_TIME;
         		
extern u8 USART2_RX_BUF[USART_REC_LEN] ;		//接收缓冲   
extern u16 USART2_RX_LEN;
extern u16 USART2_TIME;  
/////////////////////////////////////////////////////////////////////////////
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart4_init(u32 bound);

void u2_printf(char* fmt,...);  
void u3_printf(char* fmt,...);  
void u4_printf(char* fmt,...);  

void USART2_Clear(void);
void USART3_Clear(void);
void USART4_Clear(void);

//////////////////////////////////////////////////////////////////////////////
void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len);
void UART1_Send_STR(u8* cmd);
void UART2_Send_STR(u8* cmd);
s16 Str_search(u8* buf,u8 len,u8* result);//判断指定字符串是否在数组中
s16 Char_search(u8* buf,u8 len,u8 result);//判断指定字符是否在数组中

#endif

