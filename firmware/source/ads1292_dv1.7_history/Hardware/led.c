#include "led.h"




//LED IO³õÊ¼»¯º¯Êý
void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LED1_OFF;
	 // LED_2(0);

}

void LED_GPIO_DInit(void)
{
	
}






