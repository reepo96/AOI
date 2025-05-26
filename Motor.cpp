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
    GT_ClrSts(axis);      //控制轴状态寄存器复位
	GT_DrvRst(axis);      //清除指定控制轴的伺服报警信号

	//GT_AlarmOff(axis);    //控制轴驱动报警信号无效
	GT_AlarmOn(axis);   //控制轴驱动报警信号有效

	GT_LmtsOn(axis);      //控制轴限位信号有效
	//GT_LmtsOff(axis);     //控制轴限位信号无效
	if(axis<3)
		GT_EncOn(axis);       //打开控制轴的编码器输入
	else
	    GT_EncOff(axis);      //关闭控制轴的编码器输入
	//GT_CaptHome(axis);  //将控制轴设置为Home 信号捕获
	//GT_CaptIndex(axis);   // 将控制轴设置为Index 信号捕获
	//
	GT_LmtSns(limit);     //设置限位开关触发电平
	GT_EncSns(enc);       //设置控制轴编码器的记数方向
	GT_HomeSns(home);     //设置Home 信号的触发沿
	//GT_IndexSns(home); //设置Index 信号的触发沿
	//
	GT_StepPulse(axis);   //设置轴输出正负脉冲信号
	GT_AxisOn(axis);      //驱动使能
	Sleep(200);   //插入适当延时等待驱动器伺服就绪
#endif

	
}

void CMotor::AxisRunT(unsigned short axis,long pos,double vel,double acc)
{
#ifndef _MYUTEST

	long lNewPos = pos;
	long lNowPos = 0;
	GT_GetAtlPos(axis,&lNowPos);//取运动前位置
	lNowPos = lNowPos * 4;

	if(axis == 1) //x轴
	{
		if(lNowPos < pos) //向右运动
		{
			if(m_MoveDirX != RIGHT)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos + g_iMotorMovOffsetX;
			}
			m_MoveDirX = RIGHT;
		}
		else if(lNowPos > pos) //向左运动
		{
			if(m_MoveDirX != LEFT)
			{
				//上一次运动方向不是向左，则进行运动补偿
				lNewPos = pos - g_iMotorMovOffsetX;
			}
			m_MoveDirX = LEFT;
		}
	}
	else if(axis == 2) //y轴
	{
		if(lNowPos < pos) //向上运动
		{
			if(m_MoveDirY != UP)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos + g_iMotorMovOffsetY;
			}
			m_MoveDirY = UP;
		}
		else if(lNowPos > pos) //向下运动
		{
			if(m_MoveDirY != DOWN)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos - g_iMotorMovOffsetY;
			}
			m_MoveDirY = DOWN;
		}
	}

    GT_PrflT(axis); //设置为梯形曲线加减速
	GT_SetPos(axis,lNewPos); //设置控制轴目标位置
	GT_SetVel(axis,vel); //设置控制轴速度
	GT_SetAcc(axis,acc); //设置控制轴加速度
	GT_Update(axis); //刷新控制轴参数

    /*do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		Sleep(100);
	}while(status&0x400); //等待运动完成

    GT_GetCapt(axis,&TempPos); //读取捕获位置

    if(TempPos != pos)
    {
        GT_SetVel(axis,0.01);   //低速运动到目标位置
        GT_Update(axis);
        do
        {
            GT_GetSts(axis,&status);
            Sleep(100);
        }while(status&0x400); //等待运动完成
    }*/
#endif
}

