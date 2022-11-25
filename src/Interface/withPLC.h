#ifndef _WITH_PLC_H_
#define _WITH_PLC_H_
// 0 BIT0备用
#define PLC_EMU_BOX_35kV_ON 1// BIT135kV进线柜合闸
#define PLC_EMU_BOX_35kV_OFF 2 // BIT235kV进线柜分闸
#define PLC_EMU_BOX_SwitchD1_ON 3// BIT3开关柜D1合闸
#define PLC_EMU_BOX_SwitchD1_OFF 4// BIT4开关柜D1分闸
#define PLC_EMU_BOX_SwitchD2_ON 5// BIT5开关柜D2合闸
#define PLC_EMU_BOX_SwitchD2_OFF 6// BIT6开关柜D2分闸
#define PLC_EMU_DSTART 7  // BIT0系统一键放电
#define PLC_EMU_STOP   8  // BIT1系统一键停止
#define PLC_EMU_CSTART 9  // BIT2系统一键充电
#define PLC_EMU_TRANSFORMER_ROOM_OPEN 10 // BIT7变压器室外门打开
// 11 BIT3备用
// 12 BIT4备用
// 13 BIT5备用
// 14 BIT6备用
// 15 BIT7备用


void subscribeFromPlc(void);
#endif