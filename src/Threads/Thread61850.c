#include "Thread61850.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <semaphore.h>
#include <stdlib.h>
#include <bits/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "sys.h"
#include "IEC61850_type.h"
#include "withLCD.h"
#include "withBAMS.h"
static iec61850_shm_packet_t *shm_addr; /* 共享内存区的首地址指针, 也是iec61850_shm_data_t的指针 */
// static int semid;

sem_t *sem1 = NULL;
sem_t *sem2 = NULL;
sem_t *sem3 = NULL;

sem_t *mutex1_lock = NULL;
sem_t *mutex2_lock = NULL;

// static pthread_t COM_THREAD[2];

// send_fun pFun;

int g_lcd_qmegid;

int g_sys_status = 0;
// void ReadDataFrom61850()
// {
// 	// int i = 0;
// 	// data_info_t temp;
// 	sem_wait(mutex2_lock);
// 	//协调控制程序读数据区的示例代码如下：
// 	while (shm_addr->shm_que2.rpos != shm_addr->shm_que2.wpos)
// 	{
// 		i = shm_addr->shm_que2.rpos;
// 		temp = shm_addr->shm_que2.slist[i];
// 		shm_addr->shm_que2.rpos = (shm_addr->shm_que2.rpos + 1) % data_num;
// 		printf("devID=%d", temp.map.sAddr.devID);
// 	}
// 	sem_post(mutex2_lock);
// }

int ReadDataFrom61850()
{
	int i = 0;

	sem_wait(mutex2_lock);
	printf("ReadDataFrom61850 shm_addr->shm_que2.wpos= %d  shm_addr->shm_que2.rpos = %d\n ", shm_addr->shm_que2.wpos, shm_addr->shm_que2.rpos);
	while (shm_addr->shm_que2.wpos != shm_addr->shm_que2.rpos)
	{
		i = shm_addr->shm_que2.rpos;
		handleRecvFrom61850(&shm_addr->shm_que2.slist[i]);

		shm_addr->shm_que2.rpos++;
		shm_addr->shm_que2.rpos %= 1024;
	}
	sem_post(mutex2_lock);
	return 0;
}
void testCallYK(void)
{
	// YK_PARA para;
	// short xx = -12;
	// para.devid = 0;
	// para.el_tag = _SHORT_;
	// para.para_num = 1;
	// para.type = 1;
	// para.scope = 1;
	// *(short *)para.para = xx;
	// printf("testCallYK 61850\n");
	// if (ykOrder_61850 != NULL)
	// {
	// 	printf("111testCallYK 61850\n");
	// 	ykOrder_61850(1, &para, NULL);
	// }
}
void *thread_61850_read(void *arg)
{
	struct timespec waittime;
	struct timeval nowtime;
	key_t key1 = ftok(IEC61850_IPC_PATH, IEC61850_IPC_KEY);
	int shmid = shmget(key1, 0, 0);
	if (shmid == -1)
	{
		sleep(1);
		printf("thread_61850_read shmid == -1\n");
		return NULL;
		// continue;
	}

	printf("shmget ok!\n");

	//挂接共享内存
	// struct _iec61850_shm_packet *p_iec61850_shm_packet =(struct _iec61850_shm_packet *)shmat(shmid,0,0);
	shm_addr = (iec61850_shm_packet_t *)shmat(shmid, 0, 0);
	if (shm_addr == NULL)
	{
		sleep(1);
		printf("shm_addr == NULL\n");
		return NULL;
		// continue;
	}
	shm_addr->shm_que1.wpos = 0;
	shm_addr->shm_que1.rpos = 0;
	shm_addr->shm_que2.wpos = 0;
	shm_addr->shm_que2.rpos = 0;
	// O_CREAT, 0666,
	// mode_t mask = umask(0);
	if ((sem1 = sem_open(SHM_SEM1_PATH, O_RDWR, 0644, 0)) == SEM_FAILED)
	//	if ((sem1 = sem_open(SHM_SEM1_PATH, O_CREAT, 0666, 0)) == SEM_FAILED)
	{
		puts("<1>.semaphore is not exit");
		sleep(1);
		//	umask(mask);

		return NULL;
	}
	// umask(mask);
	if ((mutex1_lock = sem_open(SHM_MUTEX1_PATH, O_RDWR, 0644, 0)) == SEM_FAILED)
	{
		puts("<2>.semaphore is not exit");
		sleep(1);
		return NULL;
	}

	if ((sem2 = sem_open(SHM_SEM2_PATH, O_RDWR, 0644, 0)) == SEM_FAILED)
	{
		puts("<3>.semaphore is not exit");
		sleep(1);
		return NULL;
	}

	if ((mutex2_lock = sem_open(SHM_MUTEX2_PATH, O_RDWR, 0644, 0)) == SEM_FAILED)
	{
		puts("<4>.semaphore is not exit");
		sleep(1);
		return NULL;
	}

	g_sys_status = 1;

	// testCallYK();

	while (1)
	{
		gettimeofday(&nowtime, NULL);
		waittime.tv_sec = nowtime.tv_sec + 10;
		waittime.tv_nsec = 0;

		if (sem_timedwait(sem2, &waittime))
		{
			perror("<12>.semaphore\n");
			continue;
		}

		ReadDataFrom61850();
	}
	return NULL;
}

