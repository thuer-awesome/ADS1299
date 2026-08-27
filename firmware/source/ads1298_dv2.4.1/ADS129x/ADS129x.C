#include "ADS129x.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"
#include "Timer.h"
#include "myQueue.h"
#include "dma.h"

#include "string.h"
#include "my_protocol.h"




/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/10/07
//版本：V2.0
//说明：适用于 ADS1298 ADS1299 ADS1292单芯片
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


_ADS129x_info ADS129x_info;

/////////////////////////////////////////////////////////////////////////////////////////////
//设置默认寄存器参数,这里的参数尽量不要调整，使用其他的控制函数调整功能
#if ADS129X_DEVICE == ADS1298
	u8 ADS129x_REG_BUF[ADS129X_REGNUM]= {
		0x92,//ID		ads1298   10010010
		
		0xC6,//CONFIG1	高分辨率模式，禁用外部时钟输出，0 32000	1 16000 2 8000 3 4000 4 2000 5 1000 6 500 
		//0x45,//CONFIG1	低分辨率模式，禁用外部时钟输出，	0 16000 1 8000 2 4000 3 2000 4 1000 5 500 6 250
		
		0x10,//CONFIG2	0001 0000 内部生成测试信号，测试信号振幅为 基准电压/2400，测试信号为脉冲信号
		0xCC,//CONFIG3	1100 110x	//基准电压为2.42V，内部生成右腿驱动信号，幅值为基准电压/2，开右腿驱动电源，最后一位只读
		
		//0x07,//LOFF		xxx0 0111		//打开直流导联脱落检测 电流源模式 电流幅度12nA
		0x02,//LOFF		0000 0010		//关闭导联脱落检测 	
/////////////////////////////////////////////////////////////////////////////////	
		0x00,//CH1SET		第一个数 增益 			6-12 5-8 4-4 3-3 2-2 1-1 0-6
		0x00,//CH2SET	 	第二个数 输入模式		0-普通输入 1-输入短路 5-内部测试
		0x00,//CH3SET		
		0x00,//CH4SET		
		0x00,//CH5SET	
		0x00,//CH6SET
		0x00,//CH7SET 
		0x00,//CH8SET
/////////////////////////////////////////////////////////////////////////////////		
		0x00,//RLD_SENSP	//01	使用通道1右腿驱动 //00	禁用右腿驱动
		0x00,//RLD_SENSN	//01	使用通道1右腿驱动	//00	禁用右腿驱动		
		0xFF,//LOFF_SENSP		1111 1111	//导联脱落检测	1 2 3 4 5 6 7 8P
		0xFF,//LOFF_SENSN		0000 0010	//导联脱落检测	 2N
		0x00,//LOFF_FLIP			//导联脱落导出电流方向
		0x00,//LOFF_STATP		//导联脱落正信号状态（只读）
		0x00,//LOFF_STATN		//导联脱落负信号状态（只读）
		0x00,//GPIO			0000 0000	输出低电平
		0x00,//PACE			0000 0000	关起搏信号检测
		0x00,//RESP			0000 0000	关呼吸检测
		
		//0x02,//CONFIG4	0000 0010	//开导联脱落比较器
		0x00,//LOFF_SENSP		//导联脱落比较器断电	
		
		0x00,//WCT1		//00 关威尔逊中心端子连接   //0B  WCTA to RA 通道2N
		0x00,//WCT2		//00	关威尔逊中心端子连接  //C2  WCTB to LA 通道1P  WCTC to LL 通道2P
		//0x0B,//WCT1		//00 关威尔逊中心端子连接   //0B  WCTA to RA 通道2N
		//0xC2,//WCT2		//00	关威尔逊中心端子连接  //C2  WCTB to LA 通道1P  WCTC to LL 通道2P
	};
