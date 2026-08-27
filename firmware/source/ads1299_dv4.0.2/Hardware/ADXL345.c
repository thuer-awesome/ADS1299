#include "ADXL345.h"
//#include "spi.h"
#include "delay.h"       //延时
#include "usart.h"       //串口  		
#include "iic.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2021/08/05
//版本：V1.0
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//上电引脚初始化
void ADXL345_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	      
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void ADXL345_IIC_Init(void)
{
		IIC_Init();
		I2C_SCAN(0XFF); //检测IIC总线的设备
}
void ADXL345_IIC_Start(u8 time)
{
		IIC_Start(time);
}
void ADXL345_IIC_Stop(u8 time)
{
		IIC_Stop(time);
}
void ADXL345_IIC_SendByte(u8 time,u8 dat)
{
		IIC_Send_Byte(time,dat);
}
u8 ADXL345_IIC_RecvByte(u8 time)
{
		return IIC_Read_Byte(time);
}

//产生ACK应答
void ADXL345_IIC_ACK(u8 time)
{
		IIC_Ack(time);
}
//不产生ACK应答		    
void ADXL345_IIC_NACK(u8 time)
{
		IIC_NAck(time);
}

u8 ADXL345_IIC_Wait_Ack(u8 time)
{
		return IIC_Wait_Ack(time);
}



//******单字节写入*******************************************
void Single_Write_ADXL345(u8 REG_Address,u8 REG_data)
{
		u8 res=0;
	
    ADXL345_IIC_Start(ADXL345_IIC_TIME);                  //起始信号
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress);   //发送设备地址+写信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=1;
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,REG_Address);    //内部寄存器地址，请参考中文pdf22页 
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=2;
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,REG_data);       //内部寄存器数据，请参考中文pdf22页 
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=3;
	
    ADXL345_IIC_Stop(ADXL345_IIC_TIME);                   //发送停止信号
		
		if(res) 	
			ADXL345_DEBUG("ADXL345写失败 无应答 %d \r\n",res);	

}

//********单字节读取*****************************************
u8 Single_Read_ADXL345(u8 REG_Address)
{  	
		u8 REG_data,res=0;
	
    ADXL345_IIC_Start(ADXL345_IIC_TIME);  												//起始信号

    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress);           //发送设备地址+写信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=1;

    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,REG_Address);            //发送存储单元地址，从0开始	
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=2;

    ADXL345_IIC_Start(ADXL345_IIC_TIME);                          //起始信号	
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress+1);         //发送设备地址+读信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=3;
	
    REG_data=ADXL345_IIC_RecvByte(ADXL345_IIC_TIME);             //读出寄存器数据 NOACK
		ADXL345_IIC_NACK(ADXL345_IIC_TIME);
	
		ADXL345_IIC_Stop(ADXL345_IIC_TIME);                           //停止信号
		
		if(res) 	
			ADXL345_DEBUG("ADXL345读失败 无应答 %d \r\n",res);	
    return REG_data; 
}


//*********************************************************
//连续读出ADXL345内部加速度数据，地址范围0x32~0x37
//X0 X1 Y0 Y1 Z0 Z1  2进制补码，0为低位 1为高位
//*********************************************************
//buf 接收数据缓存区     
void Multiple_read_ADXL345(u8 *buf)
{   
		u8 i,res=0;
	
    ADXL345_IIC_Start(ADXL345_IIC_TIME);                          //起始信号
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress);           //发送设备地址+写信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=1;
		
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,0x32);                   //发送存储单元地址，从0x32开始	
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=2;
		
    ADXL345_IIC_Start(ADXL345_IIC_TIME);                          //起始信号
		
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress+1);         //发送设备地址+读信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=3;
		
		for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {     
        if (i == 5)
        {
						buf[i] = ADXL345_IIC_RecvByte(ADXL345_IIC_TIME);      //最后一个数据需要回NOACK
						ADXL345_IIC_NACK(ADXL345_IIC_TIME);					
        }
        else
        {
						buf[i] = ADXL345_IIC_RecvByte(ADXL345_IIC_TIME);       //回应ACK
						ADXL345_IIC_ACK(ADXL345_IIC_TIME);
       }
   }
   ADXL345_IIC_Stop(ADXL345_IIC_TIME);                          //停止信号
	 
	 if(res) 	
			ADXL345_DEBUG("ADXL345连续读失败 无应答 %d \r\n",res);	
}