void sendParaLcd(void)
{
	float temp;
	int i, j;

	// printf("11111111111111111111\n");
	i = shm_addr->shm_que1.wpos;
	// pcs总数
	shm_addr->shm_que1.wpos++;
	shm_addr->shm_que1.slist[i].sAddr.portID = 1;
	shm_addr->shm_que1.slist[i].sAddr.devID = 1;
	shm_addr->shm_que1.slist[i].sAddr.typeID = 2;
	shm_addr->shm_que1.slist[i].sAddr.pointID = 0;

	shm_addr->shm_que1.slist[i].data_size = 4;
	*(int *)shm_addr->shm_que1.slist[i].data = total_pcsnum;

	printf("发送的数据 d %d %d \n", total_pcsnum, *(int *)shm_addr->shm_que1.slist[i].data);
	shm_addr->shm_que1.slist[i].el_tag = _INT_;

	i = shm_addr->shm_que1.wpos;
	temp = ((float)pFrome61850->balance_rate) / 100;
	shm_addr->shm_que1.wpos++;
	shm_addr->shm_que1.slist[i].sAddr.portID = 1;
	shm_addr->shm_que1.slist[i].sAddr.devID = 1;
	shm_addr->shm_que1.slist[i].sAddr.typeID = 2;
	shm_addr->shm_que1.slist[i].sAddr.pointID = 1;

	shm_addr->shm_que1.slist[i].data_size = 4;
	*(float *)shm_addr->shm_que1.slist[i].data = temp;

	printf("发送的数据 f %f %f \n", temp, *(float *)shm_addr->shm_que1.slist[i].data);
	shm_addr->shm_que1.slist[i].el_tag = _FLOAT_;

	//每个LCD通信状态，PCS个数
	for (j = 0; j < pFrome61850->lcdnum; j++)
	{
		//通信状态，初始设置为通信正常
		i = shm_addr->shm_que1.wpos;

		shm_addr->shm_que1.wpos = (shm_addr->shm_que1.wpos + 1) % data_num;
		shm_addr->shm_que1.slist[i].sAddr.portID = 2;
		shm_addr->shm_que1.slist[i].sAddr.devID = j;
		shm_addr->shm_que1.slist[i].sAddr.typeID = 2;
		shm_addr->shm_que1.slist[i].sAddr.pointID = 0;
		shm_addr->shm_que1.slist[i].data_size = 1;
		shm_addr->shm_que1.slist[i].data[0] = 1; //通信正常

		shm_addr->shm_que1.slist[i].el_tag = _BOOL_;
		//每个LCD下的PCS个数
		i = shm_addr->shm_que1.wpos;
		shm_addr->shm_que1.wpos = (shm_addr->shm_que1.wpos + 1) % data_num;

		shm_addr->shm_que1.slist[i].sAddr.portID = 2;
		shm_addr->shm_que1.slist[i].sAddr.devID = j + 1;
		shm_addr->shm_que1.slist[i].sAddr.typeID = 2;
		shm_addr->shm_que1.slist[i].sAddr.pointID = 1;
		shm_addr->shm_que1.slist[i].data_size = 4;
		*(int *)shm_addr->shm_que1.slist[i].data = (int)pFrome61850->pcsnum[i];
		shm_addr->shm_que1.slist[i].el_tag = _INT_;
	}
}

