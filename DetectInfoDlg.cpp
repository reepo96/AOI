// DetectInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectInfoDlg.h"


// CDetectInfoDlg �Ի���

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


// CDetectInfoDlg ��Ϣ�������
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

	if(!m_DErrDataInfo.m_hWnd) ////��һ�γ�ʼ��m_tab �ؼ���pageҳ�Ľ���
	{
		CString sTabInfo = g_LoadString("IDS_DETECTINFO","�����Ϣ");
		m_TabCtrl.InsertItem(0,sTabInfo); 		

		sTabInfo = g_LoadString("IDS_DETECTERRINFO","������Ϣ");
		m_TabCtrl.InsertItem(1,sTabInfo);

		sTabInfo = g_LoadString("IDS_ORGANINFO","Ԫ����Ϣ");
		m_TabCtrl.InsertItem(2,sTabInfo);

		//��������ҳ��ҳ 
		m_DBassInfo.Create(IDD_DETECTBASEINFO,GetDlgItem(IDC_TAB1)); 
		m_DErrDataInfo.Create(IDD_DETECTERRDATAINF,GetDlgItem(IDC_TAB1)); 
		m_DOrganInfo.Create(IDD_DETECTORGANINFO,GetDlgItem(IDC_TAB1));
		
		//����ҳ���λ����m_tab�ؼ���Χ�� 
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDetectInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_DErrDataInfo.m_hWnd) 
	{
		//����ҳ���λ����m_tab�ؼ���Χ�� 
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
