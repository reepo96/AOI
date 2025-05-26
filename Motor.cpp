#include "StdAfx.h"
#include "aoi.h"
#include "Motor.h"
#include <math.h>

#define ZERO_OFFSET 30000

CMotor::CMotor(void)
{

#ifndef _MYUTEST
	GT_HardRst();
	GT_Open();
	GT_Reset();

	m_MoveDirX = NONE;
	m_MoveDirY = NONE;

	AxisInitial(1,0,7,3);
	AxisInitial(2,0,7,3);
	AxisInitial(3,0,7,3);

	AxisRunS(1,40*8000,g_iMotorSpeed,1.00,0.001);
	AxisRunS(2,40*8000,g_iMotorSpeed,1.00,0.001);
	WaitStop(1);
	WaitStop(2);

	GoHome(1,g_iMotorSpeed,10);
	GoHome(2,g_iMotorSpeed,10);

	//GoHome(3,10.00,0.001);
#endif
}

CMotor::~CMotor(void)
{
#ifndef _MYUTEST
	GoHome(1,100.00,10);
	GoHome(2,100.00,10);

	//GoHome(3,10.00,0.001);

    GT_Close();
#endif
}

void CMotor::AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc)
{
#ifndef _MYUTEST
    GT_ClrSts(axis);      //������״̬�Ĵ�����λ
	GT_DrvRst(axis);      //���ָ����������ŷ������ź�

	//GT_AlarmOff(axis);    //���������������ź���Ч
	GT_AlarmOn(axis);   //���������������ź���Ч

	GT_LmtsOn(axis);      //��������λ�ź���Ч
	//GT_LmtsOff(axis);     //��������λ�ź���Ч
	if(axis<3)
		GT_EncOn(axis);       //�򿪿�����ı���������
	else
	    GT_EncOff(axis);      //�رտ�����ı���������
	//GT_CaptHome(axis);  //������������ΪHome �źŲ���
	//GT_CaptIndex(axis);   // ������������ΪIndex �źŲ���
	//
	GT_LmtSns(limit);     //������λ���ش�����ƽ
	GT_EncSns(enc);       //���ÿ�����������ļ�������
	GT_HomeSns(home);     //����Home �źŵĴ�����
	//GT_IndexSns(home); //����Index �źŵĴ�����
	//
	GT_StepPulse(axis);   //������������������ź�
	GT_AxisOn(axis);      //����ʹ��
	Sleep(200);   //�����ʵ���ʱ�ȴ��������ŷ�����
#endif

	
}

void CMotor::AxisRunT(unsigned short axis,long pos,double vel,double acc)
{
#ifndef _MYUTEST

	long lNewPos = pos;
	long lNowPos = 0;
	GT_GetAtlPos(axis,&lNowPos);//ȡ�˶�ǰλ��
	lNowPos = lNowPos * 4;

	if(axis == 1) //x��
	{
		if(lNowPos < pos) //�����˶�
		{
			if(m_MoveDirX != RIGHT)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos + g_iMotorMovOffsetX;
			}
			m_MoveDirX = RIGHT;
		}
		else if(lNowPos > pos) //�����˶�
		{
			if(m_MoveDirX != LEFT)
			{
				//��һ���˶�����������������˶�����
				lNewPos = pos - g_iMotorMovOffsetX;
			}
			m_MoveDirX = LEFT;
		}
	}
	else if(axis == 2) //y��
	{
		if(lNowPos < pos) //�����˶�
		{
			if(m_MoveDirY != UP)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos + g_iMotorMovOffsetY;
			}
			m_MoveDirY = UP;
		}
		else if(lNowPos > pos) //�����˶�
		{
			if(m_MoveDirY != DOWN)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos - g_iMotorMovOffsetY;
			}
			m_MoveDirY = DOWN;
		}
	}

    GT_PrflT(axis); //����Ϊ�������߼Ӽ���
	GT_SetPos(axis,lNewPos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel); //���ÿ������ٶ�
	GT_SetAcc(axis,acc); //���ÿ�������ٶ�
	GT_Update(axis); //ˢ�¿��������

    /*do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		Sleep(100);
	}while(status&0x400); //�ȴ��˶����

    GT_GetCapt(axis,&TempPos); //��ȡ����λ��

    if(TempPos != pos)
    {
        GT_SetVel(axis,0.01);   //�����˶���Ŀ��λ��
        GT_Update(axis);
        do
        {
            GT_GetSts(axis,&status);
            Sleep(100);
        }while(status&0x400); //�ȴ��˶����
    }*/
#endif
}

void CMotor::AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk)
{
#ifndef _MYUTEST
	long lNewPos = pos;
	long lNowPos = 0;
	GT_GetAtlPos(axis,&lNowPos);//ȡ�˶�ǰλ��
	lNowPos = lNowPos * 4;

	if(axis == 1) //x��
	{
		if(lNowPos < pos) //�����˶�
		{
			if(m_MoveDirX != RIGHT)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos + g_iMotorMovOffsetX;
			}
			m_MoveDirX = RIGHT;
		}
		else if(lNowPos > pos) //�����˶�
		{
			if(m_MoveDirX != LEFT)
			{
				//��һ���˶�����������������˶�����
				lNewPos = pos - g_iMotorMovOffsetX;
			}
			m_MoveDirX = LEFT;
		}
	}
	else if(axis == 2) //y��
	{
		if(lNowPos < pos) //�����˶�
		{
			if(m_MoveDirY != UP)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos + g_iMotorMovOffsetY;
			}
			m_MoveDirY = UP;
		}
		else if(lNowPos > pos) //�����˶�
		{
			if(m_MoveDirY != DOWN)
			{
				//��һ���˶����������ң�������˶�����
				lNewPos = pos - g_iMotorMovOffsetY;
			}
			m_MoveDirY = DOWN;
		}
	}

    GT_PrflS(axis); //����ΪS ���߼Ӽ���
	GT_SetPos(axis,lNewPos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel); //���ÿ������ٶ�
	GT_SetMAcc(axis,macc); //���ÿ�������ٶ�
	GT_SetJerk(axis,jerk); //���ÿ�����Ӽ��ٶ�
	GT_Update(axis); //ˢ�¿��������

   /* do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		Sleep(10);
	}while(status&0x400); //�ȴ��˶����

    GT_GetCapt(axis,&TempPos); //��ȡ����λ��

    if(TempPos != pos)
    {
        GT_SetVel(axis,0.01);   //�����˶���Ŀ��λ��
        GT_Update(axis);
        do
        {
            GT_GetSts(axis,&status);
            Sleep(10);
        }while(status&0x400); //�ȴ��˶����
    }*/
