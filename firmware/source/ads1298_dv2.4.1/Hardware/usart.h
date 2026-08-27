#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "myQueue.h"


//////////////////////////////////////////////////////////////////////////
#define UART_DEBUG 1 //串口使能
#define	USART_REC_LEN 		256

#define UART_REC_LENGTH		256	//接收缓冲区
//////////////////////////////////////////////////////////////////////////////
#if UART_DEBUG == 1 
		#define Main_printf(...)    printf(__VA_ARGS__)		//串口1
#else
		#define Main_printf(...)		
#endif
//////////////////////////////////////////////////////////////////////////////
//extern u8 USART1_RX_BUF[USART_REC_LEN] ;		//接收缓冲                         
//extern u16 USART1_RX_LEN;
//extern u16 USART1_TIME;
//extern u16 USART1_RX_EVENT;

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


