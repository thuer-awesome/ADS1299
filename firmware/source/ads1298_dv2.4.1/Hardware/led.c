#include "led.h"




//LED IO³õÊ¼»¯º¯Êý
void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	  //__HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	#if USE_LED2 == 1
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	#endif
	
    LED1_OFF;
	#if USE_LED2 == 1
		LED2_OFF;	
	#endif
}

void LED_GPIO_DInit(void)
{
	
}




