#ifndef _WITH_BAMS_H_
#define _WITH_BAMS_H_
#define _ALL_ 1
#define _SOC_ 2

#define BMS_MX_CPW 0						 //电池分系统 n 最大允许充电功率
#define BMS_MX_DPW 1						 //电池分系统 n 最大允许放电功率
#define BMS_CONN_HEARTBEAT 2				 //电池分系统 n 通讯心跳
#define BMS_MAIN_VOLTAGE 3					 //电池分系统 n 总电压
#define BMS_MX_CCURRENT 4					 //电池分系统 n 最大允许充电电流
#define BMS_MX_DCURRENT 5					 //电池分系统 n 最大允许放电电流
#define BMS_SUM_CURRENT 6					 //电池分系统 n 电池总电流
#define BMS_SOC 7							 //电池分系统 n 电池 SOC
#define BMS_remaining_charging_capacity 8	 //电池分系统 n 电池剩余可充电量
#define BMS_remaining_discharging_capacity 9 //电池分系统 n 电池剩余可放电量
#define BMS_single_MX_voltage 10			 //电池分系统 n 单体最高电压
#define BMS_single_MI_voltage 11			 //电池分系统 n 单体最低电压
#define BMS_SYS_STATUS 12					 //电池分系统 n 状态0-初始化 1-停机 2-启动中 3-运行 4-待机 5-故障 9-关机 255-调试
#define BMS_SYS_NEED 13						 //电池分系统 n 需求 0-禁充禁放(PCS禁止充电放电, PCS应停机或封脉冲) 1-只能充电（PCS禁止放电） 2-只能放电（PCS禁止充电） 3-可充可放（正常）
#define BMS_FAULT_STATUS 14					 //电池分系统 n 总故障状态
typedef struct
{
	unsigned char bmsid;
	unsigned char pcsid_bms;
	unsigned char lendata;		 //收到的数据长度
	unsigned char buf_data[256]; //收到的数据部分

} BmsData;
void subscribeFromBams(void);
#endif