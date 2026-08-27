#include "myLinklist.h"
#include "malloc.h"
#include <string.h>
#include "usart.h"	 


/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/09/19
//版本：V1.8
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////


//版本 1.1-1.4 
/*基本功能函数的创建*/  
//版本 1.5  
/*新增临时数据缓存内存区域选择，创建和销毁的函数*/  
//版本 1.6  
/*修改临时数据缓存区的一些BUG,解决；内存泄露问题
新增链表拓展的函数 LinkList_Add_Area2 方便存放数组数据*/  
//版本 1.7  
/*增加内存区域选择*/  
//版本 1.8 
/*链表数据缓存区末尾补0*/  


/*
链表是一系列的存储数据元素的单元通过指针串接起来形成的，因此每个单元至少有两个域，
一个域用于数据元素的存储，另一个或两个域是指向其他单元的指针。这里具有一个数据域
和多个指针域的存储单元通常称为节点（node）。
链表的第一个节点和最后一个节点，分别称为链表的头节点和尾节点。
尾节点的特征是其 next 引用为空（null）。
链表中每个节点的 next 引用都相当于一个指针，指向另一个节点，借助这些 next 引用，
我们可以从链表的头节点移动到尾节点。
链表数据结构中主要包含单向链表、双向链表及循环链表：*/



my_LinkList_Inf  *UART2_LinkList = NULL; //链表指针
//my_LinkList_Inf  *UART1_LinkList = NULL; //链表指针
//////////////////////////////////////////////////////////////////////////////////////
//动态区链表初始化  创建头结点
//成功返回指针
my_LinkList_Inf* LinkList_Init(u8 memx)
{
		my_LinkList_Inf *p;//定义一个临时变量指向表头

		p = (my_LinkList_Inf*)mymalloc(memx,sizeof(my_LinkList_Inf));
		if(p == NULL) 
		{
				return NULL;
		}
		p->LinkList_PopBusy_flag=0;	//写入占用
		p->LinkList_Full_flag=0;
		p->LinkList_maxlegth = LINKLIST_MAX_LEN; //最大容量
		//同时要标记下一级为空
		p->my_data_inf = NULL;
		p->next = NULL;
		return p;
}


////删除整个链表，也是传参链表的头结点，这里同样借助一个指针来逐个释放节点，从头结点开始
////把头结点赋给新节点，每次将头结点往后移一位，删除节点，相当于删除了原来的头结点
//u8 LinkList_Deinit(my_LinkList_Inf* LinkList)
//{
//   	my_LinkList_Inf *p;//定义一个临时变量
//		if(LinkList == NULL ) //链表无效
//		{
//				return LINKLIST_RETURN_NULL;
//		}
//    while(LinkList != NULL) //非空
//    {
//        p=LinkList;//指向头结点
//        LinkList = LinkList->next;//头结点指向下一个节点
//				////////////////////////////////////////////////////////////
//				//销毁当前这个节点
//				p->next = NULL;
//				LinkList_datainf_free(SRAMIN,p->my_data_inf);
//        myfree(SRAMIN,p); //释放链表第一个节点
//    }
//		LinkList->LinkList_legth=0;
//		LinkList->LinkList_PopBusy_flag=0; //链表空闲
//		
//		return LINKLIST_RETURN_OK;
//}

//删除整个链表，也是传参链表的头结点，这里同样借助一个指针来逐个释放节点，从头结点开始
//把头结点赋给新节点，每次将头结点往后移一位，删除节点，相当于删除了原来的头结点
u8 LinkList_Deinit(u8 memx,my_LinkList_Inf* LinkList)
{
   	my_LinkList_Inf *p;//定义一个临时变量
		if(LinkList == NULL ) //链表无效
		{
				return LINKLIST_RETURN_NULL;
		}
    //链表下一级有数据销毁，没有数据则退出（）
		while(LinkList_Serch_Data(LinkList)==LINKLIST_RETURN_OK)
		{
				LinkList_Delete_head(memx,LinkList);//销毁
		}
		myfree(memx,LinkList);//释放内存

		return LINKLIST_RETURN_OK;
}


