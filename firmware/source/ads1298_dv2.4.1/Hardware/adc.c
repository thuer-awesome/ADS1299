#include "adc.h"
#include "delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/11/05
//版本：V1.3
//新增结构体
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////

_ADC_info ADC_info;//芯片结构

ADC_HandleTypeDef ADC1_Handler;//ADC句柄

/**
 * @brief	初始化ADC函数
 * @param   void
 * @return  void
 */
 
 //单通道（可用）
//void MY_ADC_Init(void)
//{
//    ADC1_Handler.Instance = ADC1;
//    ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; //2分频，ADCCLK=SYSCLK/2=80/2=40MHZ	
//    ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;           //12位模式
//    ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;           //右对齐
//    ADC1_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;           //非扫描模式
//    ADC1_Handler.Init.EOCSelection = DISABLE;                    //关闭EOC中断
//    ADC1_Handler.Init.ContinuousConvMode = DISABLE;              //关闭连续转换
//    ADC1_Handler.Init.NbrOfConversion = 1;                       //1个转换在规则序列中 也就是只转换规则序列1
//    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;           //禁止不连续采样模式
//    ADC1_Handler.Init.NbrOfDiscConversion = 0;                   //不连续采样通道数为0
//    ADC1_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;     //软件触发
//    ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; //使用软件触发

//    ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
//    ADC1_Handler.Init.DMAContinuousRequests = DISABLE;
//    ADC1_Handler.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//    ADC1_Handler.Init.OversamplingMode = DISABLE;

//    ADC1_Handler.Init.DMAContinuousRequests = DISABLE;           //关闭DMA请求
//    HAL_ADC_Init(&ADC1_Handler);                                 //初始化
//}

//多通道轮询采集（有问题）
//void MY_ADC_Init(void)
//{
//		ADC_ChannelConfTypeDef sConfig = {0};

//    ADC1_Handler.Instance = ADC1;
//    ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; //2分频，ADCCLK=SYSCLK/2=80/2=40MHZ	
//    ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;           //12位模式
//    ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;           //右对齐
//    ADC1_Handler.Init.ScanConvMode = ADC_SCAN_ENABLE;            //扫描模式
//    ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;        //关闭EOC中断
//		ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
//    ADC1_Handler.Init.ContinuousConvMode = ENABLE;               //转换连续进行 还是单次
//    ADC1_Handler.Init.NbrOfConversion = 5;                       //5个转换在规则序列中 也就是只转换规则序列1
//    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;           //禁止不连续采样模式
//    ADC1_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;     //软件触发
//    ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; //外部触发规则
//    ADC1_Handler.Init.DMAContinuousRequests = ENABLE;						//DMA连续请求 执行单次（转换达到目标是停止传送） 还是连续模式
//		//DMA必须配置成循环模式，否则 DMAbuffer满是会触发溢出
//		
//    ADC1_Handler.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;  //溢出时的行为，保存数据或者覆盖数据，这个参数只作用于规则组，包含DAM
//    ADC1_Handler.Init.OversamplingMode = DISABLE;

//    HAL_ADC_Init(&ADC1_Handler);                                 //初始化
//		
//		
//	 /** Configure Regular Channel 
//		*/
//		sConfig.Channel = ADC_CHANNEL_6;
//		sConfig.Rank = ADC_REGULAR_RANK_1;
//		sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
//		sConfig.SingleDiff = ADC_SINGLE_ENDED;
//		sConfig.OffsetNumber = ADC_OFFSET_NONE;
//		sConfig.Offset = 0;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel 
//		*/
//		sConfig.Channel = ADC_CHANNEL_9;
//		sConfig.Rank = ADC_REGULAR_RANK_2;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel 
//		*/
//		sConfig.Channel = ADC_CHANNEL_15;
//		sConfig.Rank = ADC_REGULAR_RANK_3;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel 
//		*/
//		sConfig.Channel = ADC_CHANNEL_16;
//		sConfig.Rank = ADC_REGULAR_RANK_4;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel 
//		*/
//		sConfig.Channel = ADC_CHANNEL_VBAT;
//		sConfig.Rank = ADC_REGULAR_RANK_5;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//			
//}


