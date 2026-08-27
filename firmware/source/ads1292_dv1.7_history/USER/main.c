#include "sys.h"         //系统配置
#include "delay.h"       //延时
#include "malloc.h"
#include "usart.h"
#include "string.h"
#include "timer.h"
#include "adc.h"
#include "nvic.h"

#include "adc.h"
#include "dma.h"
#include "wkup.h"

#include "led.h"
#include "key.h"

//#include "BG22AX.h"
#include "RF4404B4.h"
#include "ADS129x.h"

//#include "MYFATFS.h"	 
//#include "sd_card.h"
//#include "ff.h"
//#include "exfuns.h"
//#include "fatfs.h"



/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/03/01
//版本：
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
//修改芯片型号-修改sys.h-修改启动函数等
/////////////////////////////////////////////////////////////////////////////////////
//		功耗测试	ADS1299 DV3.2 8通道蓝牙+TF
//		功耗:			8M理论19.6mA		16M理论 ？     80M理论27.6mA
//		ADS1299 	AVDD	5V		7.14mA 	相当于3.3V 10.8mA	（未开启右腿驱动） 
//		ADS1299 	DVDD	3.3V	1mA  	 
//		BG224A		发射电流 4.1mA
//		SM32L4		8M 			1.9mA
//		SM32L4		80M 		10mA
//		供电驱动板耗电 		1.8mA	(HX4002 I_NO LOAD=1.8mA)
//		功耗实测
//		蓝牙发送数据	，总功耗 80M 	31mA
//		蓝牙发送数据	，总功耗 8M 	21mA		
/////////////////////////////////////////////////////////////////////////////////////
//		功耗测试	ADS1292 DV1.5.1 2通道蓝牙
//		功耗:			8M理论19.6mA		80M理论27.6mA
//		ADS1292 							0.3mA 
//		BG224A		发射电流 		4.1mA
//		SM32L4		8M 					1.9mA
//		SM32L4		80M 				10mA
//		LDO	XC6206	 					忽略
//		功耗实测
//		蓝牙发送数据	，总功耗 80M 	?mA	
//		蓝牙发送数据	，总功耗 8M 	6.22mA
//		通讯距离 2.5M
/////////////////////////////////////////////////////////////////////////////////////
//		功耗测试	ADS1292 DV1.5.2 2通道蓝牙+TF
//		功耗:			8M理论19.6mA		    80M理论27.6mA
//		ADS1292 							0.3mA 
//		BG224A		发射电流 		4.1mA
//		SM32L4		8M 					1.9mA
//		SM32L4		80M 				10mA
//		LDO	XC6206	 					忽略
//		功耗实测
//		蓝牙发送数据	，总功耗 80M 	?mA	
//		蓝牙发送数据	，总功耗 8M 	6.22mA
//		通讯距离 2.5M
//		TF存储				，总功耗 8M  2.9-8（平均4.5mA）
//		低功耗待机			总功耗 8M	 450uA (stm32 ADC 100uA LDO ?  ADS1292 1.5uA  蓝牙3uA TF 180uA)
/////////////////////////////////////////////////////////////////////////////////////
//		3.3V 	8M	 空载 		2.0Ma   符合预期（芯片功耗 1.9Ma LDO ADC等其他外围0.1mA）
//		指示灯 		0.3mA
//		串口+1292   （1292 0.3Ma  串口0.15mA）
// 		蓝牙 扫描状态  3.2mA
//		蓝牙 连接状态  3mA
//		蓝牙 发送状态  4mA  整体 6Ma
/////////////////////////////////////////////////////////////////////////////////////


/*
功能说明：
		蓝牙发送：
					开机进入蓝牙发送模式，此模式下等待蓝牙连接，手动按按键3s切换到TF存储模式，
					当120s无按键操作也无蓝牙连接后，设备进入低功耗待机模式。蓝牙配对完成后，
					进入蓝牙发送模式，按照通讯协议发送数据，当接收到上位机指令 AT+WRITETF 后，
					设备切换到TF尺寸模式。
		TF存储：
					切换到TF存储模式后，按照数据协议存储 .bin文件 ，当存储完成后，需要手动按
					按键3s保存数据，同时自动退出TF存储模式，切换到蓝牙发送模式，以此循环。
*/



/////////////////////////////////////////////////////////////////////////////////////////////
void Sys_Event(void);//系统事件处理
void Send_BlueTooth(void);//蓝牙 发送
void Send_UART1(void);//串口 发送
void Goto_Low_Power(void);
/////////////////////////////////////////////////////////////////////////////////////////////


