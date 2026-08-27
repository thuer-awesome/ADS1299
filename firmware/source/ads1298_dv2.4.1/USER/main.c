#include "sys.h"         //系统配置
#include "delay.h"       //延时
#include "malloc.h"
#include "usart.h"
#include "string.h"
#include "timer.h"
#include "adc.h"
#include "nvic.h"

#include "dma.h"
#include "wkup.h"

#include "led.h"
#include "key.h"

#include "BG22AX.h"
#include "ADS129x.h"

#include "myQueue.h"
#include "myLinklist.h"
#include "my_protocol.h"

//#include "MYFATFS.h"	 
//#include "sd_card.h"
//#include "ff.h"
//#include "exfuns.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:
//版本：
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
//		功耗测试	ADS1298 DV2.4.1 8通道蓝牙
//		功耗实测
//		主频16M 蓝牙发送数据	，总功耗 10mA		
/////////////////////////////////////////////////////////////////////////////////////

/*
功能说明：蓝牙发送：
					开机初始化完成后，进入3s的蓝牙配置模式，此模式下长按按键3s以上，会按照程序的
					参数自动配置蓝牙，如果3s内不操作按键，则跳过此步骤，进入下一步等待蓝牙连接
					（配置一次即可，发货前已经配置过了，无需用户再次操作，当修改蓝牙AT指令后，需要重新配置）
					
					等待蓝牙连接：当60s无蓝牙连接后，设备进入低功耗待机模式，蓝牙成功连接后，
					进入蓝牙发送模式，按照通讯协议发送数据，期间按键功能为预留，用户可以自行添加功能（如打标记等）
*/



/////////////////////////////////////////////////////////////////////////////////////////////
//函数区
void Sys_Event(void);//系统事件处理
void Goto_Low_Power(void);
void STOP_DATA(void);
void START_DATA(void);
void BAT_Percent_Show(void) ;

void Send_BlueTooth(void);//蓝牙 发送

void CMD_rec_EVENT(my_LinkList_Inf  *LinkList_cache) ;
/////////////////////////////////////////////////////////////////////////////////////////////



//main	
int main(void)
{			
		u16 i;
	
		work_state=SEND_BULE;		//蓝牙发送模式
//////////////////////////////////////////////////////////////////////////////////////////////	
    HAL_Init();	
	#if  LOWPOWER==1
		delay_init(16); 			//初始化延时函数    系统时钟
		SystemClock_Config_16();
	#else
		delay_init(80); 			//初始化延时函数    系统时钟
		SystemClock_Config_80();//主频 
	#endif			
		delay_ms(100);
		mem_init(SRAMIN);
////////////////////////////////////////////////////////////////////////////////////////////////
		MYDMA_ADC1_Config();//初始化DMA	
		//MYDMA_UART1_Config(); //初始化DMA
		MYDMA_UART2_Config(); //初始化DMA	
////////////////////////////////////////////////////////////////////////////////////////////////
		uart1_init(115200);		//初始化	串口
		LED_GPIO_Init(); 			//初始化	LED IO
		KEY_Init();						//初始化	按键 IO
		BG22AX_GPIO_Init();		//初始化	蓝牙 IO	
		ADS129x_GPIO_Init();		//初始化	ADS129X IO	
		MY_ADC_Init();				//初始化	ADC				
////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("红茶电子 https://shop119364888.taobao.com\r\n");
		Main_printf("程序版本 STM32L432 V1.1.2 日期 2023-06-03 配套 ADS1298 DV2.4.1\r\n");			
	#if  LOWPOWER==1
		Main_printf("低功耗模式\r\n");	
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////	
		LED1_ON;	
	#if USE_LED2 == 1
		LED2_ON;	
	#endif
		delay_s(2);  
		LED1_OFF;	
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////		
		UART2_LinkList =  LinkList_Init(SRAMIN);		//用于接收不定长数据
		my_Portocol_Info = (_my_Portocol_Info*)mymalloc(SRAMIN,sizeof(_my_Portocol_Info));//数据协议相关
		if(my_Portocol_Info == NULL || UART2_LinkList == NULL)
		{
				Main_printf("基本结构及链表内存申请失败\r\n");
		}
		else
		{
				Main_printf("基本结构及链表内存申请成功\r\n");
				my_Portocol_Info->sendbuf=NULL;
				my_Portocol_Info->Queue=NULL;
		}	
		my_Portocol_Info->Portocol = DEFAULT_PORTO;//通讯协议选择，默认
		Main_printf("SRAMIN内存当前占用 %d\r\n",mem_perused(SRAMIN));
////////////////////////////////////////////////////////////////////////////////////////////////		
	#if  LOWPOWER==1//修改系统时钟为8M，降低功耗	
		TIM1_Init(500,1600);		//指示灯
	#else
		TIM1_Init(500,8000);		//指示灯
	#endif		
		ADS129x_Device_Init();//所有芯片 首次的寄存器初始化
		delay_ms(1000);
////////////////////////////////////////////////////////////////////////////////////////////////
	#if  LOWPOWER==1
		TIM1_Init(10000,1600);		//指示灯
		TIM2_Init(2000,1600); 		//ADC  
		TIM6_Init(10,1600);	 			//1ms 串口事件检测
		TIM7_Init(KEYDOWNLONGTIME,1600);	//按键检测
	#else
		TIM1_Init(10000,8000);		//指示灯
		TIM2_Init(2000,8000); 		//ADC  
		TIM6_Init(10,8000);	 			//1ms 串口事件检测
		TIM7_Init(KEYDOWNLONGTIME,8000);	//按键检测	
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////
		while(1)//数据发送测试
		{
				if(work_state==SEND_BULE)
						Send_BlueTooth();		
		}		
}



