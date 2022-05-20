#include "iic.h"

#define DELAY_TIME 5

//I2C�����ڲ���ʱ����
void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}

//I2C���������ź�
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//I2C����ֹͣ�ź�
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//����Ӧ����Ӧ���ź�
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

//�ȴ�Ӧ��
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

//I2C���߷���һ���ֽ�����
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

//I2C���߽���һ���ֽ�����
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
	IIC_SendByte(0x90);//�̶���1001 A0-A2Ϊ0��дΪ0������Ϊ0x90
	IIC_WaitAck();
	IIC_SendByte(ch);//������,1�ǹ������裬3�ǻ���������
	IIC_WaitAck();//�ȴ�Ӧ��
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();//�ȴ�Ӧ��
	dat=IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();
	return dat;
}	

void PCF8591_DAC(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);//�̶���1001 A0-A2Ϊ0��дΪ0������Ϊ0x90
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);  // д���ѹ��255��������ѹΪ5v
	IIC_WaitAck();
	IIC_Stop();
}

void ADC_Proc(void)  // ADC������
{
	if(set_para_flag == 0)  // ��Ч���������£�Rb2���Ƴ������Ƿ������������
	{
		// Vrb2�е�ѹ������1.96���������ݿ�����0 - 4.99  �����ѹ����100��
		if((Vrb2 >= min*100)&&(Vrb2 <= max*100))  // ģ��ת����ѹ���趨��Χ�ڣ���������������
			Ranging_Show_flag = 0;  // �ڹ涨��Χ�ڣ�����������־λ����
		else  // �������趨�ĵ�ѹ���˳�����������
			Ranging_Show_flag = 1;  // ���ݲ������ǹ涨�ķ�Χ�ڣ�����������־λ��1�����˳�����������
	}
}
