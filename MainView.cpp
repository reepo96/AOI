// MainView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "MainView.h"

#include <gdiplus.h>
#include <Gdiplusheaders.h>
#include "TemplateData.h"
#include "TemplateDataMgr.h"
#include "PrgWorkSure.h"
#include "OrganSelectDlg.h"
#include "WaitDlg.h"

const double LEFTDLGWIDTH	= 0.7;	//��߶Ի���ռ�Ŀ��
const double LUDLGHEIGHT	= 0.8;	//���϶Ի���ռ�ĸ߶�
const double RUDLGHEIGHT	= 0.6;	//���϶Ի���ռ�ĸ߶�

// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

CMainView::CMainView()
	: CFormView(CMainView::IDD)
{
	CRect rect(50,50,200,200);
	m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

	m_pBigGraphView = NULL;
	m_pSmallgGraphView = NULL;
	m_pPrgWorkSureDlg = NULL;
}

CMainView::~CMainView()
{
	if(m_pRectTracker)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainView, CFormView)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()	
	ON_COMMAND(ID_COLLECT, &CMainView::OnCollect)	
	ON_WM_SIZE()	
END_MESSAGE_MAP()


// CMainView ���

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMainView ��Ϣ�������
void CMainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	
}

void CMainView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow,nStatus);	
}

void CMainView::ShowBitmap(void)
{
	CClientDC bigDC1( (CWnd *)m_pBigGraphView);
	m_pTemplateData->m_ImageObj.BitBlt(bigDC1,0,0,m_pTemplateData->m_ImageObj.GetWidth(),m_pTemplateData->m_ImageObj.GetHeight(),0,0,SRCCOPY);	//ԭʼ��С��ͼ��

	CClientDC smallDC1(this);
	//CClientDC smallDC1( (CWnd *)&m_SmallgGraphDlg);
	CRect rs1 = m_SmallGraphRs; 
	//m_SmallgGraphDlg.GetClientRect(rs1);
	m_pTemplateData->m_ImageObj.Draw(smallDC1,0,0,(rs1.right-rs1.left),(rs1.bottom-rs1.top),0,0,m_pTemplateData->m_ImageObj.GetWidth(),m_pTemplateData->m_ImageObj.GetHeight());//�Ŵ���С	
	

	return;


	CTemplateDataMgr templateDataMgr;
	CTemplateData	templateData;
	//CImage	image;

	//templateData.m_ImageObj.Load("H:\\my_project\\AOI\\screen.bmp");

	templateDataMgr.GetData(9,&templateData);
	
	/*Gdiplus::Image *pMage =new Gdiplus::Image(L"H:\\my_project\\AOI\\screen.bmp");
	IStream *pstm=NULL;
	CreateStreamOnHGlobal(image.GetBits(),FALSE,&pstm);
	if (pstm==NULL)
			return;
	pMage = Gdiplus::Image::FromStream(pstm, FALSE);*/
	
	CClientDC bigDC( (CWnd *)m_pBigGraphView);
	templateData.m_ImageObj.BitBlt(bigDC,0,0,templateData.m_ImageObj.GetWidth(),templateData.m_ImageObj.GetHeight(),0,0,SRCCOPY);	//ԭʼ��С��ͼ��

	CClientDC smallDC( (CWnd *)m_pSmallgGraphView);
	CRect rs; 
	m_pBigGraphView->GetClientRect(rs);
	templateData.m_ImageObj.Draw(smallDC,0,0,(rs.right-rs.left),(rs.bottom-rs.top),0,0,templateData.m_ImageObj.GetWidth(),templateData.m_ImageObj.GetHeight());//�Ŵ���С
	
	/*Gdiplus::Rect rect;
	rect.X=0;
	rect.Y=0;
	rect.Height = (rs.bottom - rs.top)*10;
	rect.Width  = (rs.right - rs.left)*10;
	Gdiplus::Graphics graphics(smallDC.m_hDC);
	graphics.DrawImage(pMage, rect);*/

	//�ڴ��л�ͼ
	/*HDC	hMemDc = image.GetDC();
	CDC *pDC =CDC::FromHandle(hMemDc);
	pDC->Draw3dRect(rs,RGB(0,255,0),RGB(0,255,0));
	image.ReleaseDC();
	image.BitBlt(bigDC,0,0,image.GetWidth(),image.GetHeight(),0,0,SRCCOPY);

	//
	CRgn rgn;
	rgn.CreateRectRgn(10,10,300,300);
	CBrush *pBrush = bigDC.GetCurrentBrush();
	bigDC.FrameRgn(&rgn,pBrush,2,2);

	//DrawDragRect() //��ȥ�����»��µľ���

	//
	CBrush *pNewBrush = new CBrush();
	CRect rectCtrol(50,50,200,200);
	bigDC.DrawFocusRect(&rectCtrol);
	bigDC.FrameRect(&rectCtrol,pBrush);*/

	//��ѡ����ο�
	m_pRectTracker->Draw(&bigDC);
	//m_pRectTracker->Track(
	//delete m_pRectTracker;
	//m_pRectTracker = NULL;
	//int iTest = rectTracker.HitTest()

	//����
	/*templateData.m_height = 800;
	templateData.m_width = 800;
	templateData.m_templateName = " pcb name";
	templateDataMgr.store(templateData);*/
}

