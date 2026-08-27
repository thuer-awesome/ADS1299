#include "sys.h"         //系统配置
#include "delay.h"       //延时
#include "led.h" 
#include "malloc.h"
#include "usart.h"       //串口 
#include "ADS129x.h"
#include "string.h"
#include "Timer.h"
#include "key.h"
#include "BG22AX.h"
#include "dma.h"
//#include "myQueue.h" //队列
#include "adc.h"
/////////////////////////////////////////////////////////////////////////////////////
//函数区		
		void Send_BlueTooth(void);//蓝牙 发送
		void Send_UART1(void);//串口 发送
		void SoftReset(void);//软件复位
		void SystemInit8 (void);
		void SystemInit16(void);
		void SystemInit24(void);
/////////////////////////////////////////////////////////////////////////////////////
//变量区
		u8 lowpower=0;//低电量指示
		u8 power_detective=0;//电量监测
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
//main
int main(void)
{		
		work_state=SEND_BULE;		//蓝牙发送
/////////////////////////////////////////////////////////////////////	
		//初始化系统时钟	 72M	
		SystemInit();	
		delay_init();		
		mem_init();			
		delay_ms(100);
/////////////////////////////////////////////////////////////////////	
	#if  LOWPOWER==1//修改系统时钟为8M，降低功耗	
		__disable_irq();//关总中断
		SystemInit16();
		__enable_irq();//开总中断			
		delay_init();	
		delay_ms(100);
		uart1_init(460800);//串口初始化为115200	
		Main_printf("降频\r\n");	
	#endif
/////////////////////////////////////////////////////////////////////		
		uart1_init(460800);//串口初始化为115200	
		Main_printf("上电\r\n");
		LED_Init();		
		KEY_Init();//初始化按键
		ADS129x_GPIO_Init();	//初始化ads129x GPIO
		BG22AX_GPIO_Init();
		delay_ms(100);
/////////////////////////////////////////////////////////////////////
		LED1 =LED_ON;	LED2 =LED_ON; 
		delay_s(2);
		LED1 =LED_OFF;LED2 =LED_OFF;
		Main_printf("蓝牙/串口发送 低功耗+电量监测 V1.2.1 适配匿名V7 500采样率\r\n");	
		Main_printf("配套ADS1299 DV4.0.2 2022-03-08\r\n");	
/////////////////////////////////////////////////////////////////////	
	#if LOWPOWER ==1//低功耗
		TIM3_Init(30000,1600);//按键检测
		TIM4_Init(10000,1600);//系统指示灯+电量监测		
	#else
		TIM3_Init(30000,7200);//按键检测
		TIM4_Init(10000,7200);//系统指示灯+电量监测		
	#endif
////////////////////////////////////////////////////////////////////		
		Main_printf("ADC 初始化\r\n");
		adc1_init();
		ADC_DMA_Config(ADCConvertedValue,1);
		ADC_DMA_Enable();	
		Main_printf("开启 DMA ADC\r\n");
////////////////////////////////////////////////////////////////////
		//初始化串口结构
		UART_Info =(_UART_Info*)mymalloc(sizeof(_UART_Info)); //数据结构
		UART_Info->sendbuf = (u8*)mymalloc(UART_SEND_LENGTH);	//发送缓冲区 
		UART_Info->recbuf = (u8*)mymalloc(UART_REC_LENGTH);	//发送缓冲区 
////////////////////////////////////////////////////////////////////	
		while(1)
		{				
				if(work_state == SEND_UART)
						Send_UART1();
				if(work_state == SEND_BULE)					
						Send_BlueTooth();
		}	
}


