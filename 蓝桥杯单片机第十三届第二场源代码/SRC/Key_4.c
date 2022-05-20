#include "Key_4.h"

unsigned char Key4_read()//扫描按键函数
{
	unsigned char Key_Temp;//按键位置
	unsigned char Key_Value;//返回值
	
	P3|=0x0f;//低四位全部拉高0000 1111
	Key_Temp=P3&0x0f;//识别低四位哪位按下
	
	switch(Key_Temp)
	{
		case(0x0e):Key_Value=7;break;
		case(0x0d):Key_Value=6;break;
		case(0x0b):Key_Value=5;break;
		case(0x07):Key_Value=4;break;
		default:Key_Value=0;
	}
	return Key_Value;
}

void Key4_Proc(void)     //独立按键处理函数
{
	unsigned char Key_Temp,Key_Down,Key_Up;
	unsigned char uc_Key_Value;
	
	static unsigned char Key_Old = 0;

	if(uc_Key_flag)return;//10ms
		uc_Key_flag=1;
	
	Key_Temp=Key4_read();//读取键值
	Key_Down=Key_Temp&(Key_Old^Key_Temp);//按下为按键值，其它为0
	Key_Up =~Key_Temp&(Key_Old^Key_Temp);//按下为抬起值，其它为0
	Key_Old=Key_Temp;
	
	if(Key_Down)
	{
			switch(Key_Down)//按下执行
		{
			case(4):uc_Key_Value=4; S4(); break;
			case(5):uc_Key_Value=5; S5(); break;
			case(6):uc_Key_Value=6; S6(); break;
			case(7):uc_Key_Value=7; S7(); break;
		}
	}
}

void S4()
{
	static unsigned char Key4_State = 0;
	Key4_State++;
	switch(Key4_State)
	{
		case 1: uc_SEG_MODE = 2; break;
		case 2: uc_SEG_MODE = 3; set_para_flag = 1; break;  // 进入第三个界面， set_para_flag变为无效参数
		case 3: uc_SEG_MODE = 1; set_para_flag = 0; break;  // 再次按键按下，set_para_flag变为有效参数
	}
	
	Key4_State = Key4_State%3;
}

void S5()
{
  static unsigned char Key5_State;
	Key5_State++;
	if(Key5_State == 1)
	{
		Up_Low_flag = 1;
	}
	else if(Key5_State == 2)
	{
		Up_Low_flag = 0;
	}
	
	Key5_State = Key5_State%2;
}

void S6()
{
	if(uc_SEG_MODE == 3)
	{
		if(Up_Low_flag == 0)
		{
			max = max+0.5;
			if(max > 5.0)
				max = 0.5;
		}
		else if(Up_Low_flag == 1)
		{
			min = min+0.5;
			if(min > 5.0)
				min = 0.5;
	  }
	}
}

void S7()
{
	if(uc_SEG_MODE == 3)
	{
		if(Up_Low_flag == 0)
		{
			max = max-0.5;
			if(max < 0.5)
				max = 5.0;
		}
		else if(Up_Low_flag == 1)
		{
			min = min-0.5;
			if(min < 0.5)
				min = 5,0;
		}
	}
}