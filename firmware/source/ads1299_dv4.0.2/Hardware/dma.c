#include "dma.h"
#include "delay.h"       //延时



u8 UART1_DMA_Finish=1;
u8 UART2_DMA_Finish=1;
u8 UART3_DMA_Finish=1;
u8 UART4_DMA_Finish=1;
u8 SPI1_DMA_TX_Finish=1;
u8 SPI2_DMA_TX_Finish=1;
u8 SPI3_DMA_TX_Finish=1;
u8 ADC_DMA_Finish = 1;

 
 
//DMA的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:内存地址
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
{
		DMA_InitTypeDef DMA_InitStructure;	

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//使能DMA传输

		DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

		DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
		DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
		//DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;  //内存地址寄存器递增
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常模式
		DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
		DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
} 
//void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		RCC->AHBENR |= 1<<0;				//开启DMA1时钟		
//		delay_ms(10);
//	
//		//DMA_CHx -> CNDTR = len;	//数据传输量最大65535
//		DMA_CHx -> CPAR = (u32)cpar;    //设定外设寄存器地址 发送地址
//		DMA_CHx -> CMAR = (u32)cmar;    //设定数据存储器地址 接收地址
//		
//		//CCR	寄存器 0-14为设置位		
//		DMA_CHx -> CCR  = 0;  							//复位，清零寄存器
//		
//		//被屏蔽的中断，在中断函数中不要判断其标志位，不然会触发，不清楚原因
//		DMA_CHx -> CCR  	|= 0<<1;         //传输完成中断				1允许 0禁止
//		DMA_CHx -> CCR  	|= 0<<2;         //半传输中断 				1允许 0禁止
//		DMA_CHx -> CCR		|= 0<<3;         //允许传输错误中断		1允许 0禁止	  读写一个保留的地址区域,将会产生DMA传输错误 
//	
//		//设定数据传输方向
//		DMA_CHx -> CCR  |= 1<<4;           //设定数据传输方向   0：从外设传输至内存 1：从内存传输至外设
//		DMA_CHx -> CCR  |= 0<<5;           //0：不执行循环操作 1：执行循环操作           
//		//设定外设和存储器地址增量
//		DMA_CHx -> CCR  |= 0<<6;           //0：外设地址不自增 1：外设地址自增
//		DMA_CHx -> CCR  |= 1<<7;           //0：内存地址不自增 1：内存地址自增    
//		//设定外设数据宽度
//		DMA_CHx -> CCR  |= 0<<8;           //外设数据宽度,由[9:8]两位控制
//		DMA_CHx -> CCR  |= 0<<9;           //00：8位 01：16位 10：32位 11：保留  
//		//设定存储数据宽度
//		DMA_CHx -> CCR  |= 0<<10;          //存储器数据宽度,由[11:10]两位控制
//		DMA_CHx -> CCR  |= 0<<11;          //00：8位 01：16位 10：32位 11：保留  			
//		//设定为中等优先级
//		DMA_CHx -> CCR  |= 0<<12;          //通道优先级,由[13:12]两位控制
//		DMA_CHx -> CCR  |= 0<<13;          //00：低 01：中 10：高 11：最高 
//		//存储方式
//		DMA_CHx -> CCR  |= 0<<14;          //0：非内存到内存； 1：内存传输到内存。     
//} 

////开启一次DMA传输
//void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
//{
//		DMA_Cmd(DMA_CHx, DISABLE );  //关闭当前的DMA传输（不管有没有传输完成）
//		DMA_SetCurrDataCounter(DMA_CHx,len);//要发送的数据长度
//		DMA_Cmd(DMA_CHx, ENABLE);  //开始DMA传输  	  
//}
//开启一次DMA传输
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
{
		DMA_CHx -> CCR &= ~(1<<0);	//关闭当前的DMA传输（不管有没有传输完成）
    DMA_CHx -> CNDTR = len;			//设置数据传输量
    DMA_CHx -> CCR |= 1<<0;			//开始DMA传输  
}
 

