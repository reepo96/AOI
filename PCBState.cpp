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

	GT_ExOpt(OUT_EVT_STICK_BOARD_DOWN);   //��������-����
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

	GT_ExOpt(OUT_EVT_PRESS_BOARD_LOOSE); //��ѹ������-��
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

	GT_ExOpt(OUT_EVT_STOP_BOARD_DOWN);   //��ֹͣ����-����
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ
#endif

}

CString CPCBState::GetStateDesc()
{
	switch(state)
	{
	case S_NONE:
		return g_LoadString("IDS_PCB_NONE","û��PCB��");
	case S_ACCEPT:
		return g_LoadString("IDS_PCB_ACCEPT","PCB���ѱ�����");
	case S_CLAMP:
		return g_LoadString("IDS_PCB_CLAM","PCB���ѱ��н�");
	case S_UP:
		return g_LoadString("IDS_PCB_UP","PCB��������");
	case S_DOWN:
		return g_LoadString("IDS_PCB_DOWN","PCB��������");
	case S_LOOSE:
		return g_LoadString("IDS_PCB_LOOSE","PCB���ѱ��ɿ�");
	case S_OUT:
		return g_LoadString("IDS_PCB_OUT","PCB���ѱ��ͳ�");
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
		sMsgInfo = g_LoadString("IDS_PCB_EXIST","����PCB�壬�����ٽ�����PCB��");
		if(bSthowMsg)
			::AfxMessageBox(sMsgInfo);
        return false;
    }

	if(g_iPCBEntryDir == 1)
	{
		if( !((~g_uDevInEvent)&IN_EVT_LEFT_ENTRY) )
		{
			sMsgInfo = g_LoadString("IDS_NOPCB_IN","û��PCB�屻����");
			if(bSthowMsg)
				::AfxMessageBox(sMsgInfo);
			return false;   //������ź�û�д���
		}
	}
	else
	{
		if( !((~g_uDevInEvent)&IN_EVT_RIGHT_ENTRY) )
		{
			sMsgInfo = g_LoadString("IDS_NOPCB_IN","û��PCB�屻����");
			if(bSthowMsg)
				::AfxMessageBox(sMsgInfo);
			return false;   //������ź�û�д���
		}
	}

    unsigned short output=0;

	GT_ExOpt(OUT_ALL_STOP);	//ֹͣ
    
    GT_ExOpt(OUT_EVT_STOP_BOARD_UP);//��ֹͣ����-����
    ::Sleep(100);
	GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

    GT_ExOpt(OUT_EVT_CONVEY_MOTOR); //���ʹ������
    ::Sleep(g_iPCBInTime);
	GT_ExOpt(OUT_ALL_STOP);	//ֹͣ
    
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

    GT_ExOpt(OUT_EVT_PRESS_BOARD_CLAMP); //��ѹ������-ѹ��
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

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

	GT_ExOpt(OUT_EVT_STICK_BOARD_UP); //��������-����
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

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

	GT_ExOpt(OUT_EVT_STICK_BOARD_DOWN);   //��������-����
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

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

	GT_ExOpt(OUT_EVT_PRESS_BOARD_LOOSE); //��ѹ������-��
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

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

	GT_ExOpt(OUT_EVT_STOP_BOARD_DOWN);   //��ֹͣ����-����
    ::Sleep(100);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ

    GT_ExOpt(OUT_EVT_CONVEY_MOTOR);   //���ʹ������
    ::Sleep(g_iPCBOutTime);
    GT_ExOpt(OUT_ALL_STOP);	//ֹͣ
    
    state = S_OUT;
#endif
    return true;
}
