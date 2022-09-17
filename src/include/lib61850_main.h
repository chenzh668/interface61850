#ifndef _LIB_61850_MAIN_H_
#define _LIB_61850_MAIN_H_
#include "lib61850_main.h"
typedef struct
{
	unsigned char lcdnum;
	unsigned char pcsnum;

	unsigned short balance_rate;
} PARA_61850; //从主控传到61850模块的结构

int lib61850_main(void* para);
#endif