//释放链表所有数据节点的数据，只保留头节点
u8 LinkList_Delete_all(u8 memx,my_LinkList_Inf* LinkList)
{
		if(LinkList == NULL ) //链表无效
		{
				return LINKLIST_RETURN_NULL;
		}
		//链表下一级有数据销毁，没有数据则退出（）
		while(LinkList_Serch_Data(LinkList)==LINKLIST_RETURN_OK)
		{
				LinkList_Delete_head(memx,LinkList);//销毁
		}
		LinkList->LinkList_legth=0;
		LinkList->LinkList_PopBusy_flag=0; //链表空闲
		
		return LINKLIST_RETURN_OK;
}

//取完链表第一个数据节点的数据后调用此函数
//释放链表第一个数据节点，并使链表衔接下一个节点
u8  LinkList_Delete_head(u8 memx,my_LinkList_Inf* LinkList)
{	
		my_LinkList_Inf *p = LinkList;//定义一个临时变量指向表头
		my_LinkList_Inf *q = LinkList;
		if(p == NULL ) //链表无效
		{
				return LINKLIST_RETURN_NULL;
		}
		if(p->next == NULL) //链表无数据
		{
				return LINKLIST_RETURN_EMPTY; 
		}
		////////////////////////////////////////////////////////////
    p = p->next;	//头结点指向链表第 1 个节点，之后会销毁这个
		q->next = p->next;//头结点指向链表第 1+1 个节点，头结点与其衔接
		////////////////////////////////////////////////////////////
		//销毁当前这个节点
		p->next=NULL;
		LinkList_datainf_free(memx,p->my_data_inf); //释放链表第 1 个节点数据域数组
		myfree(memx,p->my_data_inf); //释放链表第 1 个节点数据域指针
		myfree(memx,p); //释放链表第一个节点
		LinkList->LinkList_legth--; //表长-1

		return LINKLIST_RETURN_OK;
}






//LinkList 在链表末尾插入一个新的数据域
//datacache 数据源 len 长度
u8 LinkList_Add_Area2(u8 memx,my_LinkList_Inf* LinkList,u8 *datacache,u16 len)
{
		u8 res;
	
		my_LinkList_Inf *p = LinkList;//定义一个临时变量指向表头
		if(p == NULL) //链表或数据域无效
		{
				return LINKLIST_RETURN_NULL;
		}	
		if(p->LinkList_legth >= p->LinkList_maxlegth -1 )//达到最大容量
		{
				p->LinkList_Full_flag=1;
				return LINKLIST_RETURN_FULL;
		}
////////////////////////////////////////////////////////////////////////////////////////		
		while(p->next!=NULL)//找到最后一个指向空的节点
		{	
				p = p->next;
		}
		//申请下一级链表结构体的空间
		p->next =  (my_LinkList_Inf*)mymalloc(memx,sizeof(my_LinkList_Inf));
		if(p->next == NULL) 
		{
				return LINKLIST_RETURN_MEN_ERR;//申请不到内存，返回失败
		}
		//链表指向刚刚申请的空间，当前节点就不指向空了
		p = p->next;
		//Main_printf("下一级链表地址 p->next %x\r\n",p);
////////////////////////////////////////////////////////////////////////////////////////		
		//申请节点的链表数据域空间
		p->my_data_inf = (my_LinkList_datainf*)mymalloc(memx,sizeof(my_LinkList_datainf));
		if(p->my_data_inf == NULL) 
		{
				myfree(memx,p);//由于申请失败，先前申请的链表空间也要释放
				return LINKLIST_RETURN_MEN_ERR;
		} 
		//申请数据缓存区
		p->my_data_inf->data_len = len+1;//多一个字节补 \0
		p->my_data_inf->databuf = (u8*)mymalloc(memx,sizeof(u8)* p->my_data_inf->data_len); 
		if(p->my_data_inf->databuf == NULL) 
		{
				myfree(memx,p);	//由于申请失败，先前申请的链表空间也要释放
				return LINKLIST_RETURN_MEN_ERR;
		}
		////////////////////////////////////////////////////////////////////////////////////////		
		memcpy(p->my_data_inf->databuf,datacache,len); //copy数据缓存至链表
		p->my_data_inf->databuf[len] = 0;//补0
		////////////////////////////////////////////////////////////////////////////////////////
		p->next=NULL;//标记这个链表的尾部，这是最后级，指向空
		LinkList->LinkList_legth++;
		
		return LINKLIST_RETURN_OK;	//添加成功
}