void Send_BlueTooth(void)//蓝牙 发送
{
		u16 i,j,z,res;
		u8 addcheck=0,sumcheck=0;	
		u16 check;
		
		Main_printf("蓝牙发送\r\n");
////////////////////////////////////////////////////////////////////////////////////////////////		
#if 1 //蓝牙手动设置，等待期间长按按键进入，也可以通过AT指令设置（前提是成功连接了蓝牙接收器）
		Main_printf("蓝牙设置检测\r\n");
		BULE_Info.state=1;
		LED1_OFF;
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif		
	#if  LOWPOWER==1
		TIM1_Init(3000,1600);	
	#else
		TIM1_Init(3000,8000);	
	#endif		
		TIM1_Timing=0;
		BG22AX_SET_FLAG=0;
		while(BG22AX_SET_FLAG==0 && TIM1_Timing<10)	//等待3s
				BG22AX_SET();//检测按键
		if(BG22AX_SET_FLAG)
		{
				Main_printf("重设蓝牙\r\n");		
				res=BG22AX_AT_Confg();//开始设置		
				if(res)		
						Main_printf("蓝牙设置失败\r\n");							
				else									
						Main_printf("蓝牙设置成功\r\n");		
		}
		else
		{
				Main_printf("跳过设置\r\n");
		}
	#if  LOWPOWER==1
		TIM1_Init(20000,1600);
	#else
		TIM1_Init(20000,8000);
	#endif
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("初始化蓝牙\r\n");
		HAL_TIM_Base_Stop_IT(&TIM6_Handler);	//关串口数据接收
		BG22AX_Init();
////////////////////////////////////////////////////////////////////////////////////////////////
#if 1	//等待蓝牙连接	
		LED1_ON;
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif		
		BULE_Info.state=0;
	#if  LOWPOWER==1
		TIM1_Init(1000,1600);
	#else
		TIM1_Init(1000,8000);
	#endif	
		Main_printf("等待蓝牙连接\r\n");
		i=0;
		while(work_state==SEND_BULE)
		{
//				Key_Scan();	
//				if(KEY_TYPE)
//				{
//						KEY_TYPE=0;
//						POWER_OFF;
//				}	
				if(i>=6000) //60s 自动关机
				{
						KEY_TYPE=0;
						Main_printf("自动关机\r\n");
						Goto_Low_Power();
						while(1);
				}	
				if(i%200==0)
				{
						if(BG22AX_Wait_Connect()==0)
						{
								Main_printf("蓝牙成功连接\r\n");
								break;
						}
						else
						{
								Main_printf("等待蓝牙连接 ");
						}
				}	
				delay_ms(10);
				i++;
		}	
	#if  LOWPOWER==1
		TIM1_Init(10000,1600);	
	#else
		TIM1_Init(10000,8000);	
	#endif
#endif
		BULE_Info.state=1;
	  LED1_OFF;  
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////		
		Main_printf("初始化完成开始发送数据\r\n");
		ADS129x_REG_Init();//所有芯片 中途改变寄存器设置		
		delay_s(1);
		START_DATA();//准备好发送数据		
////////////////////////////////////////////////////////////////////////////////////////////////			
		while(work_state==SEND_BULE)
		{
//				Key_Scan();
//				if(KEY_TYPE)
//				{
//						KEY_TYPE=0;
//						POWER_OFF;
//				}
////////////////////////////////////////////////////////////////////////////////////////////////			
				Sys_Event(); //事件检测		
////////////////////////////////////////////////////////////////////////////////////////////////		
//				if(	ADXL345_info.State==1)
//				{
//						if(ADXL345_INT1==0) //低电平有效
//						{
//								ADXL345_Read_Data();	
//						}
//				}				
////////////////////////////////////////////////////////////////////////////////////////////////				
				if(my_Portocol_Info->Queue->Queue_Full_flag==1)//队列溢出
				{
						my_Portocol_Info->Queue->Queue_Full_flag=0;
						Main_printf(".");
				}		
////////////////////////////////////////////////////////////////////////////////////////////////				
				if(my_Portocol_Info->Queue->Queue_pop_flag==1)	//队列数据取出标志
				{
						if(UART2_DMA_Finish==DMA_CANNLE_FREE) //DMA空闲
						{
								UART2_DMA_Finish=DMA_CANNLE_BUSY;
								my_Portocol_Info->Queue->Queue_pop_flag=0;
								HAL_UART_Transmit_DMA(&UART2_Handler, (u8*)my_Portocol_Info->sendbuf,my_Portocol_Info->Send_length); //启动传输
						}
				}
				else if(( serch_queue_data(my_Portocol_Info->Queue) == QUEUE_RETURN_OK ))//队列有数据
				{
						if(my_Portocol_Info->Portocol == Use_NIMINGV7_Proto)//适配匿名V7上位机协议
						{
								for(i=0;i<my_Portocol_Info-> Queue_push_time;i++) //一帧N次采样的数据
								{
										//4（帧头）+（有效数据）+2（校验）	
										my_Portocol_Info->sendbuf[0 +i*my_Portocol_Info-> Data_all_length]=0xAA;
										my_Portocol_Info->sendbuf[1 +i*my_Portocol_Info-> Data_all_length]=0xFF;
										my_Portocol_Info->sendbuf[2 +i*my_Portocol_Info-> Data_all_length]=0xF1;	
										my_Portocol_Info->sendbuf[3 +i*my_Portocol_Info-> Data_all_length]=my_Portocol_Info-> Data_real_length;
										//获取原始数据
										for(z=0;z<ADS129X_USE_CANNLE;z++)//通道数
										{
												ADS129x_info.cannle[z] =	
																					*(*(my_Portocol_Info->Queue->databuf + my_Portocol_Info->Queue->front)+0+ i*24+ z*3) <<16 //获取原始数据
																				|	*(*(my_Portocol_Info->Queue->databuf + my_Portocol_Info->Queue->front)+1+ i*24+ z*3) <<8 
																				| *(*(my_Portocol_Info->Queue->databuf + my_Portocol_Info->Queue->front)+2+ i*24+ z*3) ;
												ADS129x_info.p_Temp[z] = get_volt(ADS129x_info.cannle[z]);	//转成有符号数
												ADS129x_info.cannle[z] = ADS129x_info.p_Temp[z];//赋值给无符号数，准备做逻辑右移
												//小端在前
												my_Portocol_Info->sendbuf[4 +z*4 +my_Portocol_Info-> Data_all_length*i] = ADS129x_info.cannle[z] >>0;
												my_Portocol_Info->sendbuf[5 +z*4 +my_Portocol_Info-> Data_all_length*i] = ADS129x_info.cannle[z] >>8;
												my_Portocol_Info->sendbuf[6 +z*4 +my_Portocol_Info-> Data_all_length*i] = ADS129x_info.cannle[z] >>16;
												my_Portocol_Info->sendbuf[7 +z*4 +my_Portocol_Info-> Data_all_length*i] = ADS129x_info.cannle[z] >>24;
										}
										
										my_Portocol_Info->sendbuf[34 +i*my_Portocol_Info-> Data_all_length] = ADS129x_info.Ads129x_leadoff_flag;
										my_Portocol_Info->sendbuf[35 +i*my_Portocol_Info-> Data_all_length] = ADS129x_info.Ads129x_leadoff_flag>>8;

										//校验位
										sumcheck = 0;addcheck=0;
										for(j=0;j< 36;j++)
										{
													sumcheck += my_Portocol_Info->sendbuf[j+i*my_Portocol_Info-> Data_all_length];	
													addcheck += sumcheck;					
										}	
										my_Portocol_Info->sendbuf[36+i*my_Portocol_Info-> Data_all_length] = sumcheck;	//校验		
										my_Portocol_Info->sendbuf[37+i*my_Portocol_Info-> Data_all_length] = addcheck;	//校验		
								}
						}
						//用户自定义协议
						//帧头[0:1]【2】							A5 A5		
						//设备号[2]【1】				 01 【预留一对多设备连接的功能】
						//功能位[3]【1】				0x05【0x05正常连续采样/0x06阻抗检测/0x07测试信号】
						//包序号[4:7]【4】						00000001包序号自增	高位在前
						//通道数[8]【1】				0x08（8）【32  16  8  4  2  1】
						//当前电量[9]【1】						0~100%
						//信号强度[10]【1】						FF	（备用）
						//当前采样率[11]【1】		0x06【0 32K /1 16K /2 8K /3 4K /4 2K /5 1000 /6 500 /7 250】
						//当前增益[12]【1】	    	0x03【00 1 01 2 02 4 03 6 04 8 05 12 06 24】
						//预留字节[13]【1】						FF	（备用）
						//导联脱落标志[14:15]【2】				有详细说明
						//心电有效数据[16:255]【240】 			8通道心电【10次采样 24*10=240】
						//加速度有效数据[256:261]【6】 	三轴加速度【1次采样 6*1=6】
						//校验和[262:263]【2】 			[0~261]字节的累加和，高位在前
						//帧尾[264:265]【2】				5A 5A
						else if(my_Portocol_Info->Portocol == Use_User_Proto)
						{
								my_Portocol_Info->sendbuf[0]=0xA5;//帧头
								my_Portocol_Info->sendbuf[1]=0xA5;//帧头
							
								my_Portocol_Info->sendbuf[2]=0x01;//设备号
								my_Portocol_Info->sendbuf[3]=0x05;//联系采样
								
								my_Portocol_Info->sendbuf[4]=my_Portocol_Info-> Data_Number>>24;//数据包编号 高位在前
								my_Portocol_Info->sendbuf[5]=my_Portocol_Info-> Data_Number>>16;
								my_Portocol_Info->sendbuf[6]=my_Portocol_Info-> Data_Number>>8;
								my_Portocol_Info->sendbuf[7]=my_Portocol_Info-> Data_Number++; //数据帧编号
							
								my_Portocol_Info->sendbuf[8]=0x08;	//通道数	
								my_Portocol_Info->sendbuf[9]=ADC_info.BAT_Percent;	//电量百分比
								my_Portocol_Info->sendbuf[10]=0xFF;	//信号强度 备用
								my_Portocol_Info->sendbuf[11]=0x06;	//采样率
								my_Portocol_Info->sendbuf[12]=0x03;	//增益
								my_Portocol_Info->sendbuf[13]=0xFF;	//预留字节 备用

//								//导联脱落标志 高字节 P 低字节 N 为1代表对应通道脱落
//								ADS129x_info.Ads129x_leadoff_flag=ADS129x_info.Ads129x_Cache[0]<<16 | ADS129x_info.Ads129x_Cache[1]<<8 | ADS129x_info.Ads129x_Cache[2];
//								ADS129x_info.Ads129x_leadoff_flag>>=4;	
//								my_Portocol_Info->sendbuf[14]=ADS129x_info.Ads129x_leadoff_flag>>8;	//导联脱落
//								my_Portocol_Info->sendbuf[16]=ADS129x_info.Ads129x_leadoff_flag;		//导联脱落
								
								//获取原始数据
								memcpy(my_Portocol_Info->sendbuf+16,*(my_Portocol_Info->Queue->databuf + my_Portocol_Info->Queue->front),240);//取数据
								
//								//加速度
//								my_Portocol_Info->sendbuf[256]=ADXL345_info.ADXL345_Speed_buf[1];
//								my_Portocol_Info->sendbuf[257]=ADXL345_info.ADXL345_Speed_buf[0];	
//								my_Portocol_Info->sendbuf[258]=ADXL345_info.ADXL345_Speed_buf[3];
//								my_Portocol_Info->sendbuf[259]=ADXL345_info.ADXL345_Speed_buf[2];
//								my_Portocol_Info->sendbuf[260]=ADXL345_info.ADXL345_Speed_buf[5];
//								my_Portocol_Info->sendbuf[261]=ADXL345_info.ADXL345_Speed_buf[4];

								check = 0;
								for(i=0;i<262;i++)//校验和 小端在前
								{
										check+=my_Portocol_Info->sendbuf[i];	
								}
								my_Portocol_Info->sendbuf[262] = check;		//校验和		
								my_Portocol_Info->sendbuf[263] = check>>8;	//校验和
								
								my_Portocol_Info->sendbuf[264]=0x5A; //帧尾
								my_Portocol_Info->sendbuf[265]=0x5A;	//帧尾
						}					
						my_Portocol_Info->Queue->Queue_pop_flag=1; //准备好一帧数据
						queue_add_front(my_Portocol_Info->Queue) ;				
				}	
		}
////////////////////////////////////////////////////////////////////////////////////////////////		
		STOP_DATA();
}




