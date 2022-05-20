#include "bsp_perip.h"

//LED相关变量
unsigned char uc_LED;
	
// 数码管相关变量
unsigned char xdata puc_SEG_Buf[10];   //数码管段码缓冲数组  有小数点需要把换红数组的长度变成10
unsigned char xdata puc_SEG_Code[8];  //数码管段码
unsigned char uc_SEG_Position; //数码管位选
unsigned char uc_SEG_flag;   //数码管刷新变量

void Timer1Init(void)		//1000微秒@12.000MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;    //定时器1开启中断
}

void HC138(unsigned char ch)
{
	P2 &= 0x1F; 
	P2 |= (ch<<5);
	P2 &= 0x1F;
}

void ALL_Init(void)
{
	P0 = 0xFF; HC138(4);
	P0 = 0x00; HC138(5);
	
	Timer1Init();
	EA = 1;
}

// LED部分
void LED_ON(unsigned char led)   //开单个LED
{
  uc_LED |=  led;   //将某位置1
}

void LED_OFF(unsigned char led)   //关单个LED
{
	uc_LED &=  ~led;   //将某位置0
}

void LED_Negate(unsigned char led)  //取反单个LED
{
  uc_LED ^= led;
}

// LED显示函数
void LED_Disp(unsigned char uc_LED)
{
	//LED
  P2 &= 0X1F;   //清空高三位，为送数据做准备
	P0 =  ~uc_LED;   //预送数据
	P2 |= 0X80;   //开相应573，即开启LED锁存器
	P0 =  ~uc_LED;    //送数据
	P2 &= 0X1F;   //关闭573
}

// 数码管部分
void SEG_Tran(unsigned char *puc_SEG_Buf,unsigned char *puc_SEG_Code)   //动态数码段码示转换
{
	unsigned char i;
	unsigned char j = 0;
	unsigned char temp;
	
	for(i=0;i<8;i++,j++)
	{
		switch(puc_SEG_Buf[j])
		{
			case '0': temp = 0xc0; break;
			case '1': temp = 0xf9; break;
			case '2': temp = 0xa4; break;
			case '3': temp = 0xb0; break;
			case '4': temp = 0x99; break;
			case '5': temp = 0x92; break;
			case '6': temp = 0x82; break;
			case '7': temp = 0xf8; break;
			case '8': temp = 0x80; break;
			case '9': temp = 0x90; break;   
			
			case 'A': temp = 0x88; break;
			case 'b': temp = 0x83; break;
			case 'C': temp = 0xC6; break;
			case 'd': temp = 0xA1; break;
			case 'E': temp = 0x86; break;
			case 'F': temp = 0x8E; break;
			case 'H': temp = 0x89; break;
			case 'L': temp = 0xC7; break;
			case 'N': temp = 0xC8; break;
			case 'P': temp = 0x8C; break; 
			case 'U': temp = 0xC1; break;
			case '-': temp = 0xBF; break;
			case ' ': temp = 0xFF; break; 
			default: temp = 0XFF;
	  }
		if(puc_SEG_Buf[j+1] == '.')    //小数点
		{
			temp = temp&0X7F;
			j++;
		}
		puc_SEG_Code[i] = temp;  // 数码管段码数组 将段码传入数组
  }
}

//-----------------------------------------动态数码管显示---------------------//
void SEG_Disp(unsigned char *puc_SEG_Code,unsigned char uc_SEG_Position)
{		
 	//消隐
	P0 = 0xff;
	P2 = P2 & 0x1f | 0xE0;   //开数码管段选573 
	P2 &= 0x1f;              //关573
	
	//数码管位选
	P0  = 1<<uc_SEG_Position; //开数码管位选573 
	P2 = P2 & 0x1f | 0xC0;
	P2 &= 0x1f;
	
	//数码管段选
	P0  = puc_SEG_Code[uc_SEG_Position]; //开数码管段选573 
	P2 = P2 & 0x1f | 0xE0;
	P2 &= 0x1f;
}

unsigned char uc_SEG_MODE = 1;  // 数码管模式标志位
unsigned char Ranging_Show_flag;  // 连续测量距离显示标志位
unsigned char Up_Low_flag;  // 0 默认为上限参数

float dianya_show;  // 电压显示参数变量
unsigned int Vrb2;  // rb2通道接收变量
float max = 4.5,min = 0.5;  // 初始上下限电压阈值
float dianya_max,dianya_min;  // 数码管上电压显示变量
unsigned char distance;  // 读取实时距离变量

/**
	* @brief 数码管处理函数
	* @param 无
	* @retval 无
*/
void SEG_Proc()
{
	if(uc_SEG_flag) return;  // 刷新频率
	uc_SEG_flag = 1;
	
	Vrb2 = PCF8591_ADC(0x03);  // 使用滑动变阻器进行AD转换
	Vrb2 = Vrb2*1.96;  // 电压扩大1.96倍
	dianya_show = Vrb2;
	
	if(uc_SEG_MODE == 1)  // 电压显示界面
	{
		set_para_flag = 0;
		
		dianya_show = (float)dianya_show/100;  // 缩小100倍电压，并强制类型转换为float
	  sprintf(puc_SEG_Buf,"U    %3.2f",(float)dianya_show);
	}
	else if(uc_SEG_MODE == 2)  // 测距界面
	{
		distance = uc_Dist;  // 读取实时检测的距离
		
		if(Ranging_Show_flag == 0)   // 连续测量标志位为0，进入连续测量界面
		{
			if(distance > 99)  // 测量距离大于三位数，数码管显示3位
			{
				sprintf(puc_SEG_Buf,"L    %03u",(unsigned int)distance);
			}
			else if(distance <= 99)  // 测量距离小于三位数，数码管显示两位
			{
				sprintf(puc_SEG_Buf,"L     %02u",(unsigned int)distance);
			}
		}
		else if(Ranging_Show_flag == 1)  // 连续测量标志位为1，退出连续测量界面
		{
			sprintf(puc_SEG_Buf,"L    AAA");
		}
	}
	else if(uc_SEG_MODE == 3)  // 参数调整上下限界面
	{
		dianya_max = max; dianya_min = min;
		
		sprintf(puc_SEG_Buf,"P  %02.1f %02.1f",(float)dianya_max,(float)dianya_min);
	}
	
	
		SEG_Tran(puc_SEG_Buf,puc_SEG_Code);  // 数码管段码转换
}
