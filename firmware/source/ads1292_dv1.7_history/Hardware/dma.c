#include "dma.h"
#include "usart.h"
#include "adc.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/06/01
//适配 HAL库
//版本：V1.3
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


u8 UART1_DMA_Finish;
u8 UART2_DMA_Finish;
u8 UART3_DMA_Finish;
u8 UART4_DMA_Finish;

u8 SPI1_DMA_Finish;
u8 SPI2_DMA_Finish;
u8 SPI3_DMA_Finish;

u8 ADC_DMA_Finish;



DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  UART2TxDMA_Handler;      //DMA句柄
DMA_HandleTypeDef  ADC1TxDMA_Handler;      //DMA句柄



/**
 * @brief	DMAx的各通道配置,这里的传输形式是固定的,这点要根据不同的情况来修改
 *			从存储器->外设模式/8位数据宽度/存储器增量模式
 * @param   DMA_Streamx		DMA通道选择,DMA1_Channel0~7/DMA2_Channel0~7
 * @param   chx				DMA请求选择,@ref DMA_REQUEST_0~DMA_REQUEST_7
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



//void ADC_DMA_NVIC_Config(u8 cmd)
//{			
//		HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 2, 2); //DMA2_Channel3_IRQn
//		if(cmd==1)
//		{
//				HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
//				__HAL_DMA_ENABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TC); //完成中断
//		}
//		else
//		{
//				HAL_NVIC_DisableIRQ(DMA2_Channel3_IRQn);
//				__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TC); //完成中断
//		}
//		__HAL_DMA_DISABLE_IT(&ADC1TxDMA_Handler, DMA_IT_TE|DMA_IT_HT); //异常中断和半完成中断
//}


void UART1_DMA_TX_NVIC_Config(u8 cmd)
{
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 2, 2);
		if(cmd==1)
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
		if(cmd==1)
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
				__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4); //清除DMA传输完成标志
				HAL_UART_DMAStop(&UART1_Handler);      				//传输完成以后关闭串口DMA
				UART1_DMA_Finish = 0;                         //置采样完成标志位  
		}	
		HAL_DMA_IRQHandler(&UART1TxDMA_Handler);
}
//串口2 TX 中断
void DMA1_Channel7_IRQHandler(void)
{	
		if(__HAL_DMA_GET_FLAG(&UART2TxDMA_Handler,DMA_FLAG_TC7))
		{
				HAL_UART_DMAStop(&UART2_Handler);      				//传输完成以后关闭串口DMA
				UART2_DMA_Finish = 0;                         //置采样完成标志位 
				__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
		}
		HAL_DMA_IRQHandler(&UART2TxDMA_Handler);		
}



////DMA ADC 中断
//void DMA2_Channel3_IRQHandler(void)
//{
//		if(ADC1TxDMA_Handler.Instance->CCR & DMA_IT_TC)	//传输完成中断
//		{
/////////////////////////////////////////////////////////////////////////////////////////
//				//Main_printf(".");
/////////////////////////////////////////////////////////////////////////////////////////
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


