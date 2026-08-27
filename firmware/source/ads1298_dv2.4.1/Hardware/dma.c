#include "dma.h"
#include "usart.h"
#include "adc.h"

#include "my_protocol.h"
#include "ADS129x.h"
/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/08/31
//适配 HAL库
//版本：V1.5
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



u8 UART1_DMA_Finish = DMA_CANNLE_FREE;
u8 UART2_DMA_Finish = DMA_CANNLE_FREE;
u8 UART3_DMA_Finish = DMA_CANNLE_FREE;
u8 UART4_DMA_Finish = DMA_CANNLE_FREE;

u8 SPI1_DMA_Finish = DMA_CANNLE_FREE;
u8 SPI2_DMA_Finish = DMA_CANNLE_FREE;
u8 SPI3_DMA_Finish = DMA_CANNLE_FREE;

u8 ADC_DMA_Finish = DMA_CANNLE_FREE;


DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  UART2TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  ADC1TxDMA_Handler;      //DMA句柄



/**
 * @brief	DMAx的各通道配置,这里的传输形式是固定的,这点要根据不同的情况来修改
 *			从存储器->外设模式/8位数据宽度/存储器增量模式
 *
 * @param   DMA_Streamx		DMA通道选择,DMA1_Channel0~7/DMA2_Channel0~7
 * @param   chx				DMA请求选择,@ref DMA_REQUEST_0~DMA_REQUEST_7
 *
 * @return  void
 */

void MYDMA_UART1_Config(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();//DMA1时钟使能
    
    __HAL_LINKDMA(&UART1_Handler,hdmatx,UART1TxDMA_Handler);  //将DMA与USART1联系起来(发送DMA)

    //Tx DMA配置
    UART1TxDMA_Handler.Instance = DMA1_Channel4;                          //通道选择
    UART1TxDMA_Handler.Init.Request = DMA_REQUEST_2;                              //请求选择
    UART1TxDMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;           //存储器到外设
    UART1TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;               //外设非增量模式
    UART1TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                   //存储器增量模式
    UART1TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  //外设数据长度:8位
    UART1TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     //存储器数据长度:8位
    UART1TxDMA_Handler.Init.Mode = DMA_NORMAL;                          //外设普通模式
    UART1TxDMA_Handler.Init.Priority = DMA_PRIORITY_HIGH;             	//中等优先级

    HAL_DMA_DeInit(&UART1TxDMA_Handler);
    HAL_DMA_Init(&UART1TxDMA_Handler);	
}

void MYDMA_UART2_Config(void)
{

    __HAL_RCC_DMA1_CLK_ENABLE();//DMA1时钟使能
    
    __HAL_LINKDMA(&UART2_Handler,hdmatx,UART2TxDMA_Handler);    //将DMA与USART1联系起来(发送DMA)

    //Tx DMA配置
    UART2TxDMA_Handler.Instance = DMA1_Channel7;                        //通道选择
    UART2TxDMA_Handler.Init.Request = DMA_REQUEST_2;                              //请求选择
    UART2TxDMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;           //存储器到外设
    UART2TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;               //外设非增量模式
    UART2TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                   //存储器增量模式
    UART2TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  //外设数据长度:8位
    UART2TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     //存储器数据长度:8位
    UART2TxDMA_Handler.Init.Mode = DMA_NORMAL;                          //外设普通模式
    UART2TxDMA_Handler.Init.Priority = DMA_PRIORITY_HIGH;             	//中等优先级

    HAL_DMA_DeInit(&UART2TxDMA_Handler);
    HAL_DMA_Init(&UART2TxDMA_Handler);	
}


void MYDMA_ADC1_Config(void) 
{
    __HAL_RCC_DMA2_CLK_ENABLE();//DMA2时钟使能

		__HAL_LINKDMA(&ADC1_Handler,DMA_Handle,ADC1TxDMA_Handler);  //将DMA与ADC联系起来(发送DMA)

    //DMA配置
    ADC1TxDMA_Handler.Instance = DMA2_Channel3;                          //通道选择
    ADC1TxDMA_Handler.Init.Request = DMA_REQUEST_0;                              //请求选择
    ADC1TxDMA_Handler.Init.Direction = DMA_PERIPH_TO_MEMORY;           //存储器到外设
    ADC1TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;               //外设非增量模式
    ADC1TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                   //存储器增量模式
    ADC1TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;  //外设数据长度:16位
    ADC1TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;     //存储器数据长度:16位
    ADC1TxDMA_Handler.Init.Mode = DMA_CIRCULAR;                        //外设循环模式
    ADC1TxDMA_Handler.Init.Priority = DMA_PRIORITY_HIGH;             	 //中等优先级

    HAL_DMA_DeInit(&ADC1TxDMA_Handler);
    HAL_DMA_Init(&ADC1TxDMA_Handler);			
}