void CMotor::AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk)
{
#ifndef _MYUTEST
	long lNewPos = pos;
	long lNowPos = 0;
	GT_GetAtlPos(axis,&lNowPos);//取运动前位置
	lNowPos = lNowPos * 4;

	if(axis == 1) //x轴
	{
		if(lNowPos < pos) //向右运动
		{
			if(m_MoveDirX != RIGHT)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos + g_iMotorMovOffsetX;
			}
			m_MoveDirX = RIGHT;
		}
		else if(lNowPos > pos) //向左运动
		{
			if(m_MoveDirX != LEFT)
			{
				//上一次运动方向不是向左，则进行运动补偿
				lNewPos = pos - g_iMotorMovOffsetX;
			}
			m_MoveDirX = LEFT;
		}
	}
	else if(axis == 2) //y轴
	{
		if(lNowPos < pos) //向上运动
		{
			if(m_MoveDirY != UP)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos + g_iMotorMovOffsetY;
			}
			m_MoveDirY = UP;
		}
		else if(lNowPos > pos) //向下运动
		{
			if(m_MoveDirY != DOWN)
			{
				//上一次运动方向不是向右，则进行运动补偿
				lNewPos = pos - g_iMotorMovOffsetY;
			}
			m_MoveDirY = DOWN;
		}
	}

    GT_PrflS(axis); //设置为S 曲线加减速
	GT_SetPos(axis,lNewPos); //设置控制轴目标位置
	GT_SetVel(axis,vel); //设置控制轴速度
	GT_SetMAcc(axis,macc); //设置控制轴加速度
	GT_SetJerk(axis,jerk); //设置控制轴加加速度
	GT_Update(axis); //刷新控制轴参数

   /* do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		Sleep(10);
	}while(status&0x400); //等待运动完成

    GT_GetCapt(axis,&TempPos); //读取捕获位置

    if(TempPos != pos)
    {
        GT_SetVel(axis,0.01);   //低速运动到目标位置
        GT_Update(axis);
        do
        {
            GT_GetSts(axis,&status);
            Sleep(10);
        }while(status&0x400); //等待运动完成
    }*/
#endif
}

short CMotor::GoHome(unsigned short axis,double vel_high,double vel_low)
{
#ifndef _MYUTEST
	long TempPos=0;
	unsigned long status;

	GT_ClrSts(axis); //清回原点轴状态
	GT_CaptHome(axis); //将回原点轴设置为原点捕获方式

    GT_PrflS(axis); //设置为S 曲线加减速
	GT_SetPos(axis,-1000000000); //设置控制轴目标位置
	GT_SetVel(axis,vel_high); //设置控制轴速度
	GT_SetMAcc(axis,1); //设置控制轴加速度
	GT_SetJerk(axis,0.001); //设置控制轴加加速度
	GT_Update(axis); //刷新控制轴参数

	
    do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		
		if(!(status&0x400))//如果运动已经完成
		{ 
			return 1; //仍然没有触发Home 信号,返回错误代码1
		}
		Sleep(100);
	}while(!(status&0x8)); //等待Home 信号触发

	GT_SmthStp(axis);//平滑停止
    do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
        Sleep(50);
	}while(status&0x400);//等待运动完成

    GT_GetCapt(axis,&TempPos); //读取捕获位置
	if(axis!=3)
		TempPos*=4;
    GT_SetPos(axis,TempPos+ZERO_OFFSET); //设置控制轴目标位置
	GT_SetVel(axis,vel_low); //设置控制轴速度低速运动到原点
	GT_Update(axis); //刷新控制轴参数
	do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		Sleep(50);
	}while(status&0x400); //等待运动完成

	GT_ClrSts(axis); //清除Home 捕获触发标志
	//GT_CaptIndex(axis); //将回原点轴设置为Index 捕获
	
	Sleep(1000); //插入适当延时，等待工作台停稳

	GT_ZeroPos(axis); //工作台位置清零建立机床坐标系

	if(axis == 1) //x轴
	{
		m_MoveDirX = LEFT;
	}
	else if(axis == 2) //y轴
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
				CString sMsgInfo = g_LoadString("IDS_ALARM_STOPMOTOR","有告警发生，电机将停止运动");
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

		GT_GetSts(axis,&status);//读取控制轴状态
        Sleep(50);
	}while(status&0x400);//等待运动完成
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

    //移动Z轴
    AxisRunS(3,iRightPos+500,50,1.00,0.001);
	WaitStop(3);
#endif
}

void CMotor::ComputerFactPos(const long lSrcPosX,const long lSrcPosY,long *pDestPosX,long *pDestPosY)
{
	*pDestPosX = lSrcPosX + lSrcPosY * tan(g_dZhouJiaoduOff);
	*pDestPosY = lSrcPosY/cos(g_dZhouJiaoduOff);
}