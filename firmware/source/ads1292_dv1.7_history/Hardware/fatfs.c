#include "fatfs.h"
#include "ff.h"
#include "diskio.h"
#include "malloc.h"
#include "usart.h"       //串口 





//检查字库文件
//返回 0 字库文件正常  其他 错误号
u8 serch_TFfont(void)
{
		FIL *MatFile; //文件对象
		u16 res,i;

		//检查字库文件，没有字库设备也能用,只能显示英文
		MatFile=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存		
		res = f_open(MatFile, GBK16_FILE_ADDR, FA_OPEN_EXISTING | FA_READ);  //打开字库文件，只读
		f_close(MatFile);//关闭文件 			
		myfree(SRAMIN,MatFile);
		
		return res; //如果打开字库不成功，返回错误号
}	


//-----------------------------------------//
//#define   FATFS_DISPLAYTIME		200

//从字库文件中查找出字模
//code 字符指针开始
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小

////读取SD卡中的汉字库
//void FATFS_Get_HzMat(u8 *font, u8 *dzk, u8 size)
//{
//		FRESULT res;	// FatFs function common result code	
//		FIL *MatFile; //文件对象
//		UINT br; 	//返回已读取字节数的指针	
//			
//		u8 qh,ql;
//		u8 i;					  
//		unsigned long foffset; 
//		u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
//		
//		qh=*font;
//		ql=*(++font);
//		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
//		{   		    
//				for(i=0;i<csize;i++)*dzk++=0x00;//填充满格
//				return; //结束访问
//		}
//		if(ql<0x7f)ql-=0x40;//注意!
//		else ql-=0x41;
//		qh-=0x81;   
//		foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
//			
//		MatFile=(FIL*)mymalloc(sizeof(FIL));//申请内存
//		if(MatFile==NULL)//内存分配出错
//		{	    
//				printf("unigbk 内存分配失败!\r\n");
//		}		
////		printf("HZ_MAT");
////		mem_perused();//内存使用率	
//		switch(size)
//		{
//				case 12:
//					res = f_open(MatFile, "SYSTEM/GBK16.FON", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
//				break;
//				case 16:
//					res = f_open(MatFile, "SYSTEM/GBK16.FON", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
//				break;
//				case 24:
//					res = f_open(MatFile, "SYSTEM/GBK16.FON", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
//				break;			
//		}    			
//		res = f_lseek(MatFile, foffset);//相对于文件起始地址移动指针	
//		res = f_read(MatFile, dzk, csize, &br);//读取csize大小的汉字点阵数据到dzk数组中  			
//		res = f_close(MatFile);//关闭文件 			
//		myfree(MatFile);	
//}


//读取SD卡中的汉字库
u8 FATFS_Get_HzMat(u8 *font, u8 *dzk, u8 size)
{
		u8 temp=0;
		u8 res;	// FatFs function common result code		
		FIL *MatFile; //文件对象
		UINT br; 	//返回已读取字节数的指针	
	
		u8 region, location;//区号和位号
		u32 index;	//字库数组内偏移地址
		u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
		
		region   = (*font)-0x81;//得到区号 gb2312 
		location = (*(font+1))-0x41;//得到位号 
		index = ((0xbe*region)+location) * csize;//字库数组内地址，因为每个区号有0xbe个汉字，每个汉点阵数为csize个
		
//////////////////////////////////////////////////////////////////////////////////////////////
		MatFile=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存
		if(MatFile==NULL)//内存分配出错
		{	    
				Main_printf("SD字库内存分配失败!\r\n");
				temp|=1;
		}
//////////////////////////////////////////////////////////////////////////////////////////////		
		if(size == 16)
				res = f_open(MatFile, GBK16_FILE_ADDR, FA_OPEN_EXISTING | FA_READ);  //打开字库文件
		if(res) //如果打开字库不成功，返回错误号
		{
				Main_printf("字库文件打开失败!\r\n");
				temp|=2;
		}
//////////////////////////////////////////////////////////////////////////////////////////////		
		res = f_lseek(MatFile, index);//相对于文件起始地址移动指针	
		if(res) //失败
		{
				Main_printf("字库指针移动失败!\r\n");
				temp|=4;
		}
//////////////////////////////////////////////////////////////////////////////////////////////		
		res = f_read(MatFile, dzk, csize, &br);//读取汉字点阵数据到	buff数组中 		
		if(res && br!=csize)
		{
				Main_printf("汉字数据读取失败!\r\n");
				temp|=8;
		}
//////////////////////////////////////////////////////////////////////////////////////////////		
		res = f_close(MatFile);//关闭文件 			
		myfree(SRAMIN,MatFile);
		return temp;
}

