#include "my_protocol.h"
	   

#include "malloc.h"
#include "myQueue.h"
#include "myLinklist.h"

//#include "VTI7064xx.h"
//#include "PSRAMmalloc.h"
//#include "PSRAMQueue.h"
//#include "MY8266wifi.h"

//#include "sd_card.h"

#include "ADS129x.h"
#include "usart.h"     //串口 


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/10/06
//版本：V1.7
//
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



_my_Portocol_Info *my_Portocol_Info=NULL;


//u8 set_write_buf_BIN(_my_Portocol_Info *Portocol_Info)	//设置TF卡写缓冲
//{		
//		u8 i;

//		TF_info->FIL_Write_Num=0;//写数据帧计数
//		TF_info->FIL_Save_Num = 16*512 ;//写N帧存储一次
//	
////////////////////////////////////////////////////////////////////////////////////////////////		
//		Main_printf("TF数据帧计数 %d\r\n",TF_info->FIL_Write_Num);
//		Main_printf("TF写 %d 帧数据存储一次\r\n",TF_info->FIL_Save_Num);	
//			
//			
//		return 0;
//}

////u8 set_write_buf_BIN(_my_Portocol_Info *Portocol_Info)	//设置TF卡写缓冲
////{		
////		u8 i;

////		TF_info->FIL_Write_Num=0;	//写数据帧计数
////		TF_info->Write_Num=0;			//写计次
////		TF_info->FIL_Package_Num = 4080/Portocol_Info-> Queue_length +1 ;//写N次为一帧
////	
////		TF_info->FIL_Save_Num = 4*256 ;//写N帧存储一次	
//////////////////////////////////////////////////////////////////////////////////////////////////
////		//存bin文件
////		TF_info->FIL_Head[0]=0xa5;
////		TF_info->FIL_Head[1]=0xa5;
////		TF_info->FIL_Head[2]=TF_info->FIL_Write_Num>>24;
////		TF_info->FIL_Head[3]=TF_info->FIL_Write_Num>>16;
////		TF_info->FIL_Head[4]=TF_info->FIL_Write_Num>>8;
////		TF_info->FIL_Head[5]=TF_info->FIL_Write_Num>>0;	
////		for(i=6;i<=12;i++)
////		{
////				TF_info->FIL_Head[i]=0XFF;
////		}
////		TF_info->FIL_Tail[0]=TF_info->FIL_Check;//校验
////		TF_info->FIL_Tail[1]=0x5a;//尾
////		TF_info->FIL_Tail[2]=0x5a;//尾
//////////////////////////////////////////////////////////////////////////////////////////////////		
////		Main_printf("TF写记次 %d\r\n",TF_info->Write_Num);
////		Main_printf("TF数据帧计数 %d\r\n",TF_info->FIL_Write_Num);
////		Main_printf("TF写 %d 次数据为一帧\r\n",TF_info->FIL_Package_Num);	
////		Main_printf("TF写 %d 帧数据存储一次\r\n",TF_info->FIL_Save_Num);	
////			
////		return 0;
////}

//u8 set_write_buf_TXT(void)	//设置TF卡写缓冲
//{	
//		u8 i;		
//		
//		TF_info->FIL_Write_Num=0;			//写数据帧计数
//		TF_info->Write_Num=0;					//写数据计数
//		TF_info->FIL_Package_Num = 1 ;//写N次为一帧
//	
//		TF_info->FIL_Save_Num = 4*256 ;//写N帧存储一次	
////////////////////////////////////////////////////////////////////////////////////////////////	
//		//存TXT文件
//		//+8838606	-0000001	+0000003						26 +\r\n	= 28	//3通道
//		//+8838606	-0000001	+0000003	+0000003	35 +\r\n	= 37	//4通道
//		//BUF1 37*110=4070 0-4069有效数据 空余剩余26个字符
////		for(i=0;i<ADS129x_info.Ads129x_Write_Num;i++)//关键位置填充TAB 及 换行回车
////		{	
////				*(TF_info->Data_buf+8 + i*SD_ONEWRITE_LEN)= 0x09;  //TAB
////				*(TF_info->Data_buf+17 + i*SD_ONEWRITE_LEN)= 0x09;  //TAB			
////				*(TF_info->Data_buf+26 + i*SD_ONEWRITE_LEN)= 0x09;  //TAB
////				*(TF_info->Data_buf+35 + i*SD_ONEWRITE_LEN)= 0x0D;	//\r
////				*(TF_info->Data_buf+36 + i*SD_ONEWRITE_LEN)= 0x0A;	//\n
////		}		
////////////////////////////////////////////////////////////////////////////////////////////////	
//		Main_printf("TF写记次 %d\r\n",TF_info->Write_Num);
//		Main_printf("TF数据帧计数 %d\r\n",TF_info->FIL_Write_Num);
//		Main_printf("TF写 %d 次数据为一帧\r\n",TF_info->FIL_Package_Num);	
//		Main_printf("TF写 %d 帧数据存储一次\r\n",TF_info->FIL_Save_Num);	
//		
//		return 0;
//}
//		
		