//检测链表是否有数据
//1 有数据 0 没有数据
u8 LinkList_Serch_Data(my_LinkList_Inf* LinkList)
{
		if(LinkList == NULL) //链表无效
		{
				return LINKLIST_RETURN_NULL;
		}
		if( LinkList->next == NULL) //链表节点无数据
		{
				return LINKLIST_RETURN_EMPTY;
		}
		else 
		{
				return LINKLIST_RETURN_OK;//链表节点有数据
		}
}


////删除释放链表第NUM个节点，并使上下节点衔接
//s8  Delete_LinkList_head(my_LinkList_Inf* LinkList,u8 num)
//{	
//////////////////////////////////////////////////////////////
//		my_LinkList_Inf *p = LinkList;//定义一个临时变量指向表头
//		my_LinkList_Inf *q = p;
//		////////////////////////////////////////////////////////////	
//		if(p == NULL ) //链表无效
//		{
//				return -1;
//		}
//		if(p->next == NULL) //链表无数据
//		{
//				return 0;
//		}		
//		////////////////////////////////////////////////////////////
//    p = p->next;//指向链表第一个节点，之后会销毁这个
//		q->next = p->next;//指向链表下一个节点
//		////////////////////////////////////////////////////////////
//		//销毁这个节点
//		p->next=NULL;
//		free_my_datainf(p->my_data_inf);
//		myfree(SRAMIN,p); //释放链表第一个节点
//			
//		LinkList->LinkList_legth--;
//		return 1;
//}
////求表长
//s8 Get_LinkList_Length(my_LinkList_Inf* LinkList)
//{
//		my_LinkList_Inf *p = LinkList;//定义一个临时变量指向表头

//		if(LinkList == NULL) //链表无效
//		{
//				return -1;
//		}
//		if( LinkList->next == NULL) //链表节点无数据
//		{
//				LinkList->LinkList_legth = 0; //无数据
//		}
//		
//		while(LinkList)
//    {
//				LinkList->LinkList_legth++;
//        p = p->next;//头结点指向下一个节点
//		}
//		
//		return 1;//链表节点有数据
//}
////////////////////////////////////////////////////////////////////////////////////////

////内部函数，复制用户数据至链表数据域，具体内容根据用户自定义数据结构调整
//static u8 LinkList_copy_datainf(my_LinkList_Inf *p,my_LinkList_datainf *data)
//{
//		if(p == NULL || data == NULL) //指针无效
//		{
//				return LINKLIST_RETURN_NULL;
//		}
//		////////////////////////////////////////////////////////////////////////////////////////		
//		//申请这一级的链表空间
//		p->my_data_inf = (my_LinkList_datainf*)mymalloc(SRAMIN,sizeof(my_LinkList_datainf));
//		if(p->my_data_inf == NULL) 
//		{
//				myfree(SRAMIN,p);//由于申请失败，先前申请的链表空间也要释放
//				return LINKLIST_RETURN_MEN_ERR;
//		} 
//		//copy data的数据结构至新申请的链表空间，包含的所有的变量，数据长度等
//		memcpy(p->my_data_inf,data,sizeof(my_LinkList_datainf)); 
//		//申请数据长度的 缓存区
//		p->my_data_inf->databuf = (u8*)mymalloc(SRAMIN,sizeof(u8)*p->my_data_inf->data_len); 
//		if(p->my_data_inf->databuf == NULL) 
//		{
//				LinkList_datainf_free(SRAMIN,p->my_data_inf);	//释放申请的临时区域
//				myfree(SRAMIN,p);	//由于申请失败，先前申请的链表空间也要释放
//				return LINKLIST_RETURN_MEN_ERR;
//		}
//////////////////////////////////////////////////////////////////////////////////////////		
//		//memset(p->my_data_inf->databuf,0,data->data_len); //清空
//		memcpy(p->my_data_inf->databuf,data->databuf,data->data_len); //copy数据缓存
//////////////////////////////////////////////////////////////////////////////////////////	
//		return LINKLIST_RETURN_OK;
//}



