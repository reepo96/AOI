// ChildFrm.cpp : CChildFrame ���ʵ��
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

unsigned short g_uDevInEvent = 0;     //�豸�����¼�
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


// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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

	//����ͼ
	iViewID = m_wndSplitter.addView(ROOT_VIEW_ID,0,1,RUNTIME_CLASS(CRootView),pContext);
	if(iViewID < 0)
		return FALSE;
		
	//�����ͼ
	iViewID = m_wndSplitter.addView(PROGRAM_VIEW_ID,0,1,RUNTIME_CLASS(CMainView),pContext);
	if(iViewID < 0)
		return FALSE;	

	//�����ͼ
	iViewID = m_wndSplitter.addView(PCBDETECT_VIEW_ID,0,1,RUNTIME_CLASS(CDetectMainView),pContext);
	if(iViewID < 0)
		return FALSE;

	//����
	iViewID = m_wndSplitter.addView(REPORT_VIEW_ID,0,1,RUNTIME_CLASS(CReportView),pContext);
	if(iViewID < 0)
		return FALSE;

	m_wndSplitter.SetColumnInfo( 0, 120, 0 );
	return TRUE;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame ���

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


// CChildFrame ��Ϣ�������

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyle(WS_CAPTION|WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE, WS_MAXIMIZE );	
	return 0;
}

//�½�ģ��
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

//����ģ��
void CChildFrame::OnSaveTemplate()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pTemplateData->m_templateName=="")
	{
		CString sInfo = g_LoadString("IDS_PLEASEINPUTPCBINFO","����֮ǰ��������PCBģ�����Ƶ���Ϣ");
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
		sErrInfo = "����ʧ��\n"+sErrInfo;
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

//��ģ��
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
			CString sMsgInfo = g_LoadString("IDS_ARE_ADJUST_TRACKER","����������ܲ�һ�£���Ҫ����������ȡ�����ٰ���ȷ������ť���粻��������ȡ������ť");
			if(IDOK == ::AfxMessageBox(sMsgInfo,MB_OKCANCEL))
			{
				g_Motor.AdjustTrackerHeight(m_pTemplateData->m_height);
			}
		}
	}

}

//�����
void CChildFrame::OnPrgGuide()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgGuide();
}

//ɨ��PCB��ͼ��
void CChildFrame::OnPrgScanpcb()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgScanpcb();
}

//�༭������
void CChildFrame::OnPrgEditarea()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);	
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->Editarea();
}

//������׼��
void CChildFrame::OnPrgMark()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->NewDatumMark();
}

//����FOV
void CChildFrame::OnPrgAddfov()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgAddfov();
}

//����FOVͼ��
void CChildFrame::OnPrgScanfov()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgScanfov();
}

//����cad�ļ�
void CChildFrame::OnPrgImportcad()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgImportcad();
}

//�����������ֵ
void CChildFrame::OnPrgCoordscale()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgCoordscale();
}

//Ԫ�����
void CChildFrame::OnPrgOrgan()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgOrgan();
}

//��������
void CChildFrame::OnPrgBarcode()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgBarcode();
}

//PCBƴ��
void CChildFrame::OnPrgSpellpcb()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgSpellpcb();
}


/******************************���*****************************/

void CChildFrame::OnTAuto()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Accept();
}

void CChildFrame::OnTClarmp()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Clamp();
}

void CChildFrame::OnTOriginx()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

    g_PCBState.Up();
}

void CChildFrame::OnTOrgan()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

   CDetectMainView	*pMainView=(CDetectMainView*)m_wndSplitter.getView(PCBDETECT_VIEW_ID);	
   pMainView->TLoosenpcb();
}

void CChildFrame::OnTSendpcb()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	sInfo = g_LoadString("IDS_SUREDELTEMPLATE","��ȷ��Ҫɾ����ǰģ����");
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
	/*::AfxMessageBox("���Ե����ʼ");
	//YT ytTest;
	//ytTest.YTInitiel();
	ZHOU zhouTest;
	//zhouTest.AxisInitial(1,0,0,0);
	//zhouTest.AxisInitial(2,0,0,0);
	//zhouTest.AxisInitial(3,0,0,0);
	zhouTest.SetAtlPos(1,0);//���ó�ʼʵ��λ��
	zhouTest.SetAtlPos(2,0);
    zhouTest.SetAtlPos(3,0);
	zhouTest.AxisRunS(1,100000,100,1,0.001);//����Ŀ��λ��
	zhouTest.AxisRunS(2,100000,100,1,0.001);//
	//zhouTest.AxisRunS(3,-200000,100,1,0.001);

	//���Ա�����
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
	print1="ʵ��λ�ã�"+print1;
	print2.Format("%d",prfPos);
	print2="�滮λ�ã�"+print2;
	print1+=print2;
	::AfxMessageBox("���������Խ����"+print1);
	//����HOME
	short z1,z2;
	z2=zhouTest.Home(2,-10000000,40000,100,10);
	if(0!=z2)
	{
		::AfxMessageBox("����2��HOME���������");
	}
	else
		::AfxMessageBox("����2��HOME�������ȷ��λ������");
	z1=zhouTest.Home(1,-10000000,0,100,10);
	if(0!=z1)
	{
		::AfxMessageBox("����1��HOME���������");
	}
	else
		::AfxMessageBox("����1��HOME�������ȷ��λ������");



	::AfxMessageBox("���Ե������");

	//GT_HookCommand(CommandHandle); //�ҽӴ�������
	//GT_Open(65535); //���˶�������
	//GT_Reset(); //��λ�˶�������
	//AxisInitial(1,0); //�������ʼ������������3-2
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
	GT_GetSts(1,&status);//��ȡ������״̬

	if(status&0x02) //��������
	{
		if(g_bStopWork == false)
		{
			g_bStopWork = true;
			GT_AbptStp(1);
			GT_AbptStp(2);
			GT_AbptStp(3);
			CString sMsgInfo = g_LoadString("IDS_ALARM_STOPWORK","�и澯������ϵͳ��ֹͣ����");
			::AfxMessageBox(sMsgInfo);
		}
	}

	GT_ExInpt(&g_uDevInEvent);

	if(g_uDevInEvent&IN_EVT_FRONT_DOOR || g_uDevInEvent&IN_EVT_BACK_DOOR ) //ǰ���ſ�		
	 {
		 if(g_bPauseWork == false)
		 {
			 g_bPauseWork = true;
			 CString sMsgInfo = g_LoadString("IDS_DOOROPEN_PAUSEWORK","�ű��򿪣�ϵͳ����ͣ����");
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
			CString sMsgInfo = g_LoadString("IDS_PUSH_YELLOWBUTTON_PAUSEDECT","��ɫ��ť�����£�ϵͳ����ͣ���");
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
	GT_ExOpt(OUT_ALL_STOP);//���
}

void CChildFrame::OnLightOn()
{
	//����
	GT_ExOpt(OUT_EVT_BLUE_LAMP&OUT_EVT_GREEN_LAMP&OUT_EVT_RED_LAMP);
}

void CChildFrame::OnPrgAngleoffset()
{
	if(m_pTemplateData == NULL)	//��ǰû��ģ��
	{
		CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
		::AfxMessageBox(sInfo);
		return ;
	}

	CMainView	*pMainView=(CMainView*)m_wndSplitter.getView(PROGRAM_VIEW_ID);
	m_wndSplitter.showView(PROGRAM_VIEW_ID);
	pMainView->SetFocus();

	pMainView->PrgAngleoffset();
}
