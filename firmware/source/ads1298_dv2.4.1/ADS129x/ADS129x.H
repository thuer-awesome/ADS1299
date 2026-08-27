#ifndef __ADS129x_H
#define __ADS129x_H	 

#include "sys.h" 


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/10/07
//版本：V2.0
//说明：适用于 ADS1298 ADS1299 ADS1292单芯片
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////
//相关功能使能位
#define ADS129X_ENABLE   1
#define ADS129X_DISABLE  0
//宏定义
#define	ADS1298		1 
#define	ADS1299		2
#define	ADS1292		3
#define	ADS129X_USE_SPI1		1
#define	ADS129X_USE_SPI2		2
#define	ADS129X_USE_SPI3		3
///////////////////////////////////////////////////////////////////////////////////////////
//芯片及关键设置
#define ADS129X_DEVICE				ADS1298	 	//芯片选择
#define ADS129X_UART_DEBUG		ADS129X_DISABLE 		//1 使用串口调试 0不使用
//#define ADS129X_CASCADE_NUM		1			//设备级联数量
#define ADS129X_USE_CANNLE		8			//1个芯片使用N个通道
#define	ADS129X_USE_DMA				ADS129X_DISABLE			//1开启DMASPI 0不使用DMASPI
////////////////////////////////////////////////////////////////////////////////////////////
//测试数据选项，0 正常采集 1 通道1为0~255递增数据
#define ADS129X_USE_TRIANGLE_TESTDATA 	ADS129X_DISABLE		
////////////////////////////////////////////////////////////////////////////////////////////
//引脚及通讯函数
#define ADS129X_USE_SPI 			ADS129X_USE_SPI3	//SPI选择
#define ADS129X_DRDY_IRQn 		EXTI15_10_IRQn
#define ADS129X_DRDY_PIN			GPIO_PIN_11
#define ADS129X_DRDY					HAL_GPIO_ReadPin(GPIOA,ADS129X_DRDY_PIN)