///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_GetHZData(u8 *buff, u8 size, u8 *china)
//{ 
//	FRESULT res;	// FatFs function common result code
//	FATFS fs;			// Work area (file system object) for logical drive
//	FIL fil; 			// file objects
//	UINT br; 			// File R count
//	
//	u8 region, location;//区号和位号
//	u32 index;//字库数组内地址
//	
//	region   = (*china)-0x81;//得到区号  gb2312 
//	location = (*(china+1))-0x41;//得到位号 
//	//字库数组内地址，因为每个区号有0xbe个汉字，每个汉点阵数为size*size个，8个点阵存为一个变量。 
//	index = ((0xbe*region)+location) * ((size*size)/8);
//																		
////	f_mount(0, &fs); //挂载一个文件系统结构
//	if(size == 16)
//		res = f_open(&fil, "system/songti16.DZK", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
//	else if(size == 32)
//		res = f_open(&fil, "system/songti16.DZK", FA_OPEN_EXISTING | FA_READ);  //打开字库文件
//	else
//			return;
//	
//	if (res) //如果打开字库不成功，返回错误号
//			return;	
//	
//	res = f_lseek(&fil, index);//找到要显示汉字在字库文件里的偏移地址并定位
//	if (res) 
//			return;
//	
//	//读取size*size大小的汉字点阵数据到	buff数组中
//	res = f_read(&fil, buff, size*size/8, &br);  
//	if (res)
//			return;
//	
//	res = f_close(&fil);//关闭文件
//	if (res) 
//			return;									  
//	
////	res = f_mount(0, NULL);//卸载
////	if (res) return;	
//}


///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark	       : None
//**********************************************************************/
//void FATFS_FileSacn(u8 *path, FATFS_FileListInfoTypeDef *filelist)
//{
//	FRESULT res;                                      // FatFs function common result code
//	FATFS fs;                                        	// Work area (file system object) for logical drive
//	FIL fil;                                         	// file objects
//	UINT br;			                                  	// File R count    
//	FILINFO fno;
//	DIR dir;
//	
//	u8 i;
//	u8 char_num;
//	u8 f_name[10];
//	u8 f_name_cnt;
//	u8 f_tmp;

//	char *fn;   /* This function is assuming non-Unicode cfg. */
//	#if _USE_LFN
//		static char lfn[_MAX_LFN + 1];
//		fno.lfname = lfn;
//		fno.lfsize = sizeof lfn;
//	#endif

//	filelist->FileQuantity		= 0;
//	filelist->FileCurNum 		= 0;
//	filelist->FileNameLen		= 0;
//	filelist->FileSongAll      = 0;
//	
//	f_mount(0, &fs);		                                 //Register volume work area (never fails)
//	res = f_opendir(&dir, (const TCHAR*)path);                             //Open the directory 
//	if (res == FR_OK) 
//	{
//		for (;;) 
//		{
//			res = f_readdir(&dir, &fno);                     //Read a directory item 
//			if (res != FR_OK || fno.fname[0] == 0) break;    //Break on error or end of dir 
//			if (fno.fname[0] == '.') continue;               //Ignore dot entry 
//			
//			#if _USE_LFN
//				fn = *fno.lfname ? fno.lfname : fno.fname;
//			#else
//				fn = fno.fname;
//			#endif

//			char_num=0;
//			while((*(fn+char_num))!='\0')				 //将目录名保存到文件列表数组f_list中
//			{
//				filelist->FileNameBuff[filelist->FileNameLen] = *(fn+char_num);
//				char_num=char_num+1;
//				filelist->FileNameLen = filelist->FileNameLen + 1;							 
//			}
//			filelist->FileNameBuff[filelist->FileNameLen] = '\0';
//			filelist->FileNameLen = filelist->FileNameLen + 1;
//			char_num = char_num+1;
//								
//			//将当前目录下的全部文件名保存到文件列表数组f_list中，最多保存40个，就是说
//			//一个目录下的文件(包含文件夹)不能超过40个，超过40的文件将不会被保存
//			if (fno.fattrib & AM_DIR) 
//			{                                                //It is a directory
//				filelist->FileType[filelist->FileQuantity] = FATFS_FILEISDIR;		
//			} 
//			else 
//			{                                                //It is a file. 
//				f_tmp = 0;
//				f_name_cnt = 0;
//				//搜索到文件名尾
//				while((*(fn+f_name_cnt))!='\0')				 
//				{
//					f_name_cnt++;
//				}
//				//将文件名.后面的后缀存到f_name中
//				f_name[0] = *(fn+f_name_cnt-3);
//				f_name[1] = *(fn+f_name_cnt-2);
//				f_name[2] = *(fn+f_name_cnt-1);