void CMainView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFormView::OnLButtonDown(nFlags, point);
}

void CMainView::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	m_pBigGraphView->SetTemplateData(pTemplateData);
	m_pBigGraphView->Invalidate();

	m_pSmallgGraphView->SetTemplateData(pTemplateData);
	m_pSmallgGraphView->Invalidate();

	m_proInfoDlg.SetTemplateData(pTemplateData);
}


void CMainView::OnZoomin()
{
	m_pBigGraphView->OnZoomin();
}

void CMainView::OnZoomout()
{
	m_pBigGraphView->OnZoomout();
}

void CMainView::OnOriginal()
{
	m_pBigGraphView->OnOriginal();
}


void CMainView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: �ڴ����ר�ô����/����û���

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMainView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rs; 
	GetClientRect(rs);

	CRect rsLeftUp;		//���϶Ի����rect
	CRect rsLeftDown;	//���¶Ի����rect
	CRect rsRightUp;	//���϶Ի����rect
	CRect rsRightDown;	//���¶Ի����rect

	rsLeftUp.left = rs.left;
	rsLeftUp.top = rs.top;
	rsLeftUp.right = (LONG)(rs.right * LEFTDLGWIDTH);
	rsLeftUp.bottom = (LONG)(rs.bottom * LUDLGHEIGHT);

	m_BigGraphRs = rsLeftUp;

	CCreateContext cc;
	cc.m_pNewViewClass = RUNTIME_CLASS(CBigGraphView);
	cc.m_pCurrentDoc = NULL;
	cc.m_pNewDocTemplate = NULL;
	cc.m_pLastView = NULL;
	cc.m_pCurrentFrame = NULL;

	if(m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
	{
		//������ͼ
		m_pBigGraphView = (CBigGraphView*)CreateNewView(&cc, this, CRect(0, 0, 0, 0), 0);
		if (m_pBigGraphView == NULL)
			return;

		m_pBigGraphView->MoveWindow(&rsLeftUp);
		m_pBigGraphView->ShowWindow(TRUE); 
	}
	
	rsLeftDown.left =  rs.left;
	rsLeftDown.top = rsLeftUp.bottom;
	rsLeftDown.right = rsLeftUp.right;
	rsLeftDown.bottom = rs.bottom;
	if(!m_commandDlg.m_hWnd)
	{
		m_commandDlg.Create(IDD_COMMANDDLG,this);
		m_commandDlg.SetBigGraphView(m_pBigGraphView);
		m_commandDlg.MoveWindow(rsLeftDown); 
		m_commandDlg.ShowWindow(TRUE); 
	}

	rsRightUp.left = rsLeftUp.right;
	rsRightUp.top = rs.top;
	rsRightUp.right = rs.right;
	rsRightUp.bottom = (LONG)(rs.bottom * RUDLGHEIGHT);
	if(!m_proInfoDlg.m_hWnd)
	{
		m_proInfoDlg.Create(IDD_PROGRAMINFODLG,this);
		m_proInfoDlg.SetBigGraphView(m_pBigGraphView);
				
		m_proInfoDlg.MoveWindow(rsRightUp); 
		m_proInfoDlg.ShowWindow(TRUE); 
	}

	rsRightDown.left = rsRightUp.left;
	rsRightDown.top = rsRightUp.bottom;
	rsRightDown.right = rsRightUp.right;
	rsRightDown.bottom = rs.bottom;

	m_SmallGraphRs = rsRightDown;
	cc.m_pNewViewClass = RUNTIME_CLASS(CSmallGraView);

	if(m_pSmallgGraphView == NULL || m_pSmallgGraphView->m_hWnd == NULL)
	{
		//������ͼ
		m_pSmallgGraphView = (CSmallGraView*)CreateNewView(&cc, this, CRect(0, 0, 0, 0), 0);
		if (m_pSmallgGraphView == NULL)
			return;
				
		m_pSmallgGraphView->MoveWindow(rsRightDown); 
		m_pSmallgGraphView->ShowWindow(TRUE); 

		m_pSmallgGraphView->SetBigGraphView(m_pBigGraphView);
	}
}

