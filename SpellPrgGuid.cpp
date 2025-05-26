// SpellPrgGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SpellPrgGuid.h"
#include "MarkPrgGuid.h"
#include "BigGraphView.h"
#include <math.h>

// CSpellPrgGuid 对话框

IMPLEMENT_DYNAMIC(CSpellPrgGuid, CDialog)

CSpellPrgGuid::CSpellPrgGuid(CWnd* pParent /*=NULL*/)
	: CDialog(CSpellPrgGuid::IDD, pParent)
{
	m_pTemplateData = NULL;
	m_CurrentWorkStep = SWS_SPELLPSTEP1;
	m_pWnd = pParent;

	m_pFov1 = NULL;
	m_pFov2 = NULL;
	m_pSpell = NULL;

}

CSpellPrgGuid::~CSpellPrgGuid()
{
}

void CSpellPrgGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpellPrgGuid, CDialog)
	ON_BN_CLICKED(IDC_OK, &CSpellPrgGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CSpellPrgGuid::OnBnClickedCancle)
END_MESSAGE_MAP()

void CSpellPrgGuid::NewFov(CFovData **ppFov)
{
	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}
	
	if(((CBigGraphView*)m_pWnd)->m_CurrentImageType != PCBIMAGE)
	{
		((CBigGraphView*)m_pWnd)->SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	}

	CRect rect;
	int iCurrentZoom = ((CBigGraphView*)m_pWnd)->m_iCurrentZoom;
	int iFovWidth = (1+iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
	int iFovHeight = (1+iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);

	*ppFov = new CFovData();
	(*ppFov)->type = 1;

	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + iFovWidth;
	rect.bottom = rect.top + iFovHeight;

	((CBigGraphView*)m_pWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	((CBigGraphView*)m_pWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
	((CBigGraphView*)m_pWnd)->Invalidate();
}

// CSpellPrgGuid 消息处理程序

BOOL CSpellPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_SPELLPCB_STEP1","请将FOV放置到拼板上对应原板第一个基准点所在位置，然后按“确定”按钮确定");
	pInfoCtrl->SetWindowTextA(sStepInfo);

	NewFov(&m_pFov1);
	m_pSpell = new CSpellPCB();

	m_CurrentWorkStep = SWS_SPELLPSTEP1;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSpellPrgGuid::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;
}

bool CSpellPrgGuid::GetFovPrgData(CFovData *pFov)
{
	CString			sMsgInfo;
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pWnd)->m_lSrcOriginY;
	long lId;

	if(pFov == NULL)
		return false;

	if(pRectTracker == NULL)
	{
		sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
		AfxMessageBox(sMsgInfo);
		return false;
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

		pFov->left = originRect.left;
		pFov->top = originRect.top;
		pFov->right = pFov->left + (FOVWIDTH/PCBMULTIPLE);
		pFov->bottom = pFov->top + (FOVHEIGHT/PCBMULTIPLE);

		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;	//可以用鼠标拖动创建橡皮框
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		((CBigGraphView*)m_pWnd)->Invalidate();

	}
	else
	{
		return false;
	}

	((CBigGraphView*)m_pWnd)->LoadFovImage(CRect(pFov->left,pFov->top,pFov->right,pFov->bottom),
		&( pFov->m_ImageObj ),true);

	lId = -1 - m_pTemplateData->m_FOVMap.size();
	while(1)
	{
		map<long,CFovData*>::iterator fovTmpIt = m_pTemplateData->m_FOVMap.find(lId);
		if(fovTmpIt !=  m_pTemplateData->m_FOVMap.end())
		{
			lId--;
		}
		else
		{
			break;
		}
	}

	pFov->CheckOrder = 0;
	pFov->fovId = lId;
	m_pTemplateData->m_FOVMap.insert(::make_pair(lId,pFov));
	m_pTemplateData->m_bDataIsChanged = true;
	m_pTemplateData->m_bFOVIsChange = true;

	((CBigGraphView*)m_pWnd)->SetCurrentShowFov(pFov);
	((CBigGraphView*)m_pWnd)->SetCurrentImage( &(pFov->m_ImageObj),FOVIMAGE);
	((CBigGraphView*)m_pWnd)->Invalidate();

	return true;
}