////////////////////////////////////////////////////////////////////////////////////////////////
//关闭相关中断，停止数据收发
void STOP_DATA(void)
{
		//关129X中断
		ADS129x_Stop_INT() ;
////////////////////////////////////////////////////////////////////////////////////////////////	
		//关ADC相关
		//ADC_DMA_NVIC_Config(0);//关 DMA ADC 中断
////////////////////////////////////////////////////////////////////////////////////////////////
		LinkList_Delete_all(SRAMIN,UART2_LinkList);
		USART2_Clear();
////////////////////////////////////////////////////////////////////////////////////////////////	
		//关DMA相关
		UART2_DMA_Finish=DMA_CANNLE_FREE;					//DMA可用
		UART2_DMA_TX_NVIC_Config(DMA_NVIC_DISABLE);						//DMA发送完成	中断	2 2
		HAL_UART_DMAStop(&UART2_Handler);      //传输完成以后关闭串口DMA
		__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
///////////////////////////////////////////////////////////////////////////////////////		
}

////////////////////////////////////////////////////////////////////////////////////////////////
//设备就绪，准备接收指令			
void START_DATA(void)
{
		Main_printf("当前工作模式 %d\r\n",work_state);	
		Main_printf("当前通讯协议 %d\r\n",my_Portocol_Info->Portocol);	
		
		KEY_TYPE=0;
		ADC_info.LowPower_flag=0;	//电量低报警
		LED1_OFF;	
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////	
		//开启串口数据接收
		LinkList_Delete_all(SRAMIN,UART2_LinkList);
		USART2_Clear();		
		HAL_TIM_Base_Start_IT(&TIM6_Handler);	//串口接收处理
		__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_RXNE); //串口接收中断	
