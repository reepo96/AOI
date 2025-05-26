// ProgramInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "ProgramInfoDlg.h"


// CProgramInfoDlg 对话框

IMPLEMENT_DYNAMIC(CProgramInfoDlg, CDialog)

CProgramInfoDlg::CProgramInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgramInfoDlg::IDD, pParent)
{
	m_pTemplateData = NULL;
	m_pBigGraphView = NULL;
}

CProgramInfoDlg::~CProgramInfoDlg()
{
}

void CProgramInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CProgramInfoDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CProgramInfoDlg::OnTcnSelchangeTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CProgramInfoDlg 消息处理程序

void CProgramInfoDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel; 
	CurSel=m_TabCtrl.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0: 
		m_PcbInfoDlg.ShowWindow(TRUE); 
		m_FOVInfoDlg.ShowWindow(FALSE); 
		m_DatumMarkInfoDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.ShowWindow(FALSE);
		m_BarInfDlg.ShowWindow(FALSE);
		m_SpellInfDlg.ShowWindow(FALSE);
		m_PcbInfoDlg.SetFocus();
		break; 
	case 1: 
		m_PcbInfoDlg.ShowWindow(FALSE); 
		m_FOVInfoDlg.ShowWindow(TRUE); 
		m_DatumMarkInfoDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.ShowWindow(FALSE);
		m_BarInfDlg.ShowWindow(FALSE);
		m_SpellInfDlg.ShowWindow(FALSE);
		m_FOVInfoDlg.SetFocus();
		break; 
	case 2: 
		m_PcbInfoDlg.ShowWindow(FALSE); 
		m_FOVInfoDlg.ShowWindow(FALSE); 
		m_DatumMarkInfoDlg.ShowWindow(TRUE);
		m_OrganInfoDlg.ShowWindow(FALSE);
		m_BarInfDlg.ShowWindow(FALSE);
		m_SpellInfDlg.ShowWindow(FALSE);
		m_DatumMarkInfoDlg.SetFocus();
		break;
	case 3: 
		m_PcbInfoDlg.ShowWindow(FALSE); 
		m_FOVInfoDlg.ShowWindow(FALSE); 
		m_DatumMarkInfoDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.ShowWindow(TRUE);
		m_BarInfDlg.ShowWindow(FALSE);
		m_SpellInfDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.SetFocus();
		break;
	case 4: 
		m_PcbInfoDlg.ShowWindow(FALSE); 
		m_FOVInfoDlg.ShowWindow(FALSE); 
		m_DatumMarkInfoDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.ShowWindow(FALSE);
		m_BarInfDlg.ShowWindow(TRUE);
		m_SpellInfDlg.ShowWindow(FALSE);
		m_BarInfDlg.SetFocus();
		break;
	case 5: 
		m_PcbInfoDlg.ShowWindow(FALSE); 
		m_FOVInfoDlg.ShowWindow(FALSE); 
		m_DatumMarkInfoDlg.ShowWindow(FALSE);
		m_OrganInfoDlg.ShowWindow(FALSE);
		m_BarInfDlg.ShowWindow(FALSE);
		m_SpellInfDlg.ShowWindow(TRUE);
		m_SpellInfDlg.SetFocus();
		break;
	default: ; 
	}

	*pResult = 0;
}