#define ADS129X_CS(n)					(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET))
#define ADS129X_RST(n)				(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET))
//#define ADS129X_OSC				PBout(1)	
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
		u8 Ads129x_DATA_State;		//数据状态

		u8 Ads129x_DRDY_flag;			//DRDY中断

		u8 Ads129x_Cascade_Num;//芯片级联数量
		u8 Ads129x_Use_Cannle;//一个芯片使用N个通道
		u16 Ads129x_Data_Move; //每次中断搬运N个数据 ADS129X_Data_Move =  Ads129x_USE_Cannle*3
		u16 Ads129x_Write_Num; //搬运N次 做一个包   ADS129x_Write_Num = ADS129x_SAMPLING_POINT / Ads129x_USE_Cannle

		u32 Ads129x_leadoff_flag;//导联脱离标志
		u8 Ads129x_test_flag;		//自增测试数据
		u16 Ads129x_err_time;

		u8 Ads129x_Cache[27];	//129x数据缓冲区
		u8 Ads129x_Databuf[24];	//1299数据缓冲区 

		u32 cannle[8];	//存储原始的数据
		u32 cannle2[8];	//存储原始的数据

		s32	p_Temp[8];	//存储原始的数据
		s32 p_Temp2[8];	//存储原始的数据

}_ADS129x_info;
extern _ADS129x_info ADS129x_info;//芯片结构
extern u8 ADS129x_REG_BUF[ADS129X_REGNUM];  //ads1299寄存器数组
/////////////////////////////////////////////////////////////////////////////////////////////
//串口DEBUG	
#if ADS129X_UART_DEBUG == ADS129X_ENABLE 
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
/////////////////////////////////////////////////////////////////////////////////////////////
	//ADS1298 寄存器编号
	#define	ADS1298_REG_ID	0X00
	#define	ADS1298_REG_CONFIG1	0X01
	#define	ADS1298_REG_CONFIG2	0X02
	#define	ADS1298_REG_CONFIG3	0X03
	#define	ADS1298_REG_LOFF	0X04
	#define	ADS1298_REG_CH1SET	0X05
	#define	ADS1298_REG_CH2SET	0X06
	#define	ADS1298_REG_CH3SET	0X07
	#define	ADS1298_REG_CH4SET	0X08
	#define	ADS1298_REG_CH5SET	0X09
	#define	ADS1298_REG_CH6SET	0X0A
	#define	ADS1298_REG_CH7SET	0X0B
	#define	ADS1298_REG_CH8SET	0X0C	
	#define	ADS1298_REG_RLD_SENSP	0X0D
	#define	ADS1298_REG_RLD_SENSN	0X0E	
	#define	ADS1298_REG_LOFF_SENSP	0X0F
	#define	ADS1298_REG_LOFF_SENSN	0X10	
	#define	ADS1298_REG_LOFF_FLIP	0X11	
	#define	ADS1298_REG_LOFF_STATP	0X12
	#define	ADS1298_REG_LOFF_STATN	0X13	
	#define	ADS1298_REG_GPIO	0X14
	#define	ADS1298_REG_PACE	0X15
	#define	ADS1298_REG_RESP	0X16
	#define	ADS1298_REG_CONFIG4	0X17
	#define	ADS1298_REG_WCT1	0X18
	#define	ADS1298_REG_WCT2	0X19
	
	//ADS1298 采样率
	#define	ADS1298_SPS_16000_LP	0
	#define	ADS1298_SPS_8000_LP	1
	#define	ADS1298_SPS_4000_LP	2
	#define ADS1298_SPS_2000_LP	3
	#define ADS1298_SPS_1000_LP	4
	#define ADS1298_SPS_500_LP	5
	#define ADS1298_SPS_250_LP	6
	#define	ADS1298_SPS_32000_HR	0
	#define	ADS1298_SPS_16000_HR	1
	#define	ADS1298_SPS_8000_HR	2
	#define	ADS1298_SPS_4000_HR	3
	#define ADS1298_SPS_2000_HR	4
	#define ADS1298_SPS_1000_HR	5
	#define ADS1298_SPS_500_HR	6
	
	//ADS1298 输入模式
	#define	ADS1298_SPS_HR	1 //高分辨率模式
	#define	ADS1298_SPS_LP	0 //低功耗模式
	
	//ADS1298 增益
	#define	ADS1298_PGA_6	0
	#define	ADS1298_PGA_1	1
	#define	ADS1298_PGA_2	2
	#define	ADS1298_PGA_3	3
	#define	ADS1298_PGA_4	4
	#define	ADS1298_PGA_8	5
	#define	ADS1298_PGA_12	6
	//ADS1298 RLD通道路由
	#define	ADS1298_RLD_CHANNLE_NONE	0X00
	#define	ADS1298_RLD_CHANNLE_1	0X01
	#define	ADS1298_RLD_CHANNLE_2	0X02
	#define	ADS1298_RLD_CHANNLE_3	0X04
	#define	ADS1298_RLD_CHANNLE_4	0X08
	#define	ADS1298_RLD_CHANNLE_5	0X10
	#define	ADS1298_RLD_CHANNLE_6	0X20
	#define	ADS1298_RLD_CHANNLE_7	0X40
	#define	ADS1298_RLD_CHANNLE_8	0X80
	//ADS1298 输入模式
	#define	ADS1298_CHANNLE_NORMAL_INPUT	0X00
	#define	ADS1298_CHANNLE_SHORT_INPUT	0X01
	#define	ADS1298_BIAS_TEST_SIGNAL	0X05
	//ADS1298 导联脱落检测
	#define	ADS1298_LEADOFF_OFF	0X00
	#define	ADS1298_LEADOFF_ON	0XFF
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//内部函数
__INLINE static void ADS129x_Read_Data(void);
__INLINE static u8 ADS129x_SPI_ReadWriteByte(u8 com);
void static ADS129x_Send_CMD(u8 data);
void static ADS129x_WR_REGS(u8 reg,u8 len,u8 *data);
void static ADS129x_Data_Start(void);//所有芯片 启动数据发送,DRDY引脚开始工作
void static ADS129x_Device_SDATAC(void);//所有芯片 进入SDATAC模式（当前模式 可以对进行寄存器设置）
void static ADS129x_Device_Reset(void);//所有芯片 退出低功耗模式+复位+停止数据发送
void static ADS129x_Write_REGBUFF(void);//所有芯片 将 ADS129x_REG_BUF 数组中的数据写入寄存器
u8 static ADS129x_Check_REGBUFF(void);//所有芯片 回读检查寄存器数据
//外部函数
//基本的初始化和数据启停指令
void ADS129x_GPIO_Init(void);//ADS129x的IO口初始化
u8 ADS129x_Device_Init(void);//所有芯片 首次的寄存器初始化
u8 ADS129x_REG_Init(void);//所有芯片 中途改变寄存器设置
void ADS129x_Goto_Lowpower(void);//所有芯片 休眠进入低功耗模式，DRDY引脚停止
void ADS129x_Start_INT(void) ;//开启中断服务函数
void ADS129x_Stop_INT(void) ;//关闭中断服务函数
s32 get_volt(u32 num);//把3个字节转成有符号32位数
//芯片参数控制指令
u8 ADS129x_HR_Change(u8 cmd);//只针对于 ADS1298//指定芯片 设置芯片工作模式

u8 ADS129x_SPS_Change_ALL(u8 cmd);
u8 ADS129x_SPS_Change(u8 cmd);//指定芯片 设置采样率

u8 ADS129x_PGA_Change(u8 cmd);//指定芯片 所有通道 修改增益
u8 ADS129x_PGA_Change_One(u8 chn,u8 cmd);

u8 ADS129x_MODE_Change(u8 cmd);//指定芯片 所有通道 修改通道模式（内部短路，方波测试，正常数据采集）

u8 ADS129x_RILOUT_Change(u8 cmd);//指定芯片 开启/关闭右腿驱动路由（CMD包含开启和关闭的右腿驱动路由,可以做与运算）

u8 ADS129x_LOFF_Change(u8 cmd);//只针对于 ADS1299 ADS1298//指定芯片 所有通道 开/关 导联脱落检测(阻抗检测)





#endif