//main	
int main(void)
{			
		u16 i;
	
		work_state=SEND_BULE;		//蓝牙发送
//////////////////////////////////////////////////////////////////////////////////////////////	
    HAL_Init();
		SystemClock_Config();//8M主频 
	 	delay_init(8); 			//初始化延时函数    系统时钟
		delay_ms(100);
//////////////////////////////////////////////////////////////////////////////////////////////	
		mem_init(SRAMIN);
////////////////////////////////////////////////////////////////////////////////////////////////
		MYDMA_ADC1_Config();//初始化DMA	
		MYDMA_UART1_Config(); //初始化DMA
		MYDMA_UART2_Config(); //初始化DMA	
////////////////////////////////////////////////////////////////////////////////////////////////
		//ALL_GPIO_Init();		//全部设置成模拟输入
		uart1_init(115200);		//初始化	串口
		LED_GPIO_Init(); 			//初始化	LED IO
		KEY_Init();						//初始化	按键 IO
		RF4404B4_GPIO_Init();	//初始化	蓝牙 IO	
		ADS129x_IO_Init();		//初始化	ADS129X IO	
		MY_ADC_Init();				//初始化	ADC		
////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("程序版本 STM32L432 V1.1.6 日期 2022-06-17 配套 ADS1292 DV1.7\r\n");	
    LED1_ON;
		delay_s(2);
    LED1_OFF;
////////////////////////////////////////////////////////////////////////////////////////////////	
		//充电检测
		if(KEY1==1) //连接了外部电源
		{		
				Main_printf("充电中，进入低功耗模式\r\n");	
				Sys_Enter_Standby();//进入低功耗模式
				NVIC_SystemReset(); // 复位
		}
////////////////////////////////////////////////////////////////////////////////////////////////		
		//初始化串口结构
		UART_Info =(_UART_Info*)mymalloc(SRAMIN,sizeof(_UART_Info));	//数据结构
		UART_Info->sendbuf = (u8*)mymalloc(SRAMIN,UART_SEND_LENGTH);	//发送缓冲区 
		UART_Info->UART_Queue =  queue_init(UART_QUEUE_SIZE,UART_QUEUE_LENGTH);	//循环队列初始化
		if(UART_Info== NULL|| UART_Info->UART_Queue == NULL ||UART_Info->sendbuf == NULL )
		{
				Main_printf("串口缓冲区 内存申请失败\r\n");
		}
		else
		{
				Main_printf("串口缓冲区 内存申请成功\r\n");
		}
		Main_printf("串口内存申请成功，当前占用 %d\r\n",mem_perused(SRAMIN));	
		UART_Info->Queue_pop_flag=0;
////////////////////////////////////////////////////////////////////////////////////////////////
	#if 0		//蓝牙出厂设置  设置连接间隔等
			res=RF4404B4_AT_Confg();//开始设置		
			if(res)		
					Main_printf("蓝牙设置失败\r\n");							
			else									
					Main_printf("蓝牙设置成功\r\n");		
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////
		Main_printf("初始化蓝牙\r\n");
		RF4404B4_Init();//初始化	
////////////////////////////////////////////////////////////////////////////////////////////////
	#if 1	//等待蓝牙连接	
		Main_printf("等待蓝牙连接\r\n");
		i=0;
		while(1)
		{
				if(KEY1==1) //充电检测
				{		
						Main_printf("充电中，进入低功耗模式\r\n");	
						Sys_Enter_Standby();//进入低功耗模式
						NVIC_SystemReset(); // 复位
				}
				if(i==200)
				{
						i=0;
						if(RF4404B4_Wait_Connect()==0)
						{
								Main_printf("蓝牙成功连接\r\n");
								break;
						}
						else
						{
								Main_printf("等待蓝牙连接 ");
						}
						LED_1_TogglePin;
				}	
				delay_ms(10);
				i++;
		}	
		LED1_OFF;
	#endif
////////////////////////////////////////////////////////////////////////////////////////////////
		ADS129x_ReInit(3);//初始化129x	
//////////////////////////////////////////////////////////////////////////////////////////////		
		Main_printf("初始化完成开始发送数据\r\n");
		for(i=0;i<10;i++)
		{
				LED_1_TogglePin;
				delay_ms(50);
		}
//////////////////////////////////////////////////////////////////////////////////////////////				
		TIM2_Init(50000,800); //ADC 采样 +指示灯开 3s	中断	3 3
		TIM7_Init(100,800);		//指示灯关 10ms					中断	3 3		
		TIM6_Init(10,800);	 	//串口数据接收用 1ms	
////////////////////////////////////////////////////////////////////////////////////////////////		
		USART2_Clear();		//串口清零		
		LowPower_flag=0;	//电量低报警
		//KEY_A1=0;				//按键检测标志
		UART1_DMA_Finish=0;	//DMA可用	
		UART2_DMA_Finish=0;	//DMA可用	
//////////////////////////////////////////////////////////////////////////////////////////////////
    HAL_TIM_Base_Start_IT(&TIM6_Handler);		//使能串口数据接收	中断	1 2
		HAL_NVIC_EnableIRQ(ADS129X_DRDY_IRQn);	//使能ADS129X				中断	1 1
		UART1_DMA_TX_NVIC_Config(1);						//开启DMA发送完成		中断	2 2	
		UART2_DMA_TX_NVIC_Config(1);						//开启DMA发送完成		中断	2 2	
////////////////////////////////////////////////////////////////////////////////////////////////
		while(1)//数据发送测试
		{
				if(work_state == SEND_BULE)				
						Send_BlueTooth();	
		}		
}



