#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define USE_LED 1  //开启后大约0.1mA


//RGB接口定义
#if USE_LED==1
		#define LED_1(n)	(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET))
		#define LED_1_TogglePin		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8)
//		#define LED_2(n)	(n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET))
//		#define LED_2_TogglePin		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3)		
#else
	#define LED_1(n)					
	#define LED_1_TogglePin
//	#define LED_2(n)					
//	#define LED_2_TogglePin			
#endif


#define LED1_ON 	LED_1(0)
#define LED1_OFF	LED_1(1)
//#define LED2_ON		LED_2(0)
//#define LED2_OFF	LED_2(1)




void LED_GPIO_Init(void);


#endif




