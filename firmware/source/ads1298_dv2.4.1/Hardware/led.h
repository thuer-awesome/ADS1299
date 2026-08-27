#ifndef _LED_H
#define _LED_H
#include "sys.h"
#include "ADS129x.h"


#define USE_LED1 1  //开启后大约0.1mA
#define USE_LED2 0  //开启后大约0.1mA




	#if USE_LED1==1
		#define LED_1(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))
		#define LED_1_TogglePin		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0)
		#define LED1_ON 	LED_1(0)
		#define LED1_OFF	LED_1(1)
	#else
		#define LED_1(n)					
		#define LED_1_TogglePin
	#endif
	
	#if USE_LED2 ==1
		#define LED_2(n)	(n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET))
		#define LED_2_TogglePin		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5)		
		#define LED2_ON		LED_2(0)
		#define LED2_OFF	LED_2(1)
	#else
		#define LED_2(n)					
		#define LED_2_TogglePin
	#endif





void LED_GPIO_Init(void);


#endif