void CSpellPrgGuid::EndCurrentWork(void)
{
	CString sMsgInfo;
	
	CMarkPrgGuid *m_pMarkPrgGuid = NULL;

	switch(m_CurrentWorkStep)
	{	
	case SWS_SPELLPSTEP1:
		if(m_pFov1 == NULL)
			return;

		if(!GetFovPrgData(m_pFov1))
			return;

		m_pMarkPrgGuid = new CMarkPrgGuid(m_pWnd,1);
		m_pMarkPrgGuid->Create(IDD_MARKPRGGUID,this);
		m_pMarkPrgGuid->SetTemplateData(m_pTemplateData);
		this->ShowWindow(SW_HIDE);

		m_pMarkPrgGuid->SetSpellGuid(this);
		m_pMarkPrgGuid->ShowWindow(SW_SHOW);
		//this->NextStep();
		break;
	case SWS_SPELLPSTEP2:
		if(m_pFov2 == NULL)
			return;

		if(!GetFovPrgData(m_pFov2))
			return;

		m_pMarkPrgGuid = new CMarkPrgGuid(m_pWnd,1);
		m_pMarkPrgGuid->Create(IDD_MARKPRGGUID,this);
		m_pMarkPrgGuid->SetTemplateData(m_pTemplateData);
		this->ShowWindow(SW_HIDE);

		m_pMarkPrgGuid->SetSpellGuid(this);
		m_pMarkPrgGuid->ShowWindow(SW_SHOW);
		//this->NextStep();
		break;	
	}
}

void CSpellPrgGuid::MarkPrgComplete(CDatumMark *pMark)
{
	long lId = 0;
	map<int,CSpellPCB*>::iterator tmpSpellIt;

	switch(m_CurrentWorkStep)
	{
		case SWS_SPELLPSTEP1:
			m_pSpell->lDatumMarkId1 = pMark->m_datumMarkId;
			m_pSpell->pDatumMark1 = pMark;
			break;
		case SWS_SPELLPSTEP2:
			m_pSpell->lDatumMarkId2 = pMark->m_datumMarkId;
			m_pSpell->pDatumMark2 = pMark;

			lId = -1 - m_pTemplateData->m_SpellPcbMap.size();
			while(1)
			{
				tmpSpellIt = m_pTemplateData->m_SpellPcbMap.find(lId);
				if(tmpSpellIt != m_pTemplateData->m_SpellPcbMap.end())
				{
					lId--;
					continue;
				}
				break;
			}

			m_pSpell->id = lId;
			m_pSpell->no = m_pTemplateData->m_SpellPcbMap.size() +1;
			m_pSpell->m_templateId = m_pTemplateData->m_templateId;
			m_pTemplateData->m_SpellPcbMap.insert(::make_pair(lId,m_pSpell));

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bFOVIsChange = true;
			m_pTemplateData->m_bDatumMarkIsChange = true;

			((CBigGraphView*)m_pWnd)->NotifyPrgDataChanged();

			break;
	}

	NextStep();
}

void CSpellPrgGuid::NextStep(void)
{
	CString		sStepInfo;
	CString		sMsgInfo;
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
	case SWS_SPELLPSTEP1:
		sStepInfo = g_LoadString("IDS_SPELLPCB_STEP2","请调整FOV到拼板上元件对应位置，然后按“确定”按钮确定");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		NewFov(&m_pFov2);
		m_CurrentWorkStep = SWS_SPELLPSTEP2;

		break;
	case SWS_SPELLPSTEP2:
		sMsgInfo = g_LoadString("IDS_NEEDNEW_SPELLPCB","要编辑新的拼板吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			NewFov(&m_pFov1);
			m_CurrentWorkStep = SWS_SPELLPSTEP1;
			m_pSpell = new CSpellPCB();
			return;
		}

		((CBigGraphView*)m_pWnd)->EndCurrentWork();
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();

		break;	
	default:
		break;
	}
}

void CSpellPrgGuid::OnBnClickedOk()
{
	EndCurrentWork();
}

void CSpellPrgGuid::OnBnClickedCancle()
{
	((CBigGraphView*)m_pWnd)->CancelCurrentWork();
	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}
