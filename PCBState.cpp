#include "StdAfx.h"
#include "PCBState.h"
#include "AOI.h"

CPCBState::CPCBState(void)
{
    state = S_NONE;
	ClearState();
}

CPCBState::~CPCBState(void)
{
	ClearState();
}

void CPCBState::ClearState()
{
#ifndef _MYUTEST
	state = S_NONE;

	GT_ExOpt(OUT_EVT_STICK_BOARD_DOWN);   //顶板气缸-落下
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

	GT_ExOpt(OUT_EVT_PRESS_BOARD_LOOSE); //板压紧气缸-松
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

	GT_ExOpt(OUT_EVT_STOP_BOARD_DOWN);   //板停止气缸-落下
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止
#endif

}

CString CPCBState::GetStateDesc()
{
	switch(state)
	{
	case S_NONE:
		return g_LoadString("IDS_PCB_NONE","没有PCB板");
	case S_ACCEPT:
		return g_LoadString("IDS_PCB_ACCEPT","PCB板已被接收");
	case S_CLAMP:
		return g_LoadString("IDS_PCB_CLAM","PCB板已被夹紧");
	case S_UP:
		return g_LoadString("IDS_PCB_UP","PCB板已升起");
	case S_DOWN:
		return g_LoadString("IDS_PCB_DOWN","PCB板已落下");
	case S_LOOSE:
		return g_LoadString("IDS_PCB_LOOSE","PCB板已被松开");
	case S_OUT:
		return g_LoadString("IDS_PCB_OUT","PCB板已被送出");
	default:
		return CString("");
	}
}

bool CPCBState::Accept(bool bSthowMsg)
{
#ifndef _MYUTEST
	CString sMsgInfo;
	if(state == S_ACCEPT )
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

    if(state != S_NONE && state != S_OUT)
    {
		sMsgInfo = g_LoadString("IDS_PCB_EXIST","已有PCB板，不能再接收新PCB板");
		if(bSthowMsg)
			::AfxMessageBox(sMsgInfo);
        return false;
    }

	if(g_iPCBEntryDir == 1)
	{
		if( !((~g_uDevInEvent)&IN_EVT_LEFT_ENTRY) )
		{
			sMsgInfo = g_LoadString("IDS_NOPCB_IN","没有PCB板被送入");
			if(bSthowMsg)
				::AfxMessageBox(sMsgInfo);
			return false;   //左入口信号没有触发
		}
	}
	else
	{
		if( !((~g_uDevInEvent)&IN_EVT_RIGHT_ENTRY) )
		{
			sMsgInfo = g_LoadString("IDS_NOPCB_IN","没有PCB板被送入");
			if(bSthowMsg)
				::AfxMessageBox(sMsgInfo);
			return false;   //右入口信号没有触发
		}
	}

    unsigned short output=0;

	GT_ExOpt(OUT_ALL_STOP);	//停止
    
    GT_ExOpt(OUT_EVT_STOP_BOARD_UP);//板停止气缸-升高
    ::Sleep(100);
	GT_ExOpt(OUT_ALL_STOP);	//停止

    GT_ExOpt(OUT_EVT_CONVEY_MOTOR); //输送带电机动
    ::Sleep(g_iPCBInTime);
	GT_ExOpt(OUT_ALL_STOP);	//停止
    
    state = S_ACCEPT;
#endif
    return true;
}

bool CPCBState::Clamp(bool bSthowMsg)
{
#ifndef _MYUTEST
	unsigned short output=0;
	if(state == S_CLAMP)
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

    if(state != S_ACCEPT && state != S_LOOSE )
    {
        Accept(bSthowMsg);
    }

	if(state != S_ACCEPT &&  state != S_LOOSE)
	{
		return false;
	}

    GT_ExOpt(OUT_EVT_PRESS_BOARD_CLAMP); //板压紧气缸-压紧
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

    state = S_CLAMP;
#endif
    return true;
}

bool CPCBState::Up(bool bSthowMsg)
{
#ifndef _MYUTEST
	unsigned short output=0;
	if(state == S_UP)
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

	if(state != S_CLAMP && state != S_DOWN )
    {
        Clamp(bSthowMsg);
    }

	if(state != S_CLAMP && state != S_DOWN )
	{
		//::AfxMessageBox(GetStateDesc());
		return false;
	}

	GT_ExOpt(OUT_EVT_STICK_BOARD_UP); //顶板气缸-升高
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

    state = S_UP;
#endif
    return true;
}

bool CPCBState::Down(bool bSthowMsg)
{
#ifndef _MYUTEST
	unsigned short output=0;
	if(state == S_DOWN)
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

	if(state != S_UP )
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return false;
	}

	GT_ExOpt(OUT_EVT_STICK_BOARD_DOWN);   //顶板气缸-落下
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

    state = S_DOWN;
#endif
    return true;
}

bool CPCBState::Loose(bool bSthowMsg)
{
#ifndef _MYUTEST
	unsigned short output=0;
	if(state == S_LOOSE)
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

	if(state != S_DOWN)
	{
		Down(bSthowMsg);
	}

	if(state != S_DOWN && state != S_CLAMP )
	{
		//::AfxMessageBox(GetStateDesc());
		return false;
	}

	GT_ExOpt(OUT_EVT_PRESS_BOARD_LOOSE); //板压紧气缸-松
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

    state = S_LOOSE;
#endif
    return true;
}

bool CPCBState::SendOut(bool bSthowMsg)
{
#ifndef _MYUTEST
	unsigned short output=0;
	if(state == S_OUT)
	{
		if(bSthowMsg)
			::AfxMessageBox(GetStateDesc());
		return true;
	}

	if(state != S_LOOSE)
	{
		Loose(bSthowMsg);
	}

	if(state != S_LOOSE && state != S_ACCEPT )
	{
		//::AfxMessageBox(GetStateDesc());
		return false;
	}

	GT_ExOpt(OUT_EVT_STOP_BOARD_DOWN);   //板停止气缸-落下
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//停止

    GT_ExOpt(OUT_EVT_CONVEY_MOTOR);   //输送带电机动
    ::Sleep(g_iPCBOutTime);
    GT_ExOpt(OUT_ALL_STOP);	//停止
    
    state = S_OUT;
#endif
    return true;
}
