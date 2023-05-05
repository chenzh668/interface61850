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
// int g_flag_RecvNeed_PCS;
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

// SendTo61850 bms_SendTo61850_Tab[] = {

// 	{2, 0, _BOOL_, 1, 1, 1},
// 	{14, 1, _BOOL_, 1, 1, 1},
// 	{0, 2, _FLOAT_, 4, 2, 10},	  // 1最大允许充电 功率 float
// 	{1, 3, _FLOAT_, 4, 2, 10},	  // 2最大允许放电 功率 float
// 	{3, 4, _FLOAT_, 4, 2, 10},	  // 4总电压  float
// 	{4, 5, _FLOAT_, 4, 2, 1000},  // 5最大允许充电 电流float
// 	{5, 6, _FLOAT_, 4, 2, 1000},  // 6最大允许放电 电流float
// 	{6, 7, _FLOAT_, 4, 2, 1000},  // 7电池总电流float
// 	{7, 8, _FLOAT_, 4, 2, 10},	  // 8电池 SOC float
// 	{8, 9, _FLOAT_, 4, 2, 10},	  // 9电池剩余可充 电量float
// 	{9, 10, _FLOAT_, 4, 2, 10},	  // 10电池剩余可放 电量float
// 	{10, 11, _FLOAT_, 4, 2, 10},  // 11单体最高电压float
// 	{11, 12, _FLOAT_, 4, 2, 10},  // 12单体最低电压float
// 	{12, 13, _U_SHORT_, 2, 2, 1}, // 13运行状态 u16
// 	{13, 14, _U_SHORT_, 2, 2, 1}  // 14需求
// };


SendTo61850 bms_SendTo61850_Tab[] = {
	{2, 0, _BOOL_, 1, 1, 1},   //3通讯中断
	{14,1, _BOOL_, 1, 1, 1},  //15总故障状态
	{0, 1, _FLOAT_, 4, 2, 1},	  // 1最大允许充电 功率 float
	{1, 2, _FLOAT_, 4, 2, 1},	  // 2最大允许放电 功率 float
	{3, 3, _FLOAT_, 4, 2, 10},	  // 4总电压  float
	{4, 4, _FLOAT_, 4, 2, 10},  // 5最大允许充电 电流float
	{5, 5, _FLOAT_, 4, 2, 10},  // 6最大允许放电 电流float
	{6, 6, _FLOAT_, 4, 2, 10},  // 7电池总电流float
	{7, 7, _FLOAT_, 4, 2, 1000},	  // 8电池 SOC float
	{8, 8, _FLOAT_, 4, 2, 1},	  // 9电池剩余可充 电量float
	{9, 9, _FLOAT_, 4, 2, 1},	  // 10电池剩余可放 电量float
	{10, 10, _FLOAT_, 4, 2, 1000},  // 11单体最高电压float
	{11, 11, _FLOAT_, 4, 2, 1000},  // 12单体最低电压float
	{12, 12, _U_SHORT_, 2, 2, 1}, // 13运行状态 u16
	{13, 13, _U_SHORT_, 2, 2, 1}  // 14需求
};


int myprintbuf_bms(unsigned char pcs_id,int len, unsigned char *buf)
{
	int i = 0;
	printf("\n pcdid:%d 61850 buflen=%d\n",pcs_id,len);
	for (i = 0; i < len; i++)
		printf("0x%x ", buf[i]);
	printf("\n");
	return 0;
}

