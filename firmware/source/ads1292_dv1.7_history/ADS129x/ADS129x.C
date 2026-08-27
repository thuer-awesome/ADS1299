#include "ADS129x.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"
#include "Timer.h"
#include "myQueue.h"
#include "dma.h"
#include "string.h"

#include "RF4404B4.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/06/06
//版本：V1.4
//说明：HAL库
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////

_ADS129x_info ADS129x_info;

#if ECG1CANNLE_RESP ==1
		//单导呼吸心电
		#if ADS129X_DEVICE == ADS1292
			u8 ADS129x_REG_BUF[ADS129X_REGNUM]={  //ads129X寄存器数组	
			0x73, //ID 				ads1292R
			0x02, //CONFIG1 	连续转换模式 后一个数代表采样率  0 125 1 250 2 500 3 1K 4 2K 5 4K
			0xA3,	//CONFIG2 	1010 0011导联脱落比较器禁用，基准电压缓冲区启用，2.42V参考，振荡器输出禁用，开测试信号为1HZ方波
			0x10,	//LOFF			导联脱落比较器阈值设置/////////////////////////////////////////////////////////////////////////////////	
		#if ADS129X_USE_SQUARE_TESTDATA == 1   //方波测试			
			0x05,	//CH1SET	 第一个数 增益 			6-12 5-8 4-4 3-3 2-2 1-1 0-6
			0x05,	//CH2SET	 第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
		#elif ADS129X_USE_NOISE_TESTDATA == 1 //噪声测试
			0x01,//CH1SET		
			0x01,//CH2SET	 	
		#else  //正常测试
			0x30,//CH1SET		
			0x00,//CH2SET	 	
			#endif	
		/////////////////////////////////////////////////////////////////////////////////	
			0x2C,	//RLD_SENS	开RLD缓冲电源，通道2 N P连入右腿驱动输出
			0x00,	//LOFF_SENS	禁用导联脱落检测
			0x00,	//LOFF_STAT	显示导联脱落状态
			0xEA,	//1110 1010 RESP1			启用呼吸调制和解调，信号相位112.5°
			0x03,	//RESP2			关通道偏移校正，呼频率32KHZ，内部RLDREF信号
			0x0C,	//GPIO			GPIO输出1
			};
		#endif
#elif ECG3CANNLE ==1
		//三导心电	
		#if ADS129X_DEVICE == ADS1292
			u8 ADS129x_REG_BUF[ADS129X_REGNUM]={  //ads129X寄存器数组	
			0x73, //ID 				ads1292R
			0x02, //CONFIG1 	连续转换模式 后一个数代表采样率  0 125 1 250 2 500 3 1K 4 2K 5 4K
			0xA3,	//CONFIG2 	1010 0011导联脱落比较器禁用，基准电压缓冲区启用，2.42V参考，振荡器输出禁用，开测试信号为1HZ方波
			0x10,	//LOFF			导联脱落比较器阈值设置
		/////////////////////////////////////////////////////////////////////////////////	
		#if ADS129X_USE_SQUARE_TESTDATA == 1   //方波测试			
			0x05,	//CH1SET	 第一个数 增益 			6-12 5-8 4-4 3-3 2-2 1-1 0-6
			0x05,	//CH2SET	 第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
		#elif ADS129X_USE_NOISE_TESTDATA == 1 //噪声测试
			0x01,//CH1SET		
			0x01,//CH2SET	 	
		#else  //正常测试
			0x00,//CH1SET		
			0x00,//CH2SET	 	
			#endif	
		/////////////////////////////////////////////////////////////////////////////////	
			0x23,	//RLD_SENS	开RLD缓冲电源，通道1 N P连入右腿驱动输出
			//0x20,	//RLD_SENS	开RLD缓冲电源，无通道连入右腿驱动输出

			0x00,	//LOFF_SENS	禁用导联脱落检测
			0x00,	//LOFF_STAT	显示导联脱落状态
			0x02,	//RESP1		默认02
			0x03,	//RESP2		关通道偏移校正，呼频率32KHZ，内部RLDREF信号
			0x0C,	//GPIO		GPIO输出1
			};
		#endif
