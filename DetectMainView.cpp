// DetectMainView.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectMainView.h"
#include "PCBState.h"

extern CPCBState   g_PCBState;

const double LEFTDLGWIDTH	= 0.7;	//左边对话框占的宽度
const double LUDLGHEIGHT	= 0.8;	//左上对话框占的高度
const double RUDLGHEIGHT	= 0.6;	//右上对话框占的高度

// CDetectMainView

IMPLEMENT_DYNCREATE(CDetectMainView, CFormView)

CDetectMainView::CDetectMainView()
	: CFormView(CDetectMainView::IDD)
{
	m_pDBigGraphView = NULL;
	m_pDSmallgView = NULL;
}

CDetectMainView::~CDetectMainView()
{
}

void CDetectMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectMainView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDetectMainView 诊断

#ifdef _DEBUG
void CDetectMainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetectMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetectMainView 消息处理程序

void CDetectMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();	
}

void CDetectMainView::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	m_pDBigGraphView->SetTemplateData(pTemplateData);
	m_pDBigGraphView->Invalidate();

	m_pDSmallgView->SetTemplateData(pTemplateData);
	m_pDSmallgView->Invalidate();

	m_DetectInfoDlg.SetTemplateData(pTemplateData);
	
}
void CDetectMainView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rs; 
	GetClientRect(rs);

	CRect rsLeftUp;		//左上对话框的rect
	CRect rsLeftDown;	//左下对话框的rect
	CRect rsRightUp;	//右上对话框的rect
	CRect rsRightDown;	//左下对话框的rect

	rsLeftUp.left = rs.left;
	rsLeftUp.top = rs.top;
	rsLeftUp.right = (LONG)(rs.right * LEFTDLGWIDTH);
	rsLeftUp.bottom = (LONG)(rs.bottom * LUDLGHEIGHT);

	m_DBigGraphRs = rsLeftUp;

	CCreateContext cc;
	cc.m_pNewViewClass = RUNTIME_CLASS(CDetectBigGraphView);
	cc.m_pCurrentDoc = NULL;
	cc.m_pNewDocTemplate = NULL;
	cc.m_pLastView = NULL;
	cc.m_pCurrentFrame = NULL;

	if(m_pDBigGraphView == NULL || m_pDBigGraphView->m_hWnd == NULL)
	{
		//创建视图
		m_pDBigGraphView = (CDetectBigGraphView*)CreateNewView(&cc, this, CRect(0, 0, 0, 0), 0);
		if (m_pDBigGraphView == NULL)
			return;		

		m_pDBigGraphView->MoveWindow(&rsLeftUp);
		m_pDBigGraphView->ShowWindow(TRUE); 
	}
	
	rsLeftDown.left =  rs.left;
	rsLeftDown.top = rsLeftUp.bottom;
	rsLeftDown.right = rsLeftUp.right;
	rsLeftDown.bottom = rs.bottom;
	if(!m_DCommandDlg.m_hWnd)
	{
		m_DCommandDlg.Create(IDD_DETECTCMDDLG,this);
		m_DCommandDlg.SetDetectBigView(m_pDBigGraphView);
		m_DCommandDlg.MoveWindow(rsLeftDown); 
		m_DCommandDlg.ShowWindow(TRUE); 
	}

	rsRightUp.left = rsLeftUp.right;
	rsRightUp.top = rs.top;
	rsRightUp.right = rs.right;
	rsRightUp.bottom = (LONG)(rs.bottom * RUDLGHEIGHT);
	if(!m_DetectInfoDlg.m_hWnd)
	{
		m_DetectInfoDlg.Create(IDD_DETECTINFODLG,this);
		m_DetectInfoDlg.SetDBigGraphView(m_pDBigGraphView);
		m_DetectInfoDlg.MoveWindow(rsRightUp); 
		m_DetectInfoDlg.ShowWindow(TRUE); 
	}

	rsRightDown.left = rsRightUp.left;
	rsRightDown.top = rsRightUp.bottom;
	rsRightDown.right = rsRightUp.right;
	rsRightDown.bottom = rs.bottom;

	m_DSmallGraphRs = rsRightDown;
	cc.m_pNewViewClass = RUNTIME_CLASS(CDetectSmallView);

	if(m_pDSmallgView == NULL || m_pDSmallgView->m_hWnd == NULL)
	{
		//创建视图
		m_pDSmallgView = (CDetectSmallView*)CreateNewView(&cc, this, CRect(0, 0, 0, 0), 0);
		if (m_pDSmallgView == NULL)
			return;
				
		m_pDSmallgView->MoveWindow(rsRightDown); 
		m_pDSmallgView->ShowWindow(TRUE); 		
	}
}

void CDetectMainView::TAuto()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
		return;

	m_pDBigGraphView->TAuto();
}

void CDetectMainView::TManual()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_MANUAL == detectType)
		return;

	m_pDBigGraphView->TManual();
}

void CDetectMainView::TStop()
{
	m_pDBigGraphView->TStop();
}

void CDetectMainView::TAcceptpcb()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	g_PCBState.Accept();
}

void CDetectMainView::TClarmp()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	g_PCBState.Clamp();
}

void CDetectMainView::TOriginx()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	m_pDBigGraphView->TOriginx();
}

void CDetectMainView::TOriginy()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	m_pDBigGraphView->TOriginy();
}

void CDetectMainView::TPcbup()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	g_PCBState.Up();
}

void CDetectMainView::TOrgan()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	m_pDBigGraphView->TOrgan();
}

void CDetectMainView::TPcbdown()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	m_pDBigGraphView->TPcbdown();
}

void CDetectMainView::TLoosenpcb()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	g_PCBState.Loose();
	m_pDBigGraphView->PcbStateChange();
}

void CDetectMainView::TSendpcb()
{
	DetectType detectType =  m_pDBigGraphView->GetDetectType();
	if(T_AUTO == detectType)
	{
		CString sInfo = ::g_LoadString("IDS_T_CURRENTAUTO","当前检测模式是自动模式，如果想手动检测，先将它改成手动模式");
		::AfxMessageBox(sInfo);
		return;
	}

	g_PCBState.SendOut();
	m_pDBigGraphView->PcbStateChange();
}


void CDetectMainView::NotifyDetectDataChanged(void)
{
	m_pDSmallgView->NotifyDetectDataChanged();
	m_DCommandDlg.NotifyDetectDataChanged();
	m_DetectInfoDlg.NotifyDetectDataChanged();
}
