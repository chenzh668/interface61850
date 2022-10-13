
#include "interface.h"

#include "sys.h"
#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
int total_pcsnum = 0;
int g_flag_RecvNeed = 0;
int g_flag_RecvNeed_LCD = 0;
float Ave_Max_PW = 0;
float Ave_Max_DPW = 0;
int sendtotask(MyData *senddata)
{
	msgLcd msg;
	msg.msgtype = 1;
	int ret;

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
	printf("\nbuflen=%d\n", len);
	for (i = 0; i < len; i++)
		printf("0x%x ", buf[i]);
	printf("\n");
	return 0;
}