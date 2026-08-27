#ifndef __myQueue_H
#define __myQueue_H	 
#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/08/31
//版本：V1.7
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////

//返回值
#define QUEUE_RETURN_OK  0	//返回成功
#define QUEUE_RETURN_NULL  1	//指针无效
#define QUEUE_RETURN_EMPTY  2	//队列数据空
#define QUEUE_RETURN_FULL  3	//队列数据满了（溢出）
//#define LINKLIST_RETURN_MEN_ERR  4	//内存相关错误
#define QUEUE_RETURN_NOT_READY  5	//内存相关错误

//队列当前状态
#define QUEUE_NORMAL  0		//队列正常
#define QUEUE_FULL  1		//队列满了
#define QUEUE_EMPTY 2		//队列空


//push()方法可以在数组的末属添加一个或多个元素
//shift()方法把数组中的第一个元素删除
//unshift()方法可以在数组的前端添加一个或多个元素
//pop()方法把数组中的最后一个元素删除


//循环队列 结构体声明	
__align(4) typedef struct {
	
		u8 Queue_Full_flag;	//队列数据满标志
		u8 Queue_pop_flag;	//队列数据取出标志	1BYTE	

		u8 Queue_data_state;			//数据状态
    u32 front, rear;			//front指向队列头，rear为队列尾部
    u32 capacity,length;	//队列大小，容量
		//capacity  队列容量 （队列缓冲区数量） 
		//length		队列长度/大小 （缓存的数据长度）
	
		u16 carry_num;				//数据搬运次数，n次打包
	
	  u8 **databuf;					//二级指针 动态内存申请
}QueueInfo;	
 

/////////////////////////////////////////////////////////////////////

//外部函数
QueueInfo *queue_init(u32 capacity,u32 length);//循环队列初始化 
u8 queue_Deinit(QueueInfo *q);//循环队列注销
u8 queue_clear(QueueInfo *q);//清空队列数据


u8 queue_push_byte(QueueInfo *q, u8 byte) ;
u8 queue_push_word(QueueInfo *q, u8 word) ;

u8 queue_add(QueueInfo *q, u8 *cache,u8 add,u16 length) ;
u8 queue_push(QueueInfo *q, u8 *cache,u16 length) ;//数据放入队列

u8 queue_pop(QueueInfo *q,u8 *sendbuf,u16 length);//从队列搬运 n个字节 至发送缓冲区
u8 queue_pop_byte(QueueInfo *q,u8 *sendbuf,u16 length);//从队列搬运 n个字节 至发送缓冲区
u8 queue_pop_word(QueueInfo *q,u8 *sendbuf,u16 length);//从队列搬运 n个字节 至发送缓冲区


u8 serch_queue_data(QueueInfo *q);//检测队列是否有数据
u8 queue_data_push(QueueInfo *q, u8 *cache,u16 length,u16 times);//队列插入数据

u8 queue_add_rear(QueueInfo *q) ;
u8 queue_add_front(QueueInfo *q) ;


#endif


 
 
 
 
 
 
 
 