//*********************************************************
//连续读出ADXL345内部加速度数据，地址范围0x32~0x37
//X0 X1 Y0 Y1 Z0 Z1  2进制补码，0为低位 1为高位
//*********************************************************
//buf 接收数据缓存区     
void Multiple_read_ADXL3452(s16 *buf2)
{   
		u8 i,res;
		u8 buf[6];

    ADXL345_IIC_Start(ADXL345_IIC_TIME);                          //起始信号
	
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress);           //发送设备地址+写信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=1;
		
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,0x32);                   //发送存储单元地址，从0x32开始	
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=2;
		
    ADXL345_IIC_Start(ADXL345_IIC_TIME);                          //起始信号
		
    ADXL345_IIC_SendByte(ADXL345_IIC_TIME,SlaveAddress+1);         //发送设备地址+读信号
		if(ADXL345_IIC_Wait_Ack(ADXL345_IIC_TIME)==1)
			res=3;
		
		for (i=0; i<6; i++)                      											//连续读取6个地址数据，存储中BUF
    {     
        if (i == 5)
        {
						buf[i] = ADXL345_IIC_RecvByte(ADXL345_IIC_TIME);      //最后一个数据需要回NOACK
						ADXL345_IIC_NACK(ADXL345_IIC_TIME);
        }
        else
        {
						buf[i] = ADXL345_IIC_RecvByte(ADXL345_IIC_TIME);       //回应ACK
						ADXL345_IIC_ACK(ADXL345_IIC_TIME);
       }
   }
		
	 buf2[0] = buf[1]<<8 | buf[0];
	 buf2[1] = buf[3]<<8 | buf[2];
	 buf2[2] = buf[5]<<8 | buf[4];

   ADXL345_IIC_Stop(ADXL345_IIC_TIME);                          //停止信号
}




//*****************************************************************
//初始化ADXL345，根据需要请参考pdf进行修改************************
u8 ADXL345_Init(void)
{
		u8 res;

		//ADXL345_GPIO_Init();
		//ADXL345_IIC_Init();

		res = Single_Read_ADXL345( DEVICE_ID);
		if( res == I_M_DEVID )
		{
				//Single_Write_ADXL345(DATA_FORMAT,0x01);   //测量范围,正负4g，13位模式
				Single_Write_ADXL345(DATA_FORMAT,0x0B);   //测量范围,正负16g，13位模式
				Single_Write_ADXL345(POWER_CTL,0x08);   //选择电源模式   参考pdf24页
			
				//Single_Write_ADXL345(BW_RATE,0x07);   //速率设定为12.5 参考pdf13页 
				//Single_Write_ADXL345(BW_RATE,0x09);   //速率设定为50 参考pdf13页
				//Single_Write_ADXL345(BW_RATE,0x0A);   //速率设定为100 参考pdf13页
				Single_Write_ADXL345(BW_RATE,0x0B);   //速率设定为200 参考pdf13页
				//Single_Write_ADXL345(BW_RATE,0x0C);   //速率设定为400 参考pdf13页

				Single_Write_ADXL345(INT_ENABLE,0x80);   //使能 DATA_READY 中断  低电平有效
				Single_Write_ADXL345(INT_MAP,0x00);   //发送各自中断到INT1引脚

				Single_Write_ADXL345(OFSX,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
				Single_Write_ADXL345(OFSY,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
				Single_Write_ADXL345(OFSZ,0x00);   //Z 偏移量 根据测试传感器的状态写入pdf29页
				return 0;	//成功
		}
		else
		{
				return 1;
		}
}

















