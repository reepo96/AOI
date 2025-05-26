// ChildFrm.cpp : CChildFrame 类的实现
//
#include "stdafx.h"
#include "AOI.h"

#include "ChildFrm.h"
#include "MyTreeView.h"
#include "RootView.h"
#include "ProgramView.h"
#include "MainView.h"
#include "DetectMainView.h"
#include "ReportView.h"
#include "TemplateDataMgr.h"
#include "TemplateSelDlg.h"
#include "UserMgrDlg.h"
#include "WaitDlg.h"
#include "SetColorDlg.h"
#include "SetCamera.h"
#include "PCBState.h"
#include "SetSystemCfg.h"
#include "MotorCtrl.h"

/***********************/
#include "YT.h"
#include "ZHOU.h"
#include "GEIO.h"

unsigned short g_uDevInEvent = 0;     //设备输入事件
extern CPCBState   g_PCBState;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_NEW_TEMPLATE, &CChildFrame::OnNewTemplate)
	ON_COMMAND(ID_SAVE_TEMPLATE, &CChildFrame::OnSaveTemplate)
	ON_COMMAND(ID_OPEN_TEMPLATE, &CChildFrame::OnOpenTemplate)
	ON_COMMAND(ID_PRG_GUIDE, &CChildFrame::OnPrgGuide)
	ON_COMMAND(ID_PRG_SCANPCB, &CChildFrame::OnPrgScanpcb)
	ON_COMMAND(ID_PRG_EDITAREA, &CChildFrame::OnPrgEditarea)
	ON_COMMAND(ID_PRG_MARK, &CChildFrame::OnPrgMark)
	ON_COMMAND(ID_PRG_ADDFOV, &CChildFrame::OnPrgAddfov)
	ON_COMMAND(ID_PRG_SCANFOV, &CChildFrame::OnPrgScanfov)
	ON_COMMAND(ID_PRG_IMPORTCAD, &CChildFrame::OnPrgImportcad)
	ON_COMMAND(ID_PRG_COORDSCALE, &CChildFrame::OnPrgCoordscale)
	ON_COMMAND(ID_PRG_ORGAN, &CChildFrame::OnPrgOrgan)
	ON_COMMAND(ID_PRG_BARCODE, &CChildFrame::OnPrgBarcode)
	ON_COMMAND(ID_PRG_SPELLPCB, &CChildFrame::OnPrgSpellpcb)
	ON_COMMAND(ID_T_AUTO, &CChildFrame::OnTAuto)
	ON_COMMAND(ID_T_MANUAL, &CChildFrame::OnTManual)
	ON_COMMAND(ID_T_STOP, &CChildFrame::OnTStop)
	ON_COMMAND(ID_T_ACCEPTPCB, &CChildFrame::OnTAcceptpcb)
	ON_COMMAND(ID_T_CLARMP, &CChildFrame::OnTClarmp)
	ON_COMMAND(ID_T_ORIGINX, &CChildFrame::OnTOriginx)
	ON_COMMAND(ID_T_ORIGINY, &CChildFrame::OnTOriginy)
	ON_COMMAND(ID_T_PCBUP, &CChildFrame::OnTPcbup)
	ON_COMMAND(ID_T_ORGAN, &CChildFrame::OnTOrgan)
	ON_COMMAND(ID_T_PCBDOWN, &CChildFrame::OnTPcbdown)
	ON_COMMAND(ID_T_LOOSENPCB, &CChildFrame::OnTLoosenpcb)
	ON_COMMAND(ID_T_SENDPCB, &CChildFrame::OnTSendpcb)
	ON_COMMAND(ID_USRMGR, &CChildFrame::OnUsrmgr)
	ON_COMMAND(ID_ZOOMIN, &CChildFrame::OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, &CChildFrame::OnZoomout)
	ON_COMMAND(ID_ORIGINAL, &CChildFrame::OnOriginal)
	ON_COMMAND(ID_SETCOLOR, &CChildFrame::OnSetcolor)
	ON_COMMAND(ID_DELTEMPLATE, &CChildFrame::OnDeltemplate)
	ON_COMMAND(ID_32859, &CChildFrame::OnTestDJ)
	ON_COMMAND(ID_CAMERA_SET, &CChildFrame::OnCameraSet)
	ON_WM_TIMER()
	ON_COMMAND(ID_SYSTEM, &CChildFrame::OnSystem)
	ON_COMMAND(ID_REMOVE_SET, &CChildFrame::OnRemoveSet)
	ON_COMMAND(ID_LIGHT_OFF, &CChildFrame::OnLightOff)
	ON_COMMAND(ID_LIGHT_ON, &CChildFrame::OnLightOn)
	ON_COMMAND(ID_PRG_ANGLEOFFSET, &CChildFrame::OnPrgAngleoffset)
