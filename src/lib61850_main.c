
#include "lib61850_main.h"
#include "Thread61850.h"
#include <stdio.h>
#include <unistd.h>
#include "sys.h"
int lib61850_main(void *para)
{
	// PARA_MODTCP Para_Modtcp;

	CreateThreads(para);
	return 0;
}
