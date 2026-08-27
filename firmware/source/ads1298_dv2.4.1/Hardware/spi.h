#ifndef __SPI_H
#define __SPI_H
#include "sys.h"



extern SPI_HandleTypeDef SPI1_Handler;  //SPI句柄
extern SPI_HandleTypeDef SPI2_Handler;  //SPI句柄
extern SPI_HandleTypeDef SPI3_Handler;  //SPI句柄
	



void SPI1_Init(void);			 //初始化SPI口
void SPI2_Init(void);			 //初始化SPI口	
void SPI3_Init(void);			 //初始化SPI口


u8 SPI1_ReadWriteByte(u8 TxData);
u8 SPI2_ReadWriteByte(u8 TxData);
u8 SPI3_ReadWriteByte(u8 TxData);


u8 SPI_Fast_ReadWriteByte(SPI_HandleTypeDef *hspi, uint8_t TxData);


u8 SPI_ReadWriteByte(SPI_HandleTypeDef* hspi,u8 TxData,u16 time);
void SPI_SetSpeed(SPI_HandleTypeDef* hspi,u8 SPI_BaudRatePrescaler);

void SPI1_SetSpeed(u8 SpeedSet);
void SPI3_SetSpeed(u8 SpeedSet);

#endif


