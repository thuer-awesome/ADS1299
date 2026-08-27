#ifndef __myLinklist_H
#define __myLinklist_H	 
#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/09/19
//版本：V1.8
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



#define LINKLIST_MAX_LEN   10			//链表最大长度


#define LINKLIST_RETURN_OK  0			//返回成功
#define LINKLIST_RETURN_NULL  1		//指针无效
#define LINKLIST_RETURN_EMPTY  2	//链表空
#define LINKLIST_RETURN_FULL  3		//链表满
#define LINKLIST_RETURN_MEN_ERR  4	//内存相关错误

/////////////////////////////////////////////////////////////////////
//自定义数据域（数据结构）
__align(4)  typedef struct
{
		u16 ID;				//编号
 		u16 data_len;	//数据长度
		u8* databuf;	//数据缓存区
}my_LinkList_datainf; 
//extern my_LinkList_datainf  *USER_LinkList_cache; //用户自定义数据 临时数据缓冲区
/////////////////////////////////////////////////////////////////////
//链表 结构体声明
__align(4) typedef struct  LinkList
{
		u8  LinkList_PopBusy_flag;	//写入占用

		u8  LinkList_Full_flag;		//达到最大长度
		u16 LinkList_maxlegth;		//最大表长

	
		u16 LinkList_legth;					//当前表长
		my_LinkList_datainf *my_data_inf; 		//数据存放区域
	  struct LinkList *next ;			//指向下一级
}my_LinkList_Inf;
extern my_LinkList_Inf  *UART2_LinkList; //链表指针
//extern my_LinkList_Inf  *UART1_LinkList; //链表指针

/////////////////////////////////////////////////////////////////////


//内部函数
//static u8 LinkList_copy_datainf(my_LinkList_Inf *p,my_LinkList_datainf *data);

static u8 LinkList_datainf_mem(u8 memx,my_LinkList_datainf *my_data_inf, u16 len);
static u8 LinkList_datainf_free(u8 memx,my_LinkList_datainf *my_data_inf);

//外部函数
my_LinkList_Inf* LinkList_Init(u8 memx);
u8 LinkList_Deinit(u8 memx,my_LinkList_Inf* LinkList);

u8 LinkList_Delete_all(u8 memx,my_LinkList_Inf* LinkList);
u8  LinkList_Delete_head(u8 memx,my_LinkList_Inf* LinkList);

u8 LinkList_Add_Area(u8 memx,my_LinkList_Inf* LinkList,my_LinkList_datainf *datainf);
u8 LinkList_Add_Area2(u8 memx,my_LinkList_Inf* LinkList,u8 *datacache,u16 len);

u8 LinkList_Serch_Data(my_LinkList_Inf* LinkList);




#endif


 
 
 
 
 
 
 
 