//蓝牙发送测试
void Send_BlueTooth(void)
{
		u8 i,j,z,k=1,res;
		u8 addcheck=0,sumcheck=0;	
		u8 datanum=0;
/////////////////////////////////////////////////////////////////////	
		ADS129x_ReInit(0);//初始化1299	
////////////////////////////////////////////////////////////////////
	#if 1	//蓝牙AT设置
		Main_printf("蓝牙设置检测\r\n");			
	#if LOWPOWER ==1//低功耗
		TIM4_Init(3000,1600);//系统指示灯+电量监测		
	#else
		TIM4_Init(3000,7200);//系统指示灯+电量监测		
	#endif
		TIM4_Timing=0;
		BG22AX_SET_FLAG=0;		
		while(BG22AX_SET_FLAG==0 && TIM4_Timing<10)	//等待3s
				BG22AX_SET();//检测蓝牙按键	
		if(BG22AX_SET_FLAG)//设置蓝牙
		{
				Main_printf("重设蓝牙\r\n");		
				res=BG22AX_AT_Confg();//开始设置		
				if(res)		
					Main_printf("蓝牙设置失败\r\n");							
				else									
					Main_printf("蓝牙设置成功\r\n");			
		}
		else
		{
				Main_printf("跳过设置\r\n");
		}
	#endif
////////////////////////////////////////////////////////////////////		
	#if LOWPOWER ==1//低功耗
		TIM4_Init(5000,1600);//系统指示灯+电量监测		
	#else
		TIM4_Init(5000,7200);//系统指示灯+电量监测		
	#endif	
////////////////////////////////////////////////////////////////////		
#if  USE_NIMING ==1	
		Main_printf("初始化蓝牙\r\n");
		BG22AX_Init();
		while(work_state==SEND_BULE)//串口发送,
		{
				Key_Scan();
				Change_WorkMode();	
				if(BG22AX_Wait_Connect()==0)
				{
						Main_printf("蓝牙成功连接\r\n");
						break;
				}
				Main_printf("等待蓝牙连接\r\n");
				delay_ms(2000);
		}
#endif	
////////////////////////////////////////////////////////////////////////		
		Main_printf("开启串口2 DMA\r\n");
		DMA_Config(DMA1_Channel7,(u32)&USART2->DR,(u32)UART_Info->sendbuf);//U2
		USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //DMA	
////////////////////////////////////////////////////////////////////////		
#if  USE_NIMING ==1
		for(i=0;i<5;i++)
		{
				UART_Info->sendbuf[0 +39*i]=0xAA;
				UART_Info->sendbuf[1 +39*i]=0xFF;
				UART_Info->sendbuf[2 +39*i]=0xF1;	
				UART_Info->sendbuf[3 +39*i]=33;//发送 8 个通道的数据 + 1字节丢包测试
		}
#endif		
////////////////////////////////////////////////////////////////////////	
	#if LOWPOWER ==1//低功耗
		TIM4_Init(10000,1600);//系统指示灯+电量监测		
	#else
		TIM4_Init(10000,7200);//系统指示灯+电量监测		
	#endif	
////////////////////////////////////////////////////////////////////
		Main_printf("开始发送\r\n");	
		power_detective=1;
		lowpower=0;
		delay_s(1);
		EXTI->IMR |= ADS129X_DRDY_LINE;//开DRDY中断		
/////////////////////////////////////////////////////////////////////////////////////////////
		while(work_state==SEND_BULE)//串口发送,
		{
				Key_Scan();
				Change_WorkMode();	
///////////////////////////////////////////////////////////////////////////////////////////		
			#if  USE_NIMING ==1
				if(ADS129x_info.Ads129x_DRDY_flag)
				{
						ADS129x_info.Ads129x_DRDY_flag=0;
					
						for(z=0;z<ADS129X_USE_CANNLE;z++)//8通道
						{
								ADS129x_info.cannle[z] =	
																	ADS129x_info.ads129x_Cache[3+z*3]<<16 //获取原始数据
																| ADS129x_info.ads129x_Cache[4+z*3]<<8 
																| ADS129x_info.ads129x_Cache[5+z*3];
							
								ADS129x_info.p_Temp[z] = get_volt(ADS129x_info.cannle[z]);	//转成有符号数
								ADS129x_info.cannle[z] = ADS129x_info.p_Temp[z];//赋值给无符号数，准备做逻辑右移
								//小端在前
								UART_Info->sendbuf[4 +z*4 +39*datanum] = ADS129x_info.cannle[z] >>0;
								UART_Info->sendbuf[5 +z*4 +39*datanum] = ADS129x_info.cannle[z] >>8;
								UART_Info->sendbuf[6 +z*4 +39*datanum] = ADS129x_info.cannle[z] >>16;
								UART_Info->sendbuf[7 +z*4 +39*datanum] = ADS129x_info.cannle[z] >>24;
						}
						UART_Info->sendbuf[36+39*datanum] = k++;	//递增数据（丢包测试）
						sumcheck = 0;	
						addcheck=0;
						for(i=0;i<37;i++)//校验
						{
								sumcheck += UART_Info->sendbuf[i+39*datanum];	
								addcheck += sumcheck;
						}	
						UART_Info->sendbuf[37+39*datanum] = sumcheck;	//校验和		
						UART_Info->sendbuf[38+39*datanum] = addcheck;	//校验和	
						
						datanum++;
						if(datanum==5)
						{
								datanum=0;
								DMA_Enable(DMA1_Channel7,195);//开启DMA
						}
				}
			#endif
		}
/////////////////////////////////////////////////////////////////////////////////////////////		
		EXTI->IMR &= ~(ADS129X_DRDY_LINE);//屏蔽外部中断		
		TIM_Cmd(TIM4, DISABLE);
		power_detective=0;	
		USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE); //DMA	
		
}



