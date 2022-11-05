#include "withBAMS.h"
#include "sys.h"
#include <stddef.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "IEC61850_type.h"
#include "interface.h"
#include "Thread61850.h"
// 变流升压舱最大允许放电功率	2	float  Bams_MX_DPW
// 变流升压舱最大允许充电功率	3	float  Bams_MX_PW
// 2最大允许放电 功率 float  2
// 4总电压  float            3
// 5最大允许充电 电流float   4
// 6最大允许放电 电流float   5
// 7电池总电流float          6
// 8电池 SOC float           7
// 9电池剩余可充 电量float   8
// 10电池剩余可放 电量float  9

// 11单体最高电压float       10
// 12单体最低电压float       11
// 13运行状态 u16            12
// 14需求     u16            13
// 3通讯中断（） bool        0
// 15总故障状态  bool        1

SendTo61850 bms_SendTo61850_Tab[] = {

	{2, 0, _BOOL_, 1, 1, 1},
	{14, 1, _BOOL_, 1, 1, 1},
	{0, 2, _FLOAT_, 4, 2, 1},	  // 1最大允许充电 功率 float
	{1, 3, _FLOAT_, 4, 2, 1},	  // 2最大允许放电 功率 float
	{3, 4, _FLOAT_, 4, 2, 1},	  // 4总电压  float
	{4, 5, _FLOAT_, 4, 2, 1},	  // 5最大允许充电 电流float
	{5, 6, _FLOAT_, 4, 2, 1},	  // 6最大允许放电 电流float
	{6, 7, _FLOAT_, 4, 2, 1},	  // 7电池总电流float
	{7, 8, _FLOAT_, 4, 2, 1},	  // 8电池 SOC float
	{8, 9, _FLOAT_, 4, 2, 1},	  // 9电池剩余可充 电量float
	{9, 10, _FLOAT_, 4, 2, 1},	  // 10电池剩余可放 电量float
	{10, 11, _FLOAT_, 4, 2, 1},	  // 11单体最高电压float
	{11, 12, _FLOAT_, 4, 2, 1},	  // 12单体最低电压float
	{12, 13, _U_SHORT_, 2, 2, 1}, // 13运行状态 u16
	{13, 14, _U_SHORT_, 2, 2, 1}  // 14需求
};

int BamsTo61850(unsigned char pcsid, unsigned char *pdata)
{
	int i = 0;
	MyData senddata;
	int ret = 0xff;
	short temp;
	for (i = 0; i < 15; i++)
	{
		senddata.data_info[i].sAddr.portID = INFO_BMS;
		senddata.data_info[i].sAddr.devID = pcsid + 1;
		senddata.data_info[i].sAddr.typeID = bms_SendTo61850_Tab[i].typeID;
		senddata.data_info[i].sAddr.pointID = bms_SendTo61850_Tab[i].pointID;
		senddata.data_info[i].data_size = bms_SendTo61850_Tab[i].data_size;
		senddata.data_info[i].el_tag = bms_SendTo61850_Tab[i].el_tag;

		if (i == 0)
		{
			senddata.data_info[i].data[0] = 1;
		}
		else if (i == 1)
		{
			senddata.data_info[i].data[0] = (unsigned char)pdata[bms_SendTo61850_Tab[i].pos_protocol];
		}
		else
		{
			if (bms_SendTo61850_Tab[i].el_tag == _FLOAT_)
			{
				temp = pdata[bms_SendTo61850_Tab[i].pos_protocol * 2] * 256 + pdata[bms_SendTo61850_Tab[i].pos_protocol * 2 + 1];
				*(float *)&senddata.data_info[i].data = (float)temp;
			}
			else if (bms_SendTo61850_Tab[i].el_tag == _U_SHORT_)
			{
				senddata.data_info[i].data[0] = pdata[bms_SendTo61850_Tab[i].pos_protocol * 2];
				senddata.data_info[i].data[1] = pdata[bms_SendTo61850_Tab[i].pos_protocol * 2 + 1];
			}
		}
	}
	senddata.num = 15;
	ret = sendtotask(&senddata);
	return ret;
}