#endif
	
	
	
///////////////////////////////////////////////////////////////////////////////////////////	
//ADS129x的IO口初始化
void ADS129x_IO_Init(void) 
{
		GPIO_InitTypeDef GPIO_InitStruct;
	
    __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	#if ADS129X_USE_SPI == ADS129X_USE_SPI1
		SPI1_Init();//初始化SPI引脚
		//SPI1_DMA_Config();//ADS129X使用DMA
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI2
		SPI2_Init();//初始化SPI引脚	
		//SPI2_DMA_Config();//ADS129X使用DMA
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI3
		SPI3_Init();//初始化SPI引脚	
		//SPI3_DMA_Config();//ADS129X使用DMA
	#endif
	
		//CS
		GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
//		//OSC 外部时钟
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		//RESRT
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//DRDY &中断线
		GPIO_InitStruct.Pin = GPIO_PIN_15 ;
		GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;//下降升沿触发
    GPIO_InitStruct.Pull = GPIO_PULLUP;					//上拉
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
		
    HAL_NVIC_SetPriority(ADS129X_DRDY_IRQn,1,1);       //抢占优先级，子优先级
		HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);            //禁止中断线0
		
		ADS129X_CS(1);	
//	ADS129X_RST=1;				
}

//中断函数，直接处理
void EXTI15_10_IRQHandler(void)
{	
		if(__HAL_GPIO_EXTI_GET_IT(ADS129X_DRDY_PIN) &&  ADS129X_DRDY==0 )
		{		
				ADS129x_Read_Data();//读n个字节的原始数据,存到缓存中
/////////////////////////////////////////////////////////////////////////////////////////////////
				if(work_state == SEND_BULE)
				{
						//每次Ads129x_Data_Move个字节，搬运Ads129x_Write_Num次
						if(queue_data_push(UART_Info->UART_Queue,ADS129x_info.Ads129x_Cache+3,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num)==1) 
						{
								UART_Info->UART_Queue->rear = (UART_Info->UART_Queue->rear+1) % UART_Info->UART_Queue->capacity; //取数据，队头自增，存数据，队尾自增	
						}
				}
				/////////////////////////////////////////////////////////////////////////////////////////////////			
//				if(work_state==WRITE_TF) 
//				{		
//						if(queue_data_push(TF_info.TF_Queue ,ADS129x_info.Ads129x_Cache+3 ,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num))
//						{
//								TF_info.TF_Queue->rear = (TF_info.TF_Queue->rear+1) % TF_info.TF_Queue->capacity; //取数据，队头自增，存数据，队尾自增	
//						}							
//				}
/////////////////////////////////////////////////////////////////////////////////////////////////				
				__HAL_GPIO_EXTI_CLEAR_IT(ADS129X_DRDY_PIN);
		}
}


///////////////////////////////////////////////////////////////////////////////////////////
//驱动程序
//通过SPI与ADS129x通信
__INLINE u8 ADS129x_SPI_ReadWriteByte(u8 com)
{	
	#if ADS129X_USE_SPI == ADS129X_USE_SPI1
		//return SPI1_ReadWriteByte(com);
		return SPI_Fast_ReadWriteByte(&SPI1_Handler,com);
		//return SPI_ReadWriteByte(&SPI1_Handler,com,1);
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI2
		//return SPI2_ReadWriteByte(com);
		return SPI_Fast_ReadWriteByte(&SPI2_Handler,com);
		//return SPI_ReadWriteByte(&SPI2_Handler,com,1);
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI3
		//return SPI3_ReadWriteByte(com);
		return SPI_Fast_ReadWriteByte(&SPI3_Handler,com);
		//return SPI_ReadWriteByte(&SPI3_Handler,com,1);
	#endif
}


