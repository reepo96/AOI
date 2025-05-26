// OrganPrgGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganPrgGuid.h"
#include "OrganProgramDlg.h"


// COrganPrgGuid 对话框

IMPLEMENT_DYNAMIC(COrganPrgGuid, CDialog)

COrganPrgGuid::COrganPrgGuid(CWnd* pParent /*=NULL*/)
	: CDialog(COrganPrgGuid::IDD, pParent)
{
	m_pWnd = pParent;
}

COrganPrgGuid::~COrganPrgGuid()
{
}

void COrganPrgGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrganPrgGuid, CDialog)
	ON_BN_CLICKED(IDC_OK, &COrganPrgGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &COrganPrgGuid::OnBnClickedCancle)
END_MESSAGE_MAP()


// COrganPrgGuid 消息处理程序

void COrganPrgGuid::OnBnClickedOk()
{
	((COrganProgramDlg*)m_pWnd)->EndCurrentWork();
}

void COrganPrgGuid::OnBnClickedCancle()
{
	((COrganProgramDlg*)m_pWnd)->CancelCurrentWork();
	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

BOOL COrganPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
