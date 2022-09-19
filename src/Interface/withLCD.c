#include "withLCD.h"
#include <stddef.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "YX_Define.h"
#include "YC_Define.h"
LCD_YC_YX_DATA yc_data[MAX_TOTAL_PCS_NUM];
// short Yc_PW_Data[MAX_TOTAL_PCS_NUM];//
unsigned int Yx_Pcs_Status = 0;

PARA_61850 Frome61850;
PARA_61850 *pFrome61850 = (PARA_61850 *)&Frome61850;

unsigned char yc_Send_Flag[][4] = {
	{Line_AB_voltage, 1, 0, 1},
	{Line_BC_voltage, 1, 0, 1},
	{Line_CA_voltage, 1, 0, 1},
	{Phase_A_current, 1, 1, 0},
	{Phase_B_current, 1, 1, 0},
	{Phase_C_current, 1, 1, 0},
	{Power_factor, 1, 0, 1},
	{Frequency, 1, 0, 1},
	{Active_power, 1, 0, 0},
	{Reactive_power, 1, 0, 1},
	{Apparent_power, 1, 0, 1},
};

YX_SEND_FLAG yx_send_flag[] = {
	{0, bPcsStoped, 1, 0, 0},
	{0, bSavingStatus, 1, 0, 0},
	{0, bFaultStatus, 1, 0, 0},
	{0, bFffLineRunning, 1, 0, 0},
	{0, bMergeCircuit, 1, 0, 0},
	{0, bPcsRunning, 1, 0, 0},

	{1, bConnectMode, 1, 0, 0},

	{4, bConstPwDischargeMode, 1, 0, 0},
	{4, bConstPwChargeMode, 1, 0, 0},
	{4, bConstCurCharging, 1, 0, 0},
	{4, bConstCurDischarging, 1, 0, 0},

};

unsigned char pcs_fault_flag[MAX_TOTAL_PCS_NUM];

int recvfromlcd(unsigned char type, void *pdata)
{
	int i;
	switch (type)
	{
	case _YC_:
	{
		// int Apparent_power;
		LCD_YC_YX_DATA temp;
		temp = *(LCD_YC_YX_DATA *)pdata;
		yc_data[temp.sn - 1] = temp;
		// Apparent_power=Yc_PW_Data[temp.sn-1];
		if (pcs_fault_flag[temp.sn - 1] == 0)
		{
			// fun_realtime//上传实时数据
		}

		if (temp.sn == pFrome61850->pcsnum - 1) //上传平均值和总和值
		{
			int sum_Apparent_power;
			if (pcs_fault_flag)
				for (i = 0; i < pFrome61850->pcsnum; i++)
				{
					if (pcs_fault_flag[i] == 0)
					{
					}
					// sum_Apparent_power+=
				}
		}
	}
	break;
	case _YX_:
	{
		unsigned char b;
		LCD_YC_YX_DATA temp;
		temp = *(LCD_YC_YX_DATA *)pdata;
		b = temp.pcs_data[0];
		if (b & (1 << bPcsRunning))
		{
			setbit(Yx_Pcs_Status, (32 - temp.sn));
			//			Yx_Pcs_Status |= (1<<(32-temp.sn));
		}
		else
		{
			clrbit(Yx_Pcs_Status, (32 - temp.sn));
		}
	}
	break;
		// case _PARA_:
		// {

		// }
		// break;

	default:
		break;
	}
	return 0;
}

void subscribeFromLcd(void)
{

	void *handle;
	char *error;
#define LIB_LCD_PATH "/usr/lib/libmodtcp.so"
	typedef int (*outData2Other)(unsigned char, void *);		   //输出数据
	typedef int (*in_fun)(unsigned char type, outData2Other pfun); //命令处理函数指针
	in_fun my_func = NULL;

	//打开动态链接库

	handle = dlopen(LIB_LCD_PATH, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();

	*(void **)(&my_func) = dlsym(handle, "SubscribeLcdData");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}

	my_func(_YC_, recvfromlcd);
	// my_func(YC_POST,recvfromdllcan);
	// my_func(CANERR_POST,recvfromdllcan);
}
