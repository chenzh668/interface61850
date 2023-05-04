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

extern unsigned char pcs_fault_flag[];
extern unsigned char flag_RecvNeed_PCS[];
void subscribeFromLcd(void);

#endif