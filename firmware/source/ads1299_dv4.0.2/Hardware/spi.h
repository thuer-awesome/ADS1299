#ifndef __SPI_H
#define __SPI_H
#include "sys.h"



u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
u8 SPI3_ReadWriteByte(u8 TxData);//SPI总线读写一个字节

 			
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   			
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
void SPI3_SetSpeed(u8 SpeedSet); //设置SPI速度   

void SPI1_Init(void);			 //初始化SPI口
void SPI2_Init(void);			 //初始化SPI口	
void SPI3_Init(void);			 //初始化SPI口


#endif


