// NextPadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "NextPadDlg.h"


// CNextPadDlg �Ի���

IMPLEMENT_DYNAMIC(CNextPadDlg, CDialog)

CNextPadDlg::CNextPadDlg(int *piSelect,CWnd* pParent /*=NULL*/)
	: CDialog(CNextPadDlg::IDD, pParent)
{
	m_pOrganPrgDlg = (COrganProgramDlg*)pParent;
	m_piSelect = piSelect;

}

CNextPadDlg::~CNextPadDlg()
{
}

void CNextPadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNextPadDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &CNextPadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CNextPadDlg::OnBnClickedCancle)
END_MESSAGE_MAP()


// CNextPadDlg ��Ϣ�������

void CNextPadDlg::OnBnClickedOk()
{
	CButton* pFrontBtn = (CButton*)GetDlgItem(IDC_FRONT);
	CButton* pLeftBtn = (CButton*)GetDlgItem(IDC_LEFT);
	CButton* pRightBtn = (CButton*)GetDlgItem(IDC_RIGHT);
	if( pFrontBtn->GetCheck() == true)
	{
		*m_piSelect = 1;
	}
	else if( pLeftBtn->GetCheck() == true)
	{
		*m_piSelect = 2;
	}
	else if( pRightBtn->GetCheck() == true)
	{
		*m_piSelect = 3;
	}

	CDialog::OnOK();
}

void CNextPadDlg::OnBnClickedCancle()
{
	CDialog::OnCancel();
}

BOOL CNextPadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CButton* pFrontBtn = (CButton*)GetDlgItem(IDC_FRONT);
	if( pFrontBtn !=NULL)
	{
		pFrontBtn->SetCheck(true);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
