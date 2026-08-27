#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"




#define KEY1			HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)
//#define POWER_CTRL(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET))


//#define KEY_1  0x01	
//#define KEY_2  0x02	
//#define KEY_PWR 0x04	

//#define POWER_ON  	POWER_CTRL(1)
//#define POWER_OFF		POWER_CTRL(0)


#define	KEYDOWNMINTIME		10000	//短按键时间1s
#define	KEYDOWNLONGTIME		30000	//长按键3s

#define		KEYUNKNOW			0 	//按键状态
#define		KEYSHORTDOWN	1
#define		KEYLONGDOWN		2

//#define KEY_IRQN 	EXTI1_IRQn


extern volatile u8 KeyMask; //按键标记
extern volatile u8 KeyDat;
extern u8 KEY_TYPE;

extern u8 KEY_A1;


u8 Power_ON(void);	//按键开机检测
void KEY_Init(void);	
void Key_Scan(void);
void Change_WorkMode(void);


#endif