////////////////////////////////////////////////////////////////////////////////////////////////	
		//开启DMA中断
		UART2_DMA_Finish=DMA_CANNLE_FREE;				//DMA可用		
		UART2_DMA_TX_NVIC_Config(DMA_NVIC_ENABLE);						//DMA发送完成		中断	2 2	
		__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
///////////////////////////////////////////////////////////////////////////////////////
		My_proto_Init(my_Portocol_Info); //初始化通讯协议相关				
////////////////////////////////////////////////////////////////////////////////////////////////
		//开ADS129X相关中断
		ADS129x_Start_INT();
////////////////////////////////////////////////////////////////////////////////////////////////		
		//开ADC相关中断
		//ADC_DMA_NVIC_Config(1);//DMA ADC 中断
}


//系统事件检查
void Sys_Event(void)
{
		u8 i,res;
///////////////////////////////////////////////////////////////////////////			
		if(LinkList_Serch_Data(UART2_LinkList)==LINKLIST_RETURN_OK)
		{
				USARTx_Send(&UART1_Handler, UART2_LinkList->next->my_data_inf->databuf, UART2_LinkList->next->my_data_inf->data_len) ;
				//Main_printf("串口0 接收长度 %d\r\n",UART0_LinkList->next->my_data_inf->data_len);
				//UART_DMA_Set_Buffer(USART0_DMA_NUM,USART0_TX_DMA_CANNEL,UART0_LinkList->next->my_data_inf->databuf);
				//UART_DMA_Enable(USART0_DMA_NUM,USART0,USART0_TX_DMA_CANNEL,UART0_LinkList->next->my_data_inf->data_len);
				CMD_rec_EVENT(UART2_LinkList); 
				res = LinkList_Delete_head(SRAMIN,UART2_LinkList);//用完销毁
		}	
///////////////////////////////////////////////////////////////////////////	
		if(TIM1_Timing >= 5)//电量监测
		{
				TIM1_Timing=0;
				BAT_Percent_Show() ;
		}
}