END_MESSAGE_MAP()


// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
	m_pTemplateData = NULL;
}

CChildFrame::~CChildFrame()
{
	if(m_pTemplateData!=NULL)
	{
		delete m_pTemplateData;
		m_pTemplateData = NULL;
	}
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	this->SetTimer(1,500,NULL);

	if (!m_wndSplitter.CreateStatic(this,1,2))
		return FALSE;
	
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyTreeView),  CSize(10,10),pContext);

	int iViewID;

	//根视图
	iViewID = m_wndSplitter.addView(ROOT_VIEW_ID,0,1,RUNTIME_CLASS(CRootView),pContext);
	if(iViewID < 0)
		return FALSE;
		
	//编程试图
	iViewID = m_wndSplitter.addView(PROGRAM_VIEW_ID,0,1,RUNTIME_CLASS(CMainView),pContext);
	if(iViewID < 0)
		return FALSE;	

	//检测试图
	iViewID = m_wndSplitter.addView(PCBDETECT_VIEW_ID,0,1,RUNTIME_CLASS(CDetectMainView),pContext);
	if(iViewID < 0)
		return FALSE;

	//报表
	iViewID = m_wndSplitter.addView(REPORT_VIEW_ID,0,1,RUNTIME_CLASS(CReportView),pContext);
	if(iViewID < 0)
		return FALSE;

	m_wndSplitter.SetColumnInfo( 0, 120, 0 );
	return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame 消息处理程序

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyle(WS_CAPTION|WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE, WS_MAXIMIZE );	
	return 0;
}

//新建模板
void CChildFrame::OnNewTemplate()
{
	CMyTreeView *pTreeView=(CMyTreeView*)m_wndSplitter.GetPane(0,0);
	if(pTreeView->IsKindOf( RUNTIME_CLASS(CMyTreeView) ) )
	{
		if(m_pTemplateData != NULL)
		{
			delete m_pTemplateData;
			m_pTemplateData = NULL;
		}
		m_pTemplateData = new CTemplateData();
		//m_pTemplateData->m_ImageObj.Load("..\\screen.bmp");

		CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
		pMainView->SetTemplateData(m_pTemplateData);

		//just for test
		//pMainView->OnCollect();

		CDetectMainView	*pDetectMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
		pDetectMainView->SetTemplateData(m_pTemplateData);
		
		pTreeView->SetTemplateData(m_pTemplateData);
		g_pTemplateData = m_pTemplateData;
	}
	else
	{
		::AfxMessageBox("error pane");
		return;
	}
}

//保存模板
void CChildFrame::OnSaveTemplate()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pTemplateData->m_templateName=="")
	{
		CString sInfo = g_LoadString("IDS_PLEASEINPUTPCBINFO","保存之前请先输入PCB模板名称等信息");
		::AfxMessageBox(sInfo);
		return;
	}

	CTemplateDataMgr templateDataMgr;

	CString	sErrInfo;

	//wait
	CWaitDlg *pWaitDlg = new CWaitDlg(this);
	pWaitDlg->Create(IDD_WAIT,this);
	pWaitDlg->ShowWindow(SW_SHOW);
	
	if(false==templateDataMgr.store(m_pTemplateData,sErrInfo))
	{
		sErrInfo = "保存失败\n"+sErrInfo;
		::AfxMessageBox(sErrInfo);
	}

	//wait end
	pWaitDlg->DestroyWindow();
	if(pWaitDlg)
	{
		delete pWaitDlg;
		pWaitDlg = NULL;
	}
}

//打开模板
void CChildFrame::OnOpenTemplate()
{
	long lOldPCBHeight = 0;

	CMyTreeView *pTreeView=(CMyTreeView*)m_wndSplitter.GetPane(0,0);
	if( !pTreeView->IsKindOf( RUNTIME_CLASS(CMyTreeView) ) )
	{
		return;
	}

	long lTemplateID=0;
	CTemplateSelDlg selDlg(&lTemplateID,this);
	
	if(IDOK==selDlg.DoModal())
	{
		if(m_pTemplateData != NULL)
		{
			lOldPCBHeight = m_pTemplateData->m_height;
			delete m_pTemplateData;
			m_pTemplateData = NULL;
		}
		m_pTemplateData = new CTemplateData();

		//wait
		CWaitDlg *pWaitDlg = new CWaitDlg(this);
		pWaitDlg->Create(IDD_WAIT,this);
		pWaitDlg->ShowWindow(SW_SHOW);
		
		CTemplateDataMgr templateDataMgr;
		templateDataMgr.GetData(lTemplateID,m_pTemplateData);

		//wait end
		pWaitDlg->DestroyWindow();
		if(pWaitDlg)
		{
			delete pWaitDlg;
			pWaitDlg = NULL;
		}

		CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
		pMainView->SetTemplateData(m_pTemplateData);

		CDetectMainView	*pDetectMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
		pDetectMainView->SetTemplateData(m_pTemplateData);

		pTreeView->SetTemplateData(m_pTemplateData);
		pTreeView->ReSelectCurrent();
		g_pTemplateData = m_pTemplateData;

		if(lOldPCBHeight != m_pTemplateData->m_height && lOldPCBHeight != 0)
		{
			CString sMsgInfo = g_LoadString("IDS_ARE_ADJUST_TRACKER","板跟轨道宽可能不一致，如要调整，请先取出板再按“确定”按钮，如不调整按“取消”按钮");
			if(IDOK == ::AfxMessageBox(sMsgInfo,MB_OKCANCEL))
			{
				g_Motor.AdjustTrackerHeight(m_pTemplateData->m_height);
			}
		}
	}

}

