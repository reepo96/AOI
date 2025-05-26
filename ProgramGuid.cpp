// ProgramGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "ProgramGuid.h"
#include "BigGraphView.h"


// CProgramGuid 对话框

IMPLEMENT_DYNAMIC(CProgramGuid, CDialog)

CProgramGuid::CProgramGuid(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramGuid::IDD, pParent)
{
	m_pBgiGraphView = pParent;
}

CProgramGuid::~CProgramGuid()
{
}

void CProgramGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgramGuid, CDialog)
	ON_BN_CLICKED(IDC_NEW, &CProgramGuid::OnBnClickedNew)
	ON_BN_CLICKED(IDC_OK, &CProgramGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEXT, &CProgramGuid::OnBnClickedNext)
	ON_BN_CLICKED(IDC_CANCLE, &CProgramGuid::OnBnClickedCancle)
END_MESSAGE_MAP()


// CProgramGuid 消息处理程序

void CProgramGuid::OnBnClickedNew()
{
	if(m_pBgiGraphView)
		((CBigGraphView*)m_pBgiGraphView)->NewCurrentWork();
}

void CProgramGuid::OnBnClickedOk()
{
	if(m_pBgiGraphView)
		((CBigGraphView*)m_pBgiGraphView)->EndCurrentWork();
}

void CProgramGuid::OnBnClickedNext()
{
	if(m_pBgiGraphView)
		((CBigGraphView*)m_pBgiGraphView)->NextStep();
}

void CProgramGuid::OnBnClickedCancle()
{
	if(m_pBgiGraphView)
	{
		((CBigGraphView*)m_pBgiGraphView)->CancelCurrentWork();
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
	}
}

BOOL CProgramGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