//电量监测
void BAT_Percent_Show(void) 
{
		ADC_info.BAT_VOL = (u16)((float)ADC_info.ADCConvertedValue[0] / ADC_info.ADCConvertedValue[1]*1200)*2;
		ADC_info.BAT_Percent=  (s8)((((float)ADC_info.BAT_VOL-2800) / 1500)*100);//计算电量百分比		

	
		if(work_state==SEND_BULE) 
		{		
				Main_printf("%d mV %d%% ",ADC_info.BAT_VOL,ADC_info.BAT_Percent);
		}	
		
		if(ADC_info.LowPower_flag==1 && ADC_info.BAT_VOL <=2800)
		{
				Main_printf("%d mV %d%% ",ADC_info.BAT_VOL,ADC_info.BAT_Percent);
				Main_printf("低电量待机\r\n");	
				Goto_Low_Power();
		}
		else if(ADC_info.BAT_VOL <=2900)
		{
				if(ADC_info.LowPower_flag==0)
				{
						ADC_info.LowPower_flag=1;
						Main_printf("%d mV %d%% ",ADC_info.BAT_VOL,ADC_info.BAT_Percent);
						Main_printf("低电量提示\r\n");	
					#if  LOWPOWER==1//修改系统时钟为8M，降低功耗	
						TIM1_Init(5000,1600);	//指示灯快闪
					#else
						TIM1_Init(5000,8000);	//指示灯快闪
					#endif		
				}
		}
}