void Send_BlueTooth(void)//蓝牙 发送
{
		u8 i,j,z,k=1,res;
		u8 addcheck=0,sumcheck=0;	
		u8 datanum=0;
		u16 buf_num=0;
		u16 check;
///////////////////////////////////////////////////////////////////////////////////////
		while(work_state==SEND_BULE)
		{
				Sys_Event(); //事件检测
/////////////////////////////////////////////////////////////////////////
			#if  USE_NIMING_V7 ==1	//双通道  采样率500 15次采样为一帧 间隔30ms
				if(UART_Info->Queue_pop_flag==1)	//队列数据取出标志
				{
						if(UART2_DMA_Finish==0) //DMA空闲
						{
								UART2_DMA_Finish=1;
								UART_Info->Queue_pop_flag=0;
							  HAL_UART_Transmit_DMA(&UART2_Handler, (u8*)UART_Info->sendbuf,UART_SEND_LENGTH); //启动传输
						}
						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
						{
								UART_Info->UART_Queue->Queue_Full_flag=0;
								Main_printf("UART queue full\r\n");
						}
				}
				else if(serch_queue_data(UART_Info->UART_Queue))//队列有数据
				{
						for(i=0;i<15;i++)//一帧15次采样的数据
						{
								//帧1数据长度为8字节		4（帧头）+2（有效数据）+2（校验）=8	
								UART_Info->sendbuf[0 +i*14]=0xAA;
								UART_Info->sendbuf[1 +i*14]=0xFF;
								UART_Info->sendbuf[2 +i*14]=0xF1;	
								UART_Info->sendbuf[3 +i*14]=8;
								//获取原始数据
								for(z=0;z<ADS129X_USE_CANNLE;z++)//通道数
								{
										ADS129x_info.cannle[z] =	
																		  *(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+0+ i*6+ z*3) <<16 //获取原始数据
																		|	*(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+1+ i*6+ z*3) <<8 
																		| *(*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front)+2+ i*6+ z*3) ;
										ADS129x_info.p_Temp[z] = get_volt(ADS129x_info.cannle[z]);	//转成有符号数
										ADS129x_info.cannle[z] = ADS129x_info.p_Temp[z];//赋值给无符号数，准备做逻辑右移
										//小端在前
										UART_Info->sendbuf[4 +z*4 +14*i] = ADS129x_info.cannle[z] >>0; //低位在前
										UART_Info->sendbuf[5 +z*4 +14*i] = ADS129x_info.cannle[z] >>8;
										UART_Info->sendbuf[6 +z*4 +14*i] = ADS129x_info.cannle[z] >>16;
										UART_Info->sendbuf[7 +z*4 +14*i] = ADS129x_info.cannle[z] >>24;
								}
								//校验位
								sumcheck = 0;addcheck=0;
								for(j=0;j< 12 ;j++)
								{
											sumcheck += UART_Info->sendbuf[j+i*14];	
											addcheck += sumcheck;					
								}	
								UART_Info->sendbuf[12+i*14] = sumcheck;	//校验		
								UART_Info->sendbuf[13+i*14] = addcheck;	//校验		
						}
						UART_Info->Queue_pop_flag=1; //准备好一帧数据
						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//取数据，队头自增，存数据，队尾自增
				}
			#endif
			#if USE_USER_PROTO==1 //双通道  采样率500 40次采样为一帧 间隔80ms
				if(UART_Info->Queue_pop_flag==1)	//队列数据取出标志
				{
						if(UART2_DMA_Finish==0) //DMA空闲
						{
								UART2_DMA_Finish=1;
								UART_Info->Queue_pop_flag=0;
							  HAL_UART_Transmit_DMA(&UART2_Handler, (u8*)UART_Info->sendbuf,UART_SEND_LENGTH); //启动传输
						}
						else	if(UART_Info->UART_Queue->Queue_Full_flag==1)
						{
								UART_Info->UART_Queue->Queue_Full_flag=0;
								Main_printf("UART queue full\r\n");
						}
				}
				else if(serch_queue_data(UART_Info->UART_Queue))//队列有数据
				{
						//帧头
						UART_Info->sendbuf[0]=0xA5;
						UART_Info->sendbuf[1]=0xA5;
						//获取原始数据
						memcpy(UART_Info->sendbuf+2,*(UART_Info->UART_Queue->databuf + UART_Info->UART_Queue->front),240);//取数据
						//校验位
						UART_Info->sendbuf[242]=BAT_Percent;	
						UART_Info->sendbuf[243]=0XFF;	
						UART_Info->sendbuf[244]=0XFF;	
						UART_Info->sendbuf[245]=0XFF;	
						UART_Info->sendbuf[246]=buf_num>>8;
						UART_Info->sendbuf[247]=buf_num;
						check = 0;
						for(i=0;i<248;i++)//校验
						{
								check+=UART_Info->sendbuf[i];	
						}
						UART_Info->sendbuf[248] = check>>8;	//校验和		
						UART_Info->sendbuf[249] = check;	//校验和			
						
						UART_Info->Queue_pop_flag=1; //准备好一帧数据
						UART_Info->UART_Queue->front = (UART_Info->UART_Queue->front+1) % UART_Info->UART_Queue->capacity;//取数据，队头自增，存数据，队尾自增
				}
			#endif
		}