//通讯协议 初始化 / 切换
//返回值 0 正常 其他异常
u8 My_proto_Init(_my_Portocol_Info *Portocol_Info)
{
		u8 res;
		if(Portocol_Info == NULL)
		{
				Portocol_Info = (_my_Portocol_Info*)mymalloc(SRAMIN,sizeof(_my_Portocol_Info));//数据协议相关
				if(Portocol_Info == NULL)
				{
						Main_printf("Portocol_Info 结构 内存申请失败\r\n");
						return 1;
				}
				else
				{
						Main_printf("Portocol_Info 结构 内存申请成功\r\n");
						Portocol_Info->sendbuf=NULL;
						Portocol_Info->Queue=NULL;
				}	
		}
		Portocol_Info-> Send_Time=0;
		Portocol_Info-> Data_Number=0;
//////////////////////////////////////////////////////////////////////////////////////////////////		
		myfree(SRAMIN,Portocol_Info->sendbuf);//释放内存
		res = queue_Deinit(Portocol_Info->Queue);//循环队列注销
		if(res == QUEUE_RETURN_OK)
		{
				Main_printf("释放后 系统SRAM占用 %d %%\r\n",mem_perused(SRAMIN));
		}
		else
		{
				Main_printf("队列 释放异常%d，如设备正常可以忽略\r\n",res);	
		}		
//////////////////////////////////////////////////////////////////////////////////////////////////
		if(work_state==SEND_BULE)
		{
				if(Portocol_Info->Portocol == Use_NIMINGV7_Proto)
				{
						Portocol_Info-> Queue_push_time = 10;		//N次采样一帧
						//Portocol_Info-> Data_real_length = 34;	//一帧数据有效数据长度匿名 32+2 导联脱落
						Portocol_Info-> Data_real_length = 32;	//一帧数据有效数据长度匿名 32 字节
				
						Portocol_Info-> Data_all_length = Portocol_Info-> Data_real_length + 4 + 2; //一帧数据长度 
						Portocol_Info-> Queue_length = (ADS129x_info.Ads129x_Data_Move)* Portocol_Info-> Queue_push_time ; //队列容量  缓存 N 次采样
						Portocol_Info-> Queue_size = 40;			//队列大小
						Portocol_Info->Send_length = Portocol_Info->Data_all_length * Portocol_Info-> Queue_push_time	; // 一帧数据总长度
				}
				else 	if(Portocol_Info->Portocol == Use_User_Proto)
				{
						Portocol_Info-> Queue_push_time = 10	;	//N次采样为一帧数据  12*10=120
						Portocol_Info->Send_length =266; // 一帧数据总长度
						Portocol_Info-> Data_all_length =266; //一帧数据长度 

						Portocol_Info-> Data_real_length =0;	//一帧数据有效数据长度 
						Portocol_Info-> Queue_length = (ADS129x_info.Ads129x_Data_Move)* Portocol_Info-> Queue_push_time ; //队列容量  缓存 N 次采样
						Portocol_Info-> Queue_size = 50;			//队列大小
				}
		}
		

		Main_printf("Portocol %d\r\n",Portocol_Info->Portocol);
		Main_printf("Queue_push_time %d\r\n",Portocol_Info->Queue_push_time);
		Main_printf("Queue_real_length %d\r\n",Portocol_Info->Data_real_length);
		Main_printf("Data_all_length %d\r\n",Portocol_Info->Data_all_length);
		Main_printf("Send_length %d\r\n",Portocol_Info->Send_length);
		Main_printf("Queue_length %d\r\n",Portocol_Info->Queue_length);
		Main_printf("Queue_size %d\r\n",Portocol_Info->Queue_size);
		
		
		Portocol_Info->sendbuf = (u8*)mymalloc(SRAMIN,sizeof(u8)*Portocol_Info->Send_length);	//发送缓冲区
		if(Portocol_Info->sendbuf == NULL )
		{
				Main_printf("Portocol_Info->sendbuf 内存申请失败\r\n");
				return 2;
		}
		else
		{
				Main_printf("Portocol_Info->sendbuf 内存申请成功\r\n");
		}		
//////////////////////////////////////////////////////////////////////////////////////////////////	
		Portocol_Info->Queue =  queue_init(Portocol_Info-> Queue_size,Portocol_Info-> Queue_length) ;//循环队列初始化
		if(Portocol_Info->Queue == NULL )
		{
				Main_printf("Portocol_Info队列 内存申请失败\r\n");
				return 3;
		}
		else
		{
				Main_printf("Portocol_Info队列  内存申请成功\r\n");
		}
		Main_printf("系统SRAM占用 %d %%\r\n",mem_perused(SRAMIN));
		
		return 0;
}






