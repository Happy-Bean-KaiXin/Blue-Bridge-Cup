#ifndef __BSP_PERIP_H
#define __BSP_PERIP_H

#include "include.h"

#define L1 0x01
#define L2 0x02
#define L3 0x04
#define L4 0x08
#define L5 0x10
#define L6 0x20
#define L7 0x40
#define L8 0x80
#define ALL_LED 0xFF

//LED相关变量
extern unsigned char uc_LED;
	
// 数码管相关变量
extern unsigned char xdata puc_SEG_Buf[10];   //数码管段码缓冲数组  有小数点需要把换红数组的长度变成10
extern unsigned char xdata puc_SEG_Code[8];  //数码管段码
extern unsigned char uc_SEG_Position; //数码管位选
extern unsigned char uc_SEG_flag;   //数码管刷新变量

// 做题需要标志位声明
extern unsigned char Ranging_Show_flag;  // 连续测量距离显示标志位
extern unsigned char uc_SEG_MODE;
extern unsigned char Up_Low_flag;  // 0 默认为上限参数

// 做题需要变量声明
extern unsigned int Vrb2;
extern float max,min;
extern unsigned char distance;  // 读取实时距离变量

void Timer1Init(void);		//1000微秒@12.000MHz

void HC138(unsigned char ch);
void ALL_Init(void);

// -------LED------ //
void LED_ON(unsigned char led);
void LED_OFF(unsigned char led);
void LED_Negate(unsigned char led);  //取反单个LED
void LED_Disp(unsigned char uc_LED);

// -------SEG------ //
extern void SEG_Tran(unsigned char *puc_SEG_Buf,unsigned char *puc_SEG_Code);   //动态数码段码示转换
extern void SEG_Disp(unsigned char *puc_SEG_Code,unsigned char uc_SEG_Position); //动态数码管显示
extern void SEG_Proc();

#endif