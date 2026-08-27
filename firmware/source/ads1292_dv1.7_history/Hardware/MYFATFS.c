#include "MYFATFS.h"	 
#include "fatfs.h"

#include "delay.h"       //延时
#include "malloc.h"
#include "usart.h"       //串口 
#include "ff.h"
#include "exfuns.h"
#include "sd_card.h"

#include "led.h" 
#include "string.h"
#include "myQueue.h"


//TF卡相关	
#define FILEDIR "0:/ADS129X_DATA\0"	//文件路径
#define FILENAME 	"ADS129xDATA_001" //文件名 不要超20个字符 




/////////////////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//红茶电子科技-红茶
//淘宝 https://shop119364888.taobao.com
//创建日期:2022/03/02
//版本：V1.1
//版权所有，盗版必究。
/////////////////////////////////////////////////////////////////////////////////////////////



//记录前的操作 打开/创建文件夹，申请TF写缓存，创建文件，返回文件结构体指针
FIL* Before_Write(void)
{
		u8 res;

		DIR recdir;	 	//目录			
		FIL* f_rec;		//文件结构体指针
		u8 *file_str;	//带文件路径的文件名指针	
////////////////////////////////////////////////////////////////////////////	
		Main_printf("打开目录...\r\n");	
		while(f_opendir(&recdir,FILEDIR))//打开/创建文件夹
		{	 					
				Main_printf("没有目录，创建...\r\n");	
				if(f_mkdir(FILEDIR))//创建该目录 
				{
						Main_printf("创建目录失败\r\n");	
				}	
				else
				{
						Main_printf("创建目录成功\r\n");	
				}
				delay_ms(500);
		}
		Main_printf("打开目录成功\r\n");	
//////////////////////////////////////////////////////////////////			
		//申请缓存
		//set_write_buf_TXT();//设置SD卡写缓冲	
		set_write_buf_BIN();	//设置TF卡写缓冲

		Main_printf("内存使用率 %d 申请内存...\r\n",mem_perused(SRAMIN));			
		f_rec=(FIL *)mymalloc(SRAMIN,sizeof(FIL));	//开辟FIL字节的内存区域
		if(f_rec==NULL)//内存分配出错
		{	  
				Main_printf(" 失败");
				myfree(SRAMIN,f_rec);
				return NULL;
		}	
		Main_printf("内存申请成功 当前内存使用率 %d\r\n",mem_perused(SRAMIN));	
//////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//创建&打开 文件	
		file_str = GET_File_name();//得到自增文件名
		//file_str = Creat_File_RTC();//得到实时时间文件名
		Main_printf("要创建的带路径的文件名%s\r\n",file_str);		
		res=f_open(f_rec,(const TCHAR*)file_str, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);//创建一个新文件。如果文件已存在，则它将被截断并覆盖	
		if(res)
		{		
				while(res)	//重复创建	
				{		
						res = f_open(f_rec,(const TCHAR*)file_str, FA_CREATE_NEW|FA_WRITE|FA_READ);
						Main_printf("创建文件失败 %d\r\n",res);
						delay_ms(500);	
				}
		}	
		Main_printf("创建文件成功\r\n");	
///////////////////////////////////////////////////////////////////////////	
		Main_printf("移动指针...");			
		f_lseek(f_rec,f_rec->obj.objsize);//将指针指向文件末
		Main_printf("暂存...");	
		f_sync(f_rec);//暂存	
		Main_printf("准备完成！\r\n");	
/////////////////////////////////////////////////////////////////	
		return f_rec;			
}


u8 set_write_buf_BIN(void)	//设置TF卡写缓冲
{		
		u8 i;

		TF_info->FIL_Check=0xFF;//校验
		TF_info->FIL_Save_Num=0;//写次数
	
		TF_info->Write_Num=0;//写n次为一帧数据
		TF_info->FIL_Package_Num = 4080/SD_QUEUE_LENGTH +1 ;//写N次为一帧
		TF_info->FIL_Save_Num = 256 ;//写N帧存储一次
	
		TF_info->FIL_Head[0]=0xa5;
		TF_info->FIL_Head[1]=0xa5;
		TF_info->FIL_Head[2]=TF_info->FIL_Write_Num>>24;
		TF_info->FIL_Head[3]=TF_info->FIL_Write_Num>>16;
		TF_info->FIL_Head[4]=TF_info->FIL_Write_Num>>8;
		TF_info->FIL_Head[5]=TF_info->FIL_Write_Num>>0;
		for(i=6;i<=12;i++)
		{
				TF_info->FIL_Head[i]=0XFF;
		}
		TF_info->FIL_Tail[0]=TF_info->FIL_Check;//校验
		TF_info->FIL_Tail[1]=0x5a;//尾
		TF_info->FIL_Tail[2]=0x5a;//尾
//////////////////////////////////////////////////////////////////////////////////////////////		
		Main_printf("TF写 %d 次数据为一帧\r\n",TF_info->FIL_Package_Num);	
		Main_printf("TF写 %d 帧数据存储一次\r\n",TF_info->FIL_Save_Num);	
		
			
		return 0;
}




