#include "sys.h"


u8 work_state;		//工作状态


void ALL_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
	
	  __HAL_RCC_GPIOA_CLK_ENABLE();  
    __HAL_RCC_GPIOB_CLK_ENABLE();  
    __HAL_RCC_GPIOC_CLK_ENABLE();  

    GPIO_Initure.Pin=GPIO_PIN_All ;	
    GPIO_Initure.Mode=GPIO_MODE_ANALOG; //全部模拟输入 
    GPIO_Initure.Pull=GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA,&GPIO_Initure);	
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);	
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}



//内部时钟 16M  /1 * 4 /8  = 8M
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;		// 16M/1=16M
	
  //RCC_OscInitStruct.PLL.PLLN = 10;	// 16M*10=160M
	RCC_OscInitStruct.PLL.PLLN = 4;	// 16M*4=64M

  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;	//这个没有用到
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;	//这个没有用到
	
  //RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;	//160M/2=80M
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV8;	//64M/8=8M
	
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

////内部时钟 16M  /1 * 4 /4  = 16M
//void SystemClock_Config(void)
//{
//RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLM = 1;		// 16M/1=16M
//	
//  //RCC_OscInitStruct.PLL.PLLN = 10;	// 16M*10=160M
//	RCC_OscInitStruct.PLL.PLLN = 4;	// 16M*4=64M

//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;	//这个没有用到
//  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;	//这个没有用到
//	
//  //RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;	//160M/2=80M
//	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;	//64M/4=16M
//	
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure the main internal regulator output voltage 
//  */
//  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}

////内部时钟 16M   /1 * 10 /2  = 80M
//void SystemClock_Config(void)
//{
//RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//  RCC_OscInitStruct.PLL.PLLM = 1;		// 16M/1=16M
//  RCC_OscInitStruct.PLL.PLLN = 10;	// 16M*10=160M

//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;	//这个没有用到
//  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;	//这个没有用到
//  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;	//160M/2=80M
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB busses clocks 
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure the main internal regulator output voltage 
//  */
//  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}





/**
 * @brief	THUMB指令不支持汇编内联、
 *			采用如下方法实现执行汇编指令WFI 
 *
 * @param   void
 *
 * @return  __asm
 */
__asm void WFI_SET(void)
{
	WFI;		  
}
/**
 * @brief	关闭所有中断(但是不包括fault和NMI中断)
 *
 * @param   void
 *
 * @return  __asm
 */
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
/**
 * @brief	开启所有中断
 *
 * @param   void
 *
 * @return  __asm
 */
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
/**
 * @brief	设置栈顶地址
 *
 * @param   addr	栈顶地址
 *
 * @return  __asm
 */
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}




