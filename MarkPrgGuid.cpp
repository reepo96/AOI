// MarkPrgGuid.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "MarkPrgGuid.h"
#include "BigGraphView.h"

// CMarkPrgGuid 对话框

IMPLEMENT_DYNAMIC(CMarkPrgGuid, CDialog)

CMarkPrgGuid::CMarkPrgGuid(CWnd* pParent /*=NULL*/,int type)
	: CDialog(CMarkPrgGuid::IDD, pParent)
{
	m_pFovData = NULL;	
	m_CurrentWorkStep = MWS_SEARCHAREA;

	m_pCurrentMarkData = NULL;
	m_pWnd = pParent;
	m_pSpellGuid = NULL;
	m_pTemplateData = NULL;
	m_iType = type;	
}

CMarkPrgGuid::~CMarkPrgGuid()
{
}

void CMarkPrgGuid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMarkPrgGuid, CDialog)
	ON_BN_CLICKED(IDC_OK, &CMarkPrgGuid::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CMarkPrgGuid::OnBnClickedCancle)
END_MESSAGE_MAP()


// CMarkPrgGuid 消息处理程序

BOOL CMarkPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_MARK_SEARCHAREA_STEP","请框选出基准点搜索区域");
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

void CMarkPrgGuid::EndCurrentWork(void)
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
	case MWS_SEARCHAREA:
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

			m_pCurrentMarkData = new CDatumMark();
			m_pCurrentMarkData->searchBottom = originRect.bottom;
			m_pCurrentMarkData->searchLeft = originRect.left;
			m_pCurrentMarkData->searchRight = originRect.right;
			m_pCurrentMarkData->searchTop = originRect.top;

			m_pCurrentMarkData->bVisible = true;
			m_pTemplateData->m_bDatumMarkIsChange = true;
			m_pTemplateData->m_bDataIsChanged = true;

			long lId = -1 - m_pTemplateData->m_DatumMarkMap.size();
			while(1)
			{
				map<long,CDatumMark *>::iterator tmpMarkIt = m_pTemplateData->m_DatumMarkMap.find(lId);
				if(tmpMarkIt != m_pTemplateData->m_DatumMarkMap.end())
				{
					lId--;
					continue;
				}
				break;
			}

			m_pCurrentMarkData->m_datumMarkId = lId;
			m_pCurrentMarkData->type = m_iType;
			m_pTemplateData->m_DatumMarkMap.insert(::make_pair(lId,m_pCurrentMarkData));

			m_pCurrentMarkData->fovId = m_pFovData->fovId;
			m_pCurrentMarkData->m_pFovData = m_pFovData;

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
	case MWS_BODY:	
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

			//取得基准点的图像
			((CBigGraphView*)m_pWnd)->GetFactImage(originRect,&(m_pCurrentMarkData->m_ImageObj));
			if(m_pCurrentMarkData->m_ImageObj.IsNull())
				return ;
			
			m_pCurrentMarkData->bottom = originRect.bottom;
			m_pCurrentMarkData->left = originRect.left;
			m_pCurrentMarkData->right = originRect.right;
			m_pCurrentMarkData->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
	
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//不可以用鼠标拖动创建橡皮框
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->NotifyPrgDataChanged();
			((CBigGraphView*)m_pWnd)->Invalidate();

			if(m_pSpellGuid != NULL)
			{
				m_pSpellGuid->ShowWindow(SW_SHOW);
				m_pSpellGuid->MarkPrgComplete(m_pCurrentMarkData);
			}
			
		}
		else
		{
			return;
		}
		
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
		break;
	}
}

void CMarkPrgGuid::NextStep(void)
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
		sStepInfo = g_LoadString("IDS_MARK_BODY_STEP","请框选出基准点主体区域");
		pInfoCtrl->SetWindowTextA(sStepInfo);

		if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
		{
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
		}		
		((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;
		m_CurrentWorkStep = MWS_BODY;

		break;	
	default:
		break;
	}
}

void CMarkPrgGuid::OnBnClickedOk()
{
	EndCurrentWork();	
}

void CMarkPrgGuid::OnBnClickedCancle()
{
	//add at 2008-03-17
	if(m_pCurrentMarkData != NULL)
	{
		long lMarkId = m_pCurrentMarkData->m_datumMarkId;
		map<long,CDatumMark *>::iterator it = m_pTemplateData->m_DatumMarkMap.find(lMarkId);
		if(it != m_pTemplateData->m_DatumMarkMap.end())
		{
			m_pTemplateData->m_DatumMarkMap.erase(it);
		}
		delete m_pCurrentMarkData;
		m_pCurrentMarkData = NULL;
	}

	if(m_pSpellGuid != NULL)
	{
		m_pSpellGuid->OnBnClickedCancle();
	}
	else
	{
		((CBigGraphView*)m_pWnd)->CancelCurrentWork();
	}

	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

void CMarkPrgGuid::SetTemplateData(CTemplateData* pTemplateData)
{
	m_pTemplateData = pTemplateData;
}