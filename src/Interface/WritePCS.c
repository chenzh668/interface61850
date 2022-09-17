#include "WritePCS.h"

// 变流升压舱最大允许放电功率	1	1	2	2	float
// 变流升压舱最大允许充电功率	1	1	2	3	float
// 变流升压舱实际放电功率	    1	1	2	4	float
// 变流升压舱实际充电功率	    1	1	2	5	float
// 电池Soc	                   1	1	2	6	float
// 故障PCS数量	               1	1	2	7	int
// 变流升压舱预警状态	       1	1	1	1	bool
// 变流升压舱告警状态	       1	1	1	2	bool
// 变流升压舱故障状态	       1	1	1	3	bool


void WritePCSData(unsigned char type )
{
	// data_info_t mydata;
	// mydata.map.data = mydata.buf;
	// switch(type)
	// {
	// 	switch(type)
	// 	{
	// 		case  YC:
	// 		break;
	// 		case  YX:
	// 		break;
	// 		default:
	// 		break;
	// 	}
	// }
}


