#include "Timer.h"
#include "led.h" 
#include "usart.h"//串口 
#include "adc.h"
#include "key.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/03/01
//版本：V1.0
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



TIM_HandleTypeDef TIM1_Handler;      //定时器句柄
TIM_HandleTypeDef TIM2_Handler;      //定时器句柄
TIM_HandleTypeDef TIM3_Handler;      //定时器句柄
TIM_HandleTypeDef TIM4_Handler;      //定时器句柄
TIM_HandleTypeDef TIM5_Handler;      //定时器句柄
TIM_HandleTypeDef TIM6_Handler;      //定时器句柄
TIM_HandleTypeDef TIM7_Handler;      //定时器句柄


u16 TIM1_FLAG=0;
u16 TIM2_FLAG=0;
u16 TIM3_FLAG=0;
u16 TIM4_FLAG=0;
u16 TIM5_FLAG=0;
u16 TIM6_FLAG=0;
u16 TIM7_FLAG=0;

u16 TIM1_Timing=0;
u16 TIM2_Timing=0;
u16 TIM3_Timing=0;
u16 TIM4_Timing=0;
u16 TIM5_Timing=0;
u16 TIM6_Timing=0;
u16 TIM7_Timing=0;



//通用定时器中断初始化函数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK:80Mhz)
//arr		自动重装值。
//psc		时钟预分频数
void TIM2_Init(u16 arr, u16 psc)
{
	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
		TIM_OC_InitTypeDef sConfigOC = {0};
		TIM_MasterConfigTypeDef sMasterConfig = {0};
		
    TIM2_Handler.Instance = TIM2;                          	//通用定时器
		TIM2_Handler.Init.Period = arr-1;                        	//自动装载值
    TIM2_Handler.Init.Prescaler = psc-1;                     	//分频系数
    TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    	//向上计数器
    TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
		TIM2_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&TIM2_Handler);
	
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig) != HAL_OK)
		{
				
		}
		if (HAL_TIM_PWM_Init(&TIM2_Handler) != HAL_OK)
		{
			
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig) != HAL_OK)
		{
			
		}			
		sConfigOC.OCMode = TIM_OCMODE_PWM1;
		sConfigOC.Pulse = TIM2_Handler.Init.Period / 2; //重装载一半
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		if (HAL_TIM_PWM_ConfigChannel(&TIM2_Handler, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		{
			
		}
	
		HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2); //使能PWM输出  CUBE不自动生成这句，需要手动开启	
		
    HAL_TIM_Base_Start_IT(&TIM2_Handler);					//使能定时器和定时器更新中断：TIM_IT_UPDATE
}


void TIM6_Init(u16 arr, u16 psc)
{
    TIM6_Handler.Instance = TIM6;                          	//通用定时器
	  TIM6_Handler.Init.Period = arr-1;                        	//自动装载值
    TIM6_Handler.Init.Prescaler = psc-1;                     	//分频系数
    TIM6_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    	//向上计数器
    TIM6_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM6_Handler);

		HAL_TIM_Base_Stop_IT(&TIM6_Handler);	
    //HAL_TIM_Base_Start_IT(&TIM6_Handler);					//使能定时器和定时器更新中断：TIM_IT_UPDATE
}
void TIM7_Init(u16 arr, u16 psc)
{
    TIM7_Handler.Instance = TIM7;                          	//通用定时器
	  TIM7_Handler.Init.Period = arr-1;                        	//自动装载值
    TIM7_Handler.Init.Prescaler = psc-1;                     	//分频系数
    TIM7_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    	//向上计数器
    TIM7_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);

		//HAL_TIM_Base_Stop_IT(&TIM7_Handler);	
    HAL_TIM_Base_Start_IT(&TIM7_Handler);					//使能定时器和定时器更新中断：TIM_IT_UPDATE
	
		__HAL_TIM_DISABLE(&TIM7_Handler);//关TIM		
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	  if(htim->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
        HAL_NVIC_SetPriority(TIM2_IRQn, 3, 3);  //设置中断优先级，抢占优先级3，子优先级3
        HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM2中断
    }
    else if(htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();            //使能TIM6时钟
        HAL_NVIC_SetPriority(TIM6_IRQn, 1, 2);  //设置中断优先级，抢占优先级1，子优先级3
        HAL_NVIC_EnableIRQ(TIM6_IRQn);          //开启ITM6中断
    }
		else if(htim->Instance == TIM7)
    {
        __HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
        HAL_NVIC_SetPriority(TIM7_IRQn, 3, 3);  //设置中断优先级，抢占优先级3，子优先级3
        HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断
    }
}


////定时器中断服务函数 函数中处理事件
void TIM2_IRQHandler(void)
{
		if (__HAL_TIM_GET_IT_SOURCE(&TIM2_Handler, TIM_IT_UPDATE) != RESET)
		{
				TIM2_FLAG++;
			
			  LED1_ON;
				__HAL_TIM_ENABLE(&TIM7_Handler);//TIM	
				__HAL_TIM_CLEAR_IT(&TIM2_Handler, TIM_IT_UPDATE);
		}			
}

void TIM7_IRQHandler(void)
{
		if (__HAL_TIM_GET_IT_SOURCE(&TIM7_Handler, TIM_IT_UPDATE) != RESET)
		{
			  LED1_OFF;
				__HAL_TIM_ENABLE(&TIM7_Handler);//TIM		
				__HAL_TIM_CLEAR_IT(&TIM7_Handler, TIM_IT_UPDATE);
		}			
}




//定时器中断服务程序
void TIM6_IRQHandler(void)
{
		if (__HAL_TIM_GET_IT_SOURCE(&TIM6_Handler, TIM_IT_UPDATE) != RESET)
		{
				if(USART2_RX_EVENT==0)
				{
						USART2_TIME++;	
						if(USART2_TIME==20)//20ms 解包
						{		
								USART2_TIME=0;
								if(USART2_RX_LEN)
								{
										USART2_RX_EVENT=1;
										__HAL_UART_DISABLE_IT(&UART2_Handler, UART_IT_RXNE); //接收中断
									  HAL_TIM_Base_Stop_IT(&TIM6_Handler);		//串口数据接收	中断	1 2
								}
						}
				}
				__HAL_TIM_CLEAR_IT(&TIM6_Handler, TIM_IT_UPDATE);
		}			
}






////定时器中断服务函数 使用回调函数处理事件
//void TIM2_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM2_Handler);
//}
//void TIM3_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM3_Handler);
//}
//void TIM4_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM4_Handler);
//}
//void TIM5_IRQHandler(void)
//{
//    HAL_TIM_IRQHandler(&TIM5_Handler);
//}


//回调函数，定时器中断服务函数调用,这个函数占用时间长，不要使用
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim == (&TIM2_Handler))
//    {
//				TIM2_FLAG=1;
//			
//				LED_1(0);
//				TIM3->CNT=0;
//				__HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);
//				HAL_TIM_Base_Start_IT(&TIM3_Handler);	
//    }
//		if(htim == (&TIM3_Handler))
//    {
//				//TIM3_FLAG=1;

//				LED_1(1);
//				HAL_TIM_Base_Stop_IT(&TIM3_Handler);
//    }
//		if(htim == (&TIM6_Handler))
//    {
//			  //Main_printf("TIM6");
//				TIM6_FLAG=1;
//    }
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
