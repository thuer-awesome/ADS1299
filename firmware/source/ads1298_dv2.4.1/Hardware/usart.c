#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "ADS129x.h"




UART_HandleTypeDef UART1_Handler; //UART句柄
UART_HandleTypeDef UART2_Handler; //UART句柄
UART_HandleTypeDef UART3_Handler; //UART句柄




//HAL库串口底层初始化，时钟使能，引脚配置，中断配置
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if(huart->Instance == USART1) //如果是串口1，进行串口1 MSP初始化
    {
			  __HAL_RCC_USART1_CLK_ENABLE();		//使能USART1时钟
        __HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟

        GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;	//PA9 A10
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;		//复用推挽输出
        GPIO_InitStruct.Pull = GPIO_NOPULL;			//上拉
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;		//高速
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;	//复用为USART1
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	   	//初始化
    }
		else if(huart->Instance == USART2) //如果是串口2，进行串口2 MSP初始化
		{
				__HAL_RCC_USART2_CLK_ENABLE();
				__HAL_RCC_GPIOA_CLK_ENABLE();
			
				GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			
				//A0 CTS
				GPIO_InitStruct.Pin = GPIO_PIN_0;
				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull = GPIO_NOPULL;
				GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		}
//		else if(huart->Instance == USART3) //如果是串口2，进行串口2 MSP初始化
//		{
//				__HAL_RCC_USART3_CLK_ENABLE();
//				__HAL_RCC_GPIOB_CLK_ENABLE();
//			
//				GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
//				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//				GPIO_InitStruct.Pull = GPIO_PULLUP;
//				GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//				GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
//				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//		}
}

//串口复位
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
		if(huart->Instance==USART1)
		{
				__HAL_RCC_USART1_CLK_DISABLE();
				HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
		}
		else if(huart->Instance==USART2)
		{
				__HAL_RCC_USART2_CLK_DISABLE();
				HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
			
				HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
		}
//		else if(huart->Instance==USART3)
//		{
//				__HAL_RCC_USART3_CLK_DISABLE();
//				HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);
//		}
}


u8 USART1_value ;           

//初始化串口函数
void uart1_init(u32 bound)
{
    //UART 初始化设置
    UART1_Handler.Instance = USART1;					  //USART1
    UART1_Handler.Init.BaudRate = bound;				  //波特率
    UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
	  UART1_Handler.Init.StopBits = UART_STOPBITS_1;	  //一个停止位
	  UART1_Handler.Init.Parity = UART_PARITY_NONE;		  //无奇偶校验位
		UART1_Handler.Init.Mode = UART_MODE_TX_RX;		  //收发模式
		//UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_CTS;//硬件流控
    UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控	
	
	  UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
		UART1_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART1_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if( HAL_UART_Init(&UART1_Handler) != HAL_OK) //底层硬件初始化
		{
				
		}
//		//该函数会开启接收中断：标志位 UART_IT_RXNE
//		HAL_UART_Receive_IT(&UART1_Handler,  (uint8_t *)&USART1_value, 1);	
//		
//		//__HAL_UART_ENABLE_IT(&UART1_Handler, UART_IT_RXNE); //开启接收中断
//		__HAL_UART_DISABLE_IT(&UART1_Handler, UART_IT_RXNE); //接收中断
//		
//		HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);				//抢占优先级3，子优先级3
//		
//		//HAL_NVIC_EnableIRQ(USART1_IRQn);					//使能USART中断通道
//		HAL_NVIC_DisableIRQ(USART1_IRQn);					//使能USART中断通道
}


u8 USART2_value ;           

//初始化串口函数
void uart2_init(u32 bound)
{
    //UART 初始化设置
    UART2_Handler.Instance = USART2;					  //USART1
    UART2_Handler.Init.BaudRate = bound;				  //波特率
    UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
    UART2_Handler.Init.StopBits = UART_STOPBITS_1;	  //一个停止位
    UART2_Handler.Init.Parity = UART_PARITY_NONE;		  //无奇偶校验位
    UART2_Handler.Init.Mode = UART_MODE_TX_RX;		  //收发模式
	  UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_CTS;//硬件流控
		//UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;	//无硬件流控

	  UART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16; //16倍过采样
	  UART2_Handler.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		UART2_Handler.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if( HAL_UART_Init(&UART2_Handler) != HAL_OK) //底层硬件初始化
		{
				
		}	
		//该函数会开启接收中断：标志位 UART_IT_RXNE
		HAL_UART_Receive_IT(&UART2_Handler,  (uint8_t *)&USART2_value, 1);
		
		__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_RXNE); //开启接收中断
		//__HAL_UART_DISABLE_IT(&UART2_Handler, UART_IT_RXNE); //开启接收中断
		
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);				//抢占优先级3，子优先级3
		
		HAL_NVIC_EnableIRQ(USART2_IRQn);					//使能USART中断通道
		//HAL_NVIC_DisableIRQ(USART2_IRQn);					//使能USART中断通道
}




