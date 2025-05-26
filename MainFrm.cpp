// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "AOI.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_NEW_TEMPLATE, &CMainFrame::OnUpdateNewTemplate)
	ON_UPDATE_COMMAND_UI(ID_OPEN_TEMPLATE, &CMainFrame::OnUpdateOpenTemplate)
	ON_UPDATE_COMMAND_UI(ID_SAVE_TEMPLATE, &CMainFrame::OnUpdateSaveTemplate)
	ON_UPDATE_COMMAND_UI(ID_PRG_ADDFOV, &CMainFrame::OnUpdatePrgAddfov)		
	ON_UPDATE_COMMAND_UI(ID_PRG_GUIDE, &CMainFrame::OnUpdatePrgGuide)
	ON_UPDATE_COMMAND_UI(ID_PRG_IMPORTCAD, &CMainFrame::OnUpdatePrgImportcad)
	ON_UPDATE_COMMAND_UI(ID_PRG_MARK, &CMainFrame::OnUpdatePrgMark)
	ON_UPDATE_COMMAND_UI(ID_PRG_ORGAN, &CMainFrame::OnUpdatePrgOrgan)
	ON_UPDATE_COMMAND_UI(ID_PRG_SCANFOV, &CMainFrame::OnUpdatePrgScanfov)
	ON_UPDATE_COMMAND_UI(ID_PRG_SCANPCB, &CMainFrame::OnUpdatePrgScanpcb)
	ON_UPDATE_COMMAND_UI(ID_PRG_SPELLPCB, &CMainFrame::OnUpdatePrgSpellpcb)
	ON_UPDATE_COMMAND_UI(ID_T_AUTO, &CMainFrame::OnUpdateTAuto)
	ON_UPDATE_COMMAND_UI(ID_T_MANUAL, &CMainFrame::OnUpdateTManual)
	ON_UPDATE_COMMAND_UI(ID_T_STOP, &CMainFrame::OnUpdateTStop)
	ON_COMMAND(ID_LOGIN, &CMainFrame::OnLogin)	
	ON_UPDATE_COMMAND_UI(ID_LOGIN, &CMainFrame::OnUpdateLogin)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	this->m_bAutoMenuEnable = FALSE;

#ifdef _DEBUG
	CM1.Checkpoint();
#endif
}

CMainFrame::~CMainFrame()
{
#ifdef _DEBUG
	CM2.Checkpoint();
	CM3.Difference(CM1,CM2);
#endif
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	UINT nToolBarID;
	if(g_language == ENGLISH)
	{
		nToolBarID = IDR_MAINFRAME_E;
	}
	else
	{
		nToolBarID = IDR_MAINFRAME;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(nToolBarID))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU; //WS_MAXIMIZEBOX

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnUpdateNewTemplate(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateOpenTemplate(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1 || g_UserInf.enableDetect == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateSaveTemplate(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgAddfov(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgGuide(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgImportcad(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgMark(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgOrgan(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgScanfov(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgScanpcb(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePrgSpellpcb(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableProgram == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateTAuto(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableDetect == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateTManual(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableDetect == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateTStop(CCmdUI *pCmdUI)
{
	if(g_UserInf.enableDetect == 1)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnLogin()
{
	((CAOIApp*)AfxGetApp())->OnLogin();
}


void CMainFrame::OnUpdateLogin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
