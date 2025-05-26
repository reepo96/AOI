// MarkPrgGuid.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "MarkPrgGuid.h"
#include "BigGraphView.h"

// CMarkPrgGuid �Ի���

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


// CMarkPrgGuid ��Ϣ�������

BOOL CMarkPrgGuid::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_MARK_SEARCHAREA_STEP","���ѡ����׼����������");
	pInfoCtrl->SetWindowTextA(sStepInfo);

	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}
	
	((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","��ȷ�����������Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

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
				
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//������������϶�������Ƥ��
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();
			
		}
		else
		{
			return;
		}

		//ִ����һ��
		this->NextStep();		
		break;
	case MWS_BODY:	
		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_BODY","��ȷ�������Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

			//ȡ�û�׼���ͼ��
			((CBigGraphView*)m_pWnd)->GetFactImage(originRect,&(m_pCurrentMarkData->m_ImageObj));
			if(m_pCurrentMarkData->m_ImageObj.IsNull())
				return ;
			
			m_pCurrentMarkData->bottom = originRect.bottom;
			m_pCurrentMarkData->left = originRect.left;
			m_pCurrentMarkData->right = originRect.right;
			m_pCurrentMarkData->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
	
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//������������϶�������Ƥ��
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
		sStepInfo = g_LoadString("IDS_CURENTSTEPSTILLWORK","��ǰ�����Դ��ڱ��״̬���밴ȷ����ť��������״̬�����ܽ�����һ�����");
		AfxMessageBox(sStepInfo);
		return;
	}

	switch(m_CurrentWorkStep)
	{
	case BWS_SEARCHAREA:
		sStepInfo = g_LoadString("IDS_MARK_BODY_STEP","���ѡ����׼����������");
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