//串口发送测试
void Send_UART1(void)
{
		u8 i,j,z,k=1,res;
		u8 addcheck=0,sumcheck=0;	
/////////////////////////////////////////////////////////////////////	
		ADS129x_ReInit(0);//初始化1299			
////////////////////////////////////////////////////////////////////
		//开启串口DMA
		Main_printf("开启串口1 DMA\r\n");
		DMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)UART_Info->sendbuf);//U1
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //DMA	
		UART1_DMA_TX_NVIC_Config(1);
////////////////////////////////////////////////////////////////////////	
#if  USE_NIMING ==1
		UART_Info->sendbuf[0]=0xAA;
		UART_Info->sendbuf[1]=0xFF;
		UART_Info->sendbuf[2]=0xF1;	
		UART_Info->sendbuf[3]=33;//发送 8 个通道的数据 + 1字节丢包测试
#endif			
////////////////////////////////////////////////////////////////////////	
	#if LOWPOWER ==1//低功耗
		TIM4_Init(10000,1600);//系统指示灯+电量监测		
	#else
		TIM4_Init(10000,7200);//系统指示灯+电量监测		
	#endif	
////////////////////////////////////////////////////////////////////	
		printf("开始发送\r\n");	
		power_detective=1;
		lowpower=0;
		UART1_DMA_Finish=1;
		delay_s(1);
		EXTI->IMR |= ADS129X_DRDY_LINE;//开DRDY中断	
////////////////////////////////////////////////////////////////////////	
		while(work_state==SEND_UART)
		{
				Key_Scan();
				Change_WorkMode();	
///////////////////////////////////////////////////////////////////////////////////////////		
			#if  USE_NIMING ==1
				if(ADS129x_info.Ads129x_DRDY_flag)
				{
						if(UART1_DMA_Finish == 1)
						{
								UART1_DMA_Finish=0;
								ADS129x_info.Ads129x_DRDY_flag=0;
	
								for(z=0;z<ADS129X_USE_CANNLE;z++)//每个芯片8通道
								{
										ADS129x_info.cannle[z] =	
																			ADS129x_info.ads129x_Cache[3+z*3]<<16 //获取原始数据
																		| ADS129x_info.ads129x_Cache[4+z*3]<<8 
																		| ADS129x_info.ads129x_Cache[5+z*3];
									
										ADS129x_info.p_Temp[z] = get_volt(ADS129x_info.cannle[z]);	//转成有符号数
										ADS129x_info.cannle[z] = ADS129x_info.p_Temp[z];//赋值给无符号数，准备做逻辑右移
										//小端在前
										UART_Info->sendbuf[4 +z*4 ] = ADS129x_info.cannle[z] >>0;
										UART_Info->sendbuf[5 +z*4 ] = ADS129x_info.cannle[z] >>8;
										UART_Info->sendbuf[6 +z*4 ] = ADS129x_info.cannle[z] >>16;
										UART_Info->sendbuf[7 +z*4 ] = ADS129x_info.cannle[z] >>24;
								}
								UART_Info->sendbuf[36] = k;	//测试数据		
								//校验
								for(i=0;i<37;i++)
								{
										sumcheck += UART_Info->sendbuf[i];	
										addcheck += sumcheck;
								}	
								UART_Info->sendbuf[37] = sumcheck;	//校验和		
								UART_Info->sendbuf[38] = addcheck;	//校验和	
								sumcheck = 0;	
								addcheck=0;
								k++;//递增数据（丢包测试）
								DMA_Enable(DMA1_Channel4,39);//开启DMA
						}
				}
			#endif						
		}
