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
#include "WritePCS.h"
#include "withLCD.h"
static iec61850_shm_packet_t *shm_addr; /* 共享内存区的首地址指针, 也是iec61850_shm_data_t的指针 */
//static int semid;

sem_t *sem1 = NULL;
sem_t *sem2 = NULL;
sem_t *sem3 = NULL;

sem_t *mutex1_lock = NULL;
sem_t *mutex2_lock = NULL;
static pthread_t COM_THREAD[2];

send_fun pFun;
void ReadDataFrom61850()
{
	int i = 0;
	data_info_t temp;
	sem_wait(mutex2_lock);
	//协调控制程序读数据区的示例代码如下：
	while (shm_addr->shm_que2.rpos != shm_addr->shm_que2.wpos)
	{
		i = shm_addr->shm_que2.rpos;
		temp = shm_addr->shm_que2.slist[i];
		shm_addr->shm_que2.rpos = (shm_addr->shm_que2.rpos + 1) % data_num;
		printf("devID=%d", temp.map.sAddr.devID);
	}
	sem_post(mutex2_lock);
}
void *thread_61850_read(void *arg)
{

     subscribeFromLcd();
	while (1)
	{

		usleep(10);
	}
	return NULL;
}
void *thread_61850_write(void *arg)
{
   while(1)
   {

   }



	return NULL;
}

void CreateThreads(void *para)
{
	pthread_t ThreadID;
	pthread_attr_t Thread_attr;
	int i;
	memcpy((unsigned char *)pFrome61850, (unsigned char *)para, sizeof(PARA_61850));
	for(i=0;i<pFrome61850->pcsnum;i++)
	{
		pcs_fault_flag[i]=0;
	}
	if (FAIL == CreateSettingThread(&ThreadID, &Thread_attr, (void *)thread_61850_write, NULL, 1, 1))
	{
		printf(" thread_61850_write CREATE ERR!\n");
		exit(1);
	}
	if (FAIL == CreateSettingThread(&ThreadID, &Thread_attr, (void *)thread_61850_read, NULL, 1, 1))
	{
		printf(" thread_61850_read CREATE ERR!\n");
		exit(1);
	}
	printf("lib61850start !!!!  pcsnum=%d  thread_61850 CREATE success!\n",pFrome61850->pcsnum);
}

void xxxxx(POINT_ADDR sAddr, void *data, int el_tag, int num)
{
	// 	int portID;
	// int devID;
	// int typeID;
	// int pointID;
	data_info_t data_temp;
	int i = 0;


			

	for (i = 0; i < num; i++)
	{
		data_temp.map.data = data_temp.buf;
		data_temp.map.el_tag = el_tag;
		data_temp.map.sAddr = sAddr;
		switch (el_tag)
		{
		case _BOOL:
			data_temp.map.data = (char*)data;
			data_temp.map.data_size = 1;
			break;
		case _INT:
		case _UINT:
		case _FLOAT:

			memcpy(data_temp.map.data, (char*)data, 4);
			data_temp.map.data_size = 4;
			break;

		default:
			return;
		}

	}
}