// FootProgram.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "FootProgram.h"


// CFootProgram 对话框

IMPLEMENT_DYNAMIC(CFootProgram, CDialog)

CFootProgram::CFootProgram(CWnd* pParent /*=NULL*/)
	: CDialog(CFootProgram::IDD, pParent)
	, m_iFootNum(0)
{
#ifdef _DEBUG
	CM1.Checkpoint();
#endif

	m_pOrganPrgDlg = (COrganProgramDlg*)pParent;

	CPadData *pCurrentPad = m_pOrganPrgDlg->GetCurrentPad();
	if(pCurrentPad != NULL)
	{
		m_iFootNum = pCurrentPad->m_footList.size();
	}
	else
	{
		m_iFootNum = 0;
	}
}

CFootProgram::~CFootProgram()
{
#ifdef _DEBUG
	CM2.Checkpoint();
	CM3.Difference(CM1,CM2);
#endif
}

void CFootProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOOTNUM, m_iFootNum);
}


BEGIN_MESSAGE_MAP(CFootProgram, CDialog)
	ON_BN_CLICKED(IDC_FOOTNUM_ADD, &CFootProgram::OnBnClickedFootnumAdd)
	ON_BN_CLICKED(IDC_FOOTNUM_DOWN, &CFootProgram::OnBnClickedFootnumDown)
	ON_BN_CLICKED(IDC_SPACE_ADD_M, &CFootProgram::OnBnClickedSpaceAddM)
	ON_BN_CLICKED(IDC_SPACE_ADD, &CFootProgram::OnBnClickedSpaceAdd)
	ON_BN_CLICKED(IDC_SPACE_DOWN, &CFootProgram::OnBnClickedSpaceDown)
	ON_BN_CLICKED(IDC_SPACE_DOWN_M, &CFootProgram::OnBnClickedSpaceDownM)
	ON_BN_CLICKED(IDC_OK, &CFootProgram::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CFootProgram::OnBnClickedCancle)
END_MESSAGE_MAP()


// CFootProgram 消息处理程序

void CFootProgram::OnBnClickedFootnumAdd()
{
	m_iFootNum++;
	m_pOrganPrgDlg->AddFootNum(1);
	this->UpdateData(FALSE);
}

void CFootProgram::OnBnClickedFootnumDown()
{
	m_iFootNum--;
	m_iFootNum = (m_iFootNum<0)?0:m_iFootNum;
	m_pOrganPrgDlg->AddFootNum(-1);
	UpdateData(FALSE);
}

void CFootProgram::OnBnClickedSpaceAddM()
{
	m_pOrganPrgDlg->AddFootSpaceBetween(20);
}

void CFootProgram::OnBnClickedSpaceAdd()
{
	m_pOrganPrgDlg->AddFootSpaceBetween(1);
}

void CFootProgram::OnBnClickedSpaceDown()
{
	m_pOrganPrgDlg->AddFootSpaceBetween(-1);
}

void CFootProgram::OnBnClickedSpaceDownM()
{
	m_pOrganPrgDlg->AddFootSpaceBetween(-20);
}

void CFootProgram::OnBnClickedOk()
{
	CButton *pCheck = (CButton*)GetDlgItem(IDC_OKANDNEW);

	if(pCheck->GetCheck())
	{
		m_pOrganPrgDlg->SetWorkStepAfterFoot(OWS_PAD);
	}
	else
	{
		m_pOrganPrgDlg->SetWorkStepAfterFoot(OWS_SERCHAREA);
	}

	m_pOrganPrgDlg->EndCurrentWork();

	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

void CFootProgram::OnBnClickedCancle()
{
	m_pOrganPrgDlg->NextOrganProgram(PS_COMPLETE);

	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

BOOL CFootProgram::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CButton *pCheck = (CButton*)GetDlgItem(IDC_OKANDNEW);
	if(pCheck)
		pCheck->SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
