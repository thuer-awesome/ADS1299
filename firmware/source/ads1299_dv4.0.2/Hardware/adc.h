#ifndef __ADC_H
#define __ADC_H	 
#include "sys.h"



#define	BAT_100			2606  //2606*0.806=2100mV 分压后电压为4200mV
#define	BAT_0				1861	//1861*0.806=1500mV	分压后电压为3000mV
#define	BAT_LEVEL		746 	//总等级746



#define ADCBUF_SIZE 1
extern volatile u16 ADCConvertedValue[ADCBUF_SIZE];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数

typedef union 
{
     u16 ADC_buf;
		 u8 adc_buf[2];
}_ADC_UNION;
extern _ADC_UNION  ADC_UNION;



extern s8 BAT_Percent;
extern u8 Bat_Flag;


void adc1_init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);




#endif