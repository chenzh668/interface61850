#ifndef _WITH_LCD_H_
#define _WITH_LCD_H_
#include "interface.h"
#include "lib61850_main.h"

//所有的LCD整机信息数据数据标识1都用2来表示，数据标识2编号从1-6;
//每个LCD下模块信息，数据标识1都用3来表示，数据标识2编号从1-36，每个LCD模块信息占用6个编号，LCD1模块信息数据标识2从1-6，LCD2模块信息数据标识2从7-12，LCD3模块信息数据标识2从13-18，LCD4模块信息数据标识2从19-24，LCD5模块信息数据标识2从25-30，LCD6模块信息数据标识2从31-36.
/*typedef struct
{
	int portID;标识符1 ： lcd整机=2 lcd每个模块=3
	int devID; 标识符2 ： lcd整机=1~6
	int typeID;标识符3 ： =2
	int pointID;标识符4 ：数据编号
}POINT_ADDR;*/
typedef struct
{
	int sn;
	int lcdid;
	int pcsid;
	unsigned short pcs_data[MAX_YCDATA_NUM];
	unsigned char data_len;
} LCD_YC_YX_DATA; //
typedef struct
{
	int posWord;
	int pos;
	unsigned char flag_real;
	unsigned char flag_sum;
	unsigned char flag_average;

} YX_SEND_FLAG; //
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

typedef struct
{
	unsigned char item;	   //项目编号
	unsigned char el_tag;  //  数据类型
	unsigned char data[5]; //参数
} YK_PARA;				   //遥测、遥控参数

typedef int (*CallbackYK)(unsigned char, void *pdata); //遥控回调函数签名
typedef int (*YKOrder)(unsigned char, YK_PARA *, CallbackYK);
extern YKOrder ykOrder_61850;

extern PARA_61850 *pFrome61850;
extern unsigned char pcs_fault_flag[];
void handleRecvFrom61850(data_info_t *pdata);
void subscribeFromLcd(void);
int backYkFromLce(unsigned char type, void *pdata);
#endif