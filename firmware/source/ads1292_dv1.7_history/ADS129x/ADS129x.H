#ifndef __ADS129x_H
#define __ADS129x_H	 

#include "sys.h" 




///////////////////////////////////////////////////////////////////////////////////////////
//宏定义
#define	ADS1298		1 
#define	ADS1299		2
#define	ADS1292		3
#define	ADS129X_USE_SPI1		1
#define	ADS129X_USE_SPI2		2
#define	ADS129X_USE_SPI3		3
///////////////////////////////////////////////////////////////////////////////////////////
//设备功能选择，只能二选一
#define ECG1CANNLE_RESP		0	//单导心电+呼吸
#define ECG3CANNLE				1	//三导心电/肌电
///////////////////////////////////////////////////////////////////////////////////////////
//芯片及关键设置
#define ADS129X_DEVICE				ADS1292	 	//芯片选择
#define ADS129X_UART_DEBUG		0 		//1 使用串口调试 0不使用
#define ADS129X_CASCADE_NUM		1			//设备级联数量
#define ADS129X_USE_CANNLE		2			//1个芯片使用N个通道
#define	ADS129X_USE_DMA				0			//1开启DMASPI 0不使用DMASPI
////////////////////////////////////////////////////////////////////////////////////////////
//注意导联脱落功能会影响输入阻抗：无导联脱落	1000M  	电流源导联脱落	500M  	上拉电阻导联脱落	10M	
#define	ADS129X_USE_LEADOFF			0				//导联脱落检测功能 1开启 0关闭 		
////////////////////////////////////////////////////////////////////////////////////////////
//测试数据选项，都为0表示正常采集外部数据（优先级 锯齿波>方波>噪声） 
#define ADS129X_USE_TRIANGLE_TESTDATA 	0 		//使用自增测试数据			
#define ADS129X_USE_SQUARE_TESTDATA 		0 		//使用内部方波数据
#define	ADS129X_USE_NOISE_TESTDATA			0			//内部噪声测试 		
////////////////////////////////////////////////////////////////////////////////
//引脚及通讯函数
#define ADS129X_USE_SPI 			ADS129X_USE_SPI3	//SPI选择

#define ADS129X_CS(n)					(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET))
//#define ADS129X_RST				PBout(1)	 //低电平有效
//#define ADS129X_OSC				PBout(1)	

#define ADS129X_DRDY_PIN			GPIO_PIN_15
//#define ADS129X_DRDY_LINE			EXTI4_IRQn//DRDY中断通道
#define ADS129X_DRDY					HAL_GPIO_ReadPin(GPIOA,ADS129X_DRDY_PIN)
#define ADS129X_DRDY_IRQn 		EXTI15_10_IRQn
////////////////////////////////////////////////////////////////////////////////////////////
//芯片寄存器数量及中断读取的字节数
#if ADS129X_DEVICE == ADS1298 
	#define ADS129X_REGNUM 26			//寄存器数量
	#define ADS129X_DATANUM 27		//一次DRDY中断的数据读取数量
#elif ADS129X_DEVICE == ADS1299
	#define ADS129X_REGNUM 24			//寄存器数量
	#define ADS129X_DATANUM 27		//数据读取数量
#elif ADS129X_DEVICE == ADS1292
	#define ADS129X_REGNUM 12			//寄存器数量
	#define ADS129X_DATANUM 9			//数据读取数量
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
__align(4) typedef struct
{
		u8 Ads129x_DRDY_flag;			//DRDY中断

		u8 Ads129x_Cascade_Num;//芯片级联数量
		u8 Ads129x_Use_Cannle;//一个芯片使用N个通道
		u16 Ads129x_Data_Move; //每次中断搬运N个数据 ADS129X_Data_Move =  Ads129x_USE_Cannle*3
		u16 Ads129x_Write_Num; //搬运N次 做一个包   ADS129x_Write_Num = ADS129x_SAMPLING_POINT / Ads129x_USE_Cannle

		u32 Ads129x_leadoff_flag;//导联脱离标志
		u8 Ads129x_test_flag;		//自增测试数据
		u16 Ads129x_err_time;

		u8 Ads129x_Cache[27];	//129x数据缓冲区
		u32 cannle[12];	//存储原始的数据
		s32	p_Temp[12];	//存储原始的数据
	
}_ADS129x_info;
extern _ADS129x_info ADS129x_info;//芯片结构
extern	u8 ADS129x_REG_BUF[ADS129X_REGNUM];//ads1299寄存器数组
/////////////////////////////////////////////////////////////////////////////////////////////
//串口DEBUG	
#if ADS129X_UART_DEBUG == 1 
		#define ADS129X_DEBUG(...)    Main_printf(__VA_ARGS__)		//串口1
#else
		#define ADS129X_DEBUG(...)		
#endif
////////////////////////////////////////////////////////////////////////////////	
//命令定义
//注意： 开机默认处于 RDATAC 模式时，要读写寄存器必须先发送SDATAC命令
//如果器件处于 RDATAC 模式，则必须先发出 SDATAC命令，然后才能将任何其他命令发送到器件中。
//系统命令
	#define ADS129X_WAKEUP	0X02	//从待机模式唤醒
	#define ADS129X_STANDBY	0X04	//进入待机模式
	#define ADS129X_RESET	0X06	//复位
	#define ADS129X_START	0X08	//启动转换
	#define ADS129X_STOP	0X0A	//停止转换
//数据读取命令
	#define ADS129X_RDATAC	0X10	//启用连续的数据读取模式,默认使用此模式
	#define ADS129X_SDATAC	0X11	//停止连续的数据读取模式
	#define ADS129X_RDATA	0X12	//通过命令读取数据;支持多个读回。
//寄存器读取命令
	//r rrrr=要读、写的寄存器首地址	 //	n nnnn=要读写的寄存器数量
	#define ADS129X_RREG	0X20	//读取  001r rrrr(首字节) 000n nnnn(2字节) 
	#define ADS129X_WREG	0X40	//写入  010r rrrr(首字节) 000n nnnn(2字节)
/////////////////////////////////////////////////////////////////////////////////////////////
//外部函数
s32 get_volt(u32 num);//把采到的3个字节转成有符号32位数

void ADS129x_GPIO_Init(void); //初始化ADS1292引脚
void ADS129x_ReInit(u8 cmd);//重复初始化
u8 ADS129x_SPI_ReadWriteByte(u8 com);		//SPI通信
__INLINE void ADS129x_Read_Data(void);

void ADS129x_goto_lowpower(void);

#endif






