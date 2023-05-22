
#include "withPLC.h"
#include "interface.h"
#include <stddef.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
YKOrder ykOrder_61850_plc = NULL;

int recvfromplc(unsigned char type, void *pdata)
{
	static unsigned short temp_last;
	unsigned short temp = *(unsigned short *)pdata;
	int ret = 0;
	MyData senddata;
	int i;
	printf("61850接口模块收到 backYkFromLcd type=%d data=%x\n", type, temp);
	// if (temp_last != temp)
	{
		// for (i = 0; i < (PLC_EMU_CSTART+1); i++)
		for (i = 0; i < (PLC_EMU_TRANSFORMER_ROOM_OPEN+1); i++)
		{
			senddata.data_info[i].sAddr.portID = INFO_PLC;
			senddata.data_info[i].sAddr.devID = 1;
			senddata.data_info[i].sAddr.typeID = 1;
			senddata.data_info[i].sAddr.pointID = i;
			if (i == 0)
			{
				senddata.data_info[i].data[0] = 1;
			}
			else
			{
				if (i >= 7)
				{
					if (i == 7)
					{
						if ((temp & (1 << i)) > 0)
						{
							senddata.data_info[10].data[0] = 1;
						}
						else
						{
							senddata.data_info[10].data[0] = 0;
						}
					}
					else
					{
						if ((temp & (1 << i)) > 0)
						{
							senddata.data_info[i - 1].data[0] = 1;
						}
						else
						{
							senddata.data_info[i - 1].data[0] = 0;
						}
					}
				}else{
					if ((temp & (1 << i)) > 0)
					{
						senddata.data_info[i].data[0] = 1;
					}
					else
					{
						senddata.data_info[i].data[0] = 0;
					}	
				}
				
			}

			senddata.data_info[i].data_size = 1;
			senddata.data_info[i].el_tag = _BOOL_;
		}

		i++;
		senddata.data_info[i].sAddr.portID = 1;
		senddata.data_info[i].sAddr.devID = 1;
		senddata.data_info[i].sAddr.typeID = 1;
		senddata.data_info[i].sAddr.pointID = 16;
		senddata.data_info[i].data_size = 1;
		senddata.data_info[i].el_tag = _BOOL_;
		if ((temp & (1 << 0)) > 0){
			senddata.data_info[i].data[0] = 1;
		}else{
			senddata.data_info[i].data[0] = 0;
		}	

		senddata.num = i;
		ret = sendtotask(&senddata);
		temp_last = temp;
	}
	return ret;
}

void subscribeFromPlc(void)
{

	void *handle;
	char *error;
#define LIB_LCD_PATH "/usr/local/lib/libplc.so"
	// typedef int (*outData2Other)(unsigned char, void *);		   //输出数据
	// typedef int (*in_fun)(unsigned char type, outData2Other pfun); //命令处理函数指针

	printf("订阅PLC数据！！！！\n");
	//打开动态链接库

	handle = dlopen(LIB_LCD_PATH, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();

	*(void **)(&ykOrder_61850_plc) = dlsym(handle, "ykOrderFromBms");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}

	printf("1订阅PLC数据！！！！\n");

	ykOrder_61850_plc(_BMS_YX_, NULL, recvfromplc);
}