//编程向导
void CChildFrame::OnPrgGuide()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgGuide();
}

//扫描PCB板图像
void CChildFrame::OnPrgScanpcb()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgScanpcb();
}

//编辑区域编程
void CChildFrame::OnPrgEditarea()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->Editarea();
}

//创建基准点
void CChildFrame::OnPrgMark()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->NewDatumMark();
}

//创建FOV
void CChildFrame::OnPrgAddfov()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgAddfov();
}

//加载FOV图像
void CChildFrame::OnPrgScanfov()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgScanfov();
}

//导入cad文件
void CChildFrame::OnPrgImportcad()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgImportcad();
}

//计算坐标相对值
void CChildFrame::OnPrgCoordscale()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgCoordscale();
}

//元件编程
void CChildFrame::OnPrgOrgan()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgOrgan();
}

//条形码编程
void CChildFrame::OnPrgBarcode()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgBarcode();
}

//PCB拼版
void CChildFrame::OnPrgSpellpcb()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgSpellpcb();
}


/******************************检测*****************************/

void CChildFrame::OnTAuto()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TAuto();
}

void CChildFrame::OnTManual()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TManual();
}

void CChildFrame::OnTStop()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TStop();
}

void CChildFrame::OnTAcceptpcb()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Accept();
}

void CChildFrame::OnTClarmp()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Clamp();
}

void CChildFrame::OnTOriginx()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TOriginx();
}

void CChildFrame::OnTOriginy()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TOriginy();
}

void CChildFrame::OnTPcbup()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Up();
}

void CChildFrame::OnTOrgan()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TOrgan();
}

void CChildFrame::OnTPcbdown()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	m_wndSplitter.showView(PCBDETECT_VIEW_ID);
	pMainView->SetFocus();

	pMainView->TPcbdown();
}

void CChildFrame::OnTLoosenpcb()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

   CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);	
   pMainView->TLoosenpcb();
}

void CChildFrame::OnTSendpcb()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);	
	pMainView->TSendpcb();
}

void CChildFrame::OnUsrmgr()
{
	CUserMgrDlg userMgr;
	userMgr.DoModal();
}

void CChildFrame::OnZoomin()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->OnZoomin();
}

void CChildFrame::OnZoomout()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->OnZoomout();
}

void CChildFrame::OnOriginal()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->OnOriginal();
}


void CChildFrame::OnSetcolor()
{
	CSetColorDlg	colorDlg;
	if(IDOK==colorDlg.DoModal())
	{
		CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
		pMainView->ColorChanged();
	}
}

