// SetColorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SetColorDlg.h"
#include "SettingDataMgr.h"


// CSetColorDlg 对话框

IMPLEMENT_DYNAMIC(CSetColorDlg, CDialog)

CSetColorDlg::CSetColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColorDlg::IDD, pParent)
{
	m_EditAreaColor = g_EditAreaColor;
	m_FovColor = g_FovColor;
	m_MarkColor = g_MarkColor;
	m_OrganColor = g_OrganColor;
	m_BarCodeColor = g_BarCodeColor;
}

CSetColorDlg::~CSetColorDlg()
{
}

void CSetColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR_EDITA, m_btnEditAreaColor);
}


BEGIN_MESSAGE_MAP(CSetColorDlg, CDialog)	
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_COLOR_EDITA, &CSetColorDlg::OnBnClickedColorEdita)
	ON_BN_CLICKED(IDC_COLOR_FOV, &CSetColorDlg::OnBnClickedColorFov)
	ON_BN_CLICKED(IDC_COLOR_MARK, &CSetColorDlg::OnBnClickedColorMark)
	ON_BN_CLICKED(IDC_COLOR_ORGAN, &CSetColorDlg::OnBnClickedColorOrgan)
	ON_BN_CLICKED(IDC_COLOR_BARCODE, &CSetColorDlg::OnBnClickedColorBarcode)
	ON_BN_CLICKED(IDC_SAVE, &CSetColorDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CANCEL, &CSetColorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetColorDlg 消息处理程序

BOOL CSetColorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);
			
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetColorDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect   rect   =   lpDrawItemStruct->rcItem;   
	CDC   dc;   
	dc.Attach(lpDrawItemStruct->hDC);   

	dc.SetBkMode(TRANSPARENT);

	if(nIDCtl == IDC_COLOR_EDITA)
	{
		CBrush   brush(m_EditAreaColor);
		//dc.SetTextColor(RGB(255,0,0));   
		dc.FillRect(&rect,   &brush);
	}
	else if(nIDCtl == IDC_COLOR_FOV)
	{
		CBrush   brush(m_FovColor);
		dc.FillRect(&rect,   &brush);
	}
	else if(nIDCtl == IDC_COLOR_MARK)
	{
		CBrush   brush(m_MarkColor);
		dc.FillRect(&rect,   &brush);
	}
	else if(nIDCtl == IDC_COLOR_ORGAN)
	{
		CBrush   brush(m_OrganColor);
		dc.FillRect(&rect,   &brush);
	}
	else if(nIDCtl == IDC_COLOR_BARCODE)
	{
		CBrush   brush(m_BarCodeColor);
		dc.FillRect(&rect,   &brush);
	}

	if(lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.Draw3dRect(&rect,RGB(1,1,1),RGB(255,255,255));
	}
	else
	{
		dc.Draw3dRect(&rect,RGB(255,255,255),RGB(1,1,1));
	}	

	dc.Detach();

	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CSetColorDlg::OnBnClickedColorEdita()
{
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_EditAreaColor = colorDlg.GetColor();
		this->Invalidate();
	}
}

void CSetColorDlg::OnBnClickedColorFov()
{
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_FovColor = colorDlg.GetColor();
		this->Invalidate();
	}
}

void CSetColorDlg::OnBnClickedColorMark()
{
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_MarkColor = colorDlg.GetColor();
		this->Invalidate();
	}
}

void CSetColorDlg::OnBnClickedColorOrgan()
{
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_OrganColor = colorDlg.GetColor();
		this->Invalidate();
	}
}

void CSetColorDlg::OnBnClickedColorBarcode()
{
	CColorDialog colorDlg;
	if(IDOK == colorDlg.DoModal())
	{
		m_BarCodeColor = colorDlg.GetColor();
		this->Invalidate();
	}
}

void CSetColorDlg::OnBnClickedSave()
{
	CSettingDataMgr SettingMgr;

	g_EditAreaColor = m_EditAreaColor;
	g_FovColor = m_FovColor;
	g_MarkColor = m_MarkColor;
	g_OrganColor = m_OrganColor;
	g_BarCodeColor = m_BarCodeColor;

	CString sErrInfo;
	if(false==SettingMgr.UpdateColorSettingData(sErrInfo))
	{
		::AfxMessageBox(sErrInfo);
		return;
	}

	CDialog::OnOK();
}

void CSetColorDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
