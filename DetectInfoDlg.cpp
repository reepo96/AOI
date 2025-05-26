// DetectInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectInfoDlg.h"


// CDetectInfoDlg 对话框

IMPLEMENT_DYNAMIC(CDetectInfoDlg, CDialog)

CDetectInfoDlg::CDetectInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectInfoDlg::IDD, pParent)
{

}

CDetectInfoDlg::~CDetectInfoDlg()
{
	
}

void CDetectInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CDetectInfoDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDetectInfoDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDetectInfoDlg 消息处理程序
BOOL CDetectInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDetectInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	m_DOrganInfo.SetTemplateData(pTemplateData);
}

void CDetectInfoDlg::SetDBigGraphView(CDetectBigGraphView * pBigGraphView)
{
	m_pDBigView = pBigGraphView;

	m_DErrDataInfo.SetDBigView(pBigGraphView);
	m_DOrganInfo.SetDBigView(pBigGraphView);
	m_DBassInfo.SetDBigView(pBigGraphView);
}

void CDetectInfoDlg::NotifyDetectDataChanged(void)
{
	m_DErrDataInfo.NotifyDetectDataChanged();
	m_DOrganInfo.NotifyDetectDataChanged();
	m_DBassInfo.NotifyDetectDataChanged();	
}

BOOL CDetectInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_DErrDataInfo.m_hWnd) ////第一次初始化m_tab 控件和page页的建立
	{
		CString sTabInfo = g_LoadString("IDS_DETECTINFO","检测信息");
		m_TabCtrl.InsertItem(0,sTabInfo); 		

		sTabInfo = g_LoadString("IDS_DETECTERRINFO","错误信息");
		m_TabCtrl.InsertItem(1,sTabInfo);

		sTabInfo = g_LoadString("IDS_ORGANINFO","元件信息");
		m_TabCtrl.InsertItem(2,sTabInfo);

		//建立属性页各页 
		m_DBassInfo.Create(IDD_DETECTBASEINFO,GetDlgItem(IDC_TAB1)); 
		m_DErrDataInfo.Create(IDD_DETECTERRDATAINF,GetDlgItem(IDC_TAB1)); 
		m_DOrganInfo.Create(IDD_DETECTORGANINFO,GetDlgItem(IDC_TAB1));
		
		//设置页面的位置在m_tab控件范围内 
		CRect rs; 
		this->GetClientRect(rs);
		m_TabCtrl.MoveWindow(rs); 
	
		rs.top+=24; 
		rs.bottom-=4; 
		rs.left+=4; 
		rs.right-=4; 

		m_DBassInfo.MoveWindow(rs); 
		m_DErrDataInfo.MoveWindow(rs); 
		m_DOrganInfo.MoveWindow(rs); 
		
		m_DBassInfo.ShowWindow(TRUE); 		
		m_TabCtrl.SetCurSel(0); 
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDetectInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_DErrDataInfo.m_hWnd) 
	{
		//设置页面的位置在m_tab控件范围内 
		CRect rs; 
		this->GetClientRect(rs);
		m_TabCtrl.MoveWindow(rs); 
	
		rs.top+=24; 
		rs.bottom-=4; 
		rs.left+=4; 
		rs.right-=4; 

		m_DBassInfo.MoveWindow(rs); 
		m_DErrDataInfo.MoveWindow(rs); 
		m_DOrganInfo.MoveWindow(rs);		
	}
}

void CDetectInfoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel; 
	CurSel=m_TabCtrl.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0: 
		m_DBassInfo.ShowWindow(TRUE);
		m_DErrDataInfo.ShowWindow(FALSE); 
		m_DOrganInfo.ShowWindow(FALSE); 		
		m_DBassInfo.SetFocus();
		break; 
	case 1: 
		m_DBassInfo.ShowWindow(FALSE);
		m_DErrDataInfo.ShowWindow(TRUE); 
		m_DOrganInfo.ShowWindow(FALSE); 		
		m_DErrDataInfo.SetFocus();
		break; 	
	case 2: 
		m_DBassInfo.ShowWindow(FALSE);
		m_DErrDataInfo.ShowWindow(FALSE); 
		m_DOrganInfo.ShowWindow(TRUE); 		
		m_DOrganInfo.SetFocus();
		break;
	default: ; 
	}

	*pResult = 0;
}
