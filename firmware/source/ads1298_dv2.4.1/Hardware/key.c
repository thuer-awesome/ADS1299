#include "key.h"
#include "delay.h"
#include "led.h"
#include "Timer.h"
#include "usart.h"


//按键及电源控制初始化
void KEY_Init(void) //IO初始化
{ 
		 GPIO_InitTypeDef GPIO_InitStruct;
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	  //__HAL_RCC_GPIOB_CLK_ENABLE();
	  //__HAL_RCC_GPIOC_CLK_ENABLE();

//		//KEY PWR
//		GPIO_InitStruct.Pin = GPIO_PIN_15;
//	  GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //输入
//    GPIO_InitStruct.Pull = GPIO_PULLUP;			 //s拉
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
//	
//		//PWR_CTRL
//	  GPIO_InitStruct.Pin = GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
		//KEY
		GPIO_InitStruct.Pin = GPIO_PIN_8;
	  GPIO_InitStruct.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_InitStruct.Pull = GPIO_PULLUP;			 //s拉
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
//		//KEY 充电检测
//		GPIO_InitStruct.Pin=GPIO_PIN_1;           //PA8
//    GPIO_InitStruct.Mode=GPIO_MODE_IT_RISING_FALLING;  //中断,双边沿
//    GPIO_InitStruct.Pull=GPIO_PULLDOWN;        //下拉
//    GPIO_InitStruct.Speed=GPIO_SPEED_FAST;     //快速
//    HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);

//    HAL_NVIC_SetPriority(KEY_IRQN,3,3);//抢占优先级2，子优先级2
//		//HAL_NVIC_EnableIRQ(KEY_IRQN);					//使能充电监测			中断	2 3
//		HAL_NVIC_DisableIRQ(KEY_IRQN);

}



//u8 KEY_A1=0;

////外部中断线0中断服务函数
//// 刚从停机唤醒，由于时钟未配置正确，
//void EXTI1_IRQHandler(void)
//{
//		if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) )
//		{	
//				KEY_A1=1;
//				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
//				NVIC_SystemReset(); // 复位
//		}	
//}




////开机检测
//u8 Power_ON(void)
//{
//		u8 ret=0;
//		if (KEY1 == 0)//按键按下
//		{		
//				LED1_ON;
//				while( KEY1 == 0)
//				{
//						delay_ms(10);
//						ret++;
//						if ( ret > 150 )//1.5s
//						{
//								POWER_ON;
//								LED2_ON;
//								//while( KEYPWR == 0);
//								//LED2=LED_OFF;
//								return 0;//开机成功
//						}
//				}
//				delay_ms(100);			
//				LED1_OFF;
//		}
//		return 1;	//开机失败
//}


volatile u8 KeyMask=0;//按键标记
volatile u8 KeyDat=0;
u8 KEY_TYPE=0;
void Key_Scan(void)
{
	if ( KEY1 == 0)//KEY1按下
	{		
			if(KeyDat & 0x01)//检查按键按下标志位
			{
					if(TIM7->CNT >= KEYDOWNMINTIME)//延时大于1s（按键有效）
					{
							if(TIM7->CNT >= KEYDOWNLONGTIME)//延时大于2s（长按键）
							{		
									KeyMask=KEYLONGDOWN;
									LED1_ON;//按键有效								
							}								
							else//短按键事件	
							{				
									KeyMask=KEYSHORTDOWN; 	
									LED1_OFF;//按键有效								
							}
					}	
			}
			else//按键按下瞬间
			{						
					__HAL_TIM_DISABLE(&TIM1_Handler);//定时器		
					__HAL_TIM_ENABLE(&TIM7_Handler);//使能TIM	
					TIM7->CNT=0;
					LED1_ON;//按键按下瞬间
					KeyDat |= 0x01;	//首次按键标记
					KeyMask=KEYUNKNOW ;//瞬间按下为无效状态				
			}
	}	
	else//无按下按下，或者按键弹起
	{
			if(KeyDat & 0x01)	//按键按下后弹起
			{	
					KeyDat &= ~0x01;//清按键标志
		
					__HAL_TIM_ENABLE(&TIM1_Handler);//定时器		
					__HAL_TIM_DISABLE(&TIM7_Handler);//关定时器		
					TIM7->CNT=0;//清空定时器计数器	
					LED1_OFF;
					if(KeyMask==KEYSHORTDOWN )//短按键事件
					{				
							KeyMask=KEYUNKNOW;//清变量		
							KEY_TYPE		= KEYSHORTDOWN; 				
					}					
					else if(KeyMask==KEYLONGDOWN)//长按键事件
					{					
							KeyMask=KEYUNKNOW;//清变量
							KEY_TYPE		= KEYLONGDOWN; 				
					}
			}		
	}
}
	
void Change_WorkMode(void)
{
		if(KEY_TYPE)
		{
				KEY_TYPE=0;
				work_state++;
				if(work_state == 3)
						work_state=1;
				//Main_printf("切换 工作模式%d\r\n",work_state);
		}
}

