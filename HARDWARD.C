#include "SYSCFG.h"
#ifndef __HARDWARD_H
#define __HARDWARD_H 

#define Ver 5000

#define  Max8bit	255
#define  Max10bit	1023
#define  Max12bit	4095
#define  Max16bit	65535
#define  Max24bit	2^24-1
#define  Max32bit	2^32-1

#define  ADV8(x)    ((u32)(x)*Max8bit/Ver)  	//8bit ADת������
#define  ADV10(x)  	((u32)(x)*Max10bit/Ver)  	//10bit ADת������
#define  ADV12(x)  	((u32)(x)*Max12bit/Ver)  	//12bit ADת������
#define  ADV16(x)   ((u32)(x)*Max16bit/Ver)  	//16bit ADת������

#define KEY_Press 	1
#define KEY_Lift 		0

#define	Lid_ON				1
#define	Lid_OFF			0

#define ON 	1
#define OFF	0

#define HIGHT	1
#define LOW 	0

#define True 	1
#define Fault		0

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#define Nop() 		asm("nop")
#define ClrWdt() 	asm("clrwdt")
#define Stop() 		asm("stop")
//#define SLEEP() 	asm("SLEEP")
#define  MEM(x,y)   (*(((u8 *)(&x))+y)) //ȡ������ĳһλ,X����,��y���ֽ�?

//��־��
typedef struct {
    unsigned char bit0         : 1;
    unsigned char bit1         : 1;
    unsigned char bit2         : 1;
    unsigned char bit3         : 1;
    unsigned char bit4         : 1;     
    unsigned char bit5         : 1; 
    unsigned char bit6         : 1;     
    unsigned char bit7         : 1;     
} BITS_T;

/*-------------------------------------------------
 *	�������ƣ�DelayUs
 *	���ܣ�   ����ʱ���� --16M-2T--��ſ�1%����.
 *	���������Time ��ʱʱ�䳤�� ��ʱʱ��Time Us
 *	���ز������� 
 -------------------------------------------------*/
void DelayUs(unsigned char Time)
{
	unsigned char a;
	for(a=0;a<Time;a++)
	{
		NOP();
	}
}                  
/*-------------------------------------------------
 *	�������ƣ�DelayMs
 *	���ܣ�   ����ʱ����
 *	���������Time ��ʱʱ�䳤�� ��ʱʱ��Time ms
 *	���ز������� 
 -------------------------------------------------*/
void DelayMs(unsigned int Time)
{
	unsigned int a,b;
	for(a=0;a<Time;a++)
	{
		for(b=0;b<5;b++)
		{
		 	DelayUs(49); 	//��1%
		}
	}
}
/*-------------------------------------------------
 *	�������ƣ�DelayS
 *	���ܣ�   ����ʱ����
 *	���������Time ��ʱʱ�䳤�� ��ʱʱ��Time S
 *	���ز������� 
 -------------------------------------------------*/
void DelayS(unsigned char Time)
{
	unsigned char a,b;
	for(a=0;a<Time;a++)
	{
		for(b=0;b<10;b++)
		{
		 	DelayMs(100); 	
		}
	}
}

/*----------------------------------------------------
 *	�������ƣ�EEPROMread
 *	���ܣ�   ��EEPROM����
 *	���������EEAddr ���ȡ���ݵĵ�ַ
 *	���ز�����ReEEPROMread  ��Ӧ��ַ����������
 ----------------------------------------------------*/
u8 EEPROMread(u8 EEAddr)
{
	u8 ReEEPROMread;

	EEADR = EEAddr;    
	RD = 1;
	ReEEPROMread = EEDAT;     //EEPROM�Ķ����� ReEEPROMread = EEDATA;
	return ReEEPROMread;
}

/*---------------------------------------------------- 
 *	�������ƣ�EEPROMwrite
 *	���ܣ�   д���ݵ�EEPROM
 *	���������EEAddr  ��Ҫд�����ݵĵ�ַ
 *	          Data    ��Ҫд�������
 *	���ز�������
 ----------------------u8------------------------------*/
void EEPROMwrite(u8 EEAddr,u8 Data)
{
	GIE = 0;				//д���ݱ���ر��Д�
	while(GIE); 			//�ȴ�GIEد
	EEADR = EEAddr; 	 	//EEPROM�ĵ�ַ
	EEDAT = Data;		 	//EEPROM��д����  EEDATA = Data;
	EEIF = 0;
	EECON1 |= 0x34;			//��λWREN1,WREN2,WREN3��������.
	WR = 1;					//��λWR�������
	while(WR);      		//�ȴ�EEд�����
	GIE = 1;
	DelayMs(9);
}
 
           
/*-------------------------------------------------
 *  ������  GET_ADC_DATA
 *	���ܣ� ��ȡͨ��ADC��
 *  ����� AN_CN ͨ�����
 *  ����� INT����AD�����β������˛�
 --------------------------------------------------*/
/*u16 GET_ADC_DATA (u8 AN_CH) 
{ 
	u8 i;
	u8 	ADCON0Buff; 
	u16  tBuffer = 0;
	u16  ADC_DATA=0;
	 
	ADCON0Buff = ADCON0 & 0B11100011; 	//���ͨ����

	AN_CH <<=2;              
	ADCON0Buff |=  AN_CH;   			//(BIT4-BIT2)CHS=010,ѡ��AN2ͨ��      

	ADCON0 = ADCON0Buff; 				//���¼���ͨ����
    delay_4us(20); 
	GO_DONE = 1;             			//����ADC 
	while( GO_DONE==1 );    			//�ȴ�ADCת�����

	ADC_DATA =  ADRESH;
	ADC_DATA <<=8;
	ADC_DATA |= ADRESL;      			//10Bit ADCֵ����
	tBuffer =  ADC_DATA;
	return  tBuffer;
} 

*/  
//10λAD�D�Q����,�ο���ѹΪVCC
//adchs ��ģ�Mͨ��
u16 ADC10(u8 adchs)	
{
	u16 adz;
	ADCON1=0x10;
	ADCON0= 0x81 | adchs;
	Nop();
	Nop();
	Nop();
	GO_DONE= 1;
	while(GO_DONE);
	MEM(adz,1)=ADRESH;
	MEM(adz,0)=ADRESL;
	return adz;
}

//8λAD�D�Q����,�ο���ѹΪVCC
//adchs ��ģ�Mͨ��
u8 ADC8(u8 adchs)	
{
	ADCON1 = 0x10;
	ADCON0 = 0x01 | adchs;
	Nop();
	Nop();
	Nop();
	Nop();
	GO_DONE = 1;
	while(GO_DONE);
	return (ADRESH);
}

#endif