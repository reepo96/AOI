// OrganEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganEdit.h"


// COrganEdit 对话框

IMPLEMENT_DYNAMIC(COrganEdit, CDialog)

COrganEdit::COrganEdit(COrganData **pOrganData,CWnd* pParent /*=NULL*/)
	: CDialog(COrganEdit::IDD, pParent)
	, m_sName(_T(""))
	, m_sFootPrint(_T(""))
	, m_dMidX(0)
	, m_dMidY(0)
	, m_iLimit(0)
	, m_dPadX(0)
	, m_dPadY(0)
{
	m_pOrganData = new COrganData();
	*pOrganData = m_pOrganData;
}

COrganEdit::~COrganEdit()
{
}

void COrganEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, m_sName);
	DDX_Text(pDX, IDC_FOOTPRINT, m_sFootPrint);
	DDX_Text(pDX, IDC_MIDX, m_dMidX);
	DDX_Text(pDX, IDC_MIDY, m_dMidY);
	DDX_Text(pDX, IDC_LIMIT, m_iLimit);
	DDX_Text(pDX, IDC_PADX, m_dPadX);
	DDX_Text(pDX, IDC_PADY, m_dPadY);
}


BEGIN_MESSAGE_MAP(COrganEdit, CDialog)
	ON_BN_CLICKED(IDC_OK, &COrganEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &COrganEdit::OnBnClickedCancel)
END_MESSAGE_MAP()


// COrganEdit 消息处理程序

void COrganEdit::OnBnClickedOk()
{
	this->UpdateData(TRUE);
	if(m_sName == "" || m_sFootPrint == "")
	{
		::AfxMessageBox("Must input name and footprint");
		return;
	}

	m_pOrganData->dMidX = m_dMidX;
	m_pOrganData->dMidY = m_dMidY;
	m_pOrganData->dPadX = m_dPadX;
	m_pOrganData->dPadY = m_dPadY;
	m_pOrganData->iLimit = m_iLimit;
	m_pOrganData->sFootPrint = m_sFootPrint;
	m_pOrganData->sName = m_sName;

	CDialog::OnOK();
}

void COrganEdit::OnBnClickedCancel()
{
	delete m_pOrganData;
	m_pOrganData = NULL;
	CDialog::OnCancel();
}

BOOL COrganEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
