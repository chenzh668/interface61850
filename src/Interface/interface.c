
#include "interface.h"

#include "sys.h"
#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
int total_pcsnum = 0;
//int g_flag_RecvNeed_PCS = 0;
//int g_flag_RecvNeed_LCD = 0;
float Ave_Max_PW = 0;
float Ave_Max_DPW = 0;
int sendtotask(MyData *senddata)
{
	msgLcd msg;
	msg.msgtype = 1;
	int ret;
	if (g_sys_status == 0)
		return 0;
	memcpy(msg.data, (char *)senddata, sizeof(MyData));

	if (msgsnd(g_lcd_qmegid, &msg, sizeof(msgLcd), IPC_NOWAIT) != -1)
	{

		// printf("succ succ succ succ !!!!!!!");
		ret = 1;
	}
	else
	{
		printf("msgsnd err err err!!!!\n");
		ret = 0;
	}
	return ret;
}

unsigned int countRecvFlag(int num_read)
{
	unsigned int flag = 0;
	int i;
	for (i = 0; i < num_read; i++)
	{
		flag |= 1 << i;
	}
	return flag;
}
int myprintbuf(int len, unsigned char *buf)
{
	int i = 0;
	printf("\n 61850 buflen=%d\n", len);
	for (i = 0; i < len; i++)
		printf("0x%x ", buf[i]);
	printf("\n");
	return 0;
}

void handleRecvFrom61850(data_info_t *pdata)
{
	data_info_t temp = *(data_info_t *)pdata;
	int k;
	int type = 0;
	//	int item = 0;
	YK_PARA para;

	printf("FromEMS数据标识 %d %d %d %d\n", temp.sAddr.portID, temp.sAddr.devID, temp.sAddr.typeID, temp.sAddr.pointID);
	para.el_tag = temp.el_tag;
	switch (temp.el_tag)
	{
	case _INT_:
	{
		int tem;
		tem = *(int *)temp.data;
		*(int *)para.data = tem;

		printf("收到整形数=%d\n", tem);
	}

	break;
	case _FLOAT_:
	{
		float tem;
		tem = *(float *)temp.data;
		*(float *)para.data = tem;
		printf("收到浮点数=%f\n", tem);
	}

	break;
	case _BOOL_:
	{
		printf("11收到BOOL数据 %x\n", temp.data[0]);
		para.data[0] = temp.data[0];
	}
	break;
	default:
		return;
	}

	if (temp.sAddr.portID == 1 && (temp.sAddr.typeID == 1 || temp.sAddr.typeID == 9))
	{
		k = 1;
		type = _BMS_YX_;
		if (temp.sAddr.pointID == 0 && temp.sAddr.devID == 1 && temp.sAddr.typeID == 9)
		{
			printf("从EMS获得EMS通信状态");
			para.item = EMS_communication_status; // EMS通信状态
		}
		else if (temp.sAddr.typeID == 9 && temp.sAddr.devID == 0)
		{
			if (temp.sAddr.pointID == 0)
				para.item = one_FM_GOOSE_link_status_A; //一次调频A网GOOSE链路状态
			else if (temp.sAddr.pointID == 1)
				para.item = one_FM_GOOSE_link_status_B; //一次调频B网GOOSE链路状态
		}

		else if (temp.sAddr.typeID == 1)
		{
			if (temp.sAddr.pointID == 1)
				para.item = one_FM_Enable; //一次调频使能
			else if (temp.sAddr.pointID == 2)
				para.item = one_FM_Disable; //一次调频切除
		}
	}
	else if (temp.sAddr.portID == INFO_EMU && temp.sAddr.devID == 1 && temp.sAddr.typeID == 5)
	{
		k = 2;
		para.item = temp.sAddr.pointID;
		if (para.item >= 8 && para.item <= 13)
			type = _BMS_PLC_YK_;
		else
			type = _BMS_YK_;

		printf("EMS遥控要求！！！para.item=%d k=%d\n", para.item, k);
	}
	else if (temp.sAddr.portID == INFO_EMU && temp.sAddr.typeID == 6)
	{
		type = _BMS_YK_;
		if (temp.sAddr.devID == 1)
		{
			if (temp.sAddr.pointID == 1)
				para.item = EMS_PW_SETTING;
			else if (temp.sAddr.pointID == 2)
				para.item = EMS_QW_SETTING;
		}
		else if (temp.sAddr.devID == 0)
		{
			if (temp.sAddr.pointID == 1)
				para.item = ONE_FM_PW_SETTING;
			else if (temp.sAddr.pointID == 2)
				para.item = ONE_FM_QW_SETTING;
		}
	}
	else if (temp.sAddr.portID == INFO_PCS && temp.sAddr.typeID == 5 && temp.sAddr.pointID == 1)
	{
		type = _PCS_YK_;
		para.item = temp.sAddr.devID;
		para.el_tag = temp.el_tag;
		para.data[0] = temp.data[0];
	}

		printf("testCallYK 61850 \n");
		if (type == _BMS_YX_)
		{
			if(ykOrder_61850!=NULL)
			   ykOrder_61850(type, &para, NULL);
		}
			
		else if(type==_BMS_YK_ || type==_PCS_YK_)
		{
			if(ykOrder_61850!=NULL)
               ykOrder_61850(type, &para, backYkFromLce);

		}
		else if(type==_BMS_PLC_YK_)
		{
			if(ykOrder_61850_plc!=NULL)
               ykOrder_61850_plc(type, &para, NULL);

		}
		else
		 	printf("handleRecvFrom61850 未知类型type=%d！\n",type);		

}

int backYkFromLce(unsigned char type, void *pdata)
{
	int xx;
	xx = *(int *)pdata;
	printf("61850接口模块收到 backYkFromLce type=%d xx=%d\n", type, xx);
	return 0;
}