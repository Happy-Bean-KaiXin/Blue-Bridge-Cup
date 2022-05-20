#include "bsp_perip.h"

//LED��ر���
unsigned char uc_LED;
	
// �������ر���
unsigned char xdata puc_SEG_Buf[10];   //����ܶ��뻺������  ��С������Ҫ�ѻ�������ĳ��ȱ��10
unsigned char xdata puc_SEG_Code[8];  //����ܶ���
unsigned char uc_SEG_Position; //�����λѡ
unsigned char uc_SEG_flag;   //�����ˢ�±���

void Timer1Init(void)		//1000΢��@12.000MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x18;		//���ö�ʱ��ֵ
	TH1 = 0xFC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;    //��ʱ��1�����ж�
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

// LED����
void LED_ON(unsigned char led)   //������LED
{
  uc_LED |=  led;   //��ĳλ��1
}

void LED_OFF(unsigned char led)   //�ص���LED
{
	uc_LED &=  ~led;   //��ĳλ��0
}

void LED_Negate(unsigned char led)  //ȡ������LED
{
  uc_LED ^= led;
}

// LED��ʾ����
void LED_Disp(unsigned char uc_LED)
{
	//LED
  P2 &= 0X1F;   //��ո���λ��Ϊ��������׼��
	P0 =  ~uc_LED;   //Ԥ������
	P2 |= 0X80;   //����Ӧ573��������LED������
	P0 =  ~uc_LED;    //������
	P2 &= 0X1F;   //�ر�573
}

// ����ܲ���
void SEG_Tran(unsigned char *puc_SEG_Buf,unsigned char *puc_SEG_Code)   //��̬�������ʾת��
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
		if(puc_SEG_Buf[j+1] == '.')    //С����
		{
			temp = temp&0X7F;
			j++;
		}
		puc_SEG_Code[i] = temp;  // ����ܶ������� �����봫������
  }
}

//-----------------------------------------��̬�������ʾ---------------------//
void SEG_Disp(unsigned char *puc_SEG_Code,unsigned char uc_SEG_Position)
{		
 	//����
	P0 = 0xff;
	P2 = P2 & 0x1f | 0xE0;   //������ܶ�ѡ573 
	P2 &= 0x1f;              //��573
	
	//�����λѡ
	P0  = 1<<uc_SEG_Position; //�������λѡ573 
	P2 = P2 & 0x1f | 0xC0;
	P2 &= 0x1f;
	
	//����ܶ�ѡ
	P0  = puc_SEG_Code[uc_SEG_Position]; //������ܶ�ѡ573 
	P2 = P2 & 0x1f | 0xE0;
	P2 &= 0x1f;
}

unsigned char uc_SEG_MODE = 1;  // �����ģʽ��־λ
unsigned char Ranging_Show_flag;  // ��������������ʾ��־λ
unsigned char Up_Low_flag;  // 0 Ĭ��Ϊ���޲���

float dianya_show;  // ��ѹ��ʾ��������
unsigned int Vrb2;  // rb2ͨ�����ձ���
float max = 4.5,min = 0.5;  // ��ʼ�����޵�ѹ��ֵ
float dianya_max,dianya_min;  // ������ϵ�ѹ��ʾ����
unsigned char distance;  // ��ȡʵʱ�������

/**
	* @brief ����ܴ�����
	* @param ��
	* @retval ��
*/
void SEG_Proc()
{
	if(uc_SEG_flag) return;  // ˢ��Ƶ��
	uc_SEG_flag = 1;
	
	Vrb2 = PCF8591_ADC(0x03);  // ʹ�û�������������ADת��
	Vrb2 = Vrb2*1.96;  // ��ѹ����1.96��
	dianya_show = Vrb2;
	
	if(uc_SEG_MODE == 1)  // ��ѹ��ʾ����
	{
		set_para_flag = 0;
		
		dianya_show = (float)dianya_show/100;  // ��С100����ѹ����ǿ������ת��Ϊfloat
	  sprintf(puc_SEG_Buf,"U    %3.2f",(float)dianya_show);
	}
	else if(uc_SEG_MODE == 2)  // ������
	{
		distance = uc_Dist;  // ��ȡʵʱ���ľ���
		
		if(Ranging_Show_flag == 0)   // ����������־λΪ0������������������
		{
			if(distance > 99)  // �������������λ�����������ʾ3λ
			{
				sprintf(puc_SEG_Buf,"L    %03u",(unsigned int)distance);
			}
			else if(distance <= 99)  // ��������С����λ�����������ʾ��λ
			{
				sprintf(puc_SEG_Buf,"L     %02u",(unsigned int)distance);
			}
		}
		else if(Ranging_Show_flag == 1)  // ����������־λΪ1���˳�������������
		{
			sprintf(puc_SEG_Buf,"L    AAA");
		}
	}
	else if(uc_SEG_MODE == 3)  // �������������޽���
	{
		dianya_max = max; dianya_min = min;
		
		sprintf(puc_SEG_Buf,"P  %02.1f %02.1f",(float)dianya_max,(float)dianya_min);
	}
	
	
		SEG_Tran(puc_SEG_Buf,puc_SEG_Code);  // ����ܶ���ת��
}