void *thread_61850_write(void *arg)
{
	int ret_value = 0;
	//	key_t key1 = ftok(IEC61850_IPC_PATH, IEC61850_IPC_KEY);
	msgLcd pmsg;
	MyData senddata;
	int i, j;
	int flag = 0;
	// int shmid = shmget(key1, 0, 0);
	// if (shmid == -1)
	// {

	// 	sleep(1);
	// 	return NULL;
	// 	// continue;
	// }

	// printf("shmget ok!  size=%d\n", sizeof(data_info_t));

	while (!g_sys_status)
	{
		sleep(1);
	}

	//发送参数
	sem_wait(mutex1_lock);
	sendParaLcd();
	sem_post(mutex1_lock);

	sem_post(sem1);
	while (1)
	{
		ret_value = os_rev_msgqueue(g_lcd_qmegid, &pmsg, sizeof(msgLcd), 0, 5);
		if (ret_value >= 0)
		{
			memcpy((char *)&senddata, pmsg.data, sizeof(MyData));
			flag = 1;
			if (senddata.num > 0)
			{

				sem_wait(mutex1_lock);
				for (i = 0; i < senddata.num; i++)
				{
					j = shm_addr->shm_que1.wpos;
					shm_addr->shm_que1.slist[j].sAddr = senddata.data_info[i].sAddr;
					shm_addr->shm_que1.slist[j].data_size = senddata.data_info[i].data_size;
					shm_addr->shm_que1.slist[j].el_tag = senddata.data_info[i].el_tag;
					memcpy(shm_addr->shm_que1.slist[j].data, senddata.data_info[i].data, senddata.data_info[i].data_size);
					shm_addr->shm_que1.wpos = (shm_addr->shm_que1.wpos + 1) % data_num;
				}
				sem_post(mutex1_lock);
			}
		}
		else if (flag == 1)
		{
			flag = 0;
			sem_post(sem1);
		}
		// else
		// {
		// 	sem_wait(mutex1_lock);
		//     sendParaLcd();
		//     sem_post(mutex1_lock);

		//    sem_post(sem1);

		// }

		// else
		// {
		// 	sem_wait(mutex1_lock);
		// 	sendParaLcd();
		// 	sem_post(mutex1_lock);
		// 		sem_post(sem1);
		// }
	}

	return NULL;
}

void CreateThreads(void *para)
{
	pthread_t ThreadID;
	pthread_attr_t Thread_attr;
	key_t key = 0;
	int i;

	memcpy((unsigned char *)pFrome61850, (unsigned char *)para, sizeof(PARA_61850));

	printf("从主程序获得的参数 %d  %d\n", pFrome61850->lcdnum, pFrome61850->balance_rate);
	for (i = 0; i < pFrome61850->lcdnum; i++)
	{
		total_pcsnum += pFrome61850->pcsnum[i];
	}
	for (i = 0; i < total_pcsnum; i++)
	{
		pcs_fault_flag[i] = 0;
	}
	if (total_pcsnum > 36)
		total_pcsnum = 36;
	g_flag_RecvNeed = countRecvFlag(total_pcsnum);
	g_flag_RecvNeed_LCD = countRecvFlag(pFrome61850->lcdnum);
	printf("61850从主程序获得的参数 %d  %d total_pcs=%d  flagneed=%x\n", pFrome61850->lcdnum, pFrome61850->balance_rate, total_pcsnum, g_flag_RecvNeed);

	if (FAIL == CreateSettingThread(&ThreadID, &Thread_attr, (void *)thread_61850_read, NULL, 1, 1))
	{
		printf(" thread_61850_read CREATE ERR!\n");
		//exit(1);
	}
	if (FAIL == CreateSettingThread(&ThreadID, &Thread_attr, (void *)thread_61850_write, NULL, 1, 1))
	{
		printf(" thread_61850_write CREATE ERR!\n");
		//exit(1);
	}

	g_lcd_qmegid = os_create_msgqueue(&key, 1);
    //subscribeFromLcd();
	subscribeFromBams();
	printf("thread_61850 CREATE success!\n");
}

void merge(POINT_ADDR sAddr, void *data, int el_tag, int num)
{
	// 	int portID;
	// int devID;
	// int typeID;
	// int pointID;
	data_info_t data_temp;
	int i = 0;

	for (i = 0; i < num; i++)
	{
		memcpy(data_temp.data, data_temp.data, 16);
		data_temp.el_tag = el_tag;
		data_temp.sAddr = sAddr;
		switch (el_tag)
		{
		case _BOOL_:
			data_temp.data_size = 1;
			break;
		case _INT_:
		case _UINT_:
		case _FLOAT_:

			// memcpy(data_temp.data, (char *)data, 4);
			data_temp.data_size = 4;
			break;

		default:
			return;
		}
	}
}

// static inline uint16_t bswap_16(uint16_t x)
// {
//     return (x >> 8) | (x << 8);
// }

// static inline uint32_t bswap_32(uint32_t x)
// {
//     x = ((x << 8) &0xFF00FF00) | ((x >> 8) &0x00FF00FF);
//     return (x >> 16) | (x << 16);
// }