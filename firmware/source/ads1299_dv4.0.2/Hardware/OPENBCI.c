#include "OPENBCI.h"
#include "usart.h"
#include "delay.h"       //延时
#include "string.h"
#include "malloc.h"
#include "Timer.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2021/08/05
//版本：V1.0
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//变量区
_OPENBCI_Info *OPENBCI_Info ; //数据结构


static u8 OPENBCI_8Channel_VERSION[] ={"OpenBCI V3 8-16 channel\nOn Board ADS1299 Device ID: 0x3E\nLIS3DH Device ID: 0x33\nFirmware: v3.1.1\n$$$"};
static u8 OPENBCI_16Channel_VERSION[] ={"OpenBCI V3 8-16 channel\nOn Board ADS1299 Device ID: 0x3E\nOn Daisy ADS1299 Device ID 0x3E\nLIS3DH Device ID: 0x33\nFirmware: v3.1.1\n$$$"};

static u8 OPENBCI_Success_default[] ={"Success: default$$$"};

static u8 OPENBCI_Conncet_Channel_1[] ={"Success: Channel set for 1$$$"};
static u8 OPENBCI_Conncet_Channel_2[] ={"Success: Channel set for 2$$$"};
static u8 OPENBCI_Conncet_Channel_3[] ={"Success: Channel set for 3$$$"};
static u8 OPENBCI_Conncet_Channel_4[] ={"Success: Channel set for 4$$$"};
static u8 OPENBCI_Conncet_Channel_5[] ={"Success: Channel set for 5$$$"};
static u8 OPENBCI_Conncet_Channel_6[] ={"Success: Channel set for 6$$$"};
static u8 OPENBCI_Conncet_Channel_7[] ={"Success: Channel set for 7$$$"};
static u8 OPENBCI_Conncet_Channel_8[] ={"Success: Channel set for 8$$$"};
static u8 OPENBCI_Conncet_Channel_9[] ={"Success: Channel set for 9$$$"};
static u8 OPENBCI_Conncet_Channel_10[] ={"Success: Channel set for 10$$$"};
static u8 OPENBCI_Conncet_Channel_11[] ={"Success: Channel set for 11$$$"};
static u8 OPENBCI_Conncet_Channel_12[] ={"Success: Channel set for 12$$$"};
static u8 OPENBCI_Conncet_Channel_13[] ={"Success: Channel set for 13$$$"};
static u8 OPENBCI_Conncet_Channel_14[] ={"Success: Channel set for 14$$$"};
static u8 OPENBCI_Conncet_Channel_15[] ={"Success: Channel set for 15$$$"};
static u8 OPENBCI_Conncet_Channel_16[] ={"Success: Channel set for 16$$$"};

static u8 OPENBCI_START[32] ={0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0,0X00,0X00,0XC0

,0X00,0X00,0X00,0X20,0XFE,0XE0,0X0F,0X60,0XC0};

u8 OPENBCI_Info_Init()
{
		OPENBCI_Info =(_OPENBCI_Info*)mymalloc(sizeof(_OPENBCI_Info)); //数据结构
	
		OPENBCI_Info->Connect_Flag=0;
	
//		OPENBCI_Info->sendbuf = (u8*)mymalloc(OPENBCI_SEND_LENGTH);	//发送缓冲区 
//		OPENBCI_Info->recbuf = (u8*)mymalloc(OPENBCI_REC_LENGTH);	//发送缓冲区 
//		if(OPENBCI_Info == NULL ||  OPENBCI_Info->sendbuf == NULL || OPENBCI_Info->recbuf== NULL)
//		{
//				OPENBCI_printf("OPENBCI内存申请失败\r\n");	
//				myfree(OPENBCI_Info->sendbuf);//释放内存
//				myfree(OPENBCI_Info->recbuf);//释放内存
//				myfree(OPENBCI_Info);//释放内存
//				return 1; 
//		}
//		else
//		{
//				OPENBCI_printf("内存申请成功，当前占用 %d\r\n",mem_perused());
//				return 0; 
//		}
}

void OPENBCI_Info_DInit()
{
//		myfree(OPENBCI_Info->sendbuf);//释放内存
//		myfree(OPENBCI_Info->recbuf);//释放内存
		myfree(OPENBCI_Info);//释放内存
}


