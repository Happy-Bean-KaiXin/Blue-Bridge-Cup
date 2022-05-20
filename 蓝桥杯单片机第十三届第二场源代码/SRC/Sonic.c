#include "Sonic.h"

unsigned int ui_Sonic_flag;   // 300ms刷新一次
unsigned char uc_Dist;

void Timer0Init(void)		//12微秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xF4;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0停止计时
}

unsigned char Sonic(void)
{
	unsigned char Dist = 0;
	unsigned char num = 10;
	
  //TX引脚发送10个频率为40Hz方波
  Timer0Init();  //初始化定时器0，初始化定时器初值
  TX = 0;        //发射引脚拉为低电平
  
  while(num--)  //发送10个
  {
    TR0 = 1;
    
	while(!TF0);  //计时时间到，控制周期
	TF0 = 0;      //手动清除标志位	
	TX ^= 1;      //取反
		
    Timer0Init();  //初始化定时器0，初始化定时器初值	
  }


//接收方波
	TR0 = 0;		//定时器0停止计时
	TF0 = 0;		//清除TF0标志
	TL0 = 0x00;	//设置定时初值
	TH0 = 0x00;	//设置定时初值
	TR0 = 1;

  while(RX && !TF0);  //等待收到脉冲	或定时溢出
  TR0 = 0;  //定时器0停止计时
	
  if(TF0)   //定时溢出
  {
		 TF0 = 0; //清除标志位		
		 Dist = 255;
  }
  else  //RX接收引脚接收到低电平
  {
		Dist = ((TH0<<8)+TL0)*0.017;
  }
	
  return Dist;
}

void Soinc_Proc(void)
{
	if(ui_Sonic_flag) return;  // 刷新频率
		ui_Sonic_flag = 1;
	
		uc_Dist = Sonic();  //测距
}