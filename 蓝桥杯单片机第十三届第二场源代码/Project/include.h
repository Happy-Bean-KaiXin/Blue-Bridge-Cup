#ifndef __INCLUDE_H
#define __INCLUDE_H

#include <STC15F2K60S2.H>
#include <intrins.h>
#include <stdio.h>

// ��־λ����
extern unsigned char uc_Key_flag;
extern unsigned char set_para_flag;

// ��������


// ��������
void LED_Proc();
void DAC_Proc();

#include "bsp_perip.h"
#include "iic.h"
#include "Sonic.h"
#include "Key_4.h"

#endif