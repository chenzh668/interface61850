

#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include "Thread61850.h"

#define MAX_YCDATA_NUM 32 //每个模块（pcs）抄取的YC数量最大值
#define MAX_TOTAL_PCS_NUM 36
#define MAX_LCD_NUM 6
#define _ZJYX_ 0
#define _YX_ 1
#define _YC_ 2
#define _ZJYC_ 3

#define _BMS_YX_ 0
#define _BMS_YK_ 1
#define _PCS_YK_ 2

#define _SHORT_ 2
#define _BOOL_ 3
#define _UINT_ 4
#define _INT_ 5
#define _U_SHORT_ 6
#define _FLOAT_ 7

enum _PORTID
{
	INFO_EMU = 1, // emu整机信息
	INFO_LCD = 2, // emu下lcd信息
	INFO_PCS = 3, // emu下lcd下pcs信息
	INFO_BMS = 5,
};
//******************************************************
// 3-bool, 5-int, 6-unsigned int,7-float
enum _DATA_TYP
{
	_BOOL = 3,
	_INT = 5,
	_UINT = 6,
	_FLOAT = 7,
};

typedef struct
{
	int pos_protocol; //协议中的位置
	int pointID;	  //共享内存中的编号
	char el_tag;	  //数据类型
	int data_size;
	int typeID;
	int precision; //数据精度
} SendTo61850;	   //
typedef struct
{
	int pos_protocol; //协议中的位置
	int pointID;	  //共享内存中的编号
	char el_tag;	  //数据类型
	int data_size;
	int typeID;
	int precision;	 //数据精度
	int flag;		 //计算标志，1总和 2平均 0无需计算
} SendTo61850_count; //
typedef struct
{
	int msgtype;
	char data[2048];
} msgLcd;

typedef struct
{
	data_info_t data_info[MAX_YCDATA_NUM]; //数据
	int num;
} MyData;

extern int total_pcsnum;
extern int g_flag_RecvNeed_PCS;
extern int g_flag_RecvNeed_LCD;
extern float Ave_Max_PW;
extern float Ave_Max_DPW;
int sendtotask(MyData *senddata);
unsigned int countRecvFlag(int num_read);
int myprintbuf(int len, unsigned char *buf);
#endif