#endif	
		
	
	
	
///////////////////////////////////////////////////////////////////////////////////////////	
//ADS129x的IO口初始化
void ADS129x_GPIO_Init(void) 
{
		GPIO_InitTypeDef GPIO_InitStruct;
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
/////////////////////////////////////////////////////////////////////////////////
	#if ADS129X_USE_SPI == ADS129X_USE_SPI1
		SPI1_Init();//初始化SPI引脚	
		#if ADS129X_USE_DMA == 1
			SPI1_DMA_Config();
		#endif
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI2
		SPI2_Init();//初始化SPI引脚	
		#if ADS129X_USE_DMA == 1
			SPI2_DMA_Config();
		#endif
	#elif	ADS129X_USE_SPI == ADS129X_USE_SPI3
		SPI3_Init();//初始化SPI引脚	
		#if ADS129X_USE_DMA == ADS129X_ENABLE
			SPI3_DMA_Config();
		#endif
	#endif	
/////////////////////////////////////////////////////////////////////////////////	
		//CS
		GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
		//RESRT
		GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
//		//OSC 外部时钟
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		//DRDY &中断线
		GPIO_InitStruct.Pin = GPIO_PIN_11 ;
		GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;//下降升沿触发
    GPIO_InitStruct.Pull = GPIO_PULLUP;					//上拉
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
/////////////////////////////////////////////////////////////////////////////////		
    HAL_NVIC_SetPriority(ADS129X_DRDY_IRQn,1,1);       //抢占优先级，子优先级
		HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);            //禁止中断线0
/////////////////////////////////////////////////////////////////////////////////		
		ADS129X_CS(1);	
		ADS129X_RST(1);		//取消复位				
}



//中断函数，直接处理
void EXTI15_10_IRQHandler(void)
{	
		if(__HAL_GPIO_EXTI_GET_IT(ADS129X_DRDY_PIN) &&  ADS129X_DRDY==0 )
		{		
				ADS129x_Read_Data();//读n个字节的原始数据,存到缓存中
/////////////////////////////////////////////////////////////////////////////////////////////////		
				//每次Ads129x_Data_Move个字节，搬运Ads129x_Write_Num次
				if(queue_data_push(my_Portocol_Info->Queue,ADS129x_info.Ads129x_Databuf,ADS129x_info.Ads129x_Data_Move,my_Portocol_Info-> Queue_push_time)==QUEUE_RETURN_OK) 
				{
						queue_add_rear(my_Portocol_Info->Queue) ;//存数据，队尾自增	
				}
/////////////////////////////////////////////////////////////////////////////////////////////////				
				__HAL_GPIO_EXTI_CLEAR_IT(ADS129X_DRDY_PIN);
		}
}


