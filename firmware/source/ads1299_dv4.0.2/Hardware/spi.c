#include "spi.h"
#include "delay.h" 


//129x  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 					 
//TF			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 					 
					  
void SPI1_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;   
	SPI_InitTypeDef SPI_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	
	//SCK  PA5  //MISO  PA6	//MOSI  PA7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;                                                                                                   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
  GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化串口输入
		
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 					  //SCLK空闲为低，上升沿锁存数据
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1,&SPI_InitStructure); 	
	SPI_Cmd(SPI1,ENABLE); //使能SPI外设
}


//SPI 速度设置函数
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_32  32分频  (SPI 2.25M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
//SPI1 速度设置函数
void SPI1_SetSpeed(u8 SpeedSet)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SpeedSet));
		SPI1->CR1&=0XFFC7;
		SPI1->CR1|=SpeedSet;	//设置SPI1速度 
		SPI_Cmd(SPI1,ENABLE); 
} 

//SPI1 读写一个字节
//TxData 要写入的字节
//返回值 读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
				u8 retry=0;
				while((SPI1->SR&1<<1)==0)//检查指定的标志位设置与否；发送缓存空标志位  
        {
             retry++;
             if(retry>200)return 0;
        }                          
        SPI1->DR=TxData;          //通过外设SPIx发送一个byte
        
				retry=0;				
        while((SPI1->SR&1<<0)==0) //检查指定的标志位设置与否；接收缓存非空标志位
        {
             retry++;
             if(retry>200)return 0;
        }                                                             
        return SPI1->DR;          //返回通过外设SPIx最近接收的数据			
}




//void SPI2_Init(void) //129x
//{
// 	GPIO_InitTypeDef GPIO_InitStructure;
//  SPI_InitTypeDef  SPI_InitStructure;

//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
//	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
//	
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //双线双向全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 				//设置为主SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 		//SPI发送8bit数据
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 					  //SCLK空闲为低，上升沿锁存数据
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 					//第二个时钟沿数据被采样
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//片选由软件控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // 时钟分频36/2=18M
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 		//高位在前
//	SPI_InitStructure.SPI_CRCPolynomial = 7; 							//CRC计算值的多项式
// 	SPI_Init(SPI2,&SPI_InitStructure); 	

//	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
//}



////SPI 速度设置函数
////SpeedSet:
////SPI_BaudRatePrescaler_2   2分频   
////SPI_BaudRatePrescaler_8   8分频   
////SPI_BaudRatePrescaler_16  16分频  
////SPI_BaudRatePrescaler_256 256分频 
//void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
//	SPI2->CR1&=0XFFC7;
//	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
//	SPI_Cmd(SPI2,ENABLE); 
//} 

////SPIx 读写一个字节
////TxData:要写入的字节
////返回值:读取到的字节
//u8 SPI2_ReadWriteByte(u8 TxData)
//{		
//				u8 retry=0;
//				while((SPI2->SR&1<<1)==0)//检查指定的标志位设置与否；发送缓存空标志位  
//        {
//             retry++;
//             if(retry>200)return 0;
//        }                          
//        SPI2->DR=TxData;          //通过外设SPIx发送一个byte
//        
//				retry=0;				
//        while((SPI2->SR&1<<0)==0) //检查指定的标志位设置与否；接收缓存非空标志位
//        {
//             retry++;
//             if(retry>200)return 0;
//        }                                                             
//        return SPI2->DR;          //返回通过外设SPIx最近接收的数据					    
//}