int BamsTo61850(unsigned char pcsid, unsigned char *pdata)
{
	
	int i = 0;
	MyData senddata;
	int ret = 0xff;

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
				float temp_f;
				//unsigned short xxx;
				short yyy;
				// xxx=pdata[bms_SendTo61850_Tab[i].pos_protocol * 2] * 256 + pdata[bms_SendTo61850_Tab[i].pos_protocol * 2 + 1];
                // yyy=(xxx/256)*256+xxx%256;
				// temp_f = (float)(yyy);
				yyy = pdata[bms_SendTo61850_Tab[i].pos_protocol * 2] * 256 + pdata[bms_SendTo61850_Tab[i].pos_protocol * 2 + 1];
		        temp_f = (float)(yyy);
				if(bms_SendTo61850_Tab[i].pos_protocol==6)
				{
					printf("111解析出的电池总电流 %x %x %d %hx %f\n",pdata[bms_SendTo61850_Tab[i].pos_protocol * 2],pdata[bms_SendTo61850_Tab[i].pos_protocol * 2+1],yyy,yyy,temp_f);
					
				}
				temp_f /= bms_SendTo61850_Tab[i].precision;
				*(float *)&senddata.data_info[i].data = temp_f;
				// printf("61850 temp_f:%f 发送的是:%f\n",temp_f,*(float *)&senddata.data_info[i].data);

			}
			else if (bms_SendTo61850_Tab[i].el_tag == _U_SHORT_)
			{
				short short_data =  pdata[bms_SendTo61850_Tab[i].pos_protocol * 2] * 256 + pdata[bms_SendTo61850_Tab[i].pos_protocol * 2 + 1];	
				*(unsigned short *)senddata.data_info[i].data = short_data;
			}
		}
	}
	senddata.num = 15;
	ret = sendtotask(&senddata);
	return ret;
}

