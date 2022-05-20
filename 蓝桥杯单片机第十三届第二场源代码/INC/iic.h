#ifndef _IIC_H
#define _IIC_H

#include "include.h"

sbit SDA = P2^1;
sbit SCL = P2^0;

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 

unsigned char PCF8591_ADC(unsigned char ch);
void PCF8591_DAC(unsigned char dat);

// 主函数调用
void ADC_Proc(void);  // ADC处理函数

#endif
