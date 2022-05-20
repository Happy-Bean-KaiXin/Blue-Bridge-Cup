#include "Sonic.h"

unsigned int ui_Sonic_flag;   // 300msˢ��һ��
unsigned char uc_Dist;

void Timer0Init(void)		//12΢��@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xF4;		//���ö�ʱ��ֵ
	TH0 = 0xFF;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0ֹͣ��ʱ
}

unsigned char Sonic(void)
{
	unsigned char Dist = 0;
	unsigned char num = 10;
	
  //TX���ŷ���10��Ƶ��Ϊ40Hz����
  Timer0Init();  //��ʼ����ʱ��0����ʼ����ʱ����ֵ
  TX = 0;        //����������Ϊ�͵�ƽ
  
  while(num--)  //����10��
  {
    TR0 = 1;
    
	while(!TF0);  //��ʱʱ�䵽����������
	TF0 = 0;      //�ֶ������־λ	
	TX ^= 1;      //ȡ��
		
    Timer0Init();  //��ʼ����ʱ��0����ʼ����ʱ����ֵ	
  }


//���շ���
	TR0 = 0;		//��ʱ��0ֹͣ��ʱ
	TF0 = 0;		//���TF0��־
	TL0 = 0x00;	//���ö�ʱ��ֵ
	TH0 = 0x00;	//���ö�ʱ��ֵ
	TR0 = 1;

  while(RX && !TF0);  //�ȴ��յ�����	��ʱ���
  TR0 = 0;  //��ʱ��0ֹͣ��ʱ
	
  if(TF0)   //��ʱ���
  {
		 TF0 = 0; //�����־λ		
		 Dist = 255;
  }
  else  //RX�������Ž��յ��͵�ƽ
  {
		Dist = ((TH0<<8)+TL0)*0.017;
  }
	
  return Dist;
}

void Soinc_Proc(void)
{
	if(ui_Sonic_flag) return;  // ˢ��Ƶ��
		ui_Sonic_flag = 1;
	
		uc_Dist = Sonic();  //���
}