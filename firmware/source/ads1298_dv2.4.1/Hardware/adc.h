#ifndef __ADC_H
#define __ADC_H
#include "sys.h"



#define	BAT_100		2606  //2606*0.806=2100mV 分压后电压为4200mV
#define	BAT_0		1861	//1861*0.806=1500mV	分压后电压为3000mV
#define	BAT_LEVEL		746 //总等级746

#define ADCBUF_SIZE 2


__align(4) typedef struct
{
		u16 ADCConvertedValue[ADCBUF_SIZE];//用来存放ADC转换结果，也是DMA的目标地址
	
		u8 ADC_buf[20];
		
		u16 BAT_VOL;
		s8 BAT_Percent;
		u8 LowPower_flag;
		u8 power_detective;
		
		//QueueInfo *ADXL345_Queue;//TF数据缓冲区指针
}_ADC_info;
extern _ADC_info ADC_info;//芯片结构


extern ADC_HandleTypeDef ADC1_Handler;//ADC句柄



void MY_ADC_Init(void); 				//ADC通道初始化
u16  Get_Adc(u32 ch); 		        //获得某个通道值
u16 Get_Adc_Average(u32 ch, u8 times); //得到某个通道给定次数采样的平均值


#endif






