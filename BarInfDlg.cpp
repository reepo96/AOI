// BarInfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "BarInfDlg.h"
#include "MainView.h"


// CBarInfDlg 对话框

IMPLEMENT_DYNAMIC(CBarInfDlg, CDialog)

CBarInfDlg::CBarInfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarInfDlg::IDD, pParent)
	, m_lSCenterX(0)
	, m_lSCenterY(0)
	, m_lBCenterY(0)
	, m_lBCenterX(0)
	, m_lSWidth(0)
	, m_lBWidth(0)
	, m_lSHeight(0)
	, m_lBHeight(0)
{

}

CBarInfDlg::~CBarInfDlg()
{
}

void CBarInfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_S_CENTERX, m_lSCenterX);
	DDX_Text(pDX, IDC_S_CENTERY, m_lSCenterY);
	DDX_Text(pDX, IDC_B_CENTERY, m_lBCenterY);
	DDX_Text(pDX, IDC_B_CENTERX, m_lBCenterX);
	DDX_Text(pDX, IDC_S_WIDTH, m_lSWidth);
	DDX_Text(pDX, IDC_B_WIDTH, m_lBWidth);
	DDX_Text(pDX, IDC_S_HEIGHT, m_lSHeight);
	DDX_Text(pDX, IDC_B_HEIGHT, m_lBHeight);
}


BEGIN_MESSAGE_MAP(CBarInfDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW, &CBarInfDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_MODIFY, &CBarInfDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CBarInfDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CBarInfDlg 消息处理程序

void CBarInfDlg::SetTemplateData(CTemplateData* pTemplateData)
{
	m_pTemplateData = pTemplateData;

	if(pTemplateData == NULL)
		return;

	if(pTemplateData->m_pBar != NULL)
	{
		CRect searchRt(pTemplateData->m_pBar->barSerchLeft,pTemplateData->m_pBar->barSerchTop,
			pTemplateData->m_pBar->barSerchRight,pTemplateData->m_pBar->barSerchBottom);
		m_lSCenterX = searchRt.CenterPoint().x;
		m_lSCenterY = searchRt.CenterPoint().y;
		m_lSWidth = searchRt.Width();
		m_lSHeight = searchRt.Height();

		m_lBCenterX = pTemplateData->m_pBar->GetMidP().x;
		m_lBCenterY = pTemplateData->m_pBar->GetMidP().y;
		m_lBWidth = pTemplateData->m_pBar->GetWidth();
		m_lBHeight = pTemplateData->m_pBar->GetHeight();		
	}
	else
	{
		m_lSCenterX = 0;
		m_lSCenterY = 0;
		m_lSWidth = 0;
		m_lSHeight = 0;

		m_lBCenterX = 0;
		m_lBCenterY = 0;
		m_lBWidth = 0;
		m_lBHeight = 0;
	}

	this->UpdateData(FALSE);
}

void CBarInfDlg::NotifyPrgDataChanged(void)
{
	if(m_pTemplateData->m_pBar != NULL)
	{
		CRect searchRt(m_pTemplateData->m_pBar->barSerchLeft,m_pTemplateData->m_pBar->barSerchTop,
			m_pTemplateData->m_pBar->barSerchRight,m_pTemplateData->m_pBar->barSerchBottom);
		m_lSCenterX = searchRt.CenterPoint().x;
		m_lSCenterY = searchRt.CenterPoint().y;
		m_lSWidth = searchRt.Width();
		m_lSHeight = searchRt.Height();

		m_lBCenterX = m_pTemplateData->m_pBar->GetMidP().x;
		m_lBCenterY = m_pTemplateData->m_pBar->GetMidP().y;
		m_lBWidth = m_pTemplateData->m_pBar->GetWidth();
		m_lBHeight = m_pTemplateData->m_pBar->GetHeight();		
	}
	else
	{
		m_lSCenterX = 0;
		m_lSCenterY = 0;
		m_lSWidth = 0;
		m_lSHeight = 0;

		m_lBCenterX = 0;
		m_lBCenterY = 0;
		m_lBWidth = 0;
		m_lBHeight = 0;
	}

	this->UpdateData(FALSE);
}

BOOL CBarInfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBarInfDlg::OnBnClickedNew()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->PrgBarcode();
}

void CBarInfDlg::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBarInfDlg::OnBnClickedDelete()
{
	if(m_pTemplateData->m_pBar != NULL)
	{
		delete m_pTemplateData->m_pBar;
		m_pTemplateData->m_pBar = NULL;
		m_pTemplateData->m_bDataIsChanged = true;

		m_lSCenterX = 0;
		m_lSCenterY = 0;
		m_lSWidth = 0;
		m_lSHeight = 0;

		m_lBCenterX = 0;
		m_lBCenterY = 0;
		m_lBWidth = 0;
		m_lBHeight = 0;

		this->UpdateData(FALSE);
	}
}
