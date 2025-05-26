// UserEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "UserEdit.h"
#include "UserInf.h"

// CUserEdit 对话框

IMPLEMENT_DYNAMIC(CUserEdit, CDialog)

CUserEdit::CUserEdit(CUserMgrDlg* pParent /*=NULL*/)
	: CDialog(CUserEdit::IDD, pParent)
	, m_sUserName(_T(""))
	, m_sPassword(_T(""))
	, m_sConfirm(_T(""))
{
	m_pUserMgrDlg = pParent;
}

CUserEdit::~CUserEdit()
{
}

void CUserEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	DDX_Text(pDX, IDC_PASSWARD, m_sPassword);
	DDX_Text(pDX, IDC_CONFIRM, m_sConfirm);
	DDX_Control(pDX, IDC_PROGRAM, m_ckPrg);
	DDX_Control(pDX, IDC_DETECT, m_ckDetect);
	DDX_Control(pDX, IDC_REPORT, m_ckReport);
	DDX_Control(pDX, IDC_SYSTEM, m_ckSystem);
}


BEGIN_MESSAGE_MAP(CUserEdit, CDialog)
	ON_BN_CLICKED(IDC_OK, &CUserEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CUserEdit::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserEdit 消息处理程序

void CUserEdit::OnBnClickedOk()
{
	this->UpdateData(TRUE);
	if(m_sPassword != m_sConfirm)
	{
		CString sInfo = ::g_LoadString("IDS_PSW_NO_CFM","密码和确认密码不一致");
		::AfxMessageBox(sInfo);
		return;
	}

	CUserInf *pUser=new CUserInf();
	pUser->userName = m_sUserName;
	pUser->password = m_sPassword;

	if(m_ckPrg.GetCheck()== BST_CHECKED)
	{
		pUser->enableProgram = 1;
	}
	else
	{
		pUser->enableProgram = 0;
	}

	if(m_ckDetect.GetCheck()== BST_CHECKED)
	{
		pUser->enableDetect = 1;
	}
	else
	{
		pUser->enableDetect = 0;
	}

	if(m_ckReport.GetCheck()== BST_CHECKED)
	{
		pUser->enableReport = 1;
	}
	else
	{
		pUser->enableReport = 0;
	}

	if(m_ckSystem.GetCheck()== BST_CHECKED)
	{
		pUser->enableSystem = 1;
	}
	else
	{
		pUser->enableSystem = 0;
	}

	m_pUserMgrDlg->NewUser(pUser);

	CDialog::OnOK();
}

void CUserEdit::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

BOOL CUserEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
