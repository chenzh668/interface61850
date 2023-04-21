

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

#define _BMS_PLC_YK_ 4

#define _SHORT_ 2
#define _BOOL_ 3
#define _UINT_ 4
#define _INT_ 5
#define _U_SHORT_ 6
#define _FLOAT_ 7

#define EMS_communication_status 0
#define one_FM_GOOSE_link_status_A 1 //  一次调频A网GOOSE链路状态
#define one_FM_GOOSE_link_status_B 2 //  一次调频B网GOOSE链路状态
#define one_FM_Enable 3				 //  一次调频使能
#define one_FM_Disable 4			 //  一次调频使能

#define Emu_Startup 1				  //【整机】启机命令
#define Emu_Stop 2					  //【整机】停机命令
#define Parallel_Away_conversion_en 3 //并转离切换使能
#define Away_Parallel_conversion_en 4 //离转并切换使能
#define EMS_SET_MODE 5				  //产品运行模式设置
#define EMS_VSG_MODE 6				  // VSG工作模式设置
#define EMS_PQ_MODE 7				  // PQ工作模式设置
#define BOX_35kV_ON 8				  // 35kV进线柜合闸
#define BOX_35kV_OFF 9,				  // 35kV进线柜分闸
#define BOX_SwitchD1_ON 10			  //开关柜D1合闸
#define BOX_SwitchD1_OFF 11			  //开关柜D1分闸
#define BOX_SwitchD2_ON 12			  //开关柜D2合闸
#define BOX_SwitchD2_OFF 13			  //开关柜D2分闸
#define EMS_PW_SETTING 14			  //有功功率
#define EMS_QW_SETTING 15			  //无功功率
#define ONE_FM_PW_SETTING 16		  //一次调频有功功率
#define ONE_FM_QW_SETTING 17		  //一次调频无功功率

enum _PORTID
{
	INFO_EMU = 1, // emu整机信息
	INFO_LCD = 2, // emu下lcd信息
	INFO_PCS = 3, // emu下lcd下pcs信息
	INFO_PLC = 4, // plc信息
	INFO_BMS = 5, // BAMS信息
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
	int pos_protocol; //协议中的位置            //
	int pointID;	  //共享内存中的编号    	// 数据表示4
	char el_tag;	  //数据类型
	int data_size;
	int typeID;                                //数据标识3
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
typedef struct
{
	unsigned char item;	   //项目编号
	unsigned char el_tag;  //  数据类型
	unsigned char data[5]; //参数
} YK_PARA;				   //遥测、遥控参数

typedef int (*CallbackYK)(unsigned char, void *pdata); //遥控回调函数签名
typedef int (*YKOrder)(unsigned char, YK_PARA *, CallbackYK);
extern YKOrder ykOrder_61850;
extern YKOrder ykOrder_61850_plc;
extern PARA_61850 *pFrome61850;
extern int total_pcsnum;
extern float Ave_Max_PW;
extern float Ave_Max_DPW;
int sendtotask(MyData *senddata);
unsigned int countRecvFlag(int num_read);
int myprintbuf(int len, unsigned char *buf);
int backYkFromLce(unsigned char type, void *pdata);
void handleRecvFrom61850(data_info_t *pdata);
#endif