// BarPrgGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "BarPrgGuid.h"
#include "BigGraphView.h"


// CBarPrgGuid 对话框

IMPLEMENT_DYNAMIC(CBarPrgGuid, CDialog)

CBarPrgGuid::CBarPrgGuid(bool bDoNext,CWnd* pParent /*=NULL*/)
	: CDialog(CBarPrgGuid::IDD, pParent)
{
	m_pFovData = NULL;
	m_bDoNext = bDoNext;
	m_CurrentWorkStep = BWS_SEARCHAREA;

	m_pCurrentBarData = NULL;
	m_pWnd = pParent;
	m_pTemplateData = NULL;
}

CBarPrgGuid::~CBarPrgGuid()
{
}

void CBarPrgGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBarPrgGuid, CDialog)
	ON_BN_CLICKED(IDC_OK, &CBarPrgGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CBarPrgGuid::OnBnClickedCancle)
END_MESSAGE_MAP()


// CBarPrgGuid 消息处理程序

BOOL CBarPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_BAR_SEARCHAREA_STEP","请切换到包含条形码的FOV，然后框选出条形码搜索区域");
	pInfoCtrl->SetWindowTextA(sStepInfo);

	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}
	
	((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBarPrgGuid::EndCurrentWork(void)
{
	CString sMsgInfo;
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pWnd)->m_lSrcOriginY;

	m_pFovData = ((CBigGraphView*)m_pWnd)->m_pCurrentShowFov;
	if(m_pFovData == NULL)
		return;

	switch(m_CurrentWorkStep)
	{	
	case BWS_SEARCHAREA:
		if(((CBigGraphView*)m_pWnd)->m_pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","你确定搜索区域已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			m_pCurrentBarData = new CBarData();
			m_pCurrentBarData->barSerchBottom = originRect.bottom;
			m_pCurrentBarData->barSerchLeft = originRect.left;
			m_pCurrentBarData->barSerchRight = originRect.right;
			m_pCurrentBarData->barSerchTop = originRect.top;

			m_pTemplateData->m_pBar = m_pCurrentBarData;
			m_pTemplateData->m_bDataIsChanged = true;

			m_pCurrentBarData->fovId = m_pFovData->fovId;
			m_pCurrentBarData->m_pFovData = m_pFovData;

			//m_pFovData->m_pBar = m_pCurrentBarData;
				
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		//执行下一步
		this->NextStep();		
		break;
	case BWS_BODY:	
		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_BODY","你确定主体已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT
			
			m_pCurrentBarData->bottom = originRect.bottom;
			m_pCurrentBarData->left = originRect.left;
			m_pCurrentBarData->right = originRect.right;
			m_pCurrentBarData->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
	
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		((CBigGraphView*)m_pWnd)->EndCurrentWork(m_bDoNext);
		this->DestroyWindow();
		break;
	}
}

void CBarPrgGuid::NextStep(void)
{
	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);

	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pWnd)->m_pRectTracker;
	if(pRectTracker != NULL)
	{
		sStepInfo = g_LoadString("IDS_CURENTSTEPSTILLWORK","当前步骤仍处于编程状态，请按确定按钮结束其编程状态，才能进行下一步编程");
		AfxMessageBox(sStepInfo);
		return;
	}

	switch(m_CurrentWorkStep)
	{
	case BWS_SEARCHAREA:
		sStepInfo = g_LoadString("IDS_BAR_BODY_STEP","请按下CTRL键，拖动鼠标形成条形码主体区域，然后再微调，最后按“确定”按钮确定");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}		
		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;
		m_CurrentWorkStep = BWS_BODY;

		break;	
	default:
		break;
	}
}

void CBarPrgGuid::OnBnClickedOk()
{
	EndCurrentWork();
}

void CBarPrgGuid::OnBnClickedCancle()
{
	((CBigGraphView*)m_pWnd)->CancelCurrentWork();
	this->DestroyWindow();
}

void CBarPrgGuid::SetTemplateData(CTemplateData* pTemplateData)
{
	m_pTemplateData = pTemplateData;
}