u8 OPENBCI_Connect_Init()
{
		u8 res,i;
	
		OPENBCI_Info_Init();
	
		TIM2_Init(10,7200);		//串口数据接收
		TIM4_Init(1000,7200);//100ms 定时器		
	
		while(1)
		{
				if(serch_queue_data(OPENBCI_UART_INFO->USART_Queue)) //数据接收
				{
						OPENBCI_UART_INFO->reclength = *(*(OPENBCI_UART_INFO->USART_Queue->databuf + OPENBCI_UART_INFO->USART_Queue->front)+0);
						if(queue_pop(OPENBCI_UART_INFO->USART_Queue,OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength))//从队列搬运 n个字节 至发送缓冲区
						{
									for(i=0;i<OPENBCI_UART_INFO->reclength;i++)
									{
												Main_printf("%02x ",*(OPENBCI_UART_INFO->recbuf+i));
									}
									Main_printf("\r\n");
//////////////////////////////////////////////////////////////////////////////////////////////////////
								//起始数据 s v
								if(OPENBCI_Info->Connect_Flag==0 && OPENBCI_UART_INFO->reclength == 1 ) 
								{
										if(OPENBCI_UART_INFO->recbuf[0]=='s')
										{
												OPENBCI_Info->Connect_Flag=1;
												TIM4_Timing=0;
										}
								}
								else if(OPENBCI_Info->Connect_Flag==1 && OPENBCI_UART_INFO->reclength == 1) 
								{
										if(OPENBCI_UART_INFO->recbuf[0]=='v')
										{
												OPENBCI_Info->Connect_Flag=2; //软件点击了开始
											#if OPENBCI_USE_CANNLE==OPENBCI_USE_8_CANNLE
												OPENBCI_Send_STR(OPENBCI_8Channel_VERSION);//发送版本号
											#else
												OPENBCI_Send_STR(OPENBCI_16Channel_VERSION);//发送版本号
											#endif
												TIM4_Timing=0;
										}
								}
//////////////////////////////////////////////////////////////////////////////////////////////////////								
								else 	if(OPENBCI_Info->Connect_Flag==2 &&  OPENBCI_UART_INFO->reclength == 2) 
								{
										if(OPENBCI_UART_INFO->recbuf[0]==0x2f && OPENBCI_UART_INFO->recbuf[1]==0x30)  // "/0"
										{
												OPENBCI_Info->Connect_Flag=3;
												OPENBCI_Send_STR(OPENBCI_Success_default);//default
												TIM4_Timing=0;
										}
								}
//////////////////////////////////////////////////////////////////////////////////////////////////////
								else 	if(OPENBCI_Info->Connect_Flag==3 && OPENBCI_UART_INFO->reclength == 9 ) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_1_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=4;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_1);
												TIM4_Timing=0;
										}
								}
								else 	if(OPENBCI_Info->Connect_Flag==4 && OPENBCI_UART_INFO->reclength == 9 ) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_2_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=5;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_2);
												TIM4_Timing=0;
										}
								}
								else 	if(OPENBCI_Info->Connect_Flag==5 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_3_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=6;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_3);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==6 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_4_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=7;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_4);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==7 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_5_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=8;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_5);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==8 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_6_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=9;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_6);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==9 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_7_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=10;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_7);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==10 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_8_SERCH);//查询字符串
										if(res==0)
										{
											#if OPENBCI_USE_CANNLE==OPENBCI_USE_8_CANNLE
												OPENBCI_Info->Connect_Flag=19; //19表示通讯完成，等待开始采集指令
											#else
												OPENBCI_Info->Connect_Flag=11;
											#endif
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_8);
												TIM4_Timing=0;
										}
								}
							#if OPENBCI_USE_CANNLE==OPENBCI_USE_16_CANNLE
								else 	if(OPENBCI_Info->Connect_Flag==11 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_9_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=12;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_9);
												TIM4_Timing=0;
										}
								}
								else 	if(OPENBCI_Info->Connect_Flag==12 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_10_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=13;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_10);
												TIM4_Timing=0;
										}
								}								
								else 	if(OPENBCI_Info->Connect_Flag==13 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_11_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=14;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_11);
												TIM4_Timing=0;
										}
								}		
								else 	if(OPENBCI_Info->Connect_Flag==14 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_12_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=15;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_12);
												TIM4_Timing=0;
										}
								}		
								else 	if(OPENBCI_Info->Connect_Flag==15 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_13_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=16;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_13);
												TIM4_Timing=0;
										}
								}		
								else 	if(OPENBCI_Info->Connect_Flag==16 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_14_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=17;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_14);
												TIM4_Timing=0;
										}
								}		
								else 	if(OPENBCI_Info->Connect_Flag==17 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_15_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=18;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_15);
												TIM4_Timing=0;
										}
								}	
								else 	if(OPENBCI_Info->Connect_Flag==18 && OPENBCI_UART_INFO->reclength == 9) 
								{
										res = Str_search(OPENBCI_UART_INFO->recbuf,OPENBCI_UART_INFO->reclength,OPENBCI_CANNEL_16_SERCH);//查询字符串
										if(res==0)
										{
												OPENBCI_Info->Connect_Flag=19;
												OPENBCI_Send_STR(OPENBCI_Conncet_Channel_16);
												TIM4_Timing=0;
												TIM4_Init(5000,7200);//500ms 系统指示灯
										}
								}									
							#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////
								//通讯完成，等待开始采集指令
								if(OPENBCI_UART_INFO->reclength == 1 && OPENBCI_Info->Connect_Flag==19) 
								{
										if(OPENBCI_UART_INFO->recbuf[0]=='b') //开始采集
										{
												OPENBCI_Info->Connect_Flag=20;
												USARTx_Send(OPENBCI_UART,OPENBCI_START, 32);
												break;
										}
								}
						}
				}
//////////////////////////////////////////////////////////////////////////////////////////////////////
				if(TIM4_Timing==20)//2s无数据,重置
				{
						TIM4_Timing=0;
						//Main_printf("等待 %d\r\n",OPENBCI_Info->Connect_Flag);
						if(OPENBCI_Info->Connect_Flag < 19)
						{
								OPENBCI_Info->Connect_Flag=0;
						}
				}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
		//初始化完成
		TIM_Cmd(TIM2, DISABLE); 
		TIM4_Init(10000,7200);//1000ms 系统指示灯
		OPENBCI_Info_DInit();
		
		USART_ITConfig(OPENBCI_UART, USART_IT_RXNE, DISABLE); 			//串口2接收中断 

}