//读取x字节数据 //STAT CH1 CH2 ...
//9字节 72M时钟下函数耗时大约10us  8M时钟下 函数耗时大约 120us
void ADS129x_Read_Data(void)
{		
		u8 i;	
		
		ADS129X_CS(0);
///////////////////////////////////////////////////////////////////////////////////////		
//		SPI2_DMA_RX(ADS129x_info.ads129x_Cache,ADS129X_DATANUM); //开启DMASPI采集 传输耗时 12us
///////////////////////////////////////////////////////////////////////////////////////	
		for(i=0;i<ADS129X_DATANUM;i++)
		{	
				ADS129x_info.Ads129x_Cache[i]=ADS129x_SPI_ReadWriteByte(0X00);	
		}
///////////////////////////////////////////////////////////////////////////////////////		
		ADS129X_CS(1);	
///////////////////////////////////////////////////////////////////////////////////////			
	#if ADS129X_USE_TRIANGLE_TESTDATA ==1 //通道1使用自增测试数据
		ADS129x_info.Ads129x_Cache[3]=0;//通道1
		ADS129x_info.Ads129x_Cache[4]=0;	
		ADS129x_info.Ads129x_Cache[5]=ADS129x_info.Ads129x_test_flag++;	
	#endif	
		ADS129x_info.Ads129x_DRDY_flag=1;//DRDY中断标志
		ADS129x_info.Ads129x_err_time=0;
}

//写命令
void ADS129x_Send_CMD(u8 data)
{
		ADS129X_CS(0);
		delay_us(10);
		ADS129x_SPI_ReadWriteByte(data);		
		delay_us(10);	
		ADS129X_CS(1);
}
//读写寄存器		
//输入参数 ：读写命令/要读写寄存器个数/数据指针
void ADS129x_WR_REGS(u8 reg,u8 len,u8 *data)
{
		u8 i;
		ADS129X_CS(0);
		delay_us(10);
		ADS129x_SPI_ReadWriteByte(reg);
		delay_us(10);
		ADS129x_SPI_ReadWriteByte(len-1);		
		if(reg&ADS129X_WREG) //写
		{
				for(i=0;i<len;i++)
				{
						delay_us(10);		
						ADS129x_SPI_ReadWriteByte(*data);
						data++;				
				}			
		}
		else if(reg&ADS129X_RREG)//读		
		{
				for(i=0;i<len;i++)
				{
						delay_us(10);		
						*data = ADS129x_SPI_ReadWriteByte(0xff);
						data++;
				}
		}		
		delay_us(10);	
		ADS129X_CS(1);
}

//将 ADS129x_REG_BUF 数组中的数据写入寄存器
u8 ADS129x_WRITE_REGBUFF(void)
{
		u8 i,res=0;
		u8 ADS129x_REG_Cache[ADS129X_REGNUM];	//寄存器数据缓存
////////////////////////////////////////////////////////////////	
		ADS129x_WR_REGS(ADS129X_WREG|0,ADS129X_REGNUM,ADS129x_REG_BUF);//写寄存器
		delay_ms(1);		
		ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//回读
		delay_ms(1);	
////////////////////////////////////////////////////////////////	
		ADS129X_DEBUG("要写的寄存器数据:\r\n");
		for(i=0;i<ADS129X_REGNUM;i++	)	//要写的寄存器数据											
				ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_BUF[i]);	
		ADS129X_DEBUG("回读的寄存器数据:\r\n");
//////////////////////////////////////////////////////////	
		for(i=0;i<ADS129X_REGNUM;i++	)	//检查REGNUM个寄存器	
		{				
				ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_Cache[i]); //回读到的数据							
				if(ADS129x_REG_BUF[i] != ADS129x_REG_Cache[i])
				{
				#if ADS129X_DEVICE == ADS1298 
						if(i!=0 && i!=18 && i!=19 && i != 20)	//ID 导联脱落和GPIO相关,不做判断
								res=1;
						else
								continue;		
				#elif ADS129X_DEVICE == ADS1299
						if(i!=0 && i!=18 && i!=19 && i != 20 	//ID 导联脱落和GPIO相关,不做判断
						&& i!=9 && i!=10 && i!=11 && i != 12 	) //1299-4 9 10 11 12 都是0，前面都设置对了的话，这个也不做判断了
								res=1;
						else
								continue;
				#elif ADS129X_DEVICE == ADS1292			
						if(i!= 0 && i!=8 && i != 11)					//ID 导联脱落和GPIO相关,不做判断
								res=1;
						else
								continue;			
				#endif							
				}
		}	
		if(res == 0)
				ADS129X_DEBUG("寄存器初始化成功\r\n");
		else		
				ADS129X_DEBUG("寄存器初始化失败\r\n");
		return res;		
}