//初始化DMA中断 cmd 1 使能
void UART1_DMA_TX_NVIC_Config(u8 cmd)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		if(cmd==1)
		{
				DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE); //传输完成中断
			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
		else
		{
				DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE); //传输完成中断
			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
}
void UART2_DMA_TX_NVIC_Config(u8 cmd)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		if(cmd==1)
		{
				DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE); //传输完成中断
			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
		else
		{
				DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,DISABLE); //传输完成中断
			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
}
//void UART3_DMA_TX_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//	
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE); //传输完成中断
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,DISABLE); //传输完成中断
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
//		}
//}
//void UART4_DMA_TX_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA2_Channel4_5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//	
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE); //传输完成中断
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
//		}
//		else
//		{
//				DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,DISABLE); //传输完成中断
//			//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
//				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
//		}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//初始化ADCDMA
void ADC_DMA_Config(u16 *buffer ,u16 len)
{	
		DMA_InitTypeDef DMA_InitStructure;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能时钟
	
    DMA_DeInit(DMA1_Channel1);    //将通道一寄存器设为默认值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//该参数用以定义DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;//该参数用以定义DMA内存基地址(转换结果保存的地址)
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//该参数规定了外设是作为数据传输的目的地还是来源，此处是外设作为来源
    DMA_InitStructure.DMA_BufferSize = len;//定义指定DMA通道的DMA缓存的大小,单位为数据单位。
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设定外设地址寄存器递增与否,此处设为不变 Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//用来设定内存地址寄存器递增与否,此处设为递增，Enable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道拥有高优先级 分别4个等级 低、中、高、非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道的内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA的通道
		
		DMA_Cmd(DMA1_Channel1, ENABLE);
		//DMA_Cmd(DMA1_Channel1, DISABLE);
}
//开启DMA传输
void ADC_DMA_Enable(void)
{
		ADC_DMACmd(ADC1, ENABLE);//开启DMA ADC采集	

		//DMA_Cmd(DMA1_Channel1, DISABLE);
		//DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道1 
}
void ADC_DMA_NVIC_Config(u8 cmd)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		DMA_ClearITPendingBit(DMA_IT_TC); //清除更新中断请求位
		if(cmd==1)
		{
				DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //传输完成中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
		else
		{
				DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,DISABLE); //传输完成中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_HT,ENABLE);  //半发送中断
				//DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);  //发送错误中断
		}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//u8 SPI1_DMA_Config(void)
//{
//		DMA_InitTypeDef DMA_InitStructure;

//		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
//		delay_ms(10);

//		//DMA通道2用于SPI->RAM的数据传输 SPI1 RX
//		DMA_DeInit(DMA1_Channel2);	
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI1->DR;
//		//DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
//		DMA_InitStructure.DMA_BufferSize=512;
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh; 
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel2,&DMA_InitStructure); //对DMA通道2进行初始化

//		//DMA通道3用于RAM->SPI的数据传输		SPI TX
//		DMA_DeInit(DMA1_Channel3);
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI1->DR;
//		//DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
//		DMA_InitStructure.DMA_BufferSize=512;
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh; 
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//		DMA_Init(DMA1_Channel3,&DMA_InitStructure); //对DMA通道3进行初始化

//		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE); //使能SPI的DMA发送请求
//		SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE); //使能SPI的DMA接收请求

