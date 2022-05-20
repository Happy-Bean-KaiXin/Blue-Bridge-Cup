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

//LED��ر���
extern unsigned char uc_LED;
	
// �������ر���
extern unsigned char xdata puc_SEG_Buf[10];   //����ܶ��뻺������  ��С������Ҫ�ѻ�������ĳ��ȱ��10
extern unsigned char xdata puc_SEG_Code[8];  //����ܶ���
extern unsigned char uc_SEG_Position; //�����λѡ
extern unsigned char uc_SEG_flag;   //�����ˢ�±���

// ������Ҫ��־λ����
extern unsigned char Ranging_Show_flag;  // ��������������ʾ��־λ
extern unsigned char uc_SEG_MODE;
extern unsigned char Up_Low_flag;  // 0 Ĭ��Ϊ���޲���

// ������Ҫ��������
extern unsigned int Vrb2;
extern float max,min;
extern unsigned char distance;  // ��ȡʵʱ�������

void Timer1Init(void);		//1000΢��@12.000MHz

void HC138(unsigned char ch);
void ALL_Init(void);

// -------LED------ //
void LED_ON(unsigned char led);
void LED_OFF(unsigned char led);
void LED_Negate(unsigned char led);  //ȡ������LED
void LED_Disp(unsigned char uc_LED);

// -------SEG------ //
extern void SEG_Tran(unsigned char *puc_SEG_Buf,unsigned char *puc_SEG_Code);   //��̬�������ʾת��
extern void SEG_Disp(unsigned char *puc_SEG_Code,unsigned char uc_SEG_Position); //��̬�������ʾ
extern void SEG_Proc();

#endif