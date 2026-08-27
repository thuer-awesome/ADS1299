#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h"



//STM32F103C8T6
//	FLASH 64K 中容量
//	SRAM	20K
//STM32F103RCT6
//	FLASH 256K 大容量
//	SRAM	48K
//STM32F103ZET6
//	FLASH 512K 大容量
//	SRAM	64K
//STM32L452CEU6
//	FLASH 512K 大容量
//	SRAM	160K 
//	80MHz
//STM32L442KC
//	FLASH 256K 大容量
//	SRAM	48+16 = 64K 
//	80MHz

//STM32F4 内部内存分为两大块： 
//1，普通内存（又分为主要内存和辅助内存，地址从：0X20000000开始，共128KB），这部分内存任何外设都可以访问。 
//2，CCM 内存（地址从：0X10000000开始，共64KB），这部分内存仅 CPU 可以访问，DMA之类的不可以直接访问，使用时得特别注意！！
//STM32F405RGT6
//	FLASH 1024K 
//	SRAM	192K 
//	168MHz
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
#define SRAMBANK 	2	//定义支持的SRAM块数.	
////////////////////////////////////////////////////////////////////////////////////////////////
//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE		32  	  						//内存块大小为32字节
#define MEM1_MAX_SIZE			40*1024  						//最大管理内存 10K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小
////////////////////////////////////////////////////////////////////////////////////////////////
////mem2内存参数设定.
//#define MEM2_BLOCK_SIZE		32  	  						//内存块大小为32字节
//#define MEM2_MAX_SIZE			60 *1024  						//最大管理内存60K
//#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
////////////////////////////////////////////////////////////////////////////////////////////////
//mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!) STM32F4独有
#define MEM3_BLOCK_SIZE		32  	  						//内存块大小为32字节
#define MEM3_MAX_SIZE			15 *1024  						//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小
////////////////////////////////////////////////////////////////////////////////////////////////	
//定义内存池
#define SRAMIN			0	//内部内存池
#define SRAMCCM  		1	//CCM内存池(对于F4此部分SRAM仅仅CPU可以访问!!!)
#define SRAMEX  		2	//外部内存池
//////////////////////////////////////////////////////////////////////////////////////////////// 
//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);							//初始化
	u8 (*perused)(u8);		  	    //内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	//在mallco.c里面定义
////////////////////////////////////////////////////////////////////////////////////////////////
void mymemset(void *s,u8 c,u32 count);	//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存     
u32 my_mem_malloc(u8 memx,u32 size);	//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);		//内存释放(内部调用)
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
u8 mem_perused(u8 memx);				//获得内存使用率(外/内部调用)  
void mem_init(u8 memx);				//内存管理初始化函数(外/内部调用)

//void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存(外部调用)



#endif