//读取x字节数据 //STAT CH1 CH2 ...
//9字节 72M时钟下函数耗时大约10us  8M时钟下 函数耗时大约 120us
__INLINE static void ADS129x_Read_Data(void)//72M时钟下函数耗时大约10us  8M时钟下 函数耗时大约 100us
{		
		u8 i,j;
	
		ADS129X_CS(0);
///////////////////////////////////////////////////////////////////////////////////////
	#if ADS129X_USE_DMA == ADS129X_ENABLE //使用DMA SPI
		#if ADS129X_USE_SPI == ADS129X_USE_SPI1
			SPI1_DMA_RX(ADS129x_info.Ads129x_Cache,ADS129X_DATANUM); //开启DMA 18M耗时12us
		#elif	ADS129X_USE_SPI == ADS129X_USE_SPI2
			SPI2_DMA_RX(ADS129x_info.Ads129x_Cache,ADS129X_DATANUM); //开启DMA 18M耗时12us
		#elif	ADS129X_USE_SPI == ADS129X_USE_SPI3
			SPI3_DMA_RX(ADS129x_info.Ads129x_Cache,ADS129X_DATANUM); //开启DMA 18M耗时12us
		#endif
			delay_us(10);
	#else
			for(j=0;j<ADS129X_DATANUM;j++)//SPI采集，18M时钟下函数耗时大约45us
			{
					ADS129x_info.Ads129x_Cache[j]=ADS129x_SPI_ReadWriteByte(0X00);	
			}
	#endif
		ADS129X_CS(1);	
///////////////////////////////////////////////////////////////////////////////////////				
	#if ADS129X_DEVICE == ADS1298 //标准线序 I II V1~V6
		memcpy(ADS129x_info.Ads129x_Databuf,ADS129x_info.Ads129x_Cache+3,24);//芯片1 通道1-8
	#endif				
//	#if ADS129X_DEVICE == ADS1298 //定制线序  I II V6 V2 V5 V1 V4 V3 适配 麦迪克斯 翰纬 中旗 谷山丰 博声 迪姆 凯沃尔
//		memcpy(ADS129x_info.Ads129x_Databuf+0,ADS129x_info.Ads129x_Cache+3,3);//芯片1 LA
//		memcpy(ADS129x_info.Ads129x_Databuf+3,ADS129x_info.Ads129x_Cache+6,3);//芯片1 LL
//		memcpy(ADS129x_info.Ads129x_Databuf+21,ADS129x_info.Ads129x_Cache+9,3);//芯片1 V6
//		memcpy(ADS129x_info.Ads129x_Databuf+9,ADS129x_info.Ads129x_Cache+12,3);//芯片1 V2
//		memcpy(ADS129x_info.Ads129x_Databuf+18,ADS129x_info.Ads129x_Cache+15,3);//芯片1 V5
//		memcpy(ADS129x_info.Ads129x_Databuf+6,ADS129x_info.Ads129x_Cache+18,3);//芯片1 V1
//		memcpy(ADS129x_info.Ads129x_Databuf+15,ADS129x_info.Ads129x_Cache+21,3);//芯片1 V4
//		memcpy(ADS129x_info.Ads129x_Databuf+12,ADS129x_info.Ads129x_Cache+24,3);//芯片1 V3				
//	#endif
//	#if ADS129X_DEVICE == ADS1298 //定制线序  I II V2 V1 V3 V4 V6 V5 适配无锡中健科仪1304 2302 图瑞H300 1200 邦健12PLUS
//		memcpy(ADS129x_info.Ads129x_Databuf+0,ADS129x_info.Ads129x_Cache+3,3);//芯片1 LA
//		memcpy(ADS129x_info.Ads129x_Databuf+3,ADS129x_info.Ads129x_Cache+6,3);//芯片1 LL
//		memcpy(ADS129x_info.Ads129x_Databuf+9,ADS129x_info.Ads129x_Cache+9,3);//芯片1 V2
//		memcpy(ADS129x_info.Ads129x_Databuf+6,ADS129x_info.Ads129x_Cache+12,3);//芯片1 V1
//		memcpy(ADS129x_info.Ads129x_Databuf+12,ADS129x_info.Ads129x_Cache+15,3);//芯片1 V3
//		memcpy(ADS129x_info.Ads129x_Databuf+15,ADS129x_info.Ads129x_Cache+18,3);//芯片1 V4
//		memcpy(ADS129x_info.Ads129x_Databuf+21,ADS129x_info.Ads129x_Cache+21,3);//芯片1 V6
//		memcpy(ADS129x_info.Ads129x_Databuf+18,ADS129x_info.Ads129x_Cache+24,3);//芯片1 V5				
//	#endif			
///////////////////////////////////////////////////////////////////////////////////////
	#if ADS129X_USE_TRIANGLE_TESTDATA ==1 //通道1使用自增测试数据
		ADS129x_info.Ads129x_Databuf[0]=0;	//通道1
		ADS129x_info.Ads129x_Databuf[1]=0;	
		ADS129x_info.Ads129x_Databuf[2]=ADS129x_info.Ads129x_test_flag++;	
	#endif
///////////////////////////////////////////////////////////////////////////////////////		
		ADS129x_info.Ads129x_DRDY_flag=1;//DRDY中断
		ADS129x_info.Ads129x_err_time=0;//异常复位
}


