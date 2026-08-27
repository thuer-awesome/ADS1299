#ifndef __DMA_H
#define __DMA_H
#include "sys.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/06/1
//适配 HAL库
//版本：V1.3
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


extern u8 UART1_DMA_Finish;
extern u8 UART2_DMA_Finish;
extern u8 UART3_DMA_Finish;
extern u8 UART4_DMA_Finish;

extern u8 SPI1_DMA_Finish;
extern u8 SPI2_DMA_Finish;
extern u8 SPI3_DMA_Finish;

extern u8 ADC_DMA_Finish;


extern DMA_HandleTypeDef  UART1TxDMA_Handler;      //DMA句柄
extern DMA_HandleTypeDef  UART2TxDMA_Handler;      //DMA句柄
extern DMA_HandleTypeDef  ADC1TxDMA_Handler;      //DMA句柄



//串口DMA
void MYDMA_UART1_Config(void);
void MYDMA_UART2_Config(void);
//ADC
void MYDMA_ADC1_Config(void);



void SPI1_DMA_NVIC_Config(u8 cmd);
void SPI2_DMA_NVIC_Config(u8 cmd);
void SPI3_DMA_NVIC_Config(u8 cmd);


void UART1_DMA_TX_NVIC_Config(u8 cmd);
void UART2_DMA_TX_NVIC_Config(u8 cmd);
void UART3_DMA_TX_NVIC_Config(u8 cmd);

void ADC_DMA_NVIC_Config(u8 cmd);


#endif