//		return 0;
//}
u8 SPI2_DMA_Config(void)
{
	 DMA_InitTypeDef DMA_InitStructure;

	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	 //DMA通道5用于RAM->SPI的数据传输 SPI TX
	 DMA_DeInit(DMA1_Channel5);
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI2->DR;
	 //DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
	 DMA_InitStructure.DMA_BufferSize=512;
	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	 DMA_Init(DMA1_Channel5,&DMA_InitStructure); //对DMA通道5进行初始化
		
	 //DMA通道4用于SPI->RAM的数据传输
	 DMA_DeInit(DMA1_Channel4);	
	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI2->DR;
	 //DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	 DMA_InitStructure.DMA_BufferSize=512;
	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	 DMA_Init(DMA1_Channel4,&DMA_InitStructure); //对DMA通道4进行初始化
	 	 
	 SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE); //使能SPI的DMA接收请求
	 SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE); //使能SPI的DMA发送请求

	 return 0;
}
//u8 SPI3_DMA_Config(void)
//{
//	 DMA_InitTypeDef DMA_InitStructure;
//	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
//	
//	 //DMA通道5用于RAM->SPI的数据传输 SPI TX
//	 DMA_DeInit(DMA2_Channel2);
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI3->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST; 
//	 DMA_InitStructure.DMA_BufferSize=512;
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA2_Channel2,&DMA_InitStructure);
//		
//	 //DMA通道4用于SPI->RAM的数据传输 RX
//	 DMA_DeInit(DMA2_Channel1);	
//	 DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&SPI3->DR;
//	 //DMA1_Init.DMA_MemoryBaseAddr=...; //启动传输前装入实际RAM地址
//	 DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
//	 DMA_InitStructure.DMA_BufferSize=512;
//	 DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
//	 DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
//	 DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
//	 DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
//	 DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
//	 DMA_InitStructure.DMA_Priority=DMA_Priority_High; 
//	 DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
//	 DMA_Init(DMA2_Channel1,&DMA_InitStructure); //对DMA通道4进行初始化
//		 
//	 SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE); //使能SPI的DMA发送请求
//	 SPI_I2S_DMACmd(SPI3,SPI_I2S_DMAReq_Rx,ENABLE); //使能SPI的DMA接收请求

//	 return 0;
//}


////启动DMA传输 启用中断
//u8 SPI1_DMA_TX(u8 *buffer,u16 len)
//{
//		DMA1_Channel3->CNDTR=len; //设置要传输的数据长度
//		DMA1_Channel3->CMAR=(uint32_t)buffer; //设置RAM缓冲区地址
//		
//		DMA_Cmd(DMA1_Channel3,ENABLE); //启动DMA传输 RAM->SPI
//	
//		//while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)); //等待DMA通道3传输完成
//		//DMA_ClearFlag(DMA1_FLAG_TC3); //清除通道3传输完成状态标记
//		//DMA_Cmd(DMA1_Channel3,DISABLE); //使DMA通道3停止工作
//	  return 0;
//}
////启动DMA传输 
//u8 SPI1_DMA_RX(u8 *buffer,u16 len)
//{
//    u8 temp=0xff;
//    	
//		DMA1_Channel2->CNDTR=len; //设置传输的数据长度
//		DMA1_Channel2->CMAR=(uint32_t)buffer; //设置内存缓冲区地址
//		
//		/*SPI作为主机进行数据接收时必须要主动产生时钟，因此此处必须有DMA通道3的配合*/
//		DMA1_Channel3->CNDTR=len; 
//		DMA1_Channel3->CMAR=(uint32_t)&temp; //temp=0xff
//		DMA1_Channel3->CCR&=~DMA_MemoryInc_Enable; //内存地址非自增
//		
//		DMA_Cmd(DMA1_Channel2,ENABLE); //首先启动DMA通道2
//		DMA_Cmd(DMA1_Channel3,ENABLE); //再启动DMA通道3
//	
//		while(!DMA_GetFlagStatus(DMA1_FLAG_TC2)); //等待DMA通道2接收数据完成
//		DMA_ClearFlag(DMA1_FLAG_TC2); 
//		DMA_ClearFlag(DMA1_FLAG_TC3); //清除DMA通道2与3的传输完成标志
//		DMA_Cmd(DMA1_Channel2,DISABLE);
//		DMA_Cmd(DMA1_Channel3,DISABLE); //使DMA通道2与3停止工作
//		DMA1_Channel3->CCR|=DMA_MemoryInc_Enable; //将DMA通道3恢复为内存地址自增方式
//	
//	  return 0;
//}

