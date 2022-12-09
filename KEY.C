#ifndef __KEY_H
#define __KEY_H

#include	"SYSCFG.h"
#include "Hardward.c"

#define KEY    RA6

#define KEY_ShortTime		2		//50ms
#define KEY_LongTime		100	//1s
#define KEY_EndTime		300	//3s

bit KEY_ShortOK;	//�̰���Ч
bit KEY_LongOK;	//������Ч
bit KEY_EndOK;		//��������
u8 KEY_Phase;		//�����׶�
u16 KEY_TimeCount;	//����ʱ��

bit State;

void Key_Init(void)
{
	TRISA6 = 1;	//����ģʽ	//0-�����1-���룻
	WPUA6 = 1;	//������
}

void KEY_Scanf(void)
{
	switch(KEY_Phase)
    {
		case 0:
					//����1
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >=  KEY_LongTime)
						{
							//KEY_Phase = 1;	//ע�͵�Ϊ ����ת��������
						}
					}
					else
					{
						if(KEY_TimeCount >= KEY_ShortTime)		KEY_ShortOK = True;
						KEY_TimeCount = 0;
					}
		break;
        
        
        case 1:
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >= KEY_EndTime)
						{
							KEY_Phase = 2;
                            //KEY_LongOK = True;
						}
					}
					else
					{
						KEY_Phase = 0;
						KEY_TimeCount = 0;
                        KEY_LongOK = True;
					}
		break;
        
        
        case 2:
					if(KEY == KEY_Lift)	//�ȴ�����
					{
						KEY_Phase = 0;
						KEY_TimeCount = 0;
                        KEY_LongOK = True;
					}
		break;
        

        default:
					KEY_Phase = 0;
					KEY_TimeCount = 0;
		break;
	}
}

void Key_Handle(void)
{
	if(KEY_ShortOK == True)
    {
		KEY_ShortOK = Fault;
	}
    else {/*do nothing*/}
}

#endif
