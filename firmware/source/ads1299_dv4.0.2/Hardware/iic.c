#include "iic.h"
#include "delay.h"
#include "usart.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2021/04/27
//版本：V1.2
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//设置SDA接口的IO口方向
void SDA_OUT(void)  //设置为输出	
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	          //初始化SDA
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  //推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure); 	
}


void SDA_IN(void)  //设置为输入
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	       //初始化SDA
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //浮空输入
	  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	IIC_SCL=1; 
	IIC_SDA=1;
}



//功能说明: I2C总线位延迟，最快400KHz
void i2c_Delay(u8 time)	
{
	uint8_t i;

		//CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		//循环次数为10时，SCL频率 = 205KHz 
		//循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	//循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us
		//循环次数为1时，oled可用
    //IAR工程编译效率高，不能设置为7
	for (i = 0; i < time; i++);
}

//数据有效性
//IIC总线进行数据传送时，时钟信号为高电平期间，数据线上的数据必须保持稳定；
//只有在时钟线上的信号为低电平期间，数据线上的高电平或低电平状态才允许变化。 
//即：数据在时钟线SCL的上升沿到来之前就需准备好。并在在下降沿到来之前必须稳定。


//产生IIC起始信号
//SCL保持高电平期间，SDA上的电平被拉低，定义为I2C总线的启动信号
void IIC_Start(u8 time)	
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;		//都为高，总线空闲状态
	i2c_Delay(time);
 	IIC_SDA=0;//SCL为高，SDA拉低，总线启动信号
	i2c_Delay(time);
// 	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
//	i2c_Delay(time);	
}	 


//产生IIC停止信号
//SCL保持高电平期间，SDA返回高电平，表示数据停止
void IIC_Stop(u8 time)	
{
	SDA_OUT();//sda线输出
	
	IIC_SCL=0;
	IIC_SDA=0; 	
	i2c_Delay(time);
	IIC_SCL=1; 
	i2c_Delay(time);
	IIC_SDA=1;//发送I2C总线结束信号
	//i2c_Delay(time);
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(u8 time)	
{
	u8 ucErrTime=0;
	
	SDA_IN();      //SDA设置为输入 
	
	IIC_SCL=1;	
	i2c_Delay(time);
 	
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(time);
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 


void IIC_Ack(u8 time)	
{
	SDA_OUT();
		
	IIC_SCL=0;	
	IIC_SDA=0;
	i2c_Delay(time);
	IIC_SCL=1;	
	i2c_Delay(time);
 	IIC_SCL=0;
	//i2c_Delay(time);
}


void IIC_NAck(u8 time)	
{
	SDA_OUT();
	
	IIC_SCL=0;
	IIC_SDA=1;
	i2c_Delay(time);
 	IIC_SCL=1;
	i2c_Delay(time);
 	IIC_SCL=0;
	//i2c_Delay(time);
}
  
void IIC_Send_Byte(u8 time,u8 txd)
{
    u8 t;
		SDA_OUT();
	
    IIC_SCL=0;//拉低时钟开始数据传输
		i2c_Delay(time);

    for(t=0;t<8;t++)
    {
        IIC_SDA=(txd&0x80)>>7;
				txd<<=1;
			
 				IIC_SCL=1;
				i2c_Delay(time);
 				IIC_SCL=0;
				i2c_Delay(time);				
    }
} 

//读1个字节 注意数据读取不同的芯片可能有区别
u8 IIC_Read_Byte(u8 time)
{
		u8 i,receive=0;
		
		SDA_IN();//SDA设置为输入
		for(i=0;i<8;i++ )
		{
				IIC_SCL=1;		
				i2c_Delay(time);
							
				receive<<=1;
				if(READ_SDA)
						receive++;  
				
				IIC_SCL=0;		
				i2c_Delay(time);
		}					  
		return receive;
}
//读1个字节 注意数据读取不同的芯片可能有区别
//u8 IIC_Read_Byte(u8 time)
//{
//		u8 i,receive=0;
//		
//		SDA_IN();//SDA设置为输入
//		for(i=0;i<8;i++ )
//		{
//				IIC_SCL=0;
//				i2c_Delay(time);
//				IIC_SCL=1;		
//			
//				receive<<=1;
//				if(READ_SDA)
//						receive++;  
//				i2c_Delay(time);
//			
//		}					  
//		return receive;
//}


//功能说明: 检测指定地址的I2C总线设备，CPU读取设备应答来判断该设备是否存在
//形    参：TIME IIC速度  _Address：设备的I2C总线地址
//返 回 值: 返回值 0 表示当前地址有设备，返回1表示未探测到
u8 IIC_CheckDevice(u8 time,u8 _Address)
{
	uint8_t ucAck;

	IIC_Init();					// 配置GPIO 
	IIC_Start(time);		// 发送启动信号 

	// 发送设备地址+读写控制bit（0 = wr， 1 = rd) bit7 先传 
	IIC_Send_Byte(time,_Address | I2C_WR);
	ucAck = IIC_Wait_Ack(time);	// 检测设备的ACK应答 
	IIC_Stop(time);			//发送停止信号 
	return ucAck;
}


void I2C_SCAN(u8 addr)
{
		u8 res=1,getflag=1;
		
	  while(addr>0)
    {
				res=IIC_CheckDevice(200,addr);
				if(!res)
				{
					getflag=0;
					IIC_DEBUG("get answer at %x\r\n",addr);
				}
				addr--;
    }
		if(getflag)
				IIC_DEBUG("there is no device\r\n");
}