/////////////////////////////////////////////////////////////////////////////////////////////		
		EXTI->IMR &= ~(ADS129X_DRDY_LINE);//屏蔽外部中断		
		TIM_Cmd(TIM4, DISABLE); 
		power_detective=0;	
		USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE); //DMA	
}




void SoftReset(void)//软件复位
{
		//__disable_irq()只是禁止CPU去响应中断，没有真正的去屏蔽中断的触发
		//__enable_irq()开启中断后，由于相应的中断标志没有清空，因而还会触发中断
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}


//修改HCLK时钟为8M
void SystemInit8 (void)
{
	  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
	
		RCC->CR |= (uint32_t)0x00000001;		//打开HSI(内部高速时钟8M)
		RCC->CFGR &= (uint32_t)0xF0FF0000;	//RCC_CFGR寄存器初始化									
		RCC->CR &= (uint32_t)0xFEF6FFFF;		//将RCC_CR寄存器HSEON,CSSON,PLLON位置0
		RCC->CR &= (uint32_t)0xFFFBFFFF;		//将RCC_CR寄存器HSEBYP位置0
		RCC->CFGR &= (uint32_t)0xFF80FFFF;	//将RCC_CFGR寄存器PLLSRC, PLLXTPRE, 	
		RCC->CIR = 0x009F0000;		//关闭所有的中断和对应的位(初始化中断) 
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);	//使能HSE(外部高速时钟)
		do//等待 HSE 启动稳定，并做超时处理
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		//判断HSE是否就绪  
		if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;
		else			HSEStatus = (uint32_t)0x00;  	
		if (HSEStatus == (uint32_t)0x01)// HSE 启动成功，则继续往下处理
		{
				// SYSCLK 周期与闪存访问时间的比例设置			
				FLASH->ACR |= FLASH_ACR_PRFTBE;//使能 FLASH 预存取缓冲区
				FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);//Flash 0等待状态		
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;	// 0： 0 < SYSCLK <= 24M// 1： 24< SYSCLK <= 48M			// 2： 48< SYSCLK <= 72M 				    

							
				RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;    /* HCLK = SYSCLK */      
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;    /* PCLK2 = HCLK */ 
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;    /* PCLK1 = HCLK */
				
				//选择HSE作为系统时钟源
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
				RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;    
				//等待HSE作为系统时钟源
				while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04)
				{
				}
				SystemCoreClockUpdate();//更新时钟		
		}
		else		
			NVIC_SystemReset(); // 复位
}