////初始化串口函数
//void uart3_init(u32 bound)
//{
//    //UART 初始化设置
//    UART3_Handler.Instance = USART3;					  //USART1
//    UART3_Handler.Init.BaudRate = bound;				  //波特率
//    UART3_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
//    UART3_Handler.Init.StopBits = UART_STOPBITS_1;	  //一个停止位
//    UART3_Handler.Init.Parity = UART_PARITY_NONE;		  //无奇偶校验位
//    UART3_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
//    UART3_Handler.Init.Mode = UART_MODE_TX_RX;		  //收发模式

//		if( HAL_UART_Init(&UART3_Handler) != HAL_OK) //底层硬件初始化
//		{
//				
//		}
//	  __HAL_UART_ENABLE_IT(&UART3_Handler, UART_IT_RXNE); //开启接收中断
//		__HAL_UART_DISABLE_IT(&UART3_Handler, UART_IT_RXNE); //开启接收中断

//			//HAL_NVIC_EnableIRQ(USART3_IRQn);					//使能USART3中断通道
//				HAL_NVIC_DisableIRQ(USART3_IRQn);					//使能USART3中断通道

//				HAL_NVIC_SetPriority(USART3_IRQn, 2, 3);				//抢占优先级3，子优先级3
//}


// 串口1中断服务程序
/**
 * @remark	下面代码我们直接把中断控制逻辑写在中断服务函数内部
 * 			说明：采用HAL库处理逻辑，效率不高。
 */
 
////串口1缓冲区
//u8 USART1_RX_BUF[USART_REC_LEN] ;           
//u8 USART1_TX_BUF[USART_REC_LEN] ; 
//u16 USART1_RX_LEN;
//u16 USART1_TIME;
//u16 USART1_RX_EVENT;

//void USART1_IRQHandler(void)
//{
//		if((__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_RXNE) != RESET)) //接收中断
//    {
//				USART1_TIME=0;
//				USART1_RX_BUF[USART1_RX_LEN] = (uint8_t)(UART1_Handler.Instance->RDR & (uint8_t)UART1_Handler.Mask);//接收这个字节

//				USART1_RX_LEN++;
//				if(USART1_RX_LEN == USART_REC_LEN)
//						USART1_RX_LEN=0;
//				
//				__HAL_UART_CLEAR_FLAG(&UART1_Handler,UART_FLAG_RXNE);
//    }
//}    


//串口2缓冲区
u8 USART2_RX_BUF[USART_REC_LEN] ;           
u8 USART2_TX_BUF[USART_REC_LEN] ; 
u16 USART2_RX_LEN;
u16 USART2_TIME;
u16 USART2_RX_EVENT;

//串口中断 精简后
void USART2_IRQHandler(void)
{
	  if((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_RXNE) != RESET)) //接收中断
    {
				USART2_TIME=0;
				USART2_RX_BUF[USART2_RX_LEN] = (uint8_t)(UART2_Handler.Instance->RDR & (uint8_t)UART2_Handler.Mask);//接收这个字节
				
				USART2_RX_LEN++;		
				if(USART2_RX_LEN == USART_REC_LEN)
						USART2_RX_LEN=0;
				
				__HAL_UART_CLEAR_FLAG(&UART2_Handler,UART_FLAG_RXNE);
    }
}


////串口中断 调用库函数
//void USART2_IRQHandler(void)
//{
//	  if((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_RXNE) != RESET)) //接收中断
//    {	
//				__HAL_UART_CLEAR_FLAG(&UART2_Handler,UART_FLAG_RXNE);

//				//HAL_UART_Receive_IT(&UART2_Handler,  (uint8_t *)&USART2_value, 1);
//        HAL_UART_Receive(&UART2_Handler,&USART2_value, 1, 1);//接收这个字节

//				USART2_RX_BUF[USART2_RX_LEN] = USART2_value;
//				USART2_RX_LEN++;
//			
//				if(USART2_RX_LEN == USART_REC_LEN)
//						USART2_RX_LEN=0;
//				
//				//Main_printf(".");
//    }
//}

////使用终端接收
//void USART2_IRQHandler(void)
//{
//	  //HAL_UART_IRQHandler(&UART2_Handler);
//		UART2_Handler.RxISR(&UART2_Handler);
////////////////////////////////////////////////////////////////////////////////	
//		HAL_UART_Receive_IT(&UART2_Handler,  (uint8_t *)&USART2_value, 1);
//	
//		
//		USART2_RX_BUF[USART2_RX_LEN] = USART2_value;
//		USART2_RX_LEN++;

//		if(USART2_RX_LEN == USART_REC_LEN)
//				USART2_RX_LEN=0;
//		
//		__HAL_UART_CLEAR_FLAG(&UART2_Handler,UART_FLAG_RXNE);			
//}



////串口中断 调用库函数
//void USART2_IRQHandler(void)
//{
//	  if((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_RXNE) != RESET)) //接收中断
//    {	
//				__HAL_UART_CLEAR_FLAG(&UART2_Handler,UART_FLAG_RXNE);

//				//HAL_UART_Receive_IT(&UART2_Handler,  (uint8_t *)&USART2_value, 1);
//        HAL_UART_Receive(&UART2_Handler,&USART2_value, 1, 1);//接收这个字节