//////////////////////////////////////////////////////////////////////////////////////////////////////
//下面3个函数配合使用，用于复杂的数据的搬运
//LinkList 在链表末尾插入一个新的数据域
//datainf  临时数据缓冲区 （指针和相关内容要提前准备好）
u8 LinkList_Add_Area(u8 memx,my_LinkList_Inf* LinkList,my_LinkList_datainf *datainf)
{
		my_LinkList_Inf *p = LinkList;//定义一个临时变量指向表头
	
		if(LinkList == NULL || datainf == NULL) //链表或数据域无效
		{
				return LINKLIST_RETURN_NULL;
		}	
		if(p->LinkList_legth >= p->LinkList_maxlegth -1 )//达到最大容量
		{
				p->LinkList_Full_flag=1;
				return LINKLIST_RETURN_FULL;
		}
		while(p->next!=NULL)//找到最后一个指向空的节点
		{	
				p = p->next;
		}
		//申请下一级链表结构体的空间
		p->next =  (my_LinkList_Inf*)mymalloc(SRAMIN,sizeof(my_LinkList_Inf));
		if(p->next == NULL) 
		{
				return LINKLIST_RETURN_MEN_ERR;//申请不到内存，返回失败
		}
		//链表指向刚刚申请的空间，当前节点就不指向空了
		p = p->next;
		//Main_printf("下一级链表地址 p->next %x\r\n",p);
		
		//申请节点的链表数据域空间
		p->my_data_inf = (my_LinkList_datainf*)mymalloc(SRAMIN,sizeof(my_LinkList_datainf));
		if(p->my_data_inf == NULL) 
		{
				myfree(SRAMIN,p);//由于申请失败，先前申请的链表空间也要释放
				return LINKLIST_RETURN_MEN_ERR;
		} 
		memcpy(p->my_data_inf,datainf,sizeof(my_LinkList_datainf));//copy数据域结构，包含数据指针，数据长度等
		////////////////////////////////////////////////////////////////////////////////////////
		//申请数据缓存区
		p->my_data_inf->databuf = (u8*)mymalloc(SRAMIN,sizeof(u8)*p->my_data_inf->data_len); 
		if(p->my_data_inf->databuf == NULL) 
		{
				LinkList_datainf_free(SRAMIN,p->my_data_inf);	//释放申请的临时区域
				myfree(SRAMIN,p);	//由于申请失败，先前申请的链表空间也要释放
				return LINKLIST_RETURN_MEN_ERR;
		}
		////////////////////////////////////////////////////////////////////////////////////////		
		memcpy(p->my_data_inf->databuf,datainf->databuf,datainf->data_len); //copy数据缓存至链表
		LinkList_datainf_free(SRAMIN,datainf); //删除临时数据缓冲区
		////////////////////////////////////////////////////////////////////////////////////////
		p->next=NULL;//标记这个链表的尾部，这是最后级，指向空
		LinkList->LinkList_legth++;
		
		return LINKLIST_RETURN_OK;	//添加成功
}

//临时数据缓冲区 用户数据域 申请内存
//需要提前申请用户自定义数据的指针
//数据域申请的内存，需调用 LinkList_Add_Area 函数销毁
u8 LinkList_datainf_mem(u8 memx,my_LinkList_datainf *my_data_inf, u16 len)
{
		if(my_data_inf == NULL) //指针无效
		{
			 return LINKLIST_RETURN_NULL;
		}	
		
		my_data_inf ->data_len = len;
		my_data_inf ->databuf =  (u8*)mymalloc(memx,sizeof(u8)*my_data_inf ->data_len);
		if(my_data_inf ->databuf == NULL) //内存申请失败
		{
			 return LINKLIST_RETURN_MEN_ERR;
		}
		else
		{
			 return LINKLIST_RETURN_OK;
		}
}


//临时数据缓冲区 用户数据域 释放内存
static u8 LinkList_datainf_free(u8 memx,my_LinkList_datainf *my_data_inf)
{
		if(my_data_inf == NULL || my_data_inf ->databuf == NULL) //指针无效
		{
			 return LINKLIST_RETURN_NULL;
		}
		myfree(memx,my_data_inf ->databuf); //释放

		return LINKLIST_RETURN_OK;
}



