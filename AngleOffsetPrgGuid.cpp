// AngleOffsetPrgGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "AngleOffsetPrgGuid.h"
#include "BigGraphView.h"
#include <math.h>

// CAngleOffsetPrgGuid 对话框

IMPLEMENT_DYNAMIC(CAngleOffsetPrgGuid, CDialog)

CAngleOffsetPrgGuid::CAngleOffsetPrgGuid(CWnd* pParent /*=NULL*/)
	: CDialog(CAngleOffsetPrgGuid::IDD, pParent)
{
	m_pTemplateData = NULL;
	m_CurrentWorkStep = PWS_STEP1;
	m_pWnd = pParent;

	m_pFov1 = NULL;
	m_pFov2 = NULL;
}

CAngleOffsetPrgGuid::~CAngleOffsetPrgGuid()
{
}

void CAngleOffsetPrgGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAngleOffsetPrgGuid, CDialog)
	ON_BN_CLICKED(IDC_OK, &CAngleOffsetPrgGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CAngleOffsetPrgGuid::OnBnClickedCancle)
END_MESSAGE_MAP()


// CAngleOffsetPrgGuid 消息处理程序

void CAngleOffsetPrgGuid::OnBnClickedOk()
{
	EndCurrentWork();
}

void CAngleOffsetPrgGuid::OnBnClickedCancle()
{
	((CBigGraphView*)m_pWnd)->CancelCurrentWork();
	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

BOOL CAngleOffsetPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP1","请置FOV到一个参照点上，然后按“确定”按钮确定");
	pInfoCtrl->SetWindowTextA(sStepInfo);

	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}

	if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != PCBIMAGE)
	{
		((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	}

	int iCurrentZoom = ((CBigGraphView*)m_pWnd)->m_iCurrentZoom;
	int iFovWidth = (1+iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
	int iFovHeight = (1+iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);

	if(m_pFov1 != NULL)
	{
		delete m_pFov1;
		m_pFov2 = NULL;
	}
	m_pFov1 = new CFovData();

	CRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + iFovWidth;
	rect.bottom = rect.top + iFovHeight;

	((CBigGraphView*)m_pWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	((CBigGraphView*)m_pWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
	((CBigGraphView*)m_pWnd)->Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAngleOffsetPrgGuid::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;
}

void CAngleOffsetPrgGuid::EndCurrentWork(void)
{
	CString sMsgInfo;
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pWnd)->m_lSrcOriginY;

	switch(m_CurrentWorkStep)
	{	
	case PWS_STEP1:
		if(m_pFov1 == NULL)
			return;

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUERFOVAREA","你确定此FOV区域已经编程好了吗？");
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

			m_pFov1->left = originRect.left;
			m_pFov1->top = originRect.top;
			m_pFov1->right = m_pFov1->left + (FOVWIDTH/PCBMULTIPLE);
			m_pFov1->bottom = m_pFov1->top + (FOVHEIGHT/PCBMULTIPLE);

			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		this->NextStep();
		break;
	case PWS_STEP2:
		if(m_pFov1 == NULL)
			return;

		if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != FOVIMAGE)
		{
			((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov1);
			((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov1->m_ImageObj),FOVIMAGE);
			sMsgInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
			AfxMessageBox(sMsgInfo);
			return;
		}

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","你确定吗？");
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

			m_rect1 = originRect;
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

		this->NextStep();
		break;
	case PWS_STEP3:
		if(m_pFov2 == NULL)
			return;

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUERFOVAREA","你确定此FOV区域已经编程好了吗？");
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

			m_pFov2->left = originRect.left;
			m_pFov2->top = originRect.top;
			m_pFov2->right = m_pFov2->left + (FOVWIDTH/PCBMULTIPLE);
			m_pFov2->bottom = m_pFov2->top + (FOVHEIGHT/PCBMULTIPLE);

			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		this->NextStep();
		break;

	case PWS_STEP4:
		if(m_pFov2 == NULL)
			return;

		if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != FOVIMAGE)
		{
			((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov2);
			((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov2->m_ImageObj),FOVIMAGE);
			sMsgInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
			AfxMessageBox(sMsgInfo);
			return;
		}

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","你确定吗？");
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

			m_rect2 = originRect;
			CaclAngel(1);
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

		this->NextStep();
		break;
	case PWS_STEP5:
		if(m_pFov2 == NULL)
			return;

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUERFOVAREA","你确定此FOV区域已经编程好了吗？");
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

			m_pFov2->left = originRect.left;
			m_pFov2->top = originRect.top;
			m_pFov2->right = m_pFov2->left + (FOVWIDTH/PCBMULTIPLE);
			m_pFov2->bottom = m_pFov2->top + (FOVHEIGHT/PCBMULTIPLE);

			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		this->NextStep();
		break;

	case PWS_STEP6:
		if(m_pFov2 == NULL)
			return;

		if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != FOVIMAGE)
		{
			((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov2);
			((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov2->m_ImageObj),FOVIMAGE);
			sMsgInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
			AfxMessageBox(sMsgInfo);
			return;
		}

		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","你确定吗？");
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

			m_rect2 = originRect;
			CaclAngel(2);
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

		((CBigGraphView*)m_pWnd)->EndCurrentWork();
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
		break;
	}
}

void CAngleOffsetPrgGuid::NextStep(void)
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

	CRect rect;
	int iCurrentZoom = ((CBigGraphView*)m_pWnd)->m_iCurrentZoom;
	int iFovWidth = (1+iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
	int iFovHeight = (1+iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);

	switch(m_CurrentWorkStep)
	{
	case PWS_STEP1:
		if(m_pFov1 == NULL)
			return;

		((CBigGraphView*)m_pWnd)->LoadFovImage(CRect(m_pFov1->left,m_pFov1->top,m_pFov1->right,m_pFov1->bottom),
			&( m_pFov1->m_ImageObj ),false);
		((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov1);
		((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov1->m_ImageObj),FOVIMAGE);

		sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP2","请在此FOV中框选出参照点");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}
		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;
		m_CurrentWorkStep = PWS_STEP2;

		break;
	case PWS_STEP2:
		sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP3","请置FOV到刚才选定参照点在X轴直线上对应参照点上，然后按“确定”按钮确定");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}

		if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != PCBIMAGE)
		{
			((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
		}

		iCurrentZoom = ((CBigGraphView*)m_pWnd)->m_iCurrentZoom;
		iFovWidth = (1+iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
		iFovHeight = (1+iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);

		if(m_pFov2 != NULL)
		{
			delete m_pFov2;
			m_pFov2 = NULL;
		}
		m_pFov2 = new CFovData();

		rect.left = 0;
		rect.top = 0;
		rect.right = rect.left + iFovWidth;
		rect.bottom = rect.top + iFovHeight;

		((CBigGraphView*)m_pWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
		((CBigGraphView*)m_pWnd)->Invalidate();

		m_CurrentWorkStep = PWS_STEP3;

		break;
	case PWS_STEP3:
		if(m_pFov2 == NULL)
			return;

		((CBigGraphView*)m_pWnd)->LoadFovImage(CRect(m_pFov2->left,m_pFov2->top,m_pFov2->right,m_pFov2->bottom),
			&( m_pFov2->m_ImageObj ),false);
		((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov2);
		((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov2->m_ImageObj),FOVIMAGE);

		sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP4","请在此FOV中框选出对应的参照点");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}
		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;
		m_CurrentWorkStep = PWS_STEP4;

		break;
	case PWS_STEP4:
		sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP5","请置FOV到刚才选定参照点在Y轴直线上对应参照点上，然后按“确定”按钮确定");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}

		if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != PCBIMAGE)
		{
			((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
		}

		iCurrentZoom = ((CBigGraphView*)m_pWnd)->m_iCurrentZoom;
		iFovWidth = (1+iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
		iFovHeight = (1+iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);

		if(m_pFov2 != NULL)
		{
			delete m_pFov2;
			m_pFov2 = NULL;
		}
		m_pFov2 = new CFovData();

		rect.left = 0;
		rect.top = 0;
		rect.right = rect.left + iFovWidth;
		rect.bottom = rect.top + iFovHeight;

		((CBigGraphView*)m_pWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
		((CBigGraphView*)m_pWnd)->Invalidate();

		m_CurrentWorkStep = PWS_STEP5;

		break;
	case PWS_STEP5:
		if(m_pFov2 == NULL)
			return;

		((CBigGraphView*)m_pWnd)->LoadFovImage(CRect(m_pFov2->left,m_pFov2->top,m_pFov2->right,m_pFov2->bottom),
			&( m_pFov2->m_ImageObj ),false);
		((CBigGraphView*)m_pWnd)->SetCurrentShowFov(m_pFov2);
		((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pFov2->m_ImageObj),FOVIMAGE);

		sStepInfo = g_LoadString("IDS_ANGLE_OFFSET_STEP6","请在此FOV中框选出对应的参照点");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}
		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;
		m_CurrentWorkStep = PWS_STEP6;

		break;

	default:
		break;
	}
}

void CAngleOffsetPrgGuid::CaclAngel(int type)
{
	if(m_pFov1 == NULL || m_pFov2 == NULL)
		return;

	long x = (m_pFov2->left*PCBMULTIPLE + m_rect2.CenterPoint().x)
		- (m_pFov1->left*PCBMULTIPLE +m_rect1.CenterPoint().x);
	long y = (m_pFov2->top*PCBMULTIPLE + m_rect2.CenterPoint().y)
		- (m_pFov1->top*PCBMULTIPLE +m_rect1.CenterPoint().y);

	if(type == 1)//x轴偏差
	{
		m_pTemplateData->m_dJiaoDuX = ::atan(double(y)/double(x));
		m_pTemplateData->m_bDataIsChanged = true;
	}
	else if(type == 2)//x轴偏差
	{
		m_pTemplateData->m_dJiaoDuY = ::atan(double(x)/double(y));
		m_pTemplateData->m_bDataIsChanged = true;
	}
}