////启动DMA传输
//u8 SPI2_DMA_TX(u8 *buffer,u16 len)
//{
//		DMA1_Channel5->CNDTR=len; //设置要传输的数据长度
//		DMA1_Channel5->CMAR=(uint32_t)buffer; //设置RAM缓冲区地址
//		
//		DMA_Cmd(DMA1_Channel5,ENABLE); //启动DMA传输 RAM->SPI
//		while(!DMA_GetFlagStatus(DMA1_FLAG_TC5)); //等待DMA通道5传输完成
//		DMA_ClearFlag(DMA1_FLAG_TC5); //清除通道5传输完成状态标记
//		DMA_Cmd(DMA1_Channel5,DISABLE); //使DMA通道5停止工作
//	  return 0;
//}
//启动DMA传输
u8 SPI2_DMA_RX(u8 *buffer,u16 len)
{
    u8 temp=0xff;
    	
		DMA1_Channel4->CNDTR=len; //设置传输的数据长度
		DMA1_Channel4->CMAR=(uint32_t)buffer; //设置内存缓冲区地址
		
		/*SPI作为主机进行数据接收时必须要主动产生时钟，因此此处必须有DMA通道5的配合*/
		DMA1_Channel5->CNDTR=len; 
		DMA1_Channel5->CMAR=(uint32_t)&temp; //temp=0xff
		DMA1_Channel5->CCR&=~DMA_MemoryInc_Enable; //内存地址非自增
		
		DMA_Cmd(DMA1_Channel4,ENABLE); //首先启动DMA通道4 RX
		DMA_Cmd(DMA1_Channel5,ENABLE); //再启动DMA通道5 TX
		while(!DMA_GetFlagStatus(DMA1_FLAG_TC4)); //等待DMA通道4接收数据完成
		DMA_ClearFlag(DMA1_FLAG_TC4); 
		DMA_ClearFlag(DMA1_FLAG_TC5); //清除DMA通道4与5的传输完成标志
		DMA_Cmd(DMA1_Channel4,DISABLE);
		DMA_Cmd(DMA1_Channel5,DISABLE); //使DMA通道4与5停止工作
		
		DMA1_Channel5->CCR|=DMA_MemoryInc_Enable; //将DMA通道5恢复为内存地址自增方式
	  return 0;
}
//启动DMA传输
////buf:数据缓存区
//u8 SPI3_DMA_TX(u8 *buffer,u16 len)
//{
//		DMA2_Channel2->CNDTR=len; //设置要传输的数据长度
//		DMA2_Channel2->CMAR=(uint32_t)buffer; //设置RAM缓冲区地址
//		
//		DMA_Cmd(DMA2_Channel2,ENABLE); //启动DMA传输 RAM->SPI
//		while(!DMA_GetFlagStatus(DMA2_FLAG_TC2)); //等待DMA通道1传输完成
//		DMA_ClearFlag(DMA2_FLAG_TC2); //清除通道1传输完成状态标记
//		DMA_Cmd(DMA2_Channel2,DISABLE); //使DMA通道1停止工作
//	  return 0;
//}




////初始化DMA中断 cmd 1 使能
//void SPI1_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel3_IRQn; //TX 中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel3_IRQn,DMA_IT_TC,ENABLE); //传输完成中断
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel3_IRQn,DMA_IT_TC,DISABLE); //传输完成中断
//		}
//}
////初始化DMA中断 cmd 1 使能
//void SPI2_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel5_IRQn; //TX 中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE); //传输完成中断
//		}
//		else
//		{
//				DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE); //传输完成中断
//		}
//}

////初始化DMA中断 cmd 1 使能
//void SPI3_DMA_NVIC_Config(u8 cmd)
//{
//		NVIC_InitTypeDef NVIC_InitStructure;
//	
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    NVIC_InitStructure.NVIC_IRQChannel=DMA2_Channel2_IRQn; //TX 中断
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//		if(cmd==1)
//		{
//				DMA_ITConfig(DMA2_Channel2,DMA_IT_TC,ENABLE); //传输完成中断
//		}
//		else
//		{
//				DMA_ITConfig(DMA2_Channel2,DMA_IT_TC,DISABLE); //传输完成中断
//		}
//}





