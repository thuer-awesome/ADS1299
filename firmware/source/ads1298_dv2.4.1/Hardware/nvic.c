#include "nvic.h"
#include "delay.h"       //延时
#include "usart.h"
#include "myQueue.h"

#include "ADS129x.h"



////中断函数，直接处理
//void EXTI4_IRQHandler(void)
//{	
//		if(__HAL_GPIO_EXTI_GET_IT(ADS129X_DRDY_PIN) &&  ADS129X_DRDY==0 )
//		{
//			
////				//__disable_irq();
////				ADS129x_Read_Data();//读n个字节的原始数据,存到缓存中  //STAT CH1 CH2 ...
////			
////				if(work_state==WRITE_TF) 
////				{		
////						queue_data_push(TF_info->TF_Queue ,ADS129x_info.ads129x_Cache+3 ,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num);				
////				}
////				
////				__HAL_GPIO_EXTI_CLEAR_IT(ADS129X_DRDY_PIN);
////				//__enable_irq();
//		}
//}


////中断函数，直接处理
//void EXTI15_10_IRQHandler(void)
//{	
//		if(__HAL_GPIO_EXTI_GET_IT(ADS129X_DRDY_PIN) &&  ADS129X_DRDY==0 )
//		{
//				//Main_printf(".");	

////				//__disable_irq();
//				ADS129x_Read_Data();//读n个字节的原始数据,存到缓存中  //STAT CH1 CH2 ...
////			
////				if(work_state==WRITE_TF) 
////				{		
////						queue_data_push(TF_info->TF_Queue ,ADS129x_info.ads129x_Cache+3 ,ADS129x_info.Ads129x_Data_Move,ADS129x_info.Ads129x_Write_Num);				
////				}
////				
//				__HAL_GPIO_EXTI_CLEAR_IT(ADS129X_DRDY_PIN);
////				//__enable_irq();
//		}
//}





////中断函数，使用回调函数处理
//void EXTI2_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);//调用中断处理公用函数	
//}

//void EXTI4_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);//调用中断处理公用函数	
//}


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//		u8 i;
//		static u8 test;
//    switch(GPIO_Pin)
//		{
//			case  GPIO_PIN_2:	
//					//ADS129x_Read_Data();//读取数据
//					//queue_push(ADS129x_Queue,ADS129x_info.ads129x_Cache ,27);//搬运数据至队列				
//			break;
//			
//			case  GPIO_PIN_4:	
//				
//			break;
//			case  GPIO_PIN_0:	
//					//LowPower_flag=1;			
//					//Main_printf("进入低功耗模式");
//					//LED_1_TogglePin;

////					BLE103_LowPower();//蓝牙休眠（需要操作weak引脚）					
////					if(Check_WKUP())//关机
////					{
////							Sys_Enter_Standby();//进入待机模式
////					}
//			break;
//			
//			
//			default:
//				break;
//		}
//}