/////////////////////////////////////////////////////////////////////////////////////////////		
		HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);	//ADS129X				中断	1 1
		HAL_TIM_Base_Stop_IT(&TIM2_Handler);		//电量监测+LED	中断	3 3
    HAL_TIM_Base_Stop_IT(&TIM7_Handler);		//LED						中断	3 3
    HAL_TIM_Base_Stop_IT(&TIM6_Handler);		//串口数据接收	中断	1 2
		UART1_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
		UART2_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
		__HAL_UART_DISABLE_IT(&UART1_Handler, UART_IT_RXNE); //串口接收中断	
		__HAL_UART_DISABLE_IT(&UART2_Handler, UART_IT_RXNE); //串口接收中断

		__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4); //清除DMA传输完成标志
		__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
		HAL_UART_DMAStop(&UART1_Handler);      //传输完成以后关闭串口DMA	
		HAL_UART_DMAStop(&UART2_Handler);      //传输完成以后关闭串口DMA

		while(1);		
}






//系统事件检查
void Sys_Event(void)
{
		u8 i,res;
///////////////////////////////////////////////////////////////////////////			
		if(USART2_RX_EVENT)//串口数据接收
		{
				if(UART2_DMA_Finish==0) //串口未被占用
				{
				////////////////////////////////////////////////////////////////////	
						Main_printf("蓝牙串口数据接收：%s\r\n",USART2_RX_BUF);
				////////////////////////////////////////////////////////////////////	
						//for(i=0;i<RF4404B4_RX_LEN;i++)
						//{
						//		Main_printf("%x ",RF4404B4_RX_BUF[i]);
						//}
						//Main_printf("\r\n");
				////////////////////////////////////////////////////////////////////			
						if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+LOWPOWER")==0)//查询字符串
						{
								Main_printf("进入低功耗模式\r\n");
								Goto_Low_Power();
						}
						if(Str_search(USART2_RX_BUF,USART2_RX_LEN,"AT+BLUECFG")==0)//查询字符串
						{
								HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);	//ADS129X				中断	1 1
							
								HAL_TIM_Base_Stop_IT(&TIM2_Handler);		//电量监测+LED	中断	3 3
								HAL_TIM_Base_Stop_IT(&TIM7_Handler);		//LED						中断	3 3
								HAL_TIM_Base_Stop_IT(&TIM6_Handler);		//串口数据接收	中断	1 2
								UART1_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
								UART2_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
								__HAL_UART_DISABLE_IT(&UART1_Handler, UART_IT_RXNE); //串口接收中断	
								__HAL_UART_DISABLE_IT(&UART2_Handler, UART_IT_RXNE); //串口接收中断

								__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4); //清除DMA传输完成标志
								__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
								HAL_UART_DMAStop(&UART1_Handler);      //传输完成以后关闭串口DMA	
								HAL_UART_DMAStop(&UART2_Handler);      //传输完成以后关闭串口DMA
							
							
								res=RF4404B4_AT_Confg();//开始设置		
								if(res)		
										Main_printf("蓝牙设置失败\r\n");							
								else									
										Main_printf("蓝牙设置成功\r\n");		
								NVIC_SystemReset(); // 复位
						}						
				////////////////////////////////////////////////////////////////////	
						USART2_Clear();
						__HAL_UART_ENABLE_IT(&UART2_Handler, UART_IT_RXNE); //接收中断
						HAL_TIM_Base_Start_IT(&TIM6_Handler);		//串口数据接收	中断	1 2	
						USART2_RX_EVENT=0;
				}
		}		