// �����
void CMainView::PrgGuide(void)
{
	m_pBigGraphView->PrgGuide();
}

//����ɨ�裬���pcb���ͼ��
void CMainView::OnCollect()
{
	//wait
	/*CWaitDlg *pWaitDlg = new CWaitDlg(this);
	pWaitDlg->Create(IDD_WAIT,this);
	pWaitDlg->ShowWindow(SW_SHOW);*/

	m_pBigGraphView->LoadTemplateImage();

	//wait end
	/*pWaitDlg->DestroyWindow();
	if(pWaitDlg)
	{
		delete pWaitDlg;
		pWaitDlg = NULL;
	}*/

	m_pSmallgGraphView->Invalidate();
}

void CMainView::Editarea(void)
{
	if(m_pPrgWorkSureDlg==NULL || m_pPrgWorkSureDlg->m_hWnd == NULL || !IsWindow(m_pPrgWorkSureDlg->m_hWnd))
	{
		m_pPrgWorkSureDlg = new CPrgWorkSure(m_pBigGraphView);
		m_pPrgWorkSureDlg->Create(IDD_WORKSURE,m_pBigGraphView);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pPrgWorkSureDlg->GetDlgItem(IDC_INFO);
	CString sInfo = g_LoadString("IDS_EDITAREAINFO","������������ϵı༭����Ȼ�󰴡�ȷ������ť");
	pInfoCtrl->SetWindowTextA(sInfo);
	m_pPrgWorkSureDlg->ShowWindow(SW_SHOW);

	m_pBigGraphView->Editarea();
}

void CMainView::NewDatumMark(void)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","���ȴ���FOV����");
		::AfxMessageBox(sInfo);
		return;
	}

	/*if(m_pPrgWorkSureDlg==NULL || m_pPrgWorkSureDlg->m_hWnd == NULL)
	{
		m_pPrgWorkSureDlg = new CPrgWorkSure(m_pBigGraphView);
		m_pPrgWorkSureDlg->Create(IDD_WORKSURE,m_pBigGraphView);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pPrgWorkSureDlg->GetDlgItem(IDC_INFO);
	CString sInfo = g_LoadString("IDS_EDITMARKINFO","�밴�ջ�׼�����������ڽ��л�׼��ı��");
	pInfoCtrl->SetWindowTextA(sInfo);
	m_pPrgWorkSureDlg->ShowWindow(SW_SHOW);*/

	m_pBigGraphView->NewDatumMark();
}

