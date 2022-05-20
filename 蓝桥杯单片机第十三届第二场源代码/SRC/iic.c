#include "iic.h"

#define DELAY_TIME 5

//I2C总线内部延时函数
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C总线启动信号
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C总线停止信号
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答或非应答信号
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//I2C总线发送一个字节数据
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//I2C总线接收一个字节数据
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}

// PCF8591 
unsigned char PCF8591_ADC(unsigned char ch)
{
  unsigned char dat = 0;
	IIC_Start();
	IIC_SendByte(0x90);//固定的1001 A0-A2为0，写为0，所以为0x90
	IIC_WaitAck();
	IIC_SendByte(ch);//控制字,1是光敏电阻，3是滑动变阻器
	IIC_WaitAck();//等待应答
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();//等待应答
	dat=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return dat;
}	

void PCF8591_DAC(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);//固定的1001 A0-A2为0，写为0，所以为0x90
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);  // 写入电压，255读出来电压为5v
	IIC_WaitAck();
	IIC_Stop();
}

void ADC_Proc(void)  // ADC处理函数
{
	if(set_para_flag == 0)  // 有效参数界面下，Rb2控制超声波是否进入连续测量
	{
		// Vrb2中电压扩大了1.96倍，将数据控制在0 - 4.99  总体电压扩大100倍
		if((Vrb2 >= min*100)&&(Vrb2 <= max*100))  // 模数转化电压在设定范围内，进入连续测距界面
			Ranging_Show_flag = 0;  // 在规定范围内，将连续测距标志位置零
		else  // 不满足设定的电压，退出连续测距界面
			Ranging_Show_flag = 1;  // 数据不在我们规定的范围内，将连续测距标志位置1，即退出连续测距界面
	}
}
