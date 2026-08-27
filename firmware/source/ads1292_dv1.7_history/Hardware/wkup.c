#include "wkup.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"



RTC_HandleTypeDef RTC_Handler;  //RTC句柄

/**
 * @brief	系统进入待机模式
 */
 
void Sys_Enter_Standby(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;	
	
//	  __HAL_RCC_GPIOA_CLK_ENABLE();
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//		//设置为浮空输入
//		GPIO_InitStruct.Pin = GPIO_PIN_All;
//	  GPIO_InitStruct.Mode=GPIO_MODE_INPUT;     
//    GPIO_InitStruct.Pull = GPIO_NOPULL;				
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	

	    LED1_OFF;

		//__disable_irq(); //关总中断
		//HAL_SuspendTick(); //挂起滴答定时器
		//HAL_ResumeTick();        //释放滴答定时器
		//__enable_irq();   // 开启总中断
////////////////////////////////////////////////////////////////////////////////////////	
		//HAL_Init();
		//__HAL_RCC_AHB2_FORCE_RESET();       //复位所有IO口
////////////////////////////////////////////////////////////////////////////////////////
		__HAL_RCC_PWR_CLK_ENABLE();         //使能PWR时钟
//    __HAL_RCC_BACKUPRESET_FORCE();      //复位备份区域
//    HAL_PWR_EnableBkUpAccess();         //后备区域访问使能  
//	
//	//STM32F4,当开启了RTC相关中断后,必须先关闭RTC中断,再清中断标志位,然后重新设置
//	//RTC中断,再进入待机模式才可以正常唤醒,否则会有问题.	
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//关闭RTC写保护
//    
//    //关闭RTC相关中断，可能在RTC实验打开了
//    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
//    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
//    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
//    
//    //清除RTC相关中断标志位
//    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
//    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
//    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
//    
//    __HAL_RCC_BACKUPRESET_RELEASE();                    //备份区域复位结束
//    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //使能RTC写保护
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志
//	
//    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);           //设置WKUP用于唤醒
////////////////////////////////////////////////////////////////////////////////////////		
		//开启外部中断
		KEY_Init();
		HAL_NVIC_EnableIRQ(KEY_IRQN);					//使能充电监测			中断	2 3
////////////////////////////////////////////////////////////////////////////////////////
    //HAL_PWR_EnterSTANDBYMode();                         //进入待机模式  			
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);  //停止模式   中断唤醒  电源不进低功耗，唤醒有一点延迟
		//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);  //停止模式   中断唤醒  电源低功耗，唤醒有一点延迟
		//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFE);  //停止模式  事件唤醒	
}







///**
// * @brief	PA0 WKUP唤醒初始化
// */
//void WKUP_Init(void)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
//	
//    GPIO_Initure.Pin=GPIO_PIN_0;           //PA0
//    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //中断,上升沿
//    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
//    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//    
//		HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_A, PWR_GPIO_BIT_0);
//		HAL_PWREx_EnablePullUpPullDownConfig();

//    HAL_NVIC_SetPriority(EXTI0_IRQn,0x02,0x02);//抢占优先级2，子优先级2
//		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
//}

//u8 KEY_A0=0;
///**
// * @brief	外部中断线0中断服务函数
// */	
//void EXTI0_IRQHandler(void)
//{
//		if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) )
//		{	
//				KEY_A0=1;
//				//Main_printf("按键中断");
//				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
//		}	
//}



