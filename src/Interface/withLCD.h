#ifndef _WITH_LCD_H_
#define _WITH_LCD_H_
#include "interface.h"
#include "lib61850_main.h"
typedef struct
{
	int sn;
	int lcdid;
	int pcsid;
	unsigned short pcs_data[MAX_YCDATA_NUM];
	unsigned char data_len;
}LCD_YC_YX_DATA;//

// typedef struct
// {
// 	int iddata;

// 	unsigned short pcs_data[MAX_YCDATA_NUM];
// 	unsigned char data_len;
// }_YC_TO_EMS;//

extern  PARA_61850 *pFrome61850;  
extern unsigned char pcs_fault_flag[];

void subscribeFromLcd(void);
#endif