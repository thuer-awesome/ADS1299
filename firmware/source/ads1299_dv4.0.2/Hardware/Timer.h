#ifndef __TIMER_H
#define __TIMER_H	 
#include "sys.h"  




extern u32 Tim1_flag;


extern u16 TIM2_FLAG;
extern u8 TIM4_Timing;



void TIM1_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void TIM4_Init(u16 arr,u16 psc);
void TIM5_Init(u16 arr,u16 psc);
void TIM6_Init(u16 arr,u16 psc);
void TIM7_Init(u16 arr,u16 psc);
void TIM8_Init(u16 arr,u16 psc);


#endif