//串口信息处理
void CMD_rec_EVENT(my_LinkList_Inf  *LinkList_cache) 
{
		u8 res,i;	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+LOWPOWER")==0)//查询字符串
		{
				Main_printf("进入低功耗模式\r\n");
				Goto_Low_Power();
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+NIMINGPROTO")==0)
		{
				Main_printf("匿名协议\r\n");
				if(my_Portocol_Info->Portocol != Use_NIMINGV7_Proto)
				{
						STOP_DATA(); //停止数据发送
						USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));
						my_Portocol_Info->Portocol = Use_NIMINGV7_Proto;//协议选择
						START_DATA();//开启相关中断
				}
				else
				{
						Main_printf("无修改\r\n");
				}
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+USERPROTO")==0)
		{
				Main_printf("用户协议\r\n");
				if(my_Portocol_Info->Portocol != Use_User_Proto)
				{
						STOP_DATA(); //停止相关中断
						USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));
						my_Portocol_Info->Portocol = Use_User_Proto;//协议选择
						START_DATA();//开启相关中断
				}
				else
				{
						Main_printf("无修改\r\n");
				}
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+STOP")==0)
		{
				Main_printf("停止发数据\r\n");
				STOP_DATA(); //停止相关中断
				USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));
				for(i=0;i<10;i++)
				{
						LED_1_TogglePin;
						delay_ms(50);
				}
				START_DATA();//开启相关中断
				ADS129x_Stop_INT();	
				queue_clear(my_Portocol_Info->Queue);
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+START")==0)
		{
				Main_printf("开始发数据\r\n");
				STOP_DATA(); //停止相关中断
				USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));
				for(i=0;i<10;i++)
				{
						LED_1_TogglePin;
						delay_ms(50);
				}
				START_DATA();//开启相关中断		
				ADS129x_Start_INT();//开启int中断
				TIM1->CNT=0;
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+500SPS")==0)
		{
				Main_printf("修改采样率为500\r\n");
				STOP_DATA(); //停止相关中断
			#if ADS129X_DEVICE == ADS1299
				ADS129x_SPS_Change(ADS1299_SPS_500);
			#endif
			#if ADS129X_DEVICE == ADS1298
				ADS129x_SPS_Change(ADS1298_SPS_500_LP);
			#endif
			#if ADS129X_DEVICE == ADS1292
				ADS129x_SPS_Change(ADS1292_SPS_500);
			#endif
				ADS129x_REG_Init();//所有芯片 中途改变寄存器设置
				START_DATA();//开启相关中断	
				USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+250SPS")==0)
		{
				Main_printf("修改采样率为250\r\n");
				STOP_DATA(); //停止相关中断
			#if ADS129X_DEVICE == ADS1299
				ADS129x_SPS_Change(ADS1299_SPS_250);
			#endif
			#if ADS129X_DEVICE == ADS1298
				ADS129x_SPS_Change(ADS1298_SPS_250_LP);
			#endif
			#if ADS129X_DEVICE == ADS1292
				ADS129x_SPS_Change(ADS1292_SPS_250);
			#endif
				ADS129x_REG_Init();//所有芯片 中途改变寄存器设置
				START_DATA();//开启相关中断	
				USARTx_Send(&BG22AX_USART_Handle,(uint8_t*) "BT+OK", strlen("BT+OK"));		
		}	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+NORMAL")==0)
		{
				Main_printf("正常数据采集\r\n");
				STOP_DATA(); //停止相关中断
			
			#if ADS129X_DEVICE == ADS1299
				ADS129x_MODE_Change(ADS1299_CHANNLE_NORMAL_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1298
				ADS129x_MODE_Change(ADS1298_CHANNLE_NORMAL_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1292
				ADS129x_MODE_Change(ADS1292_CHANNLE_NORMAL_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			
				ADS129x_REG_Init();//所有芯片 中途改变寄存器设置
				START_DATA();//开启相关中断	
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+TEST")==0)
		{
				Main_printf("方波测试\r\n");
				STOP_DATA(); //停止相关中断
			#if ADS129X_DEVICE == ADS1299
				ADS129x_MODE_Change(ADS1299_BIAS_TEST_SIGNAL);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1298
				ADS129x_MODE_Change(ADS1298_BIAS_TEST_SIGNAL);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1292
				ADS129x_MODE_Change(ADS1292_BIAS_TEST_SIGNAL);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
				ADS129x_REG_Init();//所有芯片 中途改变寄存器设置
				START_DATA();//开启相关中断	
		}	
		else if(Str_search(LinkList_cache->next->my_data_inf->databuf,LinkList_cache->next->my_data_inf->data_len,"AT+SHORT")==0)
		{
				Main_printf("内部短接噪声\r\n");
				STOP_DATA(); //停止相关中断
			#if ADS129X_DEVICE == ADS1299
				ADS129x_MODE_Change(ADS1299_CHANNLE_SHORT_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1298
				ADS129x_MODE_Change(ADS1298_CHANNLE_SHORT_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
			#if ADS129X_DEVICE == ADS1292
				ADS129x_MODE_Change(ADS1292_CHANNLE_SHORT_INPUT);//修改通道模式（内部短路，方波测试，正常数据采集）
			#endif
				ADS129x_REG_Init();//所有芯片 中途改变寄存器设置
				START_DATA();//开启相关中断	
		}			
		
}


//系统进入低功耗模式
void Goto_Low_Power(void)
{
		u16 i;
///////////////////////////////////////////////////////////////////////////		
		//__disable_irq(); 	//关总中断
		//HAL_SuspendTick(); //挂起滴答定时器
		//HAL_ResumeTick();  //释放滴答定时器
		//__enable_irq();   // 开启总中断	
		STOP_DATA();	
///////////////////////////////////////////////////////////////////////////
		for(i=0;i<30;i++)
		{
				LED_1_TogglePin;
			#if USE_LED2 == 1
				LED_2_TogglePin;	
			#endif
				delay_ms(50);
		}
		LED1_OFF;	
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif
///////////////////////////////////////////////////////////////////////////	
		Main_printf("蓝牙休眠\r\n");
		HAL_TIM_Base_Stop_IT(&TIM6_Handler);	//关串口数据接收
		BG22AX_LowPower();//蓝牙休眠（需要操作引脚）	
///////////////////////////////////////////////////////////////////////////		
		Main_printf("129X休眠\r\n");
		ADS129x_Goto_Lowpower();
///////////////////////////////////////////////////////////////////////////		
//		Main_printf("关机\r\n");
//		POWER_OFF;
//		NVIC_SystemReset(); // 复位
//		while(1);	
///////////////////////////////////////////////////////////////////////////		
		Main_printf("设备进入低功耗模式\r\n");
		delay_ms(500);
		Sys_Enter_Standby();//进入待机模式
		while(1);//按键复位唤醒设备
///////////////////////////////////////////////////////////////////////////	
		//NVIC_SystemReset(); // 复位
}


/**********************************************************************
编译结果里面的几个数据的意义：
Code：表示程序所占用 FLASH 的大小（FLASH）
RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)
***********************************************************************/


