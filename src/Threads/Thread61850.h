#ifndef _THREAD_61850_H_
#define _THREAD_61850_H_
#include "lib61850_main.h"
#include "IEC61850_type.h"
typedef int (*send_fun)(int, ENTRY_MAP *); 

void CreateThreads(void* para);
#endif