void CChildFrame::OnDeltemplate()
{
	CString sInfo;
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	sInfo = g_LoadString("IDS_SUREDELTEMPLATE","你确定要删除当前模板吗？");
	if(IDNO==::AfxMessageBox(sInfo,MB_YESNO) )
	{
		return;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	pMainView->SetTemplateData(NULL);

	CDetectMainView	*pDetectMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);
	pDetectMainView->SetTemplateData(NULL);

	CMyTreeView *pTreeView=(CMyTreeView*)m_wndSplitter.GetPane(0,0);
	pTreeView->SetTemplateData(NULL);
	pTreeView->ReSelectCurrent();

	CTemplateDataMgr templateDataMgr;
	if(false == templateDataMgr.DelTemplate(m_pTemplateData,sInfo))
	{
		pMainView->SetTemplateData(NULL);		
		pDetectMainView->SetTemplateData(NULL);
		pTreeView->SetTemplateData(NULL);
		pTreeView->ReSelectCurrent();

		::AfxMessageBox(sInfo);
		return;
	}

	delete m_pTemplateData;
	m_pTemplateData = NULL;
	pTreeView->SetTemplateData(NULL);

	g_pTemplateData = NULL;

	::AfxMessageBox("Delete success.");
}
//******************************************************************
void CChildFrame::OnTestDJ()
{
	/*::AfxMessageBox("测试电机开始");
	//YT ytTest;
	//ytTest.YTInitiel();
	ZHOU zhouTest;
	//zhouTest.AxisInitial(1,0,0,0);
	//zhouTest.AxisInitial(2,0,0,0);
	//zhouTest.AxisInitial(3,0,0,0);
	zhouTest.SetAtlPos(1,0);//设置初始实际位置
	zhouTest.SetAtlPos(2,0);
    zhouTest.SetAtlPos(3,0);
	zhouTest.AxisRunS(1,100000,100,1,0.001);//设置目标位置
	zhouTest.AxisRunS(2,100000,100,1,0.001);//
	//zhouTest.AxisRunS(3,-200000,100,1,0.001);

	//测试编码器
	unsigned long status;
	long atlPos,prfPos;
	do
	{
		GT_GetSts(2,&status);
	}while(status&0x400);
	GT_GetAtlPos(1,&atlPos);
	GT_GetPrfPos(1,&prfPos);
	CString print1,print2;
	print1.Format ("%d",atlPos*4);
	print1="实际位置："+print1;
	print2.Format("%d",prfPos);
	print2="规划位置："+print2;
	print1+=print2;
	::AfxMessageBox("编码器测试结果："+print1);
	//测试HOME
	short z1,z2;
	z2=zhouTest.Home(2,-10000000,40000,100,10);
	if(0!=z2)
	{
		::AfxMessageBox("测试2轴HOME结果：出错");
	}
	else
		::AfxMessageBox("测试2轴HOME结果：正确，位置清零");
	z1=zhouTest.Home(1,-10000000,0,100,10);
	if(0!=z1)
	{
		::AfxMessageBox("测试1轴HOME结果：出错");
	}
	else
		::AfxMessageBox("测试1轴HOME结果：正确，位置清零");



	::AfxMessageBox("测试电机结束");

	//GT_HookCommand(CommandHandle); //挂接错误处理函数
	//GT_Open(65535); //打开运动控制器
	//GT_Reset(); //复位运动控制器
	//AxisInitial(1,0); //控制轴初始化，引用例程3-2
	//AxisRunS(1,200000,166.7,1,0.001);*/
}
void CChildFrame::OnCameraSet()
{
	CSetCamera setCameraDlg;
	setCameraDlg.DoModal();
}

void CChildFrame::OnTimer(UINT_PTR nIDEvent)
{
#ifndef _MYUTEST
	unsigned long status;
	GT_GetSts(1,&status);//读取控制轴状态

	if(status&0x02) //报警触发
	{
		if(g_bStopWork == false)
		{
			g_bStopWork = true;
			GT_AbptStp(1);
			GT_AbptStp(2);
			GT_AbptStp(3);
			CString sMsgInfo = g_LoadString("IDS_ALARM_STOPWORK","有告警发生，系统将停止工作");
			::AfxMessageBox(sMsgInfo);
		}
	}

	GT_ExInpt(&g_uDevInEvent);

	if(g_uDevInEvent&IN_EVT_FRONT_DOOR || g_uDevInEvent&IN_EVT_BACK_DOOR ) //前后门开		
	 {
		 if(g_bPauseWork == false)
		 {
			 g_bPauseWork = true;
			 CString sMsgInfo = g_LoadString("IDS_DOOROPEN_PAUSEWORK","门被打开，系统将暂停工作");
			::AfxMessageBox(sMsgInfo);
		 }
	 }
	else
	{
		g_bPauseWork = false;
	}

	if((~g_uDevInEvent)&IN_EVT_YELLOW_BUTTON)
	{
		if(g_bPauseDetect == false)
		{
			g_bPauseDetect = true;
			CString sMsgInfo = g_LoadString("IDS_PUSH_YELLOWBUTTON_PAUSEDECT","黄色按钮被按下，系统将暂停检测");
			::AfxMessageBox(sMsgInfo);
		}
	}
	else if((~g_uDevInEvent)&IN_EVT_GREEN_BUTTON)
	{
		g_bPauseDetect = false;
	}
#endif
	CMDIChildWnd::OnTimer(nIDEvent);
}

void CChildFrame::OnSystem()
{
	SetSystemCfg cfgDlg;
	cfgDlg.DoModal();
}

void CChildFrame::OnRemoveSet()
{
	CMotorCtrl motorCtrl;
	motorCtrl.DoModal();
}

void CChildFrame::OnLightOff()
{
	GT_ExOpt(OUT_ALL_STOP);//灭灯
}

void CChildFrame::OnLightOn()
{
	//亮灯
	GT_ExOpt(OUT_EVT_BLUE_LAMP&OUT_EVT_GREEN_LAMP&OUT_EVT_RED_LAMP);
}

void CChildFrame::OnPrgAngleoffset()
{
	if(m_pTemplateData == NULL)	//当前没有模板
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgAngleoffset();
}