//寄存器初始化
u8 ADS129x_REG_Init(void)
{	
		u8 i;
		u8 ADS129x_REG_Cache[ADS129X_REGNUM];
///////////////////////////////////////////////////////////////////////////	
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		delay_ms(10);
		ADS129x_Send_CMD(ADS129X_WAKEUP);//唤醒模式
		delay_ms(10);
		ADS129x_Send_CMD(ADS129X_RESET);//复位
		delay_ms(100);
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		delay_ms(10);
//		ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//读默认寄存器
//		ADS129X_DEBUG("读默认寄存器数据:\r\n");
//		for(i=0;i<ADS129X_REGNUM;i++	)	//读寄存器设置
//		{	
//				ADS129x_DEBUG("%d %x\r\n",i,REG_Cache[i]);		
//		}	
		return ADS129x_WRITE_REGBUFF();//写寄存器	
}



//重复初始化
void ADS129x_ReInit(u8 cmd)
{
		u8 res;
		//初始化129X
		Main_printf("Init ads129x ");	
///////////////////////////////////////////////////////////////////////////	
		while(ADS129x_REG_Init())//初始化寄存器
		{		
				Main_printf(" err");		
				delay_ms(500);	
		}	
		Main_printf(" ok\r\n");	
		ADS129x_Send_CMD(ADS129X_RDATAC);
		ADS129x_Send_CMD(ADS129X_START);	//发送开始数据转换（等效于拉高START引脚）	
		delay_ms(500);			
///////////////////////////////////////////////////////////////////////////		
		ADS129x_info.Ads129x_Use_Cannle = ADS129X_USE_CANNLE*ADS129X_CASCADE_NUM; //通道*级联
		ADS129x_info.Ads129x_Data_Move = 	ADS129x_info.Ads129x_Use_Cannle*3; //一次采样搬运N字节有效数据 3*通道数	
		
		Main_printf("采集 %d 通道数据\r\n",ADS129x_info.Ads129x_Use_Cannle);			
		Main_printf("一次采样搬运 %d 字节\r\n",ADS129x_info.Ads129x_Data_Move);		

//		if(cmd==1)//WiFi
//		{
//				//ADS129x_info.Ads129x_Write_Num = WIFI_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
//				//Main_printf("搬运 %d 次做一个包\r\n",ADS129x_info.Ads129x_Write_Num);	
//		}
//		if(cmd==2)//TF卡
//		{
//				ADS129x_info.Ads129x_Write_Num = SD_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
//				Main_printf("TF存储 搬运 %d 次做一个包\r\n",ADS129x_info.Ads129x_Write_Num);	
//		}
		if(cmd==3)//串口/蓝牙
		{
				ADS129x_info.Ads129x_Write_Num = UART_QUEUE_LENGTH/ADS129x_info.Ads129x_Data_Move ; //搬运N次做一个包
				Main_printf("蓝牙/串口发送 搬运 %d 次做一个包\r\n",ADS129x_info.Ads129x_Write_Num);	
		}	
		else
		{
				return;
		}		
}

//ADS129X 进入低功耗模式
void ADS129x_goto_lowpower(void)
{
		Main_printf("1292休眠\r\n");
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		delay_ms(10);
		ADS129x_Send_CMD(ADS129X_STANDBY);//待机模式
		delay_ms(10);
}


/*功能：把采到的3个字节转成有符号32位数 */
s32 get_volt(u32 num)
{		
			s32 change;			
			change = num;
			change <<= 8;
			change >>= 8;
			return change;
}

