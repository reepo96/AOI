// WaitLoadPCB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "WaitLoadPCB.h"


// CWaitLoadPCB �Ի���

IMPLEMENT_DYNAMIC(CWaitLoadPCB, CDialog)

CWaitLoadPCB::CWaitLoadPCB(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitLoadPCB::IDD, pParent)
{
	
}

CWaitLoadPCB::~CWaitLoadPCB()
{
}

void CWaitLoadPCB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitLoadPCB, CDialog)
	ON_BN_CLICKED(IDC_STOP, &CWaitLoadPCB::OnBnClickedStop)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWaitLoadPCB ��Ϣ�������

void CWaitLoadPCB::OnBnClickedStop()
{
	g_bStopWork = true;
}

BOOL CWaitLoadPCB::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_bIsWait = true;
	::g_SetDialogStrings(this);
	this->SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CWaitLoadPCB::OnTimer(UINT_PTR nIDEvent)
{
	if(!g_bIsWait)
	{
		this->KillTimer(1);
		this->DestroyWindow();
	}

	CDialog::OnTimer(nIDEvent);
}
