
/*******************************************************************************
* File Name          :IEC61850_type.h
* Author             :lmk
* Date First Issued  :2016/10/12
* Description        :
*                    :

******************************************************************************/
#ifndef __IEC61850_TYPE_H__
#define __IEC61850_TYPE_H__
#include <stdint.h>
#include <time.h>

// ftok ( IEC61850_IPC_PATH, IEC61850_IPC_KEY );
#define IEC61850_IPC_PATH "/etc/hosts"
#define IEC61850_IPC_KEY 0x70 //共享内存和信号量的key值
#define IEC61850_IPC_MASK 0644

#define SHM_SEM1_PATH  "shm_sem1"  //队列1事件通知信号量
#define SHM_SEM2_PATH  "shm_sem2"  //队列2事件通知信号量
#define SHM_MUTEX1_PATH "shm_mutex1"  //队列1同步访问信号量
#define SHM_MUTEX2_PATH "shm_mutex2"  //队列2同步访问信号量

#define data_num 1024
#define cmd_num 256

#define ST_UINT unsigned int
#define ST_CHAR char
//****************************************************
//数据标识1 portID

enum _PORTID
{
	INFO_EMU = 1, // emu整机信息
	INFO_LCD = 2, // emu下lcd信息
	INFO_PCS = 3, // emu下lcd下pcs信息
	INFO_BMS = 5,
};

//******************************************************
// 3-bool, 5-int, 6-unsigned int,7-float
enum _DATA_TYP
{
	_BOOL = 3,
	_INT = 5,
	_UINT = 6,
	_FLOAT = 7,
};

typedef struct
{
	int portID;
	int devID;
	int typeID;
	int pointID;
} POINT_ADDR;

typedef struct
{
	POINT_ADDR sAddr; //点地址表

	ST_UINT data_size; //数据长度
	ST_CHAR *data;	   //数据指针
	ST_CHAR el_tag;	   //数据类型，3-bool, 5-int, 6-unsigned int,7-float
} ENTRY_MAP;

//实时数据
typedef struct tag_data_info
{
	char buf[16]; //数据值
	ENTRY_MAP map;
} data_info_t;

typedef struct tag_shm_data
{
	unsigned int rpos;
	unsigned int wpos;
	data_info_t slist[data_num];
} shm_data_t;

typedef struct tag_iec61850_shm_packet
{
	shm_data_t shm_que1; //协调控制程序负责写，通信程序负责读
	shm_data_t shm_que2; //通信程序负责写,协调控制程序负责读
} iec61850_shm_packet_t;




#endif /* __IEC61850_TYPE_H__ */