/////////////////////////////////////////////////////////////////////////////////////////////////////
//DMA 中断程序
//串口1发送完成
void DMA1_Channel4_IRQHandler()  
{  
    if(DMA_GetITStatus(DMA1_FLAG_TC4))                   //判断DMA传输完成中断  
    {  
        UART1_DMA_Finish = 1;                          //置采样完成标志位  
				DMA_ClearITPendingBit(DMA1_FLAG_TC4);            //清除DMA中断标志位
				DMA_Cmd(DMA1_Channel4, DISABLE );  //关闭当前的DMA传输
    } 
}
//串口2发送完成
void DMA1_Channel7_IRQHandler()  
{  
    if(DMA_GetITStatus(DMA1_FLAG_TC7))                   //判断DMA传输完成中断  
    {  
        UART2_DMA_Finish = 1;                          	//置采样完成标志位  
				DMA_ClearITPendingBit(DMA1_FLAG_TC7);            //清除DMA中断标志位
				DMA_Cmd(DMA1_Channel7, DISABLE );  //关闭当前的DMA传输

    } 
}
////串口3发送完成
//void DMA1_Channel2_IRQHandler()  
//{     
//		if(DMA_GetITStatus(DMA1_FLAG_TC2))                   //判断DMA传输完成中断  
//		{
//        UART3_DMA_Finish = 1;                          //置采样完成标志位  
//				DMA_ClearITPendingBit(DMA1_FLAG_TC2);            //清除DMA中断标志位
//				DMA_Cmd(DMA1_Channel2, DISABLE );  //关闭当前的DMA传输
//		}	
//}
////串口4发送完成
//void DMA2_Channel4_5_IRQHandler()  
//{
//		if(DMA_GetITStatus(DMA2_FLAG_TC5))                   //判断DMA传输完成中断  
//		{
//        UART4_DMA_Finish = 1;                          //置采样完成标志位  
//				DMA_ClearITPendingBit(DMA2_FLAG_TC5);            //清除DMA中断标志位
//				DMA_Cmd(DMA2_Channel5, DISABLE );  //关闭当前的DMA传输
//		}
//}
////SP11发送完成
//void DMA1_Channel3_IRQHandler()  
//{  
//    if(DMA_GetITStatus(DMA1_FLAG_TC3))                   //判断DMA传输完成中断  
//    {  
//				SPI1_DMA_TX_Finish = 1;                          //置采样完成标志位  
//				DMA_Cmd(DMA1_Channel3, DISABLE );  						//关闭当前的DMA传输
//       	DMA_ClearITPendingBit(DMA1_FLAG_TC3);            //清除DMA中断标志位
//    } 		
//}
////SPI2发送完成
//void DMA1_Channel5_IRQHandler()  
//{  
//    if(DMA_GetITStatus(DMA1_FLAG_TC5))                   //判断DMA传输完成中断  
//    {  
//				SPI2_DMA_TX_Finish = 1;                          	//置采样完成标志位  
//				DMA_Cmd(DMA1_Channel5, DISABLE );  //关闭当前的DMA传输
//				DMA_ClearITPendingBit(DMA1_FLAG_TC5);            //清除DMA中断标志位
//    } 
//}
////SPI3发送完成
//void DMA2_Channel2_IRQHandler()  
//{     
//		if(DMA_GetITStatus(DMA2_FLAG_TC2))                   //判断DMA传输完成中断  
//		{
//				SPI3_DMA_TX_Finish = 1;                          //置采样完成标志位  
//				DMA_Cmd(DMA2_Channel2, DISABLE );  						//关闭当前的DMA传输
//				DMA_ClearITPendingBit(DMA2_FLAG_TC2);            //清除DMA中断标志位
//		}	
//}

////ADC采集完成完成
//void DMA1_Channel1_IRQHandler()  
//{  
//    if(DMA_GetITStatus(DMA1_FLAG_TC1))                   //判断DMA传输完成中断  
//    {  
//				ADC_DMA_Finish = 1;                          //置采样完成标志位  
//				//DMA_Cmd(DMA1_Channel1, DISABLE);  					//关DMA
//				//不关DMA，ADC会一直采集
//       	DMA_ClearITPendingBit(DMA1_FLAG_TC1);            //清除DMA中断标志位
//    } 		
//}