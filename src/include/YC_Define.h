
#ifndef _YC_DEFINE_H_
#define _YC_DEFINE_H_
//遥测数据定义
#define DC_Bus_voltage 0  // 0x1100	"母线电压 int16	0.1 V
#define DC_Bus_current 1  // 0x1101	"母线电流 int16	0.1A
#define DC_power_input 2  // 0x1102	"直流功率 int16	0.1 kW  正为放电，负为充电  要求：求和
#define Line_AB_voltage 3 // 0x1103	"电网AB线电压 int16	0.1 V  要求：求平均
#define Line_BC_voltage 4 // 0x1104	"电网BC线电压 int16	0.1 V  要求：求平均
#define Line_CA_voltage 5 // 0x1105	"电网CA线电压 int16	0.1 V  要求：求平均
#define Phase_A_current 6 // 0x1106	"电网A相电流 int16	0.1 A  要求：求和
#define Phase_B_current 7 // 0x1107	"电网B相电流 int16	0.1 A  要求：求和
#define Phase_C_current 8 // 0x1108	"电网C相电流 int16	0.1 A  要求：求和
#define Power_factor 9	  // 0x1109	"功率因数 int16	0.001
#define Frequency 10	  // 0x110A	"电网频率 int16	0.01 Hz  要求：求平均
#define Active_power 11	  // 0x110B	"交流有功功率 int16	0.1kW 正为放电，负为充电
#define Reactive_power 12 // 0x110C	"交流无功功率 int16	0.1kVar 正为感性，负为容性
#define Apparent_power 12 // 0x110D	"交流视在功率 int16	0.1kVA  要求：求和

#define DC_Bus_voltage 0 //停机，传输到EMS
#endif