#ifndef __TIMER_H
#define __TIMER_H	 
#include "sys.h"  



extern TIM_HandleTypeDef TIM1_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM4_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM5_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM6_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM7_Handler;      //定时器句柄


extern u16 TIM1_FLAG;
extern u16 TIM2_FLAG;
extern u16 TIM3_FLAG;
extern u16 TIM4_FLAG;
extern u16 TIM5_FLAG;
extern u16 TIM6_FLAG;


extern u16 TIM1_Timing;
extern u16 TIM2_Timing;
extern u16 TIM3_Timing;
extern u16 TIM4_Timing;
extern u16 TIM5_Timing;
extern u16 TIM6_Timing;


void TIM1_Init(u16 arr, u16 psc);
void TIM2_Init(u16 arr, u16 psc);
void TIM3_Init(u16 arr, u16 psc);
void TIM4_Init(u16 arr, u16 psc);
void TIM5_Init(u16 arr, u16 psc);
void TIM6_Init(u16 arr, u16 psc);
void TIM7_Init(u16 arr, u16 psc);



#endif
