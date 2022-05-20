#ifndef __SONIC_H
#define __SONIC_H

#include "include.h"

//-------------------------------------------Sonic超声波--------------------------------//
sbit TX = P1^0;   //接收引脚
sbit RX = P1^1;   //发射引脚

extern unsigned int ui_Sonic_flag; 
extern unsigned char uc_Dist;

void Timer0Init(void);		//12微秒@12.000MHz
extern unsigned char Sonic(void);
extern void Soinc_Proc(void);

#endif