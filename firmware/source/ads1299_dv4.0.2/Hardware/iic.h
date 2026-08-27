#ifndef __IIC_H
#define __IIC_H
#include "sys.h"

/*
I2C 总线在传送数据过程中共有三种类型信号， 它们分别是：开始信号、结束信号和应答信号。
开始信号： SCL 为高电平时， SDA 由高电平向低电平跳变，开始传送数据。
结束信号： SCL 为高电平时， SDA 由低电平向高电平跳变，结束传送数据。
应答信号：接收数据的 IC 在接收到 8bit 数据后，向发送数据的 IC 发出特定的低电平脉冲，
表示已收到数据。CPU 向受控单元发出一个信号后，等待受控单元发出一个应答信号， CPU 接
收到应答信号后，根据实际情况作出是否继续传递信号的判断。若未收到应答信号，由判断为
受控单元出现故障。
这些信号中，起始信号是必需的，结束信号和应答信号，都可以不要。 
*/



#define IIC_UART_DEBUG  1 //串口DEBUG

#if IIC_UART_DEBUG == 1 
		#define IIC_DEBUG(...)    printf(__VA_ARGS__)
#else
		#define IIC_DEBUG(...)
#endif


#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */



//IO操作函数	 
#define IIC_SCL    PAout(11) //SCL
#define IIC_SDA    PAout(12) //SDA	 
#define READ_SDA   PAin(12)  //输入SDA 


void SDA_OUT(void);  //设置为输出
void SDA_IN(void);  //设置为输入

void IIC_Init(void);
void i2c_Delay(u8 time);
	
void IIC_Start(u8 time);
void IIC_Stop(u8 time);
void IIC_Send_Byte(u8 time,u8 _ucByte);
u8 IIC_Read_Byte(u8 time);
u8 IIC_Wait_Ack(u8 time);
void IIC_Ack(u8 time);
void IIC_NAck(u8 time);

u8 IIC_CheckDevice(u8 time,u8 _Address);
void I2C_SCAN(u8 addr);

#endif
