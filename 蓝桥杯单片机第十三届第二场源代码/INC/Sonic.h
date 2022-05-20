#ifndef __SONIC_H
#define __SONIC_H

#include "include.h"

//-------------------------------------------Sonic������--------------------------------//
sbit TX = P1^0;   //��������
sbit RX = P1^1;   //��������

extern unsigned int ui_Sonic_flag; 
extern unsigned char uc_Dist;

void Timer0Init(void);		//12΢��@12.000MHz
extern unsigned char Sonic(void);
extern void Soinc_Proc(void);

#endif