#endif
}

short CMotor::GoHome(unsigned short axis,double vel_high,double vel_low)
{
#ifndef _MYUTEST
	long TempPos=0;
	unsigned long status;

	GT_ClrSts(axis); //���ԭ����״̬
	GT_CaptHome(axis); //����ԭ��������Ϊԭ�㲶��ʽ

    GT_PrflS(axis); //����ΪS ���߼Ӽ���
	GT_SetPos(axis,-1000000000); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel_high); //���ÿ������ٶ�
	GT_SetMAcc(axis,1); //���ÿ�������ٶ�
	GT_SetJerk(axis,0.001); //���ÿ�����Ӽ��ٶ�
	GT_Update(axis); //ˢ�¿��������

	
    do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		
		if(!(status&0x400))//����˶��Ѿ����
		{ 
			return 1; //��Ȼû�д���Home �ź�,���ش������1
		}
		Sleep(100);
	}while(!(status&0x8)); //�ȴ�Home �źŴ���

	GT_SmthStp(axis);//ƽ��ֹͣ
    do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
        Sleep(50);
	}while(status&0x400);//�ȴ��˶����

    GT_GetCapt(axis,&TempPos); //��ȡ����λ��
	if(axis!=3)
		TempPos*=4;
    GT_SetPos(axis,TempPos+ZERO_OFFSET); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel_low); //���ÿ������ٶȵ����˶���ԭ��
	GT_Update(axis); //ˢ�¿��������
	do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		Sleep(50);
	}while(status&0x400); //�ȴ��˶����

	GT_ClrSts(axis); //���Home ���񴥷���־
	//GT_CaptIndex(axis); //����ԭ��������ΪIndex ����
	
	Sleep(1000); //�����ʵ���ʱ���ȴ�����̨ͣ��

	GT_ZeroPos(axis); //����̨λ�����㽨����������ϵ

	if(axis == 1) //x��
	{
		m_MoveDirX = LEFT;
	}
	else if(axis == 2) //y��
	{
		m_MoveDirY = DOWN;
	}

#endif
	return 0;
}

void CMotor::WaitStop(unsigned short axis)
{
#ifndef _MYUTEST
	bool bHasPause = false;
	unsigned long status;
	do
	{
		if(g_bStopWork)
		{
			GT_AbptStp(1);
			GT_AbptStp(2);
			GT_AbptStp(3);
			break;
		}

		if(g_bPauseWork)
		{
			if(!bHasPause)
			{
				CString sMsgInfo = g_LoadString("IDS_ALARM_STOPMOTOR","�и澯�����������ֹͣ�˶�");
				::AfxMessageBox(sMsgInfo);

				GT_AbptStp(1);
				GT_AbptStp(2);
				GT_AbptStp(3);
			}

			bHasPause = true;

			Sleep(50);
			continue;
		}

		if(bHasPause)
		{
			long lDestPos = 0;
			double dSpeed = 0.00;
			double dMacc = 0.00;
			double dJerk = 0.00;
			GT_GetPos(1,&lDestPos);
			GT_GetVel(1,&dSpeed);
			GT_GetMAcc(1,&dMacc);
			GT_GetJerk(1,&dJerk);
			AxisRunS(1,lDestPos,dSpeed,dMacc,dJerk);

			GT_GetPos(2,&lDestPos);
			GT_GetVel(2,&dSpeed);
			GT_GetMAcc(2,&dMacc);
			GT_GetJerk(2,&dJerk);
			AxisRunS(2,lDestPos,dSpeed,dMacc,dJerk);

			bHasPause = false;
		}

		GT_GetSts(axis,&status);//��ȡ������״̬
        Sleep(50);
	}while(status&0x400);//�ȴ��˶����
#endif
}

short CMotor::GetPos(unsigned short axis,long *pos)
{
#ifndef _MYUTEST
	return GT_GetAtlPos(axis,pos);
#else
	return 0;
#endif
}

void CMotor::AdjustTrackerHeight(int iPcbHeight)
{
#ifndef _MYUTEST
    int iRightPos = ((double)iPcbHeight-g_dTrackMinWidth) * g_fMotorStep;

	AxisRunS(3,20*8000,50,1.00,0.001);
    
	GoHome(3,50.00,10);

    //�ƶ�Z��
    AxisRunS(3,iRightPos+500,50,1.00,0.001);
	WaitStop(3);
#endif
}

void CMotor::ComputerFactPos(const long lSrcPosX,const long lSrcPosY,long *pDestPosX,long *pDestPosY)
{
	*pDestPosX = lSrcPosX + lSrcPosY * tan(g_dZhouJiaoduOff);
	*pDestPosY = lSrcPosY/cos(g_dZhouJiaoduOff);
}