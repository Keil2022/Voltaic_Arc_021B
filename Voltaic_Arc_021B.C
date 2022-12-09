//Project: FMD_022A.prj
// Device: FT61F02X
// Memory: Flash 2KX14b, EEPROM 256X8b, SRAM 128X8b
// Author: 
//Company: 
//Version:
//   Date: 2022��8��17��11:42:04
//===========================================================
//===========================================================
#include "SYSCFG.h"
#include "Hardward.c"
#include "LED.c"
#include "KEY.c"
#include "PWM.c"

BITS_T falg;
#define STATE		falg.bit0	//0-�أ�1-��
#define KEY_OK	falg.bit1
#define Rader		falg.bit2

#define eSTATE 	0
#define	eSetAddr	1
#define eRader		2

u8 ReadAPin;

void Test(void);

//�ж����
void interrupt ISR(void)
{
	//PA��ƽ�仯�ж�**********************
	 if(PAIE && PAIF)
    {
		ReadAPin = PORTA; 	//��ȡPORTA������PAIF��־
		PAIF = 0;  				//��PAIF��־λ
		PAIE = 0;  				//���Ƚ�ֹPA6�ж�
		IOCA6 = 0;  				//��ֹPA6��ƽ�仯�ж�
    }
}

void Sys_Init(void) 
{ 
	OSCCON = 0B01010001; 
    //WDT:32KHZ;	
    //IRCF=101=4MHZ;	2Tģʽ
    //Bit0	=	1����ϵͳʱ��Ϊ�ڲ�������0����ʱ��Դ��FOSC<2��0>����������ѡ��ʱѡ��

    INTCON= 0;      //�ݽ�ֹ�����ж�
	
	//�˿�����
	//A
    TRISA = 0B00000000;    	//0-�����1-���룻
    PORTA = 0B00000000;	//���ݼĴ���
    WPUA = 0B00000000;    //0-�ر���������1-��������

	//C
    TRISC     = 0B00000000;    //0-�����1-���룻
    PORTC     = 0B00000000;	
    WPUC      = 0B00000000;    //0-�ر���������1-��������
    
    MSCKCON =	0B00000000;
	//MSCKCON.7��������λ:д0
    //MSCKCON.6���VREG_OE:1 = ��Ƭ����ѹ����PA4 �� PC5 �����ѹ����ѹ��\
											  0 = PA4 �� PC5 Ϊ��ͨ IO
    //MSCKCON.5����T2CKSRC:0 = TIMER2 ʱ��ԴΪϵͳʱ��\
												1 = TIMER2 ʱ��ԴΪ�ڲ� 32MHz
    //MSCKCON.4����SLVREN:1 = �� LVR\
											0 = ��ֹ LVR\
											�� UCFG1<1:0>��Ϊ 01 ʱ����λ��ʵ������
    
    //MSCKCON.3��������λ��д0
    //MSCKCON.2����CKMAVG:1 = ��ƽ��ģʽ���Զ��������ۼ� 4 �Σ�\
												0 = �ر�ƽ��ģʽ
    //MSCKCON.1����CKCNTI:1 = ʹ�ܿ�ʱ�Ӳ�����ʱ������\
											0 = �رտ�ʱ�Ӳ�����ʱ������
    //MSCKCON.0��������λ��д0

    CMCON0 = 0B00000111;     //�رձȽ�����CxINΪ����IO��
    
//    ADCON1 = 0B01100000;     //DIVS=0,ʱ��ѡFOSC    //ADCS[2:0]=110,��ƵFOSC/64
//    ADCON0 = 0B10011101;     //B7,ADFM=1,����Ҷ���                         
   // ANSEL = 0X00;	//�ر�����ģ������

    OPTION = 0B00001000;    //Bit3 = 1 Ԥ��Ƶ������� WDT��PS=000=1:1 WDT RATE	
											//Bit7(RAPU)=0 ENABLED PULL UP PA
                                            
	IOCA6 = 0;  						//��ֹPA6��ƽ�仯�ж�
    
	PAIE =1;   						//ʹ��PA INT�ж�
	GIE =1;    						//ʹ��ȫ���ж�
}

void PA_Level_Change_INITIAL(void)
{
	ANSEL = 0X00;			//�ر�����ģ�����빦��
    
    TRISA6 = 1;						//SET INPUT				
    
	ReadAPin = PORTA;			//��PA��ƽ�仯�ж�
	PAIF =0;   						//��PA INT�жϱ�־λ
    
	IOCA6 = 1;  				//ʹ��PA6��ƽ�仯�ж�
    
	PAIE =1;   						//ʹ��PA INT�ж�
	GIE =1;    						//ʹ��ȫ���ж�
}

//���ݳ�ʼ��
void InitRam(void)
{
    u8 set;
    
	DelayMs(20);
    set = EEPROMread(eSetAddr);
    if(set!=0x5A)
    {
		set=0x5A;
        STATE = 0;
		Rader = 1;

		EEPROMwrite(eSTATE,STATE);
		EEPROMwrite(eSetAddr, set); 
        EEPROMwrite(eRader,Rader);
    }
    STATE = EEPROMread(eSTATE);
	Rader = EEPROMread(eRader);

	KEY_OK = Fault;
}

//������
int main(void)
{
	//DelayS(2);	//�Ȼ����������λ
    
	Sys_Init();
    Key_Init();
	LED_Init();
	
	InitRam();

	while(1)
    {
		DelayMs(10);	
       
		if(KEY == 0)
        {
            KEY_OK = True;
		}
		else	//���������Ч
        {
			if(KEY_OK == True)
            {
                KEY_OK = Fault;
            }
        }
        
		//Test();
        
		PA_Level_Change_INITIAL();
		SLEEP(); 
	}
	return 0;
}

void Test(void)
{
	if(KEY == 0)
	{
		for(u8 FCount=0;FCount<100;FCount++)	//���100�β���	
		{
			LED = 1;				
			DelayMs(5);  	//10ms 
			LED = 0;
			DelayMs(15); 
		}
	}
	else
	{
		for(u8 FCount=0;FCount<100;FCount++)	//���100�β���	
		{
			LED = 1;
			DelayMs(10);  					//10ms 
			LED = 0;
			DelayMs(10); 
		}
	}
}


//===========================================================