void CMainView::PrgScanpcb(void)
{
    //wait
	/*CWaitDlg *pWaitDlg = new CWaitDlg(this);
	pWaitDlg->Create(IDD_WAIT,this);
	pWaitDlg->ShowWindow(SW_SHOW);*/

	m_pBigGraphView->LoadTemplateImage();

	//wait end
	/*pWaitDlg->DestroyWindow();
	if(pWaitDlg)
	{
		delete pWaitDlg;
		pWaitDlg = NULL;
	}*/

	m_pSmallgGraphView->Invalidate();
}

void CMainView::PrgAddfov(void)
{
	if(m_pPrgWorkSureDlg==NULL || m_pPrgWorkSureDlg->m_hWnd == NULL || !IsWindow(m_pPrgWorkSureDlg->m_hWnd))
	{
		m_pPrgWorkSureDlg = NULL;
		m_pPrgWorkSureDlg = new CPrgWorkSure(m_pBigGraphView);
		m_pPrgWorkSureDlg->Create(IDD_WORKSURE,m_pBigGraphView);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pPrgWorkSureDlg->GetDlgItem(IDC_INFO);
	CString sInfo = g_LoadString("IDS_EDITFOVINFO","�����FOV�������ĵ㣬Ȼ�󰴡�ȷ������ťȷ��");
	pInfoCtrl->SetWindowTextA(sInfo);
	m_pPrgWorkSureDlg->ShowWindow(SW_SHOW);

	m_pBigGraphView->NewFOV();
}

void CMainView::PrgScanfov(void)
{
	m_pBigGraphView->LoadAllFov();
}

void CMainView::PrgScanModifyFov(void)
{
	m_pBigGraphView->LoadModifyFov();
}

void CMainView::PrgImportcad(void)
{
	m_proInfoDlg.PrgImportcad();
}

void CMainView::PrgCoordscale(void)
{
	m_pBigGraphView->BeginCoordCalc(false);
}

void CMainView::PrgOrgan(void)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","���ȴ���FOV����");
		::AfxMessageBox(sInfo);
		return;
	}
	
	if(m_pTemplateData->m_lCoordScaleX<=0.00 || m_pTemplateData->m_lCoordScaleY<=0.00)
	{
		CString sInfo = ::g_LoadString("IDS_MUSTCOORDSCALE","Ԫ�����ǰ�����ȼ����������ֵ");
		::AfxMessageBox(sInfo);
		return;
	}

	COrganData *pOrgan=NULL;
	PrgState selectState[]={PS_NEW,PS_SKIP,PS_INVALID};
	COrganSelectDlg selectOrgan(m_pTemplateData,&pOrgan,m_pBigGraphView,&selectState[0],3);

	selectOrgan.DoModal();
	if(pOrgan != NULL)
	{
		m_pBigGraphView->PrgOrgan(true,pOrgan);
	}
}

void CMainView::PrgBarcode(void)
{
	m_pBigGraphView->PrgBar(false);
}

void CMainView::PrgSpellpcb(void)
{
	m_pBigGraphView->PrgSpellPcb();
}

// ֪ͨ��������Ѿ��ı�
void CMainView::NotifyPrgDataChanged(void)
{
	m_proInfoDlg.NotifyPrgDataChanged();
	m_pSmallgGraphView->NotifyPrgDataChanged();
	m_commandDlg.NotifyPrgDataChanged();
}

// ֪ͨ��Ƥ���Ѿ��ı�
void CMainView::NotifyTrakerChanged(void)
{
	//m_proInfoDlg.NotifyTrakerChanged();	
	m_commandDlg.NotifyTrakerChanged();
}