//			   if(f_name[0] == 'm' && f_name[1] =='p' && f_name[2] == '3')
//			   {
//			      filelist->FileType[filelist->FileQuantity] = FATFS_FILEISMP3;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'M' && f_name[1] =='P' && f_name[2] == '3')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISMP3;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'w' && f_name[1] =='m' && f_name[2] == 'a')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWMA;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'W' && f_name[1] =='M' && f_name[2] == 'A')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWMA;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'w' && f_name[1] =='a' && f_name[2] == 'v')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWAV;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'W' && f_name[1] =='A' && f_name[2] == 'V')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISWAV;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'f' && f_name[1] =='l' && f_name[2] == 'a' && f_name[3] == 'c')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISFLAC;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else if(f_name[0] == 'F' && f_name[1] =='L' && f_name[2] == 'A' && f_name[3] == 'C')
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISFLAC;
//					filelist->FileSongPos[filelist->FileQuantity] = filelist->FileSongAll;
//					filelist->FileSongNum[filelist->FileSongAll] = filelist->FileQuantity;					
//					filelist->FileSongAll = filelist->FileSongAll + 1;
//			   }
//			   else
//			   {
//					filelist->FileType[filelist->FileQuantity] = FATFS_FILEISOTHER;
//			   }			  
//			}

//			filelist->FilePostion[filelist->FileQuantity] = filelist->FileNameLen - char_num;
//			filelist->FileLen[filelist->FileQuantity] = char_num;
//			filelist->FileQuantity = filelist->FileQuantity + 1;				
//		}
//	}
//	f_mount(0, NULL);	
//}



///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_DisplayFileName(FATFS_FileListInfoTypeDef *list)
//{
//	Display_ClearFileNameZoom();
//	
//	if(list->FileQuantity == 0)
//	{
//	}
//	else if(list->FileQuantity == 1)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_TurnColor(2);
//		Display_TurnColor(3);
//	}
//	else if(list->FileQuantity == 2)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
//		if(list->FileCurNum == 0)
//		{
//			Display_TurnColor(2);
//			Display_TurnColor(3);			
//		}
//		else
//		{
//			Display_TurnColor(4);
//			Display_TurnColor(5);			
//		}	
//	}
//	else
//	{
//		if(list->FileCurNum == 0)
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
//			Display_TurnColor(2);
//			Display_TurnColor(3);				
//		}
//      else if(list->FileCurNum == (list->FileQuantity - 1))
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
//			Display_TurnColor(6);
//			Display_TurnColor(7);			
//		}
//      else
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);			
//			Display_TurnColor(4);
//			Display_TurnColor(5);			
//		}
//	}
//	Display_UpdateFileNameZoom();
//	//Display_Num(list.FileQuantity, list.FileCurNum + 1);
//}



///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_DisplayFileNameBefore(FATFS_FileListInfoTypeDef *list)
//{
//	Display_ClearFileNameZoom();
//		
//	if(list->FileQuantity == 0)
//	{
//	}
//	else if(list->FileQuantity == 1)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_TurnColor(2);
//		Display_TurnColor(3);		
//	}
//	else if(list->FileQuantity == 2)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
//		if(list->FileCurNum == 0)
//		{
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = 1;
//		}
//		else
//		{
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//		}
//		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//		Display_UpdateFileNameZoom();
//	}
//	else if(list->FileQuantity == 3)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//		Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);		
//		if(list->FileCurNum == 0)
//		{
//			Display_TurnColor(6);
//			Display_TurnColor(7);
//			list->FileCurNum = 2;
//		}
//		else if(list->FileCurNum == 1)
//		{
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//		}
//		else if(list->FileCurNum == 2)
//		{
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = 1;
//		}
//		else
//		{;}
//		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//		Display_UpdateFileNameZoom();		
//	}
//	else
//	{
//		if(list->FileCurNum == 0)
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
//			Display_TurnColor(6);
//			Display_TurnColor(7);
//			list->FileCurNum = list->FileQuantity - 1;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();	
//		}
//		else if(list->FileCurNum == 1)
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);				
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();				
//		}
//		else if(list->FileCurNum == (list->FileQuantity - 1))
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = list->FileQuantity - 2;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();	
//		}
//		else
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
//			Display_UpdateFileNameZoom();
//			Display_ClearFileNameZoom();
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 2]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum]);				
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = list->FileCurNum - 1;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
//			Display_UpdateFileNameZoom();			
//		}
//	}
//	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
//}



