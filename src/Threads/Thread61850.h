#ifndef _THREAD_61850_H_
#define _THREAD_61850_H_
#include "lib61850_main.h"
#include "IEC61850_type.h"
#include "interface.h"
// typedef int (*send_fun)(int, ENTRY_MAP *);
extern int g_sys_status;
extern int g_lcd_qmegid;
void CreateThreads(void *para);
#endif