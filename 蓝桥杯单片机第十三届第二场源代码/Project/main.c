/**
	****************************************************************
	* @file main.c
	* @author Happy_Bean
	* @version v2.0
	* @date 2022-05-17
	* @brief  蓝桥杯单片机第十三届省赛（第二场）
	****************************************************************
	* @attention 
	*
	* Diff1. 参数设置界面下退出生效
	* Diff2. 超声波测距显示
*/

#include "include.h"

unsigned char uc_Key_flag;
unsigned char LED_L8_flag;
unsigned char set_para_flag;  // 0 为有效参数  1 在设置参数界面下置1，变为无效参数

void main(void)
{
	ALL_Init();  // 关闭外设初始化
	while(1)
	{
		Key4_Proc();  // 按键扫描函数
		
		ADC_Proc();  // ADC处理函数
		SEG_Proc();  // 数码管处理函数
		Soinc_Proc();  // 超声波处理函数
		LED_Proc();  // LED处理函数
		DAC_Proc();  // DAC处理函数
		
		LED_Disp(uc_LED);  // LED显示函数
		
	}
}

void Timer1_ISR(void) interrupt 3
{
	static unsigned int flag_1s;
	
	// 按键刷新频率
	if(++uc_Key_flag == 5)
		uc_Key_flag = 0;
	
	// 数码管刷新频率
	if(++uc_SEG_flag == 1)
	{
			uc_SEG_flag = 0;
	}
		SEG_Disp(puc_SEG_Code,uc_SEG_Position);   //数码管显示
	if(++uc_SEG_Position == 8) uc_SEG_Position = 0;
	
	// 超声波刷新频率
	if(++ui_Sonic_flag == 300)
		ui_Sonic_flag = 0;
	
	if(++flag_1s == 100)
	{
		flag_1s = 0;
		LED_L8_flag ^= 1;
	}
}

/**
	* @brief LED处理函数
	* @param 无
	* @retval 无
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
	* @brief DAC处理函数
	* @param 无
	* @retval 无
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
			output_dianya = (1/15)*(uc_Dist - 20)+1;  // 这里算出来的数不一定是整数，需要进行强制类型转换
	}
	else if(Ranging_Show_flag == 1)
	{
		output_dianya = 0;
	}
	
	PCF8591_DAC((unsigned char)output_dianya*51);
}
