#include "Timer.h"
#include "led.h" 
#include "key.h"
#include "myQueue.h"
#include "usart.h"       //串口 

#include <string.h>
#include "adc.h"
#include "usart.h"	 
#include "OPENBCI.h"



//高级 TIM1 TIM8
//通用 TIM2 TIM3 TIM4 TIM5
//基本 TIM6 TIM7

//C8T6 只有 1-4 4个定时器
u32 Tim1_flag;
void TIM1_Init(u16 arr,u16 psc)
{

		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
		TIM_TimeBaseStructure.TIM_Period=arr-1; // 自动重装载寄存器的值
		TIM_TimeBaseStructure.TIM_Prescaler=psc-1; // 时钟预分频数
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; // 采样分频
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //重复寄存器，用于自动更新pwm占空比
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update|TIM_IT_Trigger); //清除更新中断请求位
		TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_Trigger, DISABLE );//允许更新										
		TIM_Cmd(TIM1, ENABLE); 
}
//定时器中断服务程序	 
void TIM1_UP_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //更新中断 
		{ 			
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清中断标志
			
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE );//
				TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_Trigger, DISABLE );//更新										

				//Main_printf("搬运次数 %d \r\n",ads129x_save_flag);

		}		
}


//定时器初始化
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM5时钟使能 
	 
		TIM_TimeBaseStructure.TIM_Period = arr-1;; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure); 		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除更新中断请求位
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );//允许更新	
		TIM_Cmd(TIM2, ENABLE); 
}






////定时器中断服务程序	 
//void TIM2_IRQHandler(void)
//{ 
//		if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //更新中断 
//		{ 			
//				USART1_TIME++;
//				USART2_TIME++;
//			
//				if(USART1_TIME==50)//50ms 解包
//				{		
//						USART1_TIME=0;
//						if(USART1_RX_LEN)
//						{
//								if(queue_push_byte(UART_Info->USART_Queue,USART1_RX_LEN))//第0个元素存放数据长度
//								{
//										if(queue_add(UART_Info->USART_Queue,USART1_RX_BUF,1,USART1_RX_LEN))//第1个元素开始，搬运数据至队列
//										{
//											
//										}
//										USART1_RX_LEN=0;
//								}	
//						}
//				}	
//				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清中断标志
//		}		
//}

	
	

//定时器初始化
//按键检测
void TIM3_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能 
	 
		TIM_TimeBaseStructure.TIM_Period = arr-1;; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除更新中断请求位
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );//允许更新										
		TIM_Cmd(TIM3, DISABLE); 
}

//定时器中断服务程序	 
void TIM3_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //更新中断 
		{ 
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清中断标志
			
				TIM_Cmd(TIM3, DISABLE); 
				TIM3->CNT=KEYDOWNLONGTIME; //长按键
		}				   
}




//定时器初始化
void TIM4_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能 
	 
		TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除更新中断请求位
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//允许更新										
		TIM_Cmd(TIM4, ENABLE); 
}

u8 TIM4_Timing=0;
extern u8 lowpower;
extern u8 power_detective;
//定时器中断服务程序	 
void TIM4_IRQHandler(void)
{ 
    //更新中断
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)   //更新中断
    {  
				TIM4_Timing++;	

				if(lowpower==0) //低电量指示
				{
						if(	power_detective==1)//开启电量监测
						{
								BAT_Percent= ((ADCConvertedValue[0]-BAT_0)*100/BAT_LEVEL);
								//Main_printf("%d %d mV %d %%\r\n",ADCConvertedValue[0],ADCConvertedValue[0]*3300/4096*2,BAT_Percent);
								if(BAT_Percent<20)//低电量
								{
										lowpower=1;
									
									#if LOWPOWER ==1//低功耗
										TIM4_Init(1000,1600);//系统指示灯+电量监测		
									#else
										TIM4_Init(1000,7200);//系统指示灯+电量监测		
									#endif	
										DS2=LED_ON;							
								}
						}
				}
				
				LED1=!LED1;
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清中断标志
    }     				   
}



////定时器初始化
////蜂鸣器时长 +按键消抖
//u8 TIM5_FLAG=0;
//void TIM5_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;

//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能 
//	 
//		TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
//		TIM_TimeBaseStructure.TIM_Prescaler =psc-1; //设置用来作为TIMx时钟频率除数的预分频值 
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	 
//		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组
//		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM中断
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//		NVIC_Init(&NVIC_InitStructure); 
//		
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除更新中断请求位
//		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE );//允许更新										
//		TIM_Cmd(TIM5, ENABLE); 
//}

//void TIM5_IRQHandler(void)
//{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //更新中断 
//		{ 
//				TIM_ClearITPendingBit(TIM5, TIM_IT_Update);//清中断标志
//				TIM5_FLAG++;
//				if(TIM5_FLAG == 1)
//				{
//						if(KEY_STATE&KEY_1 && KEY1 == 0)
//						{
//								KEY_TYPE |= 0x01;
//						}
//						if(KEY_STATE&KEY_2 && KEY2 == 0)
//						{
//								KEY_TYPE |= 0x02;
//						}
//						if(KEY_STATE&KEY_PWR && KEYPWR == 0)
//						{
//								KEY_TYPE |= 0x04;
//						}	
//						if(KEY_STATE&KEY_A && KEYA == 0)
//						{
//								KEY_TYPE |= 0x08;
//						}	
//						if(KEY_STATE&KEY_B && KEYB == 0)
//						{
//								KEY_TYPE |= 0x10;
//						}	
//						if(KEY_STATE&KEY_C && KEYC == 0)
//						{
//								KEY_TYPE |= 0x20;
//						}	
//						if(KEY_STATE&KEY_D && KEYD == 0)
//						{
//								KEY_TYPE |= 0x40;
//						}	
//				}
//				else if(TIM5_FLAG==2)//关蜂鸣器
//				{
//					#if USE_BUZZER   //蜂鸣器
//						TIM3->CCR4	= 0; //占空比为0 输出高电平 关蜂鸣器
//					#endif
//				}
//				else if(TIM5_FLAG==5)
//				{
//						TIM5_FLAG=0;
//						DS2=1;//延时消抖	
//						TIM_Cmd(TIM5, DISABLE); 
//						TIM5->CNT=0;
//						KEY_STATE=0;
//				}
//		}	
//}


/*先说TIM_ARRPreloadConfig设置为DISABLE 和ENABLE的问题，他的作用只是允许或禁止在定时器工作时
向ARR的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值。在开始初始化的时候你已经把
TIM_TimeBaseStructure.TIM_Period=2000;   //ARR的值
后来也一直是这个值，原因是你没有编写中断服务函数或者你在中断服务函数中根本就没有给ARR缓冲器重新
写入新值，所以设置为DISABLE 和ENABLE都没有影响。*/
//TIM_ARRPreloadConfig(TIM3, ENABLE);   //使能TIMx在ARR上的预装载寄存器	


//如：关掉TIM3_CH2的PWM输出：TIM3->CCER&=0xFFEF;	关闭 PWM输出
//TIM_OC1NPolarityConfig(TIM1,TIM_OCNPolarity_Low); //关闭输出后，再强制输出电平




//		TIM3->CNT=0;//清空定时器计数器
//		TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
//		TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
//		TIM3->DIER|=1<<0;   //允许更新中断	  
//		TIM3->CR1|=0x01;    //使能定时器3