void SystemInit16(void)
{
		__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
		RCC->CR |= (uint32_t)0x00000001;		//打开HSI(内部高速时钟8M)
		RCC->CFGR &= (uint32_t)0xF0FF0000;	//RCC_CFGR寄存器初始化									
		RCC->CR &= (uint32_t)0xFEF6FFFF;		//将RCC_CR寄存器HSEON,CSSON,PLLON位置0
		RCC->CR &= (uint32_t)0xFFFBFFFF;		//将RCC_CR寄存器HSEBYP位置0
		RCC->CFGR &= (uint32_t)0xFF80FFFF;	//将RCC_CFGR寄存器PLLSRC, PLLXTPRE, 	
		RCC->CIR = 0x009F0000;		//关闭所有的中断和对应的位(初始化中断) 
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);	//使能HSE(外部高速时钟)
		do//等待 HSE 启动稳定，并做超时处理
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

		//判断HSE是否就绪  
		if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;	 //就绪后赋值标志位HSEStatus
		else  HSEStatus = (uint32_t)0x00;
		if (HSEStatus == (uint32_t)0x01)	// HSE 启动成功，则继续往下处理
		{
				// SYSCLK 周期与闪存访问时间的比例设置
				FLASH->ACR |= FLASH_ACR_PRFTBE;//使能 FLASH 预存取缓冲区
				FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0; 

				//设置 AHB、 APB2、 APB1 预分频因子
				RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;	  //配置AHB预分频器分频系数为1,使HCLK = SYSCLK=72M     
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;  //配置PCLK2预分频器分频系数为1,使PCLK2 = HCLK=72M  
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;	//设置PCLK1预分频器分频系数为2,使PCLK1 = HCLK/2=36M
				
				//设置 PLL 时钟来源，设置 PLL 倍频因子 PLLCLK = HSE * 2 = 16 MHz
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |RCC_CFGR_PLLMULL));
				RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL2);
				RCC->CR |= RCC_CR_PLLON;	//RCC_CR_PLLON=0x01000000  //使能PLL时钟,RCC->CR寄存器第24位置1	

				//等待PLL时钟源就绪  
				while((RCC->CR & RCC_CR_PLLRDY) == 0)	//RCC_CR_PLLRDYRCC->CR寄存器第25位  
				{
				}
				
				/* Select PLL as system clock source */	//选择PLL作为系统时钟来源
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	//RCC_CFGR_SW_PLL=0x00000002(RCC->CFGR第1,0位设置为10,见手册)
				RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

				//等待PLL时钟源就绪  
				while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
				{
				}
				
				SystemCoreClockUpdate();//更新时钟		
		}
}

static void SystemInit24(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
		RCC->CR |= (uint32_t)0x00000001;		//打开HSI(内部高速时钟8M)
		RCC->CFGR &= (uint32_t)0xF0FF0000;	//RCC_CFGR寄存器初始化									
		RCC->CR &= (uint32_t)0xFEF6FFFF;		//将RCC_CR寄存器HSEON,CSSON,PLLON位置0
		RCC->CR &= (uint32_t)0xFFFBFFFF;		//将RCC_CR寄存器HSEBYP位置0
		RCC->CFGR &= (uint32_t)0xFF80FFFF;	//将RCC_CFGR寄存器PLLSRC, PLLXTPRE, 	
		RCC->CIR = 0x009F0000;		//关闭所有的中断和对应的位(初始化中断) 
		RCC->CR |= ((uint32_t)RCC_CR_HSEON);	//使能HSE(外部高速时钟)
		do//等待 HSE 启动稳定，并做超时处理
		{
			HSEStatus = RCC->CR & RCC_CR_HSERDY;
			StartUpCounter++;  
		} while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));


		//判断HSE是否就绪  
		if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;	 //就绪后赋值标志位HSEStatus
		else  HSEStatus = (uint32_t)0x00;
		if (HSEStatus == (uint32_t)0x01)	// HSE 启动成功，则继续往下处理
		{
				// SYSCLK 周期与闪存访问时间的比例设置
				FLASH->ACR |= FLASH_ACR_PRFTBE;//使能 FLASH 预存取缓冲区
				FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0; 

				//设置 AHB、 APB2、 APB1 预分频因子
				RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;	  //配置AHB预分频器分频系数为1,使HCLK = SYSCLK=72M     
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;  //配置PCLK2预分频器分频系数为1,使PCLK2 = HCLK=72M  
				RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;	//设置PCLK1预分频器分频系数为2,使PCLK1 = HCLK/2=36M
				
				//设置 PLL 时钟来源，设置 PLL 倍频因子 PLLCLK = (HSE / 2) * 6 = 24 MHz */
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |RCC_CFGR_PLLMULL));
				RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE  | RCC_CFGR_PLLXTPRE_HSE_Div2 | RCC_CFGR_PLLMULL6);
				RCC->CR |= RCC_CR_PLLON;	//RCC_CR_PLLON=0x01000000  //使能PLL时钟,RCC->CR寄存器第24位置1	

				//等待PLL时钟源就绪  
				while((RCC->CR & RCC_CR_PLLRDY) == 0)	//RCC_CR_PLLRDYRCC->CR寄存器第25位  
				{
				}
				/* Select PLL as system clock source */	//选择PLL作为系统时钟来源
				RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	//RCC_CFGR_SW_PLL=0x00000002(RCC->CFGR第1,0位设置为10,见手册)
				RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

				//等待PLL时钟源就绪  
				while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
				{
				}
				SystemCoreClockUpdate();//更新时钟		
		}
}