///////////////////////////////////////////////////////////////////////////				
		//充电检测
		if(KEY1==1) //连接了外部电源
		{		
				Main_printf("连接了外部电源\r\n");	
				Goto_Low_Power();
		}
///////////////////////////////////////////////////////////////////////////	
		if(TIM2_FLAG)//电量监测
		{
				TIM2_FLAG=0;

				BAT_VOL = (u16)((float)ADCConvertedValue[0] / ADCConvertedValue[1]*1200)*2;
				BAT_Percent=  (s8)((((float)BAT_VOL-2800) / 1500)*100);//计算电量百分比
				Main_printf("%d mV %d %d %d\r\n",BAT_VOL,BAT_Percent,ADCConvertedValue[0],ADCConvertedValue[1]);

				if(LowPower_flag==1 && BAT_VOL <=2800)
				{
						Main_printf("低电量待机\r\n");	
						//Goto_Low_Power();
				}
				else if(BAT_VOL <=2900)
				{
						if(LowPower_flag==0)
						{
								LowPower_flag=1;
								Main_printf("低电量提示\r\n");	
								TIM2_Init(8000,800); //800ms 低电量提示
						}
				}
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
		HAL_NVIC_DisableIRQ(ADS129X_DRDY_IRQn);	//ADS129X				中断	1 1
		HAL_TIM_Base_Stop_IT(&TIM2_Handler);		//电量监测+LED	中断	3 3
    HAL_TIM_Base_Stop_IT(&TIM7_Handler);		//LED						中断	3 3
    HAL_TIM_Base_Stop_IT(&TIM6_Handler);		//串口数据接收	中断	1 2
		UART1_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
		UART2_DMA_TX_NVIC_Config(0);						//DMA发送完成		中断	2 2	
		__HAL_UART_DISABLE_IT(&UART1_Handler, UART_IT_RXNE); //串口接收中断
		__HAL_UART_DISABLE_IT(&UART2_Handler, UART_IT_RXNE); //串口接收中断

		__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4); //清除DMA传输完成标志
		__HAL_DMA_CLEAR_FLAG(&UART2TxDMA_Handler, DMA_FLAG_TC7); //清除DMA传输完成标志
		HAL_UART_DMAStop(&UART1_Handler);      //传输完成以后关闭串口DMA	
		HAL_UART_DMAStop(&UART2_Handler);      //传输完成以后关闭串口DMA			
///////////////////////////////////////////////////////////////////////////
		for(i=0;i<30;i++)
		{
				LED_1_TogglePin;
				delay_ms(50);
		}
		LED1_OFF;
///////////////////////////////////////////////////////////////////////////		
		Main_printf("蓝牙休眠\r\n");
		RF4404B4_LowPower();//蓝牙休眠（需要操作引脚）	
///////////////////////////////////////////////////////////////////////////		
		Main_printf("1292休眠\r\n");
		ADS129x_Send_CMD(ADS129X_SDATAC);//发送停止连续读取数据命令
		delay_ms(10);
		ADS129x_Send_CMD(ADS129X_STANDBY);//待机模式
		delay_ms(10);
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