///////////////////////////////////////////////////////////////////////////////////////////
//驱动程序
//通过SPI与ADS129x通信
__INLINE static u8 ADS129x_SPI_ReadWriteByte(u8 com)
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

//芯片 写命令
void static ADS129x_Send_CMD(u8 data)
{
		ADS129X_CS(0);
		delay_us(10);
		ADS129x_SPI_ReadWriteByte(data);		
		delay_us(10);	
		ADS129X_CS(1);
}
//芯片 读写寄存器		
//输入参数 ：读写命令/要读写寄存器个数/数据指针
void static ADS129x_WR_REGS(u8 reg,u8 len,u8 *data)
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
						*data = ADS129x_SPI_ReadWriteByte(0);
						data++;
				}
		}		
		delay_us(10);	
		ADS129X_CS(1);
}



//芯片 将 ADS129x_REG_BUF 数组中的数据写入寄存器
void static ADS129x_Write_REGBUFF(void)
{
		u8 num=0;
		ADS129x_WR_REGS(ADS129X_WREG|0,ADS129X_REGNUM,ADS129x_REG_BUF);//写寄存器
}
//芯片 回读检查寄存器数据
//返回0 成功 返回1 失败
u8 static ADS129x_Check_REGBUFF(void)
{
		u8 i,j,res=0;
		u8 ADS129x_REG_Cache[ADS129X_REGNUM];	//寄存器数据缓存
/////////////////////////////////////////////////////////////////////////////////////////////////
		ADS129x_WR_REGS(ADS129X_RREG|0,ADS129X_REGNUM,ADS129x_REG_Cache);//回读	
/////////////////////////////////////////////////////////////////////////////////////////////////	
//		ADS129X_DEBUG("芯片 %d 要写寄存器数据:\r\n",j);
//		for(i=0;i<ADS129X_REGNUM;i++	)	//要写的寄存器数据											
//				ADS129X_DEBUG("%d %x\r\n",i,ADS129x_REG_BUF[j][i]);	
		ADS129X_DEBUG("芯片 %d 回读的寄存器数据:\r\n",j);
/////////////////////////////////////////////////////////////////////////////////////////////////
		for(i=0;i<ADS129X_REGNUM;i++	)	//检查REGNUM个寄存器	
		{		
				ADS129X_DEBUG("%x %x\r\n",i,ADS129x_REG_Cache[i]); //回读到的数据							
				if(ADS129x_REG_BUF[i] != ADS129x_REG_Cache[i])
				{
				#if ADS129X_DEVICE == ADS1298 
						if(i!=0 && i!=3 && i!=18 && i!=19 && i != 20 	//ID 导联脱落和GPIO相关,不做判断
						&& i!=9 && i!=10 && i!=11 && i != 12 	) //129/1296 9 10 11 12 都是0，前面都设置对了的话，这个也不做判断了
								res=1;
						else
								continue;	
				#elif ADS129X_DEVICE == ADS1299
						if(i!=0 && i!=18 && i!=19 && i != 20 	//ID 导联脱落和GPIO相关,不做判断
						&& i!=9 && i!=10 && i!=11 && i != 12 	) //1299-4/6 9 10 11 12 都是0，前面都设置对了的话，这个也不做判断了
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
				ADS129X_DEBUG("芯片 %d 寄存器初始化成功\r\n",j);
		else		
				ADS129X_DEBUG("芯片 %d 寄存器初始化失败\r\n",j);
/////////////////////////////////////////////////////////////////////////////////////////////////		
		return res;		
}

//芯片 中途改变寄存器设置
u8 ADS129x_REG_Init(void)
{			
		ADS129x_Device_SDATAC();//进入SDATAC模式+数据停止
		ADS129x_Write_REGBUFF(); //寄存器数组写入芯片
		return ADS129x_Check_REGBUFF(); //回读检查	
}

//芯片 首次的寄存器初始化+回读检查
u8 ADS129x_Device_Init(void)
{	
		u8 res,i;
	
		Main_printf("ADS129X INIT\r\n");	
/////////////////////////////////////////////////////////////////////////////////////////////////
		ADS129x_Device_Reset();	//芯片复位+进入SDATAC模式
		ADS129x_Write_REGBUFF(); //默认寄存器数组写入芯片
		ADS129x_Device_SDATAC();

	#if ADS129X_DEVICE == ADS1298
//		ADS129x_HR_Change(ADS1298_SPS_LP);//只针对于 ADS1298//指定芯片 设置芯片工作模式
//		ADS129x_SPS_Change(ADS1298_SPS_500_LP);//指定芯片 设置采样率
//		ADS129x_PGA_Change(ADS1298_PGA_6);//指定芯片 所有通道 修改增益
//	
//		ADS129x_MODE_Change(ADS1298_CHANNLE_NORMAL_INPUT);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）
//		//ADS129x_MODE_Change(ADS1298_CHANNLE_TEST_SIGNAL);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）
//		//ADS129x_MODE_Change(ADS1298_CHANNLE_SHORT_INPUT);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）

//		ADS129x_RILOUT_Change(ADS1298_RLD_CHANNLE_1);//指定芯片 开启/关闭右腿驱动路由（CMD包含开启和关闭的右腿驱动路由,可以做与运算）
//		ADS129x_LOFF_Change(ADS1298_LEADOFF_ON);//只针对于 ADS1299//指定芯片 所有通道 开/关 导联脱落检测(阻抗检测)
	
		ADS129x_HR_Change(ADS1298_SPS_HR);//只针对于 ADS1298//指定芯片 设置芯片工作模式
		ADS129x_SPS_Change(ADS1298_SPS_500_HR);//指定芯片 设置采样率
		ADS129x_PGA_Change(ADS1298_PGA_6);//指定芯片 所有通道 修改增益
	
		ADS129x_MODE_Change(ADS1298_CHANNLE_NORMAL_INPUT);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）
		//ADS129x_MODE_Change(ADS1298_CHANNLE_TEST_SIGNAL);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）
		//ADS129x_MODE_Change(ADS1298_CHANNLE_SHORT_INPUT);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）

		ADS129x_RILOUT_Change(ADS1298_RLD_CHANNLE_NONE);//指定芯片 开启/关闭右腿驱动路由（CMD包含开启和关闭的右腿驱动路由,可以做与运算）
		ADS129x_LOFF_Change(ADS1298_LEADOFF_OFF);//只针对于 ADS1299//指定芯片 所有通道 开/关 导联脱落检测(阻抗检测)	
	#endif
/////////////////////////////////////////////////////////////////////////////////////////////////	
		while(ADS129x_REG_Init()) //数据写入寄存器
		{
				Main_printf("err ");
				delay_ms(500);
		}	
		ADS129x_Data_Start();//开启数据采集
		Main_printf("ADS129X init OK\r\n");	
/////////////////////////////////////////////////////////////////////////////////////////////////			
		ADS129x_info.Ads129x_Use_Cannle = ADS129X_USE_CANNLE; //通道*级联
		ADS129x_info.Ads129x_Data_Move = 	ADS129x_info.Ads129x_Use_Cannle*3; //一次采样N字节有效数据 3*通道数
	
		Main_printf("共采集 %d 通道数据\r\n",ADS129x_info.Ads129x_Use_Cannle);			
		Main_printf("一次原始数据长度 %d 字节\r\n",ADS129x_info.Ads129x_Data_Move);	
/////////////////////////////////////////////////////////////////////////////////////////////////	
		return 0;		
}



//芯片 设置芯片模式（仅ADS1298有效）
//CONFIG1	最高位 芯片模式 LP HR
u8 ADS129x_HR_Change(u8 cmd)
{	
		if(cmd>1) //指令超范围
				return 1;
	#if ADS129X_DEVICE == ADS1298
		ADS129x_REG_BUF[ADS1298_REG_CONFIG1]=   (ADS129x_REG_BUF[ADS1298_REG_CONFIG1]&0x7F ) | (cmd<<7 & 0x80)	;
	#endif
		
		return 0; //成功
}

//芯片 设置采样率
//CONFIG1	低3位 采样率
u8 ADS129x_SPS_Change(u8 cmd)
{

		if(cmd>6) //指令超范围
				return 1;
	
	#if ADS129X_DEVICE == ADS1299
		ADS129x_REG_BUF[ADS1299_REG_CONFIG1]=   (ADS129x_REG_BUF[ADS1299_REG_CONFIG1]&0xF8 ) | (cmd<<0 & 0x07)	;
	#endif
	
	#if ADS129X_DEVICE == ADS1298
		ADS129x_REG_BUF[ADS1298_REG_CONFIG1]=   (ADS129x_REG_BUF[ADS1298_REG_CONFIG1]&0xF8 ) | (cmd<<0 & 0x07)	;
	#endif
		
	#if ADS129X_DEVICE == ADS1292
		ADS129x_REG_BUF[ADS1292_REG_CONFIG1]=   (ADS129x_REG_BUF[ADS1292_REG_CONFIG1]&0xF8 ) | (cmd<<0 & 0x07)	;
	#endif
		
		return 0; //成功
}


//芯片 增益统一控制
u8 ADS129x_PGA_Change(u8 cmd)
{		
	#if ADS129X_DEVICE == ADS1299
		ADS129x_PGA_Change_One(1,cmd);
		ADS129x_PGA_Change_One(2,cmd);
		ADS129x_PGA_Change_One(3,cmd);
		ADS129x_PGA_Change_One(4,cmd);
		ADS129x_PGA_Change_One(5,cmd);
		ADS129x_PGA_Change_One(6,cmd);
		ADS129x_PGA_Change_One(7,cmd);
		ADS129x_PGA_Change_One(8,cmd);
	#endif

	#if ADS129X_DEVICE == ADS1298
		ADS129x_PGA_Change_One(1,cmd);
		ADS129x_PGA_Change_One(2,cmd);
		ADS129x_PGA_Change_One(3,cmd);
		ADS129x_PGA_Change_One(4,cmd);
		ADS129x_PGA_Change_One(5,cmd);
		ADS129x_PGA_Change_One(6,cmd);
		ADS129x_PGA_Change_One(7,cmd);
		ADS129x_PGA_Change_One(8,cmd);
	#endif
	
	#if ADS129X_DEVICE == ADS1292
		ADS129x_PGA_Change_One(1,cmd);
		ADS129x_PGA_Change_One(2,cmd);
	#endif
}

//芯片 增益单独控制
//chn 												1~8
u8 ADS129x_PGA_Change_One(u8 chn,u8 cmd)
{
	
		if(cmd>6) //指令超范围
				return 1;
		
	#if ADS129X_DEVICE == ADS1299
		switch(chn)
		{
				case 1:
						ADS129x_REG_BUF[ADS1299_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH1SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				case 2:
						ADS129x_REG_BUF[ADS1299_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH2SET]&0x8f ) | (cmd<<4 & 0x70)	;	
				break;
				case 3:
						ADS129x_REG_BUF[ADS1299_REG_CH3SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH3SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				case 4:
						ADS129x_REG_BUF[ADS1299_REG_CH4SET]=	 (ADS129x_REG_BUF[ADS1299_REG_CH4SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 5:
						ADS129x_REG_BUF[ADS1299_REG_CH5SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH5SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 6:
						ADS129x_REG_BUF[ADS1299_REG_CH6SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH6SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 7:
						ADS129x_REG_BUF[ADS1299_REG_CH7SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH7SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 8:
						ADS129x_REG_BUF[ADS1299_REG_CH8SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH8SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				default :
					return 2;
				break;
		}
	#endif	
		
	#if ADS129X_DEVICE == ADS1298
		switch(chn)
		{
				case 1:
						ADS129x_REG_BUF[ADS1298_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH1SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				case 2:
						ADS129x_REG_BUF[ADS1298_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH2SET]&0x8f ) | (cmd<<4 & 0x70)	;	
				break;
				case 3:
						ADS129x_REG_BUF[ADS1298_REG_CH3SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH3SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				case 4:
						ADS129x_REG_BUF[ADS1298_REG_CH4SET]=	 (ADS129x_REG_BUF[ADS1298_REG_CH4SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 5:
						ADS129x_REG_BUF[ADS1298_REG_CH5SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH5SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 6:
						ADS129x_REG_BUF[ADS1298_REG_CH6SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH6SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 7:
						ADS129x_REG_BUF[ADS1298_REG_CH7SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH7SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;				
				case 8:
						ADS129x_REG_BUF[ADS1298_REG_CH8SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH8SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				default :
					return 2;
				break;
		}
	#endif	
		
	#if ADS129X_DEVICE == ADS1292
		switch(chn)
		{
				case 1:
						ADS129x_REG_BUF[ADS1292_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1292_REG_CH1SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				case 2:
						ADS129x_REG_BUF[ADS1292_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1292_REG_CH2SET]&0x8f ) | (cmd<<4 & 0x70)	;
				break;
				default :
					return 2;
				break;
		}
	#endif	

		
		return 0; //成功
}


//芯片 所有通道 修改通道模式
//CHnSET	 	低2:0位 输入模式		0-普通输入 1-输入短路 5-内部测试 ...
u8 ADS129x_MODE_Change(u8 cmd)
{
		u8 num=0;
	#if ADS129X_DEVICE == ADS1299
		if(cmd>7) //指令超范围
				return 1;
		ADS129x_REG_BUF[ADS1299_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH1SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH2SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH3SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH3SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH4SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH4SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH5SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH5SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH6SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH6SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH7SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH7SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1299_REG_CH8SET]=  (ADS129x_REG_BUF[ADS1299_REG_CH8SET]&0xf8 ) | (cmd<<0 & 0x07)	;
	#endif
	
	#if ADS129X_DEVICE == ADS1298
		if(cmd>7) //指令超范围
				return 1;
		ADS129x_REG_BUF[ADS1298_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH1SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH2SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH3SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH3SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH4SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH4SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH5SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH5SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH6SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH6SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH7SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH7SET]&0xf8 ) | (cmd<<0 & 0x07)	;
		ADS129x_REG_BUF[ADS1298_REG_CH8SET]=  (ADS129x_REG_BUF[ADS1298_REG_CH8SET]&0xf8 ) | (cmd<<0 & 0x07)	;
	#endif
		
	#if ADS129X_DEVICE == ADS1292
		if(cmd>6) //指令超范围
				return 1;
		ADS129x_REG_BUF[ADS1292_REG_CH1SET]=  (ADS129x_REG_BUF[ADS1292_REG_CH1SET]&0xf8 ) | (cmd<<0 & 0x07)		;
		ADS129x_REG_BUF[ADS1292_REG_CH2SET]=  (ADS129x_REG_BUF[ADS1292_REG_CH2SET]&0xf8 ) | (cmd<<0 & 0x07)		;
	#endif		
		
		return 0; //成功
}


//芯片 开启/关闭右腿驱动路由（CMD包含开启和关闭的右腿驱动路由,可以做与运算）
u8 ADS129x_RILOUT_Change(u8 cmd)
{		
	#if ADS129X_DEVICE == ADS1299
		ADS129x_REG_BUF[ADS1299_REG_BIAS_SENSP]=   cmd;
		ADS129x_REG_BUF[ADS1299_REG_BIAS_SENSN]= 	cmd;
	#endif

	#if ADS129X_DEVICE == ADS1298
		ADS129x_REG_BUF[ADS1298_REG_RLD_SENSP]=		cmd;
		ADS129x_REG_BUF[ADS1298_REG_RLD_SENSN]= 		cmd;
	#endif
	
	#if ADS129X_DEVICE == ADS1292
		ADS129x_REG_BUF[ADS1292_REG_RLD_SENS]=  (ADS129x_REG_BUF[ADS1292_REG_RLD_SENS]&0xf0 ) | (cmd<<0 & 0x0F)		;
	#endif	
		return 0; //成功
}


//芯片 所有通道 开/关 导联脱落检测(阻抗检测) 只针对于 ADS1299 / ADS1298
//注意导联脱落功能会影响输入阻抗
//无导联脱落	1000M  	电流源导联脱落	500M  	上拉电阻导联脱落	10M	
//CMD:0 关 其他：开启
u8 ADS129x_LOFF_Change(u8 cmd)
{	
	#if ADS129X_DEVICE == ADS1299
		if(cmd)
		{
				ADS129x_REG_BUF[ADS1299_REG_LOFF_SENSP]=   0xff;
				ADS129x_REG_BUF[ADS1299_REG_LOFF_SENSN]= 	0xff;
		}
		else
		{
				ADS129x_REG_BUF[ADS1299_REG_LOFF_SENSP]=   0x00;
				ADS129x_REG_BUF[ADS1299_REG_LOFF_SENSN]= 	0x00;
		}
	#endif
	
	#if ADS129X_DEVICE == ADS1298
		if(cmd)
		{
				ADS129x_REG_BUF[ADS1298_REG_LOFF_SENSP]=   0xff;
				ADS129x_REG_BUF[ADS1298_REG_LOFF_SENSN]= 	0xff;
		}
		else
		{
				ADS129x_REG_BUF[ADS1298_REG_LOFF_SENSP]=   0x00;
				ADS129x_REG_BUF[ADS1298_REG_LOFF_SENSN]= 	0x00;
		}
	#endif		
		return 0; //成功
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//其他的控制函数

//芯片 退出低功耗模式+复位+停止数据发送（当前模式 可以对进行寄存器设置）
void static ADS129x_Device_Reset(void)
{	
//		ADS129X_RST=0 ;	//复位	
//		delay_ms(10);
//		ADS129X_RST=1 ;	//取消复位	
//		delay_ms(100);
	
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		ADS129x_Send_CMD(ADS129X_WAKEUP);//退出低功耗模式
		ADS129x_Send_CMD(ADS129X_RESET);//复位
		delay_ms(100);
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		ADS129x_Send_CMD(ADS129X_STOP);//停止数据转换	
}


//芯片 进入SDATAC模式（当前模式 可以对进行寄存器设置）
void static ADS129x_Device_SDATAC(void)
{
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		ADS129x_Send_CMD(ADS129X_STOP);//停止数据转换	
}


//芯片 启动数据发送,DRDY引脚开始工作
void static ADS129x_Data_Start(void)
{
		ADS129x_Device_SDATAC();

		ADS129x_Send_CMD(ADS129X_RDATAC);//连续采样模式
		ADS129x_Send_CMD(ADS129X_START);//发送开始数据转换（等效于拉高START引脚）	
}

//芯片 休眠进入低功耗模式，DRDY引脚停止
void ADS129x_Goto_Lowpower(void)
{
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		ADS129x_Send_CMD(ADS129X_STANDBY);//待机模式
}

//开启中断服务函数
void ADS129x_Start_INT(void) 
{	
		ADS129x_Data_Start();
		HAL_NVIC_EnableIRQ(ADS129X_DRDY_IRQn);
	
		ADS129x_info.Ads129x_DRDY_flag=0;//DRDY中断
		ADS129x_info.Ads129x_DATA_State=1;//数据开始采集
}
//关闭中断服务函数
void ADS129x_Stop_INT(void) 
{
		HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);
	
		ADS129x_Device_SDATAC();

		ADS129x_info.Ads129x_DRDY_flag=0;//DRDY中断
		ADS129x_info.Ads129x_DATA_State=0; //数据停止采集
}

//把3个字节转成有符号32位数 
s32 get_volt(u32 num)
{		
			s32 change;			
			change = num;
			change <<= 8;
			change >>= 8;
			return change;
}