void SystemInit72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* Enable HSE */  //使能HSE，需要等待晶振稳定		RCC_CR_HSEON=0x00010000(修改的是第16位HSEON)  
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);		
  do	//等待 HSE 启动稳定，并做超时处理
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;	 //开启并就绪:RCC_CR_HSERDY=0x00020000(第17位HSERDY置1)
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));	 //循环直到HSE稳定

	//判断HSE是否就绪  
  if ((RCC->CR & RCC_CR_HSERDY) != RESET)		HSEStatus = (uint32_t)0x01;	 //就绪后赋值标志位HSEStatus
  else  HSEStatus = (uint32_t)0x00;
  if (HSEStatus == (uint32_t)0x01)	// HSE 启动成功，则继续往下处理
  {
		// SYSCLK 周期与闪存访问时间的比例设置
    FLASH->ACR |= FLASH_ACR_PRFTBE;//使能 FLASH 预存取缓冲区
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2; 

		//设置 AHB、 APB2、 APB1 预分频因子
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;	  //配置AHB预分频器分频系数为1,使HCLK = SYSCLK=72M     
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;  //配置PCLK2预分频器分频系数为1,使PCLK2 = HCLK=72M  
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;	//设置PCLK1预分频器分频系数为2,使PCLK1 = HCLK/2=36M
		
    //设置 PLL 时钟来源，设置 PLL 倍频因子 PLLCLK = HSE * 9 = 72 MHz
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
    RCC->CR |= RCC_CR_PLLON;	//RCC_CR_PLLON=0x01000000  //使能PLL时钟,RCC->CR寄存器第24位置1	

    //等待PLL时钟源就绪  
    while((RCC->CR & RCC_CR_PLLRDY) == 0)	//RCC_CR_PLLRDYRCC->CR寄存器第25位  
    {
		}
    
    /* Select PLL as system clock source */	//选择PLL作为系统时钟来源
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	//RCC_CFGR_SW_PLL=0x00000002(RCC->CFGR第1,0位设置为10,见手册)
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    //等待PLL时钟源就绪  
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
		
		SystemCoreClockUpdate();//更新时钟		
  }
}




/**********************************************************************
编译结果里面的几个数据的意义：
Code：表示程序所占用 FLASH 的大小（FLASH）
RO-data：即 Read Only-data， 表示程序定义的常量，如 const 类型（FLASH）
RW-data：即 Read Write-data， 表示已被初始化的全局变量（SRAM）
ZI-data：即 Zero Init-data， 表示未被初始化的全局变量(SRAM)
***********************************************************************/


//重映射的方式一共有三种。分别描述如下：
//1.GPIO_Remap_SWJ_JTAGDisable： /*!< JTAG-DP Disabled and SW-DP Enabled */  即能用PB3，PB4，PA15做普通IO，PA13&14用于SWD调试
//2.GPIO_Remap_SWJ_Disable：  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */  5个引脚全为普通引脚，但不能再用JTAG&SWD仿真器调试，只能用st-link调试 
//3.GPIO_Remap_SWJ_NoJTRST： /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */PB4可为普通IO口，JTAG&SWD正常使用，但JTAG没有复位
//如果你用到所有的五个引脚当做普通IO口，那么上述步骤二中的重映射配置应写为GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); 
//如果你用PB3，PB4，PA15做普通IO，PA13&14用于SWD调试，则重映射配置应写为GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
//同理可配置只用PB4可为普通IO口的情况。


