#ifndef __MY_PROTOCOL_H
#define __MY_PROTOCOL_H	 
#include "sys.h"
#include "myQueue.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/10/06
//版本：V1.7
//
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
typedef enum
{
	Use_NIMINGV7_Proto = 0x00,
	Use_User_Proto = 0x01,
	Use_OPENBCI_Proto = 0x02	
}MY_protocol;
//////////////////////////////////////////////////////////////////////////////
//默认通讯协议选择
#define DEFAULT_PORTO		Use_User_Proto
//////////////////////////////////////////////////////////////////////////

__align(4) typedef struct {
		
		//u16 reclength; 	
		u8 *sendbuf;				//发送缓冲区
		//u8 *recbuf;					//接收缓冲区
	

		u8 	Portocol; //通讯协议选择
		u16 Send_length			;//发送缓冲区  4+2+2=8   *20 =160
		u16 Queue_push_time	;//N次采样为一帧数据
		u16 Queue_length		;//队列容量 缓冲区有效数据长度	10*10次采样
		u16 Queue_size			;//队列大小
		u16 Data_real_length;//一次采样有效数据长度
		u16 Data_all_length;
	
		u16	Send_Time; 
		u16	DATA_STATE;
		u32	Data_Number;

		QueueInfo *Queue;//队列缓冲区指针
}_my_Portocol_Info;

extern _my_Portocol_Info	*my_Portocol_Info;
//////////////////////////////////////////////////////////////////////////////

	u8 set_write_buf_TXT(void);//设置SD卡写缓冲
	u8 set_write_buf_BIN(_my_Portocol_Info *Portocol_Info);	//设置TF卡写缓冲

u8 My_proto_Init(_my_Portocol_Info *Portocol_Info);
 				    
#endif
