#ifndef __ADXL345_H
#define __ADXL345_H	 
#include "sys.h"


#define ADXL345_IIC_TIME 8

#define ADXL345_INT1	PAin(8)  //INT1
//#define ADXL345_INT2	PAin(0)  //INT2


#define ADXL345_UART_DEBUG  0 //串口DEBUG
#if ADXL345_UART_DEBUG == 1 
		#define ADXL345_DEBUG(...)    Main_printf(__VA_ARGS__)
#else
		#define ADXL345_DEBUG(...)
#endif


/******************ADXL345寄存器命令定义**********************/

#define I_M_DEVID      0xE5 //器件ID=0XE5

#define DEVICE_ID           0X00   //获取器件ID,0XE5
#define THRESH_TAP          0X1D   //敲击阀值
#define OFSX                0X1E	 //x轴调整偏移值
#define OFSY                0X1F
#define OFSZ                0X20
#define DUR                 0X21
#define Latent              0X22
#define Window              0X23
#define THRESH_ACK          0X24
#define THRESH_INACT        0X25
#define TIME_INACT          0X26
#define ACT_INACT_CTL       0X27
#define THRESH_FF           0X28
#define TIME_FF             0X29
#define TAP_AXES            0X2A
#define ACT_TAP_STATUS      0X2B
#define BW_RATE             0X2C
#define POWER_CTL           0X2D
 
#define INT_ENABLE          0X2E
#define INT_MAP             0X2F
#define INT_SOURCE          0X30
#define DATA_FORMAT        0X31
#define DATA_X0            0X32
#define DATA_X1            0X33
#define DATA_Y0            0X34
#define DATA_Y1            0X35
#define DATA_Z0            0X36
#define DATA_Z1            0X37
#define FIFO_CTL            0X38
#define FIFO_STATUS         0X39
 
 

#define	SlaveAddress   0xA6	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改


void Multiple_read_ADXL345(u8 *buf);
void Multiple_read_ADXL3452(s16 *buf2);


u8 ADXL345_Init(void);
u8 Single_Read_ADXL345(u8 REG_Address);
void Single_Write_ADXL345(u8 REG_Address,u8 REG_data);

void ADXL345_ReadXYZ(u8 *BUF);
u8 ADXL345_read_byte(u16 add);

void ADXL345_GPIO_Init(void);
void ADXL345_IIC_Init(void);


#endif
