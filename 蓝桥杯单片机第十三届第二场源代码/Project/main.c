/**
	****************************************************************
	* @file main.c
	* @author Happy_Bean
	* @version v2.0
	* @date 2022-05-17
	* @brief  ���ű���Ƭ����ʮ����ʡ�����ڶ�����
	****************************************************************
	* @attention 
	*
	* Diff1. �������ý������˳���Ч
	* Diff2. �����������ʾ
*/

#include "include.h"

unsigned char uc_Key_flag;
unsigned char LED_L8_flag;
unsigned char set_para_flag;  // 0 Ϊ��Ч����  1 �����ò�����������1����Ϊ��Ч����

void main(void)
{
	ALL_Init();  // �ر������ʼ��
	while(1)
	{
		Key4_Proc();  // ����ɨ�躯��
		
		ADC_Proc();  // ADC������
		SEG_Proc();  // ����ܴ�����
		Soinc_Proc();  // ������������
		LED_Proc();  // LED������
		DAC_Proc();  // DAC������
		
		LED_Disp(uc_LED);  // LED��ʾ����
		
	}
}

void Timer1_ISR(void) interrupt 3
{
	static unsigned int flag_1s;
	
	// ����ˢ��Ƶ��
	if(++uc_Key_flag == 5)
		uc_Key_flag = 0;
	
	// �����ˢ��Ƶ��
	if(++uc_SEG_flag == 1)
	{
			uc_SEG_flag = 0;
	}
		SEG_Disp(puc_SEG_Code,uc_SEG_Position);   //�������ʾ
	if(++uc_SEG_Position == 8) uc_SEG_Position = 0;
	
	// ������ˢ��Ƶ��
	if(++ui_Sonic_flag == 300)
		ui_Sonic_flag = 0;
	
	if(++flag_1s == 100)
	{
		flag_1s = 0;
		LED_L8_flag ^= 1;
	}
}

/**
	* @brief LED������
	* @param ��
	* @retval ��
*/
void LED_Proc()
{
	if((Ranging_Show_flag == 0)&&(LED_L8_flag == 1))
	{
		LED_ON(L8);
	}
	else
	{
		LED_OFF(L8);
	}
	
	if(uc_SEG_MODE == 1)
	{
		LED_ON(L1); LED_OFF(L2); LED_OFF(L3);
	}
	else if(uc_SEG_MODE == 2)
	{
		LED_ON(L2); LED_OFF(L1); LED_OFF(L3);
	}
	else if(uc_SEG_MODE == 3)
	{
		LED_ON(L3); LED_OFF(L2); LED_OFF(L1);
	}
}

/**
	* @brief DAC������
	* @param ��
	* @retval ��
*/
void DAC_Proc()
{
	static unsigned char output_dianya;
	
	if(Ranging_Show_flag == 0)
	{
		if(uc_Dist < 20)
			output_dianya = 1;
		else if((uc_Dist <= 80)&&(uc_Dist >= 20))
			output_dianya = 5;
		else if(uc_Dist > 80)
			output_dianya = (1/15)*(uc_Dist - 20)+1;  // ���������������һ������������Ҫ����ǿ������ת��
	}
	else if(Ranging_Show_flag == 1)
	{
		output_dianya = 0;
	}
	
	PCF8591_DAC((unsigned char)output_dianya*51);
}