//DMA 触发
//void MY_ADC_Init(void)
//{
//		ADC_ChannelConfTypeDef sConfig = {0};

//    ADC1_Handler.Instance = ADC1;
//    ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV10; //分频，同步时钟模式分频	
//    ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;           //12位模式
//    ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;           //右对齐
//    ADC1_Handler.Init.ScanConvMode = ADC_SCAN_ENABLE;           //扫描模式
//    ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;        //关闭EOC中断
//		ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
//    ADC1_Handler.Init.ContinuousConvMode = ENABLE;               //转换连续进行 还是单次
//    ADC1_Handler.Init.NbrOfConversion = 5;                       //5个转换在规则序列中 也就是只转换规则序列1
//    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;           //禁止不连续采样模式
//    ADC1_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;     //软件触发
//    ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; //外部触发规则
//    ADC1_Handler.Init.DMAContinuousRequests = ENABLE;						//DMA连续请求 执行单次（转换达到目标是停止传送） 还是连续模式
//		//DMA必须配置成循环模式，否则 DMAbuffer满是会触发溢出
//		
//    ADC1_Handler.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;  //溢出时的行为，保存数据或者覆盖数据，这个参数只作用于规则组，包含DMA
//    ADC1_Handler.Init.OversamplingMode = DISABLE;		//过采样失能

//    HAL_ADC_Init(&ADC1_Handler);                                 //初始化
//		
//		
//	 /** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_6;
//		sConfig.Rank = ADC_REGULAR_RANK_1;
//		sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
//		sConfig.SingleDiff = ADC_SINGLE_ENDED;
//		sConfig.OffsetNumber = ADC_OFFSET_NONE;
//		sConfig.Offset = 0;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_9;
//		sConfig.Rank = ADC_REGULAR_RANK_2;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_15;
//		sConfig.Rank = ADC_REGULAR_RANK_3;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_16;
//		sConfig.Rank = ADC_REGULAR_RANK_4;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_VREFINT;  //内部参考，1.2V
//		sConfig.Rank = ADC_REGULAR_RANK_5;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}

//		HAL_ADCEx_Calibration_Start(&ADC1_Handler, ADC_SINGLE_ENDED);//校准

//		//开启DMA
//		HAL_ADC_Start_DMA(&ADC1_Handler,(uint32_t *)ADCConvertedValue,5);		

//}


//定时器+DMA 触发
void MY_ADC_Init(void)
{
		ADC_ChannelConfTypeDef sConfig = {0};

    ADC1_Handler.Instance = ADC1;
    ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2; //分频，同步时钟模式分频	
    ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;           //12位模式
    ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;           //右对齐
    ADC1_Handler.Init.ScanConvMode = ADC_SCAN_ENABLE;           //扫描模式
    ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;        //关闭EOC中断
		ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
    ADC1_Handler.Init.ContinuousConvMode = DISABLE;               //转换连续进行 还是单次 定时器需要关闭
    ADC1_Handler.Init.NbrOfConversion = ADCBUF_SIZE;              //n个转换在规则序列中
    ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;           //间断模式 禁止不连续采样模式
    ADC1_Handler.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T2_CC2;   //TIM2 CC2触发
    ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING; //外部触发规则
    ADC1_Handler.Init.DMAContinuousRequests = ENABLE;						//DMA连续请求 执行单次（转换达到目标是停止传送） 还是连续模式
		//DMA必须配置成循环模式，否则 DMAbuffer满是会触发溢出
		
    ADC1_Handler.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;  //溢出时的行为，保存数据或者覆盖数据，这个参数只作用于规则组，包含DMA
    ADC1_Handler.Init.OversamplingMode = DISABLE;		//过采样失能

    HAL_ADC_Init(&ADC1_Handler);                                 //初始化
		
	 /** Configure Regular Channel*/
		sConfig.Channel = ADC_CHANNEL_16;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;//ADC_SAMPLETIME_92CYCLES_5; 
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
		{
			//Error_Handler();
		}
		
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_9;
//		sConfig.Rank = ADC_REGULAR_RANK_2;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
		
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_15;
//		sConfig.Rank = ADC_REGULAR_RANK_3;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
//		/** Configure Regular Channel*/
//		sConfig.Channel = ADC_CHANNEL_16;
//		sConfig.Rank = ADC_REGULAR_RANK_4;
//		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
//		{
//			//Error_Handler();
//		}
		/** Configure Regular Channel*/
		sConfig.Channel = ADC_CHANNEL_VREFINT;  //内部参考，1.2V
		sConfig.Rank = ADC_REGULAR_RANK_2;
		if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
		{
			//Error_Handler();
		}

		HAL_ADCEx_Calibration_Start(&ADC1_Handler, ADC_SINGLE_ENDED);//校准

		//开启DMA
		HAL_ADC_Start_DMA(&ADC1_Handler,(uint32_t *)ADC_info.ADCConvertedValue,ADCBUF_SIZE);		
		//HAL_ADC_Stop_DMA(&ADC1_Handler); //关DMA
}

/**
 * @brief	ADC底层驱动，引脚配置，时钟使能，此函数会被HAL_ADC_Init()调用
 * @param   hadc	ADC句柄
 * @return  void
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    GPIO_InitTypeDef GPIO_Initure;

    //选择ADC时钟源为SYSCLK
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

    __HAL_RCC_ADC_CLK_ENABLE();       //使能ADC1时钟
//	  __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIO时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIO时钟

//	  GPIO_Initure.Pin = GPIO_PIN_1;
//		GPIO_Initure.Mode = GPIO_MODE_ANALOG;
//		GPIO_Initure.Pull = GPIO_NOPULL;
//		HAL_GPIO_Init(GPIOA, &GPIO_Initure);
	
	
	  GPIO_Initure.Pin = GPIO_PIN_1;
		GPIO_Initure.Mode = GPIO_MODE_ANALOG;
		GPIO_Initure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);
	
//    GPIO_Initure.Pin = GPIO_PIN_0|GPIO_PIN_1;        
//    GPIO_Initure.Mode = GPIO_MODE_ANALOG;   //模拟
//    GPIO_Initure.Pull = GPIO_NOPULL;        //不带上下拉
//    HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

///**
// * @brief	获得ADC值
// * @param   ch		通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
// * @return  u16		转换结果
// */
//u16 Get_Adc(u32 ch)
//{
//    ADC_ChannelConfTypeDef ADC1_ChanConf;

//    ADC1_ChanConf.Channel = ch;                                 //通道
//    ADC1_ChanConf.Rank = ADC_REGULAR_RANK_1;					//第1个序列，序列1
//    ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;    //采样时间
//	  //ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;    //采样时间
//	
//    ADC1_ChanConf.SingleDiff = ADC_SINGLE_ENDED;				//ADC单端输入
//    ADC1_ChanConf.OffsetNumber = ADC_OFFSET_NONE;				//偏移号选择
//    ADC1_ChanConf.Offset = 0;
//    HAL_ADC_ConfigChannel(&ADC1_Handler, &ADC1_ChanConf);    //通道配置

//    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
//    HAL_ADC_PollForConversion(&ADC1_Handler,10);               //轮询转换,超时时间

//    return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//返回最近一次ADC1规则组的转换结果
//}


///**
// * @brief	获取指定通道的转换值，取times次,然后平均
// * @param   ch		通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
// * @param   times	获取次数
// * @return  u16		通道ch的times次转换结果平均值
// */
//u16 Get_Adc_Average(u32 ch, u8 times)
//{
//    u32 temp_val = 0;
//    u8 t;
//    for(t = 0; t < times; t++)
//    {
//        temp_val += Get_Adc(ch);
//        //delay_ms(5);
//    }
//    return temp_val / times;
//}