static int countSumAve_Send(unsigned char pcsid, unsigned char *pdata)
{
	static float sum_Bams_MX_DPW = 0;
	static float sum_Bams_MX_PW = 0;
	static float sum_Bams_Soc = 0;
	static unsigned int sum_errpcs_num = 0;
	static int flag_recv = 0;
	MyData senddata;
	int ret = 0xff;
	short temp1, temp2;

	if ((flag_recv & (1 << pcsid)) == 0)
	{
		printf("188888 flag_recv=0x%2x  pcsid=%d data=%x %x\n", flag_recv, pcsid, pdata[0], pdata[1]);
		flag_recv |= (1 << pcsid);
		printf("99999 flag_recv=0x%2x  pcsid=%d\n", flag_recv, pcsid);
		temp1 = pdata[2 * BMS_FAULT_STATUS] * 256 + pdata[2 * BMS_FAULT_STATUS + 1];
		if (temp1 == 0)
		{
			temp2 = pdata[BMS_MX_CPW * 2] * 256 + pdata[BMS_MX_CPW * 2 + 1];
			printf("aaaaa BMS_MX_CPW=%d  temp2=%d\n", BMS_MX_CPW, temp2);
			sum_Bams_MX_PW += (float)temp2;

			temp2 = pdata[BMS_MX_DPW * 2] * 256 + pdata[BMS_MX_DPW * 2 + 1];
			sum_Bams_MX_DPW += (float)temp2;

			temp2 = pdata[BMS_SOC * 2] * 256 + pdata[BMS_SOC * 2 + 1];
			sum_Bams_Soc += (float)temp2;
		}
		else
			sum_errpcs_num++;
	}
	else
		printf("不该出现！！！！flag_recv=0x%2x pcsid=%d\n", flag_recv, pcsid);
	Ave_Max_PW = sum_Bams_MX_PW / total_pcsnum;
	Ave_Max_DPW = sum_Bams_MX_DPW / total_pcsnum;
	if (flag_recv == g_flag_RecvNeed)
	{

		senddata.data_info[0].sAddr.portID = 1;
		senddata.data_info[0].sAddr.devID = 1;
		senddata.data_info[0].sAddr.typeID = 2;
		senddata.data_info[0].data_size = 4;
		senddata.data_info[0].el_tag = _FLOAT_;
		senddata.data_info[0].sAddr.pointID = 2;
		*(float *)&senddata.data_info[0].data[0] = sum_Bams_MX_PW;

		senddata.data_info[1].sAddr.portID = 1;
		senddata.data_info[1].sAddr.devID = 1;
		senddata.data_info[1].sAddr.typeID = 2;
		senddata.data_info[1].data_size = 4;
		senddata.data_info[1].el_tag = _FLOAT_;
		senddata.data_info[1].sAddr.pointID = 3;
		*(float *)&senddata.data_info[1].data = sum_Bams_MX_DPW;

		senddata.data_info[2].sAddr.portID = 1;
		senddata.data_info[2].sAddr.devID = 1;
		senddata.data_info[2].sAddr.typeID = 2;
		senddata.data_info[2].data_size = 4;
		senddata.data_info[2].el_tag = _FLOAT_;
		senddata.data_info[2].sAddr.pointID = 6;
		*(float *)&senddata.data_info[2].data = sum_Bams_Soc / (total_pcsnum - sum_errpcs_num);

		senddata.data_info[3].sAddr.portID = 1;
		senddata.data_info[3].sAddr.devID = 1;
		senddata.data_info[3].sAddr.typeID = 2;
		senddata.data_info[3].data_size = 4;
		senddata.data_info[3].el_tag = _INT_;
		senddata.data_info[3].sAddr.pointID = 7;

		*(int *)&senddata.data_info[3].data = sum_errpcs_num;
		senddata.num = 4;
		ret = sendtotask(&senddata);

		printf("数据发送完成一次循环！！！！ret=%d\n", ret);
		flag_recv = 0;
		sum_Bams_MX_DPW = 0;
		sum_Bams_MX_PW = 0;
		sum_Bams_Soc = 0;
		sum_errpcs_num = 0;
	}
	return 0;
}
int countPcsNum_Bms(unsigned int flag_recv)
{
	int i;
	int num_pcs = 0;

	for (i = 0; i < 18; i++)
	{
		if ((flag_recv & (1 << i)) != 0)
			num_pcs++;
	}
	return num_pcs;
}
int recvfromBams_ems(unsigned char pcsid, unsigned char type, void *pdata)
{

	switch (type)
	{
	case _ALL_:
	{
		BmsData bms_data;
		static unsigned int flag_recv_bms[] = {0, 0};
		bms_data = *(BmsData *)pdata;
		unsigned char *p = bms_data.buf_data;
		int num_pcs1, num_pcs2, num_pcs;	

		flag_recv_bms[bms_data.bmsid] |= (1 << pcsid);
		if (bms_data.bmsid == 0)
		{

			BamsTo61850(pcsid, p);
			num_pcs1 = countPcsNum_Bms(flag_recv_bms[0]);
		}
		else
		{

			BamsTo61850(pcsid + 18, p);
			num_pcs2 = (flag_recv_bms[1]);
		}

		myprintbuf(bms_data.lendata, bms_data.buf_data);
        num_pcs = num_pcs1 + num_pcs2;
		printf("61850接口收到来自bms数据 recvfromBams_ems pcsid=%d num_pcs=%d\n",pcsid,num_pcs);
		// if(num_pcs>=total_pcsnum)
		//    countSumAve_Send(bms_data.buf_data);
	}
	break;
	case _SOC_:
	{
		short soc = *(short *)pdata;
		printf("收到BAMS传来的soc数据！pcsid=%d soc=%d\n", pcsid, soc);
	}
	break;
	default:
		break;
	}
	return 0;
}

void subscribeFromBams(void)
{

	void *handle;
	char *error;
#define LIB_BAMS_PATH "/usr/lib/libbams_rtu.so"
	typedef int (*outBmsData2Other)(unsigned char, unsigned char, void *); //输出数据
	typedef int (*in_fun)(unsigned char type, outBmsData2Other pfun);	   //命令处理函数指针
	in_fun my_func = NULL;

	printf("打开动态链接库 /usr/lib/libbams_rtu.so\n");

	handle = dlopen(LIB_BAMS_PATH, RTLD_LAZY);
	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();

	*(void **)(&my_func) = dlsym(handle, "SubscribeBamsData");
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);

		exit(EXIT_FAILURE);
	}
	printf("111interface61850 订阅BAMS数据！\n");
	my_func(_ALL_, recvfromBams_ems);
	my_func(_SOC_, recvfromBams_ems);
}