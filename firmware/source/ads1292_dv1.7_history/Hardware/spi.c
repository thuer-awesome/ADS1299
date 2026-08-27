#include "spi.h"
#include "delay.h" 

SPI_HandleTypeDef SPI1_Handler;  //SPI句柄
SPI_HandleTypeDef SPI2_Handler;  //SPI句柄
SPI_HandleTypeDef SPI3_Handler;  //SPI句柄

//	SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;	//TF卡
//	SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_LOW; 	//129X


//8M
void SPI1_Init(void)
{
		SPI1_Handler.Instance=SPI1;                         //SPI
		SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
		SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
		SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
		SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;    //串行同步时钟的空闲状态为x电平
		SPI1_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
		SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值
		SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
		SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
		SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
		if (HAL_SPI_Init(&SPI1_Handler) != HAL_OK)					//初始化SPI
		{
				Error_Handler(__FILE__, __LINE__);
		} 
		__HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI
}


//void SPI2_Init(void)
//{
//		SPI2_Handler.Instance=SPI2;                         //SPI
//		SPI2_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
//		SPI2_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
//		SPI2_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
//		SPI2_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;    //串行同步时钟的空闲状态为高电平
//		SPI2_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
//		SPI2_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
//		SPI2_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值
//		SPI2_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
//		SPI2_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
//		SPI2_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
//		SPI2_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
//		if (HAL_SPI_Init(&SPI2_Handler) != HAL_OK)					//初始化SPI
//		{
//				Error_Handler(__FILE__, __LINE__);
//		} 
//		__HAL_SPI_ENABLE(&SPI2_Handler);                    //使能SPI
//}

void SPI3_Init(void)
{
		//F1系列有一个函数可以禁止JTAG使能SWD，F4没有,不需要配置可以直接使用
	
		SPI3_Handler.Instance=SPI3;                         //SPI
		SPI3_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
		SPI3_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
		SPI3_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
		SPI3_Handler.Init.CLKPolarity=SPI_POLARITY_LOW;    //串行同步时钟的空闲状态为高电平
		SPI3_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
		SPI3_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		SPI3_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_2;//定义波特率预分频的值:波特率预分频值
		SPI3_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		SPI3_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
		SPI3_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
		SPI3_Handler.Init.CRCPolynomial=7;                  //禁止CRC后，此位无效，CRC值计算的多项式
		if (HAL_SPI_Init(&SPI3_Handler) != HAL_OK)					//初始化SPI
		{
				Error_Handler(__FILE__, __LINE__);
		} 
		__HAL_SPI_ENABLE(&SPI3_Handler);                    //使能SPI
}


//SPI底层驱动，时钟使能，引脚配置
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
	  if(hspi->Instance==SPI1)
		{
				__HAL_RCC_GPIOA_CLK_ENABLE();   
				__HAL_RCC_SPI1_CLK_ENABLE(); 

				GPIO_InitStruct.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
				GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
				GPIO_InitStruct.Pull=GPIO_PULLUP;                  //上拉
				GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //80Mhz           
				GPIO_InitStruct.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
				HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
//				//复用			
//				__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
//				__HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
//				//PB3,4,5
//				GPIO_InitStruct.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
//				GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
//				GPIO_InitStruct.Pull=GPIO_PULLUP;                  //上拉
//				GPIO_InitStruct.Speed=GPIO_SPEED_FAST;             //快速            
//				GPIO_InitStruct.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
//				HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);	
		}
//		else if(hspi->Instance==SPI2)
//		{
//				__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
//				__HAL_RCC_SPI2_CLK_ENABLE();        //使能SPI2时钟
//			
//				GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//				GPIO_InitStruct.Pull = GPIO_PULLUP;
//				GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//				GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//		}
		else if(hspi->Instance==SPI3)
		{
				__HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIO时钟
				__HAL_RCC_SPI3_CLK_ENABLE();        //使能SPI时钟
			
				GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull = GPIO_PULLUP;
				GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
				GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			
//				//复用						
//				__HAL_RCC_GPIOC_CLK_ENABLE();       //使能GPIO时钟
//				__HAL_RCC_SPI3_CLK_ENABLE();        //使能SPI时钟
//			
//				GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
//				GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//				GPIO_InitStruct.Pull = GPIO_PULLUP;
//				GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//				GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
//				HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		}  
}