///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_DisplayFileNameNext(FATFS_FileListInfoTypeDef *list)
//{
//	Display_ClearFileNameZoom();
//		
//	if(list->FileQuantity == 0)
//	{
//	}
//	else if(list->FileQuantity == 1)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_TurnColor(2);
//		Display_TurnColor(3);		
//	}
//	else if(list->FileQuantity == 2)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);		
//		if(list->FileCurNum == 0)
//		{
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = 1;
//		}
//		else
//		{
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//		}
//		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//		Display_UpdateFileNameZoom();
//	}
//	else if(list->FileQuantity == 3)
//	{
//		Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//		Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//		Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);		
//		if(list->FileCurNum == 0)
//		{
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = 1;
//		}
//		else if(list->FileCurNum == 1)
//		{
//			Display_TurnColor(6);
//			Display_TurnColor(7);
//			list->FileCurNum = 2;
//		}
//		else if(list->FileCurNum == 2)
//		{
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//		}
//		else
//		{;}
//		MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//		Display_UpdateFileNameZoom();		
//	}
//	else
//	{
//		if(list->FileCurNum == 0)
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = 1;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();	
//		}
//		else if(list->FileCurNum == (list->FileQuantity - 2))
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileQuantity - 3]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileQuantity - 2]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileQuantity - 1]);				
//			Display_TurnColor(6);
//			Display_TurnColor(7);
//			list->FileCurNum = list->FileQuantity - 1;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();						
//		}
//		else if(list->FileCurNum == (list->FileQuantity - 1))
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[0]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[2]);
//			Display_TurnColor(2);
//			Display_TurnColor(3);
//			list->FileCurNum = 0;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME);
//			Display_UpdateFileNameZoom();					
//		}
//		else
//		{
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum - 1]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum - 0]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);				
//			Display_TurnColor(6);
//			Display_TurnColor(7);
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
//			Display_UpdateFileNameZoom();
//			Display_ClearFileNameZoom();
//			Display_String(0, 2, list->FileNameBuff + list->FilePostion[list->FileCurNum ]);
//			Display_String(0, 4, list->FileNameBuff + list->FilePostion[list->FileCurNum + 1]);
//			Display_String(0, 6, list->FileNameBuff + list->FilePostion[list->FileCurNum + 2]);				
//			Display_TurnColor(4);
//			Display_TurnColor(5);
//			list->FileCurNum = list->FileCurNum + 1;
//			MICROSEC_Delay_ms(FATFS_DISPLAYTIME / 2);
//			Display_UpdateFileNameZoom();			
//		}
//	}
//	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
//}



///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_DisplayFileNameBack(FATFS_FileListInfoTypeDef *list)
//{
//	u8 i;
//	if(list->FilePathLen > 1)
//	{
//		for(i=list->FilePathLen; i>0; i--)
//		{
//			if(list->FilePathName[i] == '/')
//			{
//				list->FilePathName[i] = '\0';			
//				break;
//			}
//			else
//			{
//				list->FilePathName[i] = '\0';
//				list->FilePathLen = list->FilePathLen - 1;
//			}
//		}
//	}
//	FATFS_FileSacn(list->FilePathName, list);
//	list->FileLayerTol = list->FileLayerTol - 1;
//	list->FileCurNum = list->FileLayerNum[list->FileLayerTol];
//	FATFS_DisplayFileName(list);
//	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
//}



///**********************************************************************
//* Function Name  : None
//* Description    : None
//* Input          : None
//* Output         : None
//* Return         : None
//* Remark         : None
//**********************************************************************/
//void FATFS_DisplayFileNameMove(FATFS_FileListInfoTypeDef *list)
//{
//	*(list->FilePathName + list->FilePathLen) = '/';
//	list->FilePathLen = list->FilePathLen + 1;
//	memcpy(list->FilePathName + list->FilePathLen, list->FileNameBuff + list->FilePostion[list->FileCurNum], list->FileLen[list->FileCurNum]);
//	list->FilePathLen = list->FilePathLen + list->FileLen[list->FileCurNum] - 1;
//	
//	list->FileLayerNum[list->FileLayerTol] = list->FileCurNum;
//	list->FileLayerTol = list->FileLayerTol + 1;
//	
//	FATFS_FileSacn(list->FilePathName, list);
//	FATFS_DisplayFileName(list);
//	//Display_Num(list->FileQuantity, list->FileCurNum + 1);
//}