BOOL CProgramInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(!m_PcbInfoDlg.m_hWnd) ////第一次初始化m_tab 控件和page页的建立
	{
		CString sTabInfo = g_LoadString("IDS_PCBINFO","PCB板信息");
		m_TabCtrl.InsertItem(0,sTabInfo); 

		sTabInfo = g_LoadString("IDS_FOVAREA","FOV区域");
		m_TabCtrl.InsertItem(1,sTabInfo);

		sTabInfo = g_LoadString("IDS_DATUMMARKINFO","基准点信息");
		m_TabCtrl.InsertItem(2,sTabInfo);

		sTabInfo = g_LoadString("IDS_ORGANINFO","元件信息");
		m_TabCtrl.InsertItem(3,sTabInfo);

		sTabInfo = g_LoadString("IDS_BARINFO","条形码信息");
		m_TabCtrl.InsertItem(4,sTabInfo);

		sTabInfo = g_LoadString("IDS_SPELLINFO","拼板信息");
		m_TabCtrl.InsertItem(5,sTabInfo);

		//建立属性页各页 
		m_PcbInfoDlg.Create(IDD_PCBINFODLG,GetDlgItem(IDC_TAB1)); 
		m_FOVInfoDlg.Create(IDD_FOVINFODLG,GetDlgItem(IDC_TAB1));
		m_DatumMarkInfoDlg.Create(IDD_DATUMMARKINFODLG,GetDlgItem(IDC_TAB1));
		m_OrganInfoDlg.Create(IDD_ORGANINFODLG,GetDlgItem(IDC_TAB1));
		m_BarInfDlg.Create(IDD_BARINFODLG,GetDlgItem(IDC_TAB1));
		m_SpellInfDlg.Create(IDD_SPELLINFODLG,GetDlgItem(IDC_TAB1));

		//设置页面的位置在m_tab控件范围内 
		CRect rs; 
		this->GetClientRect(rs);
		m_TabCtrl.MoveWindow(rs); 
	
		rs.top+=24; 
		rs.bottom-=4; 
		rs.left+=4; 
		rs.right-=4; 

		m_PcbInfoDlg.MoveWindow(rs); 
		m_FOVInfoDlg.MoveWindow(rs); 
		m_DatumMarkInfoDlg.MoveWindow(rs);
		m_OrganInfoDlg.MoveWindow(rs);
		m_BarInfDlg.MoveWindow(rs);
		m_SpellInfDlg.MoveWindow(rs);

		m_PcbInfoDlg.ShowWindow(TRUE); 
		m_TabCtrl.SetCurSel(0); 
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProgramInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if(m_PcbInfoDlg.m_hWnd) 
	{
		//设置页面的位置在m_tab控件范围内 
		CRect rs; 
		this->GetClientRect(rs);
		m_TabCtrl.MoveWindow(rs); 
	
		rs.top+=24; 
		rs.bottom-=4; 
		rs.left+=4; 
		rs.right-=4; 

		m_PcbInfoDlg.MoveWindow(rs); 
		m_FOVInfoDlg.MoveWindow(rs); 
		m_DatumMarkInfoDlg.MoveWindow(rs);
		m_OrganInfoDlg.MoveWindow(rs);
		m_BarInfDlg.MoveWindow(rs);
		m_SpellInfDlg.MoveWindow(rs);
	}	
}

void CProgramInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	m_PcbInfoDlg.SetTemplateData(pTemplateData);
	m_OrganInfoDlg.SetTemplateData(pTemplateData);
	m_FOVInfoDlg.SetTemplateData(pTemplateData);
	m_DatumMarkInfoDlg.SetTemplateData(pTemplateData);
	m_BarInfDlg.SetTemplateData(pTemplateData);
	m_SpellInfDlg.SetTemplateData(pTemplateData);
}



void CProgramInfoDlg::SetBigGraphView(CBigGraphView * pBigGraphView)
{
	m_pBigGraphView = pBigGraphView;
	m_OrganInfoDlg.SetBigGraphView(pBigGraphView);
}

void CProgramInfoDlg::PrgImportcad(void)
{
	m_OrganInfoDlg.OnBnClickedImport();
}

void CProgramInfoDlg::NotifyPrgDataChanged(void)
{
	m_FOVInfoDlg.NotifyPrgDataChanged();
	m_OrganInfoDlg.NotifyPrgDataChanged();
	m_DatumMarkInfoDlg.NotifyPrgDataChanged();
	m_BarInfDlg.NotifyPrgDataChanged();
	m_SpellInfDlg.NotifyPrgDataChanged();
}