//SPI复位
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
		if(hspi->Instance==SPI1)
		{
				__HAL_RCC_SPI1_CLK_DISABLE();
				HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
		}
//		else if(hspi->Instance==SPI2)
//		{
//				__HAL_RCC_SPI2_CLK_DISABLE();
//				HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
//		}
		else if(hspi->Instance==SPI3)
		{
				__HAL_RCC_SPI3_CLK_DISABLE();
				HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
		}
}




//SPI 读写一个字节  快速调用		
//精简后  27组时钟耗时320us	   每个字节时间间隔 12us
//精简后  9组时钟耗时120us	   每个字节时间间隔 12us
u8 SPI_Fast_ReadWriteByte(SPI_HandleTypeDef *hspi, uint8_t TxData)
{

	u8 retry=0;			

	while((hspi->Instance->SR&1<<1)==0)		//等待发送区空 
	//while((__HAL_SPI_GET_FLAG(&SPI1_Handler, SPI_FLAG_TXE)) ==0)	//等待发送区空 		
	{
		retry++;
		if(retry>200)return 0;
	}	
	*(__IO uint8_t *)&hspi->Instance->DR = TxData;//发送一个byte  
	
	retry=0;	
	while((hspi->Instance->SR&1<<0)==0)		//等待发送区空 
	//while((__HAL_SPI_GET_FLAG(&SPI1_Handler, SPI_FLAG_RXNE)) ==0)	//等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	
	return *(__IO uint8_t *)&hspi->Instance->DR;
}



//SPI 读写一个字节  使用HAL库函数 
//27字节耗时 1600us   每个字节时间间隔 60us
//句柄 SPI1_Handler SPI2_Handler
//发送接收数据量 1
//超时时间
u8 SPI_ReadWriteByte(SPI_HandleTypeDef* hspi,u8 TxData,u16 time)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(hspi,&TxData,&Rxdata,1, time);  
		return Rxdata;          		    //返回收到的数据		
}





//设置速率
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI1_Handler);            //关闭SPI
    SPI1_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI1_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
		HAL_SPI_Init(&SPI1_Handler);
	
	  __HAL_SPI_ENABLE(&SPI1_Handler);             //使能SPI
}

//设置速率
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
		assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI3_Handler);            //关闭SPI
    SPI3_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI3_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
		HAL_SPI_Init(&SPI3_Handler);
	
	  __HAL_SPI_ENABLE(&SPI3_Handler);             //使能SPI
}

void SPI_SetSpeed(SPI_HandleTypeDef* hspi,u8 SPI_BaudRatePrescaler)
{
	
}

////SPI1速度设置函数
////SpeedSet:0~7
////SPI速度=fAPB2/2^(SpeedSet+1)
////fAPB2时钟一般为84Mhz
//void SPI1_SetSpeed(u8 SpeedSet)
//{
//	SpeedSet&=0X07;			//限制范围
//	SPI1->CR1&=0XFFC7; 
//	SPI1->CR1|=SpeedSet<<3;	//设置SPI1速度  
//	SPI1->CR1|=1<<6; 		//SPI设备使能	  
//} 
////SPI1速度设置函数
////SpeedSet:0~7
////SPI速度=fAPB2/2^(SpeedSet+1)
////fAPB2时钟一般为84Mhz
//void SPI3_SetSpeed(u8 SpeedSet)
//{
//	SpeedSet&=0X07;			//限制范围
//	SPI3->CR1&=0XFFC7; 
//	SPI3->CR1|=SpeedSet<<3;	//设置SPI1速度  
//	SPI3->CR1|=1<<6; 		//SPI设备使能	  
//} 