//得到自增文件名 .bin
u8 *GET_File_name(void)
{			
		DIR recdir;	 	//目录
		u8 res,i;
	
		u16 new_number=0;
		u16 old_number=0;
	
		FILINFO *fileinfo; //文件信息
		u16 temp;				//记录文件索引
	
		u8 *file_str;	//带文件路径的文件名指针
		//下面变量用于自增文件名
		u8 *file_buf;	//文件名指针		
		
	
		u16 file_num;//文件夹内有效文件数量

		//得到自增文件名
		file_num = BIN_get_tnum(FILEDIR);//得到目标路径下的bin文件数量
		file_str = Creat_File();//获得带文件路径的文件名	
		if( file_num != 0)//目录中已经有文件了
		{
				Main_printf("文件夹内有效文件数量 %d\r\n",file_num);			
				fileinfo = (FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存			
				while(!fileinfo)//内存分配出错
				{	    
						Main_printf("文件名内存申请出错");
						delay_ms(200);				  				  
				}  	
				res=f_opendir(&dir,FILEDIR); //打开目录（文件夹），打开目录后，文件索引为0
				if(res==FR_OK)
				{
						while(1)//记录有效文件的索引
						{
								res=f_readdir(&dir,fileinfo);			//读取目录下的一个文件
								if(res!=FR_OK||fileinfo->fname[0]==0)break;			//错误了/到末尾了,退出
							
								res=f_typetell((u8*)fileinfo->fname);	//获得文件拓展名
								if(res == 0x00)//取高四位,看看是不是bin 	 
								{
										file_buf=(u8*)fileinfo->fname;//得到这个文件名	
										new_number= (file_buf[14]-'0') + (file_buf[13]-'0')*10 +(file_buf[12]-'0')*100;
										if(new_number>old_number)
												old_number=new_number;
								}		
						}	
						Main_printf("文件最大编号 %d\r\n",old_number);
						old_number++;						
				}
				// 0:/ADS129X_DATA/ADS1299DATA_002.bin
				file_str[28] = old_number/100+'0'; //修改最后三个字符
				file_str[29] = old_number/10+'0';
				file_str[30] = old_number%10+'0';			
				myfree(SRAMIN,fileinfo);//释放内存 文件名
		}			
		return file_str;//返回字符串指针
}

////得到自增文件名 .TXT
//u8 *GET_File_name(void)
//{			
//		DIR recdir;	 	//目录
//		u8 res,i;

//	
//		FILINFO *fileinfo; //文件信息
//		u16 temp;				//记录文件索引
//	
//		u8 *file_str;	//带文件路径的文件名指针
//		//下面变量用于自增文件名
//		u8 *file_buf;	//文件名指针		
//		u16 file_num;//文件夹内有效文件数量

//		//得到自增文件名
//		file_num = TXT_get_tnum(FILEDIR);//得到目标路径下的TXT文件数量
//		file_str = Creat_File();//获得带文件路径的文件名	
//		if( file_num != 0)//目录中已经有文件了
//		{
//				Main_printf("文件夹内有效文件数量 %d\r\n",file_num);			
//				fileinfo = (FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存			
//				while(!fileinfo)//内存分配出错
//				{	    
//						Main_printf("文件名内存申请出错");
//						delay_ms(200);				  				  
//				}  	
//				res=f_opendir(&dir,FILEDIR); //打开目录（文件夹），打开目录后，文件索引为0
//				if(res==FR_OK)
//				{
//						while(1)//记录有效文件的索引
//						{
//								temp=dir.dptr;//记录当前index		
//								res=f_readdir(&dir,fileinfo);			//读取目录下的一个文件
//								if(res!=FR_OK||fileinfo->fname[0]==0)break;			//错误了/到末尾了,退出
//								res=f_typetell((u8*)fileinfo->fname);	//获得文件拓展名	
//								if(res == 0x30)//取高四位,看看是不是TXT  	 
//								{
//										file_num=temp;//记录有效文件的索引号（一般索引不是顺序的）
//								}			
//						}						
//						dir_sdi(&dir,file_num);	//改变索引，到最后一个bin文件
//						f_readdir(&dir,fileinfo);//读取目录下的一个文件
//						file_buf=(u8*)fileinfo->fname;//得到这个文件名	
//						Main_printf("最后一个文件名 %s\r\n",file_buf);
////////////////////////////////////////////////////////////////////		
//						i=0;
//						while(1)//得到最后一个文件名长度
//						{
//								if(*(file_buf+i) == 0)	
//										break;
//								i++;
//						}
//						if(file_buf[i-5] == '9')//新文件的文件名 数字加1
//						{
//								if(file_buf[i-6] == '9')
//								{
//										file_buf[i-7]+=1;
//										file_buf[i-6]='0';
//										file_buf[i-5]='0';
//								}
//								else
//								{
//										file_buf[i-6]+=1;
//										file_buf[i-5]='0';
//								}
//						}
//						else
//						{
//								file_buf[i-5]+=1;
//						}	
//						Main_printf("要创建的新文件 %s\r\n",file_buf);
//				}
//				// 0:/ADS129X_DATA/ADS1299DATA_002.txt
//				file_str[28] = file_buf[i-7]; //修改最后三个字符
//				file_str[29] = file_buf[i-6];
//				file_str[30] = file_buf[i-5];			
//				myfree(SRAMIN,fileinfo);//释放内存 文件名
//		}			
//		return file_str;//返回字符串指针
//}




//创建文件名字符串 返回自增文件名 .BIN
u8 *Creat_File(void)
{	
		//文件夹路径 	//"0:/ADS129X_DATA\0" //后面多加2个字节存放 '/' '\0'
		//带路径的文件名,数组长度 // "0:/ADS129X_DATA\0" + "ADS1299DATA_001" + ".bin"(4字节)
		u8 Directory[sizeof(FILEDIR)+3]=FILEDIR;	//文件的生成位置
		u8 static file_buf[sizeof(FILENAME) +sizeof(Directory)+4];
		
		strcpy((char*)Directory,(char*)FILEDIR);//文件路径	放到最前	
		strcat((char*)Directory,"/\0");	//加上 '/' '\0'

		strcpy((char*)file_buf,(char*)Directory);	//文件路径
		strcat((char*)file_buf,(char*)FILENAME);	//接上 文件名
		strcat((char*)file_buf,".bin\0");					//接上 拓展名 + '\0'

		return file_buf;	//例：  0:/ADS129X_DATA/ADS1299DATA_002.bin
}
////创建文件名字符串 返回自增文件名 .TXT
//u8 *Creat_File(void)
//{	
//		//文件夹路径 	//"0:/ADS129X_DATA\0" //后面多加2个字节存放 '/' '\0'
//		//带路径的文件名,数组长度 // "0:/ADS129X_DATA\0" + "ADS1299DATA_001" + ".bin"(4字节)
//		u8 Directory[sizeof(FILEDIR)+3]=FILEDIR;	//文件的生成位置
//		u8 static file_buf[sizeof(FILENAME) +sizeof(Directory)+4];
//		
//		strcpy((char*)Directory,(char*)FILEDIR);//文件路径	放到最前	
//		strcat((char*)Directory,"/\0");	//加上 '/' '\0'

//		strcpy((char*)file_buf,(char*)Directory);	//文件路径
//		strcat((char*)file_buf,(char*)FILENAME);	//接上 文件名
//		strcat((char*)file_buf,".txt\0");					//接上 拓展名 '\0'

//		return file_buf;	//例：  0:/ADS129X_DATA/ADS1299DATA_002.TXT
//}



////创建时间文件名字符串 返回实时时间
//u8 *Creat_File_RTC(void)
//{	
//		u8 *str;
//		u8 Directory[sizeof(FILEDIR)+3]=FILEDIR;	//文件的生成位置
//		u8 static file_buf[sizeof(GET_RTC_TIME(year,month,day,hour,min,second,0))+sizeof(Directory)+4];
//		
//		str = GET_RTC_TIME(year,month,day,hour,min,second,1);

//		strcpy((char*)Directory,(char*)FILEDIR);	//文件路径	放到最前	
//		strcat((char*)Directory,"/\0");	//加上 '/' '\0'
//	
//		strcpy((char*)file_buf,(char*)Directory);	 //文件路径
//		strcat((char*)file_buf,(char*)str);//接上 文件名（实时时间）
//		strcat((char*)file_buf,".bin\0");					 //接上 拓展名 + '\0'
//		
//		return file_buf; //例：  0:/ADS129X_DATA/20210330221201.bin
//}






//得到path路径下,目标文件的总个数  
//返回值:总有效文件数
u16 BIN_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO *tfileinfo;//临时文件信息		
	u8 *fn;
	u16 cnt;
		
  res=f_opendir(&tdir,(const TCHAR*)path); //打开目录（文件夹）
	tfileinfo = (FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存			
	if(res==FR_OK&&tfileinfo!=NULL)//成功打开文件夹及申请了内存
	{	
		for (cnt = 0; ;cnt++) 
		{
				res=f_readdir(&tdir,tfileinfo);       		//顺序读取目录下的文件
				if(res!=FR_OK||tfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出		
				if (tfileinfo->fname[0] == '.')continue;//.开头的文件一般都是隐藏文件，一般是不给显示
					
				fn=(u8*)tfileinfo->fname;//得到这个文件名	
				printf("文件编号 %d  index %d 文件名%s 拓展类型0x%x \r\n",cnt,tdir.dptr,fn,res);

				res=f_typetell(fn);	
				if(res == 0)//bin文件
				{
						rval++;//有效文件数增加1
				}
		}  
	} 
	myfree(SRAMIN,tfileinfo);
	return rval;
}

//得到path路径下,目标文件的总个数  
//返回值:总有效文件数
u16 TXT_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO *tfileinfo;//临时文件信息		
	u8 *fn;
	u16 cnt;
		
  res=f_opendir(&tdir,(const TCHAR*)path); //打开目录（文件夹）
	tfileinfo = (FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));	//申请内存			
	if(res==FR_OK&&tfileinfo!=NULL)//成功打开文件夹及申请了内存
	{	
		for (cnt = 0; ;cnt++) 
		{
				res=f_readdir(&tdir,tfileinfo);       		//顺序读取目录下的文件
				if(res!=FR_OK||tfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出		
				if (tfileinfo->fname[0] == '.')continue;//.开头的文件一般都是隐藏文件，一般是不给显示
					
				fn=(u8*)tfileinfo->fname;//得到这个文件名	

				res=f_typetell(fn);	
				if(res == 0x30)//TXT文件  
				{
						printf("文件编号 %d  index %d 文件名%s 拓展类型0x%x \r\n",cnt,tdir.dptr,fn,res);
						rval++;//有效文件数增加1
				}	
		}  
	} 
	myfree(SRAMIN,tfileinfo);
	return rval;
}


//串口RCT校准
//u8 Config_RTC_AT(void) 
//{
//		u16 t;  
//		u16 times=1000;//10s
//	
//		u8* Recbuf;
//		u16 Reclen;
//		
//		u16 ayear=0;
//		u8 amon=0;
//		u8 aday=0;
//		u8 ahour=0;
//		u8 amin=0;
//		u8 setflag=0;
//		RTC_ITConfig(RTC_IT_SEC, DISABLE);		//关RTC秒中断					
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
//		Recbuf = (u8*)mymalloc(200);		
//		memset(Recbuf, 0, 200);
//		
//		Main_printf("AT时间校准，输入时间字符串后换行为结尾，10s无操作自动跳过设置，串口指令示例:\r\n");
//		Main_printf("202103302231\r\n");//年月日时分

//		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//串口接受中断	
//		while(times--) //RCT设置
//		{					
//				if(USART_RX_STA & 0x8000)//接收到数据
//				{
//						//搬运数据
//						Reclen=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//						memcpy(Recbuf,USART_RX_BUF,Reclen);//复制内容
//						USART_RX_STA=0;
//					
//						if(Reclen == 12)
//						{
//								ayear=(Recbuf[0]-'0')*1000 + (Recbuf[1]-'0')*100 + (Recbuf[2]-'0')*10 +(Recbuf[3]-'0');
//								amon= (Recbuf[4]-'0')*10 +(Recbuf[5]-'0');
//								aday= (Recbuf[6]-'0')*10 +(Recbuf[7]-'0');
//								ahour= (Recbuf[8]-'0')*10 +(Recbuf[9]-'0');
//								amin= (Recbuf[10]-'0')*10 +(Recbuf[11]-'0');

//								if(ayear>2099 || ayear<1970 || amon>12 || aday>31 || ahour>60 || amin>60)
//								{
//										Main_printf("错误的指令2\r\n");
//								}
//								else
//								{
//										Main_printf("要设置的参数 %d,%d,%d,%d,%d \r\n",ayear,amon,aday,ahour,amin);
//										setflag=1;
//										break;
//								}
//						}
//						else 
//						{
//								Main_printf("错误的指令长度\r\n");
//						}
//				}
//				else
//				{
//						delay_ms(10);   
//				}			
//		}
//		myfree(Recbuf);
//		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭串口接受中断
//		if(setflag)
//		{
//				RTC_EnterConfigMode();	//进入RTC配置模式，允许配置				
//				RTC_Set(ayear,amon,aday,ahour,amin,0);//设置时间	//年 月 日 时 分 秒
//				RTC_ExitConfigMode(); //退出RTC配置模式						
//				//RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
//				Main_printf("设置成功\r\n");	
//		}
//		else
//		{
//				Main_printf("跳过设置\r\n");	
//		}
//}