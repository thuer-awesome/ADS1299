#ifndef __MYFATFS_H
#define __MYFATFS_H
#include "sys.h" 
#include "ff.h"


	u8 serch_TFfont(void);//查找字库
	u8 *Creat_File(void); //创建文件名
	u16 BIN_get_tnum(u8 *path);//得到path路径下,目标文件的总个数
	u8 *GET_File_name(void); //得到自增文件名
	u8 *Creat_File_RTC(void);//得到时间文件名
	FIL* Before_Write(void); //记录前的操作

u8 set_write_buf_BIN(void);	//设置TF卡写缓冲
u8 set_write_buf_TXT(void);	//设置TF卡写缓冲

#endif

