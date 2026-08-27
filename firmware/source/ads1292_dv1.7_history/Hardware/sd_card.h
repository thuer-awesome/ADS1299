#ifndef _MMC_SD_H_
#define _MMC_SD_H_		 
#include "sys.h"	 
#include "myQueue.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	TF_USE_SPI1		1
#define	TF_USE_SPI2		2
#define	TF_USE_SPI3		3
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//功能选择
#define TF_UART_DEBUG	0 	//串口调试
//#define TF_EN_SPI_DMA  	0  //如果使能SPIDMA	
#define TF_EN_SPI_Tx_DMA  0  //如果使能SPI发送的DMA	
#define TF_EN_SPI_Rx_DMA  0  //如果使能SPI接收的DMA	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//引脚及通讯函数
#define TF_USE_SPI 		TF_USE_SPI3	//SPI选择
//与外部的接口
//这部分应根据具体的连线来修改!
//#define	SD_CD 		PAin(0)  //SD卡插入监测
#define SD_CS(n)					(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET))
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//经测试，小包频繁写入比大包耗时要短很多，相同内存的队列，大包容易造成队列溢出时小包则很稳定
//capacity  队列容量 （队列缓冲区数量） 
//length		队列长度/大小 （缓存的数据长度）
//2通道68次采样一包，408/6=68
#define SD_QUEUE_SIZE	12 			//队列容量
#define SD_QUEUE_LENGTH	408		//队列大小
__align(4) typedef struct
{
		u8 Write_Num;				//写数据计数
		u16 FIL_Package_Num;//写数据计数，写N次为一帧

		u32 FIL_Write_Num;	//写数据帧计数
		u32 FIL_Save_Num;		//写数据帧计数，N帧数据暂存一次
	
		u8 FIL_Head[12];//头
		u8 FIL_Tail[3];//尾
		u8 FIL_Check;//校验	
	
		u8* Data_buf;//数据缓存区
		QueueInfo *TF_Queue;//TF数据缓冲区指针
}_TF_info;	//TF卡相关	
extern _TF_info *TF_info;
extern u8  SD_Type;//SD卡的类型
/////////////////////////////////////////////////////////////////////////////////////////////
// SD卡类型定义  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD卡指令表  	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
/////////////////////////////////////////////////////////////////////////////////////////////				
/////////////////////////////////////////////////////////////////////////////////////////////



//函数申明区 
u8 SD_SPI_ReadWriteByte(u8 data);
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
u8 SD_WaitReady(void);							//等待SD卡准备
u8 SD_GetResponse(u8 Response);					//获得相应
u8 SD_Initialize(void);							//初始化
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);		//读块
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);		//写块
u32 SD_GetSectorCount(void);   					//读扇区数
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD
 
 
void SD_Read_Sectorx(u32 sec);
void SD_GPIO_Init(void);

u8 SD_Initialize(void);							//初始化
#endif