static int countSumAve_Send(unsigned char pcsid, unsigned char *pdata,int numpcs)
{
	static float sum_Bams_MX_DPW = 0;//保存变流升压舱最大允许充电功率
	static float sum_Bams_MX_PW = 0; //保存变流升压舱最大允许放电功率
	static float sum_Bams_Soc = 0;
	static int sum_errpcs_num = 0;
	static int flag_recv = 0;
	static float Maximum_individual_voltage = 0; //最高单体电压
	static float Minimum_unit_voltage = 0;  //最低单体电压
	static int Maximum_individual_voltage_count = 0;  //累计 > 3.6V最高单体电压个数 等于PCS总数时 发总禁止充电信号
	static int Minimum_unit_voltage_count = 0;  //累计 < 2.8V最高单体电压个数 等于PCS总数时 发总禁止放电信号；
	float voltage = 0; 
	static int Minimum_unit_flag = 0; 


	// static unsigned char flag_recv_pcs[] = {0,0,0,0,0,0};
	//  static int flag_recv_lcd = 0;
	// int g_flag_RecvNeed_PCS =0xFFFFFFF;
	// printf("188888 flag_recv=0x%2x  pcsid=%d data=%x %x\n", flag_recv, pcsid, pdata[0], pdata[1]);
	// printf("188888 flag_recv=0x%2x  pcsid=%d \n", flag_recv, pcsid);

	MyData senddata;
	int ret = 0xff;
	short temp1, temp2;

	myprintbuf_bms(pcsid,32,pdata);

	// if ((flag_recv & (1 << pcsid)) == 0)
	// {
	// 	printf("188888 flag_recv=0x%2x  pcsid=%d data=%x %x\n", flag_recv, pcsid, pdata[0], pdata[1]);

	// 	flag_recv |= (1 << pcsid);

	// 	printf("99999 flag_recv=0x%2x  pcsid=%d\n", flag_recv, pcsid);
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

		temp2 = pdata[BMS_single_MX_voltage * 2] * 256 + pdata[BMS_single_MX_voltage * 2 + 1];
		voltage = temp2/1000.0;
		if(Maximum_individual_voltage<=voltage){
			Maximum_individual_voltage = voltage;
			// printf("+++++++ 1 Maximum_individual_voltage:%f \n",Maximum_individual_voltage);
		}
		if(voltage > 3.6)
			Maximum_individual_voltage_count++;

		temp2 = pdata[BMS_single_MI_voltage * 2] * 256 + pdata[BMS_single_MI_voltage * 2 + 1];
		voltage = temp2/1000.0;
		if(Minimum_unit_flag == 0){
			Minimum_unit_voltage = voltage;
			Minimum_unit_flag = 1;
		}
		// printf("+++++++2 voltage:%f \n",voltage);
		if(Minimum_unit_voltage>=voltage){
			Minimum_unit_voltage = voltage;
			// printf("+++++++2 Minimum_unit_voltage:%f voltage:%f \n",Minimum_unit_voltage,voltage);
		}
		if(Minimum_unit_voltage < 2.8)
			Minimum_unit_voltage_count++;


	}
	else
		sum_errpcs_num++;
	// }
	// else
	// 	printf("不该出现！！！！flag_recv=0x%2x pcsid=%d\n", flag_recv, pcsid);
	Ave_Max_PW = sum_Bams_MX_PW / total_pcsnum;
	Ave_Max_DPW = sum_Bams_MX_DPW / total_pcsnum;
	// if (flag_recv == g_flag_RecvNeed_PCS)
	
	printf(" 6185000000 numpcs:%x  total_pcsnum:%x\n",numpcs, total_pcsnum);
	if (numpcs == total_pcsnum)//上传平均值和总和值)
	{

		senddata.data_info[0].sAddr.portID = INFO_EMU;
		senddata.data_info[0].sAddr.devID = 1;
		senddata.data_info[0].sAddr.typeID = 2;
		senddata.data_info[0].data_size = 4;
		senddata.data_info[0].el_tag = _FLOAT_;
		senddata.data_info[0].sAddr.pointID = 2;
		*(float *)&senddata.data_info[0].data[0] = sum_Bams_MX_PW;

		senddata.data_info[1].sAddr.portID = INFO_EMU;
		senddata.data_info[1].sAddr.devID = 1;
		senddata.data_info[1].sAddr.typeID = 2;
		senddata.data_info[1].data_size = 4;
		senddata.data_info[1].el_tag = _FLOAT_;
		senddata.data_info[1].sAddr.pointID = 3;
		*(float *)&senddata.data_info[1].data = sum_Bams_MX_DPW;

		senddata.data_info[2].sAddr.portID = INFO_EMU;
		senddata.data_info[2].sAddr.devID = 1;
		senddata.data_info[2].sAddr.typeID = 2;
		senddata.data_info[2].data_size = 4;
		senddata.data_info[2].el_tag = _FLOAT_;
		senddata.data_info[2].sAddr.pointID = 6;
		printf("sum_Bams_Soc:%f  \n",sum_Bams_Soc);
		*(float *)&senddata.data_info[2].data = (sum_Bams_Soc / (total_pcsnum - sum_errpcs_num))/1000.0;

		senddata.data_info[3].sAddr.portID = INFO_EMU;
		senddata.data_info[3].sAddr.devID = 1;
		senddata.data_info[3].sAddr.typeID = 2;
		senddata.data_info[3].data_size = 4;
		senddata.data_info[3].el_tag = _INT_;
		senddata.data_info[3].sAddr.pointID = 7;
		*(int *)&senddata.data_info[3].data = sum_errpcs_num;


		//最高单体电压
		senddata.data_info[4].sAddr.portID = INFO_EMU;
		senddata.data_info[4].sAddr.devID = 1;
		senddata.data_info[4].sAddr.typeID = 2;
		senddata.data_info[4].data_size = 4;
		senddata.data_info[4].el_tag = _FLOAT_;
		senddata.data_info[4].sAddr.pointID = 25;
		printf("1111111111Maximum_individual_voltage:%f  \n",Maximum_individual_voltage);
		*(float *)&senddata.data_info[4].data = Maximum_individual_voltage;

		//最低单体电压
		senddata.data_info[5].sAddr.portID = INFO_EMU;
		senddata.data_info[5].sAddr.devID = 1;
		senddata.data_info[5].sAddr.typeID = 2;
		senddata.data_info[5].data_size = 4;
		senddata.data_info[5].el_tag = _FLOAT_;
		senddata.data_info[5].sAddr.pointID = 26;
		printf("1111111111Minimum_unit_voltage:%f  \n",Minimum_unit_voltage);
		*(float *)&senddata.data_info[5].data = Minimum_unit_voltage;

		//额定功率
		senddata.data_info[6].sAddr.portID = INFO_EMU;
		senddata.data_info[6].sAddr.devID = 1;
		senddata.data_info[6].sAddr.typeID = 2;
		senddata.data_info[6].data_size = 4;
		senddata.data_info[6].el_tag = _FLOAT_;
		senddata.data_info[6].sAddr.pointID = 23;
		*(float *)&senddata.data_info[6].data = 180 * (total_pcsnum - sum_errpcs_num);

		//额定容量
		senddata.data_info[7].sAddr.portID = INFO_EMU;
		senddata.data_info[7].sAddr.devID = 1;
		senddata.data_info[7].sAddr.typeID = 2;
		senddata.data_info[7].data_size = 4;
		senddata.data_info[7].el_tag = _FLOAT_;
		senddata.data_info[7].sAddr.pointID = 24;
		*(float *)&senddata.data_info[7].data = 180 * (total_pcsnum - sum_errpcs_num);

		//总禁止充电
		senddata.data_info[8].sAddr.portID = INFO_EMU;
		senddata.data_info[8].sAddr.devID = 1;
		senddata.data_info[8].sAddr.typeID = 1;
		senddata.data_info[8].data_size = 4;
		senddata.data_info[8].el_tag = _BOOL_;
		senddata.data_info[8].sAddr.pointID = 7;
		if(Maximum_individual_voltage_count == total_pcsnum){
			senddata.data_info[8].data[0] = 1;
		}else{
			senddata.data_info[8].data[0] = 0;
		}
		
		//总禁止放电
		senddata.data_info[9].sAddr.portID = INFO_EMU;
		senddata.data_info[9].sAddr.devID = 1;
		senddata.data_info[9].sAddr.typeID = 1;
		senddata.data_info[9].data_size = 4;
		senddata.data_info[9].el_tag = _BOOL_;
		senddata.data_info[9].sAddr.pointID = 8;
		if(Maximum_individual_voltage_count == total_pcsnum){
			senddata.data_info[9].data[0] = 1;
		}else{
			senddata.data_info[9].data[0] = 0;
		}


		senddata.num = 10;
		ret = sendtotask(&senddata);

		int s;
		for(s=0;s<senddata.num;s++){
			if(senddata.data_info[s].el_tag == _BOOL_){
				printf("发送bms整体数据到61850： %d %d %d %d data:%d\n",senddata.data_info[s].sAddr.portID,senddata.data_info[s].sAddr.devID,\
				senddata.data_info[s].sAddr.typeID,senddata.data_info[s].sAddr.pointID,senddata.data_info[s].data[0]);
			}
			if( senddata.data_info[s].el_tag == _INT_){
				printf("发送bms整体数据到61850： %d %d %d %d data:%d\n",senddata.data_info[s].sAddr.portID,senddata.data_info[s].sAddr.devID,\
				senddata.data_info[s].sAddr.typeID,senddata.data_info[s].sAddr.pointID,*(int *)senddata.data_info[s].data);
			}

			if(senddata.data_info[s].el_tag == _FLOAT_){
				printf("发送bms整体数据到61850： %d %d %d %d data:%f\n",senddata.data_info[s].sAddr.portID,senddata.data_info[s].sAddr.devID,\
				senddata.data_info[s].sAddr.typeID,senddata.data_info[s].sAddr.pointID,*(float *)&senddata.data_info[s].data);
			}
			
		}

		printf("数据发送完成一次循环！！！！ret=%d\n", ret);
		// flag_recv = 0;
		sum_Bams_MX_DPW = 0;
		sum_Bams_MX_PW = 0;
		sum_Bams_Soc = 0;
		sum_errpcs_num = 0;
		Maximum_individual_voltage = 0;
		Minimum_unit_voltage = 0;
		Maximum_individual_voltage_count=0;
		Minimum_unit_voltage_count = 0;
		Minimum_unit_flag = 0;
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

// int recvfromBams_ems(unsigned char pcsid_bms, unsigned char type, void *pdata)
// {
// 	int i;
// 	switch (type)
// 	{
// 	case _ALL_:
// 	{
// 		static unsigned int flag_recv_bms[] = {0, 0};
// 		int num_pcs1, num_pcs2, num_pcs;
// 		BmsData temp = *(BmsData *)pdata;
// 		unsigned char bmsid = temp.bmsid;
// 	    flag_recv_bms[bmsid] |= (1 << pcsid_bms);
// 		if (bms_data.bmsid == 0)
// 		{

// 			BamsTo61850(pcsid, p);
// 			num_pcs1 = countPcsNum_Bms(flag_recv_bms[0]);
// 		}
// 		else
// 		{

// 			BamsTo61850(pcsid + 18, p);
// 			num_pcs2 = (flag_recv_bms[1]);
// 		}

// 		myprintbuf(bms_data.lendata, bms_data.buf_data);
// 		num_pcs = num_pcs1 + num_pcs2;
// 		printf("61850接口收到来自bms数据 recvfromBams_ems pcsid=%d num_pcs=%d\n", pcsid, num_pcs);
// 		if(num_pcs>=total_pcsnum)
// 		{

// 			flag_recv_bms[0] = 0;
// 			flag_recv_bms[1] = 0;
// 		   countSumAve_Send(bms_data.buf_data);

// 	}
// 	break;
// 	case _SOC_:
// 	{
// 		short soc = *(short *)pdata;
// 		printf("收到BAMS传来的soc数据！pcsid=%d soc=%d\n", pcsid, soc);
// 	}
// 	break;
// 	default:
// 		break;
// 	}
// 	return 0;
// }
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

		if (bms_data.bmsid == 0)
		{
			BamsTo61850(pcsid, p);
		    flag_recv_bms[0] |= (1 << pcsid);
	
		}
		else if(bms_data.bmsid == 1)
		{

			BamsTo61850(pcsid + 18, p);
			flag_recv_bms[1] |= (1 << pcsid);

		}
        else
		    printf("recvfromBams_ems 出错！bms_data.bmsid=%d\n",bms_data.bmsid);
		num_pcs1 = countPcsNum_Bms(flag_recv_bms[0]);
		num_pcs2 = countPcsNum_Bms(flag_recv_bms[1]);
		myprintbuf(bms_data.lendata, bms_data.buf_data);
		num_pcs = num_pcs1 + num_pcs2;
		// g_flag_RecvNeed_PCS = num_pcs;
	
		if (bms_data.bmsid == 0)
		{
			countSumAve_Send(pcsid,bms_data.buf_data,num_pcs);
	
		}
		else if(bms_data.bmsid == 1)
		{
			countSumAve_Send(pcsid+14,bms_data.buf_data,num_pcs);

		}

		printf("61850接口收到来自bms数据 recvfromBams_ems bmsid=%d  pcsid=%d num_pcs=%d num_pcs1=%d num_pcs2=%d\n",bms_data.bmsid, pcsid, num_pcs,num_pcs1,num_pcs2);
		if(num_pcs>=total_pcsnum)
		{
			flag_recv_bms[0]=0;
			flag_recv_bms[1]=0;
			//countSumAve_Send(bms_data.buf_data);
		}
		//    
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
	static int yy = 0;
#define LIB_BAMS_PATH "/usr/lib/libbams_rtu.so"
	typedef int (*outBmsData2Other)(unsigned char, unsigned char, void *); //输出数据
	typedef int (*in_fun)(unsigned char type, outBmsData2Other pfun);	   //命令处理函数指针
	in_fun my_func = NULL;

	printf("61850打开动态链接库 /usr/lib/libbams_rtu.so yy=%d\n", yy++);

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