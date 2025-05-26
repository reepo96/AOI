// WaitLoadPCB.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "WaitLoadPCB.h"


// CWaitLoadPCB 对话框

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


// CWaitLoadPCB 消息处理程序

void CWaitLoadPCB::OnBnClickedStop()
{
	g_bStopWork = true;
}

BOOL CWaitLoadPCB::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_bIsWait = true;
	::g_SetDialogStrings(this);
	this->SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
