#ifndef __KEY_H
#define __KEY_H

#include	"SYSCFG.h"
#include "Hardward.c"

#define KEY    RA6

#define KEY_ShortTime		2		//50ms
#define KEY_LongTime		100	//1s
#define KEY_EndTime		300	//3s

bit KEY_ShortOK;	//短按有效
bit KEY_LongOK;	//长按有效
bit KEY_EndOK;		//结束按键
u8 KEY_Phase;		//按键阶段
u16 KEY_TimeCount;	//按键时间

bit State;

void Key_Init(void)
{
	TRISA6 = 1;	//输入模式	//0-输出；1-输入；
	WPUA6 = 1;	//弱上拉
}

void KEY_Scanf(void)
{
	switch(KEY_Phase)
    {
		case 0:
					//按键1
					if(KEY == KEY_Press)
					{
						KEY_TimeCount++;
						if(KEY_TimeCount >=  KEY_LongTime)
						{
							//KEY_Phase = 1;	//注释掉为 不跳转长按功能
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
					if(KEY == KEY_Lift)	//等待松手
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