void ADC_DMA_NVIC_Config(u8 cmd)
{			
		HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 2); //DMA2_Channel3_IRQn

		if(cmd==DMA_NVIC_ENABLE)
		{
				HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
				__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_ENABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TC); //完成中断
		}
		else
		{
				HAL_NVIC_DisableIRQ(DMA2_Channel3_IRQn);
				__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TC); //完成中断
		}
}


void UART1_DMA_TX_NVIC_Config(u8 cmd)
{
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 2, 2);
		if(cmd==DMA_NVIC_ENABLE)
		{
				HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
				__HAL_DMA_DISABLE_IT(&UART1TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_ENABLE_IT(&UART1TxDMA_Handler, DMA_IT_TC); //完成中断
		}
		else
		{
				HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
				__HAL_DMA_DISABLE_IT(&UART1TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_DISABLE_IT(&UART1TxDMA_Handler, DMA_IT_TC); //完成中断
		}
}


void UART2_DMA_TX_NVIC_Config(u8 cmd)
{
		HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 2, 2);
		if(cmd==DMA_NVIC_ENABLE)
		{
				HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
				__HAL_DMA_DISABLE_IT(&UART2TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_ENABLE_IT(&UART2TxDMA_Handler, DMA_IT_TC); //完成中断
		}
		else
		{
				HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);
				__HAL_DMA_DISABLE_IT(&UART2TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
				__HAL_DMA_DISABLE_IT(&UART2TxDMA_Handler, DMA_IT_TC); //完成中断
		}
}






//串口1 TX 中断
void DMA1_Channel4_IRQHandler(void)
{
		if(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler,DMA_FLAG_TC4))
		{
				UART1_DMA_Finish = DMA_CANNLE_FREE;                         //置采样完成标志位 
/////////////////////////////////////////////////////////////////////////////////////////
			
				HAL_UART_DMAStop(&UART1_Handler);      				//传输完成以后关闭串口DMA
				__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4); //清除DMA传输完成标志
		}	
		HAL_DMA_IRQHandler(&UART1TxDMA_Handler);
}
//串口2 TX 中断
void DMA1_Channel7_IRQHandler(void)
{	
		if(__HAL_DMA_GET_FLAG(&UART2TxDMA_Handler,DMA_FLAG_TC7))
		{
				UART2_DMA_Finish = DMA_CANNLE_FREE;                         //置采样完成标志位
/////////////////////////////////////////////////////////////////////////////////////////
				HAL_UART_DMAStop(&UART2_Handler);      				//传输完成以后关闭串口DMA
				__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
		}
		HAL_DMA_IRQHandler(&UART2TxDMA_Handler);		
}




//u16 rese;
//u8 SYS_cache[50];

////DMA ADC 中断
//void DMA2_Channel3_IRQHandler(void)
//{
//		u8 cache[2];

//		if(__HAL_DMA_GET_FLAG(&ADC1TxDMA_Handler,DMA_FLAG_TC3))
//		{
//				ADC_DMA_Finish = DMA_CANNLE_FREE;  //置采样完成标志位 
///////////////////////////////////////////////////////////////////////////////////////////		
////				rese++;
////				ADC_info.ADC_buf[0]=rese>>8; //MIC
////				ADC_info.ADC_buf[1]=rese; 			
//				ADC_info.ADC_buf[0]=ADC_info.ADCConvertedValue[1]>>8; //MIC
//				ADC_info.ADC_buf[1]=ADC_info.ADCConvertedValue[1]; 
///////////////////////////////////////////////////////////////////////////////////////////
//				memcpy(SYS_cache,ADS129x_info.Ads129x_Databuf,6);			//1292 6字节
//				memcpy(SYS_cache+6,ADC_info.ADC_buf,2);								//MIC 2字节
//			
//				//注意加速度原始数据低位在前
//				SYS_cache[8]=ADXL345_info.ADXL345_Speed_buf[1];
//				SYS_cache[9]=ADXL345_info.ADXL345_Speed_buf[0];
//				SYS_cache[10]=ADXL345_info.ADXL345_Speed_buf[3];
//				SYS_cache[11]=ADXL345_info.ADXL345_Speed_buf[2];
//				SYS_cache[12]=ADXL345_info.ADXL345_Speed_buf[5];
//				SYS_cache[13]=ADXL345_info.ADXL345_Speed_buf[4];
//				//memcpy(SYS_cache+8,ADXL345_info.ADXL345_Speed_buf,6);	//加速度 6字节 加速度原始数据低位在前，这个是错误的
///////////////////////////////////////////////////////////////////////////////////////////			
//				//搬运数据至队列
//				if(queue_data_push(my_Portocol_Info->Queue,SYS_cache,14,my_Portocol_Info-> Queue_push_time)==QUEUE_RETURN_OK) 
//				{
//						queue_add_rear(my_Portocol_Info->Queue) ;//存数据，队尾自增	
//				}			
///////////////////////////////////////////////////////////////////////////////////////////				
//				__HAL_DMA_CLEAR_FLAG(&ADC1TxDMA_Handler, DMA_FLAG_TC3); //清除DMA传输完成标志
//		}
//		HAL_DMA_IRQHandler(&ADC1TxDMA_Handler);	
//}


///*这里统一按200HZ存储，主程序里做降采样处理*/
////DMA ADC 中断
//void DMA2_Channel3_IRQHandler(void)
//{
//		u8 cache[10];

//		if(ADC1TxDMA_Handler.Instance->CCR & DMA_IT_TC)	//传输完成中断
//		{
//				Main_printf(".");

////				ADC_DMA_Finish = DMA_CANNLE_FREE;  //置采样完成标志位 
///////////////////////////////////////////////////////////////////////////////////////////
////				cache[0]=ADCConvertedValue[1]>>8; //MIC
////				cache[1]=ADCConvertedValue[1]; 
////			
////				cache[2]=ADCConvertedValue[2]>>8;	//x
////				cache[3]=ADCConvertedValue[2];	
////			
////				cache[4]=ADCConvertedValue[2]>>8;	//y
////				cache[5]=ADCConvertedValue[2];	
////			
////				cache[6]=ADCConvertedValue[3]>>8;	//z
////				cache[7]=ADCConvertedValue[3];	

////				cache[8]=ADCConvertedValue[4]>>8;	//心电
////				cache[9]=ADCConvertedValue[4];	
////			
////				//搬运数据至队列 USART_QUEUE_REAL_LENGTH 一次采样有效数据长度  USART_QUEUE_LENGTH 对协议的队列长度
////				if(queue_data_push(UART_Info->UART_Queue,cache,UART_Info-> UART_data_real_length,UART_Info-> UART_Queue_length/UART_Info-> UART_data_real_length)==1)	
////				{
////						Main_printf("1");
////				}
/////////////////////////////////////////////////////////////////////////////////////////
//				//清标志位
//				ADC1TxDMA_Handler.DmaBaseAddress->IFCR = (DMA_ISR_TCIF1 << (ADC1TxDMA_Handler.ChannelIndex & 0x1cU));//清除传输完成中断标志
//		}
/////////////////////////////////////////////////////////////////////////////////////////	
////		if(ADC1TxDMA_Handler.Instance->CCR & DMA_IT_TE)
////		{
////				//Main_printf("C ");	
////				//ADC1TxDMA_Handler.DmaBaseAddress->IFCR = DMA_ISR_GIF1 << (ADC1TxDMA_Handler.ChannelIndex & 0x1cU);
////				//__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TE); 
////		}
////		if(ADC1TxDMA_Handler.Instance->CCR & DMA_IT_HT)
////		{
////				//Main_printf("a ");	
////				//ADC1TxDMA_Handler.DmaBaseAddress->IFCR = DMA_ISR_HTIF1 << (ADC1TxDMA_Handler.ChannelIndex & 0x1cU);
////				//__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_HT); 
////		}
////		//__HAL_DMA_CLEAR_FLAG(&ADC1TxDMA_Handler,DMA_IT_TC); //没有作用		
//}


////开启一次DMA传输
////huart:串口句柄
////pData：传输的数据指针
////Size:传输的数据量
//void MYDMA_USART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
//{
//    HAL_DMA_Start(huart->hdmatx, (u32)pData, (uint32_t)&huart->Instance->DR, Size);//开启DMA传输
//    
//    huart->Instance->CR3 |= USART_CR3_DMAT;//使能串口DMA发送
//	
//	
//	  //HAL_UART_Transmit_DMA(&UART1_Handler, (u8*)SendBuff, SEND_BUF_SIZE); //启动传输

//}	  