//				USART2_RX_BUF[USART2_RX_LEN] = USART2_value;
//				USART2_RX_LEN++;
//			
//				if(USART2_RX_LEN == USART_REC_LEN)
//						USART2_RX_LEN=0;
//				
//				//Main_printf(".");
//    }
//}

////说明：采用HAL库处理逻辑，阻塞式效率不高。
//void USART2_IRQHandler(void)
//{
//    u8 Res;
//	  if((__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_RXNE) != RESET)) //接收中断
//    {
//        //HAL_UART_Receive(&UART2_Handler, &Res, 1, 100);//接收这个字节

//				USART2_RX_BUF[USART2_RX_LEN] = USART2_RX_BUF2[0];
//				USART2_RX_LEN++;
//			
//				if(USART2_RX_LEN == USART_REC_LEN)
//						USART2_RX_LEN=0;
//				__HAL_UART_CLEAR_FLAG(&UART2_Handler,UART_FLAG_RXNE);
//    }
//    HAL_UART_IRQHandler(&UART2_Handler);
//}



//void USART3_IRQHandler(void)
//{
//    u8 Res;
//		if((__HAL_UART_GET_FLAG(&UART3_Handler, UART_FLAG_RXNE) != RESET)) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//    {
//        HAL_UART_Receive(&UART3_Handler, &Res, 1, 100);
//				USART3_RX_BUF[USART3_RX_LEN] = Res;
//				USART3_RX_LEN++;
//				if(USART3_RX_LEN == USART3_REC_LEN)
//						USART3_RX_LEN=0;
//        __HAL_UART_CLEAR_FLAG(&UART3_Handler,UART_FLAG_RXNE);// Reset UART_FLAG_RXNE.
//    }
//    HAL_UART_IRQHandler(&UART3_Handler);
//}





////串口缓冲清零
//void USART1_Clear(void)
//{
//		memset(USART1_RX_BUF, 0, sizeof(USART1_RX_BUF));
//    USART1_RX_LEN = 0;
//}
//串口缓冲清零
void USART2_Clear(void)
{
		memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));
    USART2_RX_LEN = 0;
		USART2_TIME = 0;
}
////串口缓冲清零
//void USART3_Clear(void)
//{
//		memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
//    USART3_RX_LEN = 0;
//}




//USARTx_Send
//串口发送字符串
void USARTx_Send(UART_HandleTypeDef* huart, u8 *Data, u8 Len)  
{  
		if(huart->Instance==USART1)
		{
				HAL_UART_Transmit(&UART1_Handler, (uint8_t*)Data, Len, 100);	//发送数据
				while(__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_TC) != SET);		//等待发送结束  
		}
		else if(huart->Instance==USART2)
		{
				HAL_UART_Transmit(&UART2_Handler, (uint8_t*)Data, Len, 100);	//发送数据
				while(__HAL_UART_GET_FLAG(&UART2_Handler, UART_FLAG_TC) != SET);		//等待发送结束  
		}
//		else if(huart->Instance==USART3)
//		{
//				HAL_UART_Transmit(&UART3_Handler, (uint8_t*)Data, Len, 100);	//发送数据
//				while(__HAL_UART_GET_FLAG(&UART3_Handler, UART_FLAG_TC) != SET);		//等待发送结束  
//		}		
}  
//串口发送字符串
//void USARTx_Send(USART_TypeDef* USARTx, u8 *Data, u8 Len)  
//{  
//    u8 i = 0;  
//    for(i = 0; i < Len; i++)  
//    {  
//        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);     
//        USART_SendData(USARTx, Data[i]);                                                     
//    }  
//}




//判断指定字符串是否在数组中
//返回-1 没有指定的串
//0-125 字符串在数组中的位置
s16 Str_search(u8* buf,u8 len,u8* result)
{
			u8 *res;
			s16 num;
			
			res =  strstr((const u8 *)buf, (const u8 *)result); //返回result在buf中首次出现的地址
	
			//Main_printf("res %d\r\n",res);
	
			if(res != NULL)	//判断是否有预期的结果
			{
					num=res-buf;
				
					//Main_printf("NUM %d\r\n",num);

					if(num<len)
					{
						//Main_printf("得到预期结果,数组中第 %d 个开始\r\n",res-buf);
						return res-buf;
					}
			}
			//Main_printf("没有预期结果\r\n");
			return -1;
}


//判断指定字符是否在数组中
//返回-1 没有指定的串
//0-125 字符串在数组中的位置
s16 Char_search(u8* buf,u8 len,u8 result)
{
			u8 *res;
			u8 num;
			
			res =  strchr((const u8 *)buf, result); 	
			if(res != NULL)	//判断是否有预期的结果
			{
					num=res-buf;
					if(num<len)
					{
						//Main_printf("得到预期结果,数组中第 %d 个开始\r\n",res-buf);
						return res-buf;
					}
			}
			//Main_printf("没有预期结果\r\n");
			return -1;
}







//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR & 0X40) == 0); //循环发送,直到发送完毕
  USART1->TDR = (u8) ch;
  return ch;
}
#endif 



