#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED1 PBout(5)	
#define LED2 PBout(6)	
//#define LED3 PBout(8)	

#define DS1 LED1
#define DS2 LED2
//#define DS3 LED3

#define LED_1 LED1
#define LED_2 LED2
//#define LED_3 LED3

#define LED_R LED1
#define LED_G LED2
//#define LED_B LED3


#define LED_ON 0
#define LED_OFF 1



void LED_Init(void);//≥ı ºªØ

		 				    
#endif