void CMainView::ModifyFov(CFovData* pFovData)
{
	if(m_pPrgWorkSureDlg==NULL || m_pPrgWorkSureDlg->m_hWnd == NULL || !IsWindow(m_pPrgWorkSureDlg->m_hWnd))
	{
		m_pPrgWorkSureDlg = NULL;
		m_pPrgWorkSureDlg = new CPrgWorkSure(m_pBigGraphView);
		m_pPrgWorkSureDlg->Create(IDD_WORKSURE,m_pBigGraphView);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pPrgWorkSureDlg->GetDlgItem(IDC_INFO);
	CString sInfo = g_LoadString("IDS_MODIFYFOVINFO","�����FOV����Ȼ�󰴡�ȷ������ťȷ��");
	pInfoCtrl->SetWindowTextA(sInfo);
	m_pPrgWorkSureDlg->ShowWindow(SW_SHOW);

	m_pBigGraphView->ModifyFov(pFovData);
}

void CMainView::InvalidBigGraphView(void)
{
	m_pBigGraphView->Invalidate();
	m_pSmallgGraphView->Invalidate();
}

void CMainView::ModifyDatumMark(CDatumMark* pDatumMark)
{
	if(m_pPrgWorkSureDlg==NULL || m_pPrgWorkSureDlg->m_hWnd == NULL || !IsWindow(m_pPrgWorkSureDlg->m_hWnd))
	{
		m_pPrgWorkSureDlg = NULL;
		m_pPrgWorkSureDlg = new CPrgWorkSure(m_pBigGraphView);
		m_pPrgWorkSureDlg->Create(IDD_WORKSURE,m_pBigGraphView);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pPrgWorkSureDlg->GetDlgItem(IDC_INFO);
	CString sInfo = g_LoadString("IDS_MODIFYMARKINFO","�������׼������Ȼ�󰴡�ȷ������ťȷ��");
	pInfoCtrl->SetWindowTextA(sInfo);
	m_pPrgWorkSureDlg->ShowWindow(SW_SHOW);

	m_pBigGraphView->ModifyDatumMark(pDatumMark);
}

void CMainView::ModifyOrgan(COrganData * pOrgan)
{
	m_pBigGraphView->ModifyOrganPrg(pOrgan);
}

void CMainView::DelOrgan(COrganData * pOrgan)
{
	pOrgan->DelPrgData();
	m_pTemplateData->m_bDataIsChanged = true;
	m_pTemplateData->m_bOrganDataIsChange = true;
	
	m_pBigGraphView->Invalidate();
}

void CMainView::SetCurrentOrgan(COrganData* pOrganData)
{
	m_pBigGraphView->SetCurrentOrgan(pOrganData);
}

void CMainView::SetCurrentImage(CImage * pImage, ImageType type)
{
	m_pBigGraphView->SetCurrentImage(pImage,type);
}

void CMainView::SetCurrentShowFov(CFovData* pCurrentShowFov)
{
	m_pBigGraphView->SetCurrentShowFov(pCurrentShowFov);
}


void CMainView::ColorChanged(void)
{
	m_pBigGraphView->Invalidate();
}


void CMainView::DelSpellPrg(int no)
{
	EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	map<int,CSpellPCB*>::iterator it = m_pTemplateData->m_SpellPcbMap.begin();
	for(;it!=m_pTemplateData->m_SpellPcbMap.end();it++)
	{
		if((*it).second->no ==no )
		{
			m_pTemplateData->m_SpellPcbMap.erase(it);
			m_pTemplateData->m_bDataIsChanged = true;
			m_pBigGraphView->Invalidate();
			break;
		}
	}
	LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);
}

// ��FOV��Ϣ�Ի���任FOV
void CMainView::ChangeFovFromInfDlg(CFovData* pFovData)
{
	m_pSmallgGraphView->SetFlashRect(pFovData);
}

void CMainView::PrgAngleoffset(void)
{
	m_pBigGraphView->PrgAngleoffset();
}
