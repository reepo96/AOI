// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "LoginDlg.h"
#include "UserMgr.h"

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
	, m_sUserName(_T(""))
	, m_sPassword(_T(""))
{
	m_iCount = 3;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	DDX_Text(pDX, IDC_PASSWARD, m_sPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

void CLoginDlg::OnBnClickedOk()
{
	this->UpdateData(TRUE);

	CUserMgr userMgr;
	if( true==userMgr.Login(m_sUserName,m_sPassword))
	{
		CDialog::OnOK();
		g_sUserName = m_sUserName;
		return;
	}
	else
	{
		::AfxMessageBox("error username or password");
		if( m_iCount <=0)
		{
			CDialog::OnCancel();
			return;
		}
		m_iCount--;
	}
}

void CLoginDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
