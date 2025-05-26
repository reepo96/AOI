// CoordCalc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "CoordCalc.h"
#include "OrganSelectDlg.h"
#include "BigGraphView.h"
#include <math.h>


// CCoordCalc �Ի���

IMPLEMENT_DYNAMIC(CCoordCalc, CDialog)

CCoordCalc::CCoordCalc(bool bDoNext,CWnd* pParent /*=NULL*/)
	: CDialog(CCoordCalc::IDD, pParent)
	, m_sOrganName1(_T(""))
	, m_sOrganName2(_T(""))
	, m_dCADX1(0)
	, m_dCADY1(0)
	, m_dCADY2(0)
	, m_dCADX2(0)
	, m_lCurrentX1(0)
	, m_lCurrentX2(0)
	, m_lCurrentY1(0)
	, m_lCurrentY2(0)
	, m_lRelativeValue(0)
{
	m_CurrentWorkStep = CWS_COORDINATEMAPINI;
	m_pTemplateData = NULL;
	m_pOrgan1 = NULL;
	m_pOrgan2 = NULL;

	m_bDoNext = bDoNext;
	m_pWnd = pParent;
}

CCoordCalc::~CCoordCalc()
{
}

void CCoordCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ORGANNAME1, m_sOrganName1);
	DDX_Text(pDX, IDC_ORGANNAME2, m_sOrganName2);
	DDX_Text(pDX, IDC_CADX, m_dCADX1);
	DDX_Text(pDX, IDC_CADY, m_dCADY1);
	DDX_Text(pDX, IDC_CADY2, m_dCADY2);
	DDX_Text(pDX, IDC_CADX2, m_dCADX2);
	DDX_Text(pDX, IDC_CURRENTX, m_lCurrentX1);
	DDX_Text(pDX, IDC_CURRENTX2, m_lCurrentX2);
	DDX_Text(pDX, IDC_CURRENTY, m_lCurrentY1);
	DDX_Text(pDX, IDC_CURRENTY2, m_lCurrentY2);
	DDX_Text(pDX, IDC_RELATIVEVALUE, m_lRelativeValue);
}


BEGIN_MESSAGE_MAP(CCoordCalc, CDialog)
	ON_BN_CLICKED(IDC_SELCTIORGAN1, &CCoordCalc::OnBnClickedSelctiorgan1)
	ON_BN_CLICKED(IDC_SELCTIORGAN2, &CCoordCalc::OnBnClickedSelctiorgan2)
	ON_BN_CLICKED(IDC_OK, &CCoordCalc::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CCoordCalc::OnBnClickedCancle)
END_MESSAGE_MAP()


// CCoordCalc ��Ϣ�������

void CCoordCalc::OnBnClickedSelctiorgan1()
{
	COrganSelectDlg selectOrgan(m_pTemplateData,&m_pOrgan1,this);
	selectOrgan.DoModal();

	if(m_pOrgan1 != NULL)
	{
		m_sOrganName1 = m_pOrgan1->sName;
		m_dCADX1 = m_pOrgan1->dMidX;
		m_dCADY1 = m_pOrgan1->dMidY;

		this->UpdateData(FALSE);

		if(m_pWnd)
			((CBigGraphView*)m_pWnd)->m_pCoordCalcOrgan1 = m_pOrgan1;
	}
}

void CCoordCalc::OnBnClickedSelctiorgan2()
{
	COrganSelectDlg selectOrgan(m_pTemplateData,&m_pOrgan2,this);
	selectOrgan.DoModal();

	if(m_pOrgan2 != NULL)
	{
		m_sOrganName2 = m_pOrgan2->sName;
		m_dCADX2 = m_pOrgan2->dMidX;
		m_dCADY2 = m_pOrgan2->dMidY;

		this->UpdateData(FALSE);

		if(m_pWnd)
			((CBigGraphView*)m_pWnd)->m_pCoordCalcOrgan2 = m_pOrgan2;
	}
}

void CCoordCalc::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	if(pTemplateData == NULL)
		return;

	if(pTemplateData->m_CoordCalcOrganDataList.size()>=2)
	{
		vector<CCoordCalcOrganData*>::iterator it=	pTemplateData->m_CoordCalcOrganDataList.begin();
		m_CoordCalcOrganRect1 = CRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
		m_pOrgan1 = (*it)->pOrganData;

		it++;
		m_CoordCalcOrganRect2 = CRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
		m_pOrgan2 = (*it)->pOrganData;

		m_sOrganName1 = m_pOrgan1->sName;
		m_dCADX1 = m_pOrgan1->dMidX;
		m_dCADY1 = m_pOrgan1->dMidY;

		m_sOrganName2 = m_pOrgan2->sName;
		m_dCADX2 = m_pOrgan2->dMidX;
		m_dCADY2 = m_pOrgan2->dMidY;

		this->UpdateData(FALSE);
	}
}

// ����Ԫ��1��ǰ��������
void CCoordCalc::UpdateOrgan1(CRect rect)
{
	m_lCurrentX1 = rect.CenterPoint().x;
	m_lCurrentY1 = rect.CenterPoint().y;
	UpdateData(FALSE);
}

// ����Ԫ��2��ǰ��������
void CCoordCalc::UpdateOrgan2(CRect rect)
{
	m_lCurrentX2 = rect.CenterPoint().x;
	m_lCurrentY2 = rect.CenterPoint().y;
	UpdateData(FALSE);
}

void CCoordCalc::OnBnClickedOk()
{
	EndCurrentWork();
}

void CCoordCalc::OnBnClickedCancle()
{
	((CBigGraphView*)m_pWnd)->CancelCurrentWork();
	CDialog::OnCancel();
}

void CCoordCalc::EndCurrentWork(void)
{
	CString sMsgInfo;
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pWnd)->m_lSrcOriginY;

	switch(m_CurrentWorkStep)
	{
	case CWS_COORDINATEMAPINI:
		NextStep();
		break;
	case CWS_COORDINATEMAPSTEP1:
		if(((CBigGraphView*)m_pWnd)->m_pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","��ȷ����Ԫ�������Ѿ���̺�����");
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
			
			m_CoordCalcOrganRect1 = originRect;
			((CBigGraphView*)m_pWnd)->m_CoordCalcOrganRect1 = originRect;
	
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//������������϶�������Ƥ��
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();

			this->UpdateOrgan1(originRect);
			
		}
		else
		{
			return;
		}

		//ִ����һ��
		this->NextStep();		
		break;
	case CWS_COORDINATEMAPSTEP2:	//�����������ֵ�ڶ���
		if(pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","��ȷ����Ԫ�������Ѿ���̺�����");
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
			
			m_CoordCalcOrganRect2 = originRect;
			((CBigGraphView*)m_pWnd)->m_CoordCalcOrganRect2 = originRect;
	
			((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = false;	//������������϶�������Ƥ��
			delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
			((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
			((CBigGraphView*)m_pWnd)->Invalidate();

			this->UpdateOrgan2(originRect);

			//�����������ֵ
			CaclRelativValue();

			CCoordCalcOrganData *pCoordCalcOrganData1 = NULL;
			CCoordCalcOrganData *pCoordCalcOrganData2 = NULL;
			if(m_pTemplateData->m_CoordCalcOrganDataList.size()>=2)
			{
				vector<CCoordCalcOrganData*>::iterator it = m_pTemplateData->m_CoordCalcOrganDataList.begin();
				pCoordCalcOrganData1 = (*it);
				
				it++;
				pCoordCalcOrganData2 = (*it);
			}
			else
			{
				pCoordCalcOrganData1 = new CCoordCalcOrganData();
				pCoordCalcOrganData2 = new CCoordCalcOrganData();
				pCoordCalcOrganData1->templateId=pCoordCalcOrganData2->templateId=m_pTemplateData->m_templateId;

				m_pTemplateData->m_CoordCalcOrganDataList.push_back(pCoordCalcOrganData1);
				m_pTemplateData->m_CoordCalcOrganDataList.push_back(pCoordCalcOrganData2);
			}

			pCoordCalcOrganData1->pOrganData = m_pOrgan1;
			pCoordCalcOrganData2->pOrganData = m_pOrgan2;

			pCoordCalcOrganData1->left = m_CoordCalcOrganRect1.left;
			pCoordCalcOrganData1->right = m_CoordCalcOrganRect1.right;
			pCoordCalcOrganData1->top = m_CoordCalcOrganRect1.top;
			pCoordCalcOrganData1->bottom = m_CoordCalcOrganRect1.bottom;

			pCoordCalcOrganData2->left = m_CoordCalcOrganRect2.left;
			pCoordCalcOrganData2->right = m_CoordCalcOrganRect2.right;
			pCoordCalcOrganData2->top = m_CoordCalcOrganRect2.top;
			pCoordCalcOrganData2->bottom = m_CoordCalcOrganRect2.bottom;

			m_pTemplateData->m_bDataIsChanged = true;
			
		}
		else
		{
			return;
		}

		((CBigGraphView*)m_pWnd)->EndCurrentWork(m_bDoNext);

		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
		
		break;
	}
}

void CCoordCalc::NextStep(void)
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
	case CWS_COORDINATEMAPINI:
		if(false==CoordCalcStep1())
			break;
		sStepInfo = g_LoadString("IDS_COORDINATEMAPSTEPINFO2","������ͼ�Ͽ����һ��Ԫ�����ڣ���ȷ����ť");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		break;
	case CWS_COORDINATEMAPSTEP1:
		if(false==CoordCalcStep2())
			break;
		sStepInfo = g_LoadString("IDS_COORDINATEMAPSTEPINFO3","������ͼ�Ͽ���ڶ���Ԫ�����ڣ���ȷ����ť");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		break;
	default:
		break;
	}
}

// �����������ֵ����1
bool CCoordCalc::CoordCalcStep1(void)
{
	if(m_pOrgan1 == NULL || m_pOrgan2 == NULL)
	{
		CString sInfo = g_LoadString("IDS_MUSTSELECT_ORGAN","����ѡ������Ԫ�������ܼ����������ֵ");
		::AfxMessageBox(sInfo);
		return false;
	}

	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}
	
	((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;	//����һ����׼��ʱ������������϶�������Ƥ��
	m_CurrentWorkStep = CWS_COORDINATEMAPSTEP1;
	return true;
}

bool CCoordCalc::CoordCalcStep2(void)
{
	if( ((CBigGraphView*)m_pWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pWnd)->m_pRectTracker;
		((CBigGraphView*)m_pWnd)->m_pRectTracker = NULL;
	}	

	/*if(m_pProgramGuid == NULL)
	{
		m_pProgramGuid = new CProgramGuid(this);
		m_pProgramGuid->Create(IDD_PRGSTEP,this);
	}*/
	
	((CBigGraphView*)m_pWnd)->m_bCanDrawRectTracker = true;	//����һ����׼��ʱ������������϶�������Ƥ��
	m_CurrentWorkStep = CWS_COORDINATEMAPSTEP2;
	return true;
}

// ��������Ԫ������CAD����ϵ�뱾ϵͳ����ϵ֮������ֵ
void CCoordCalc::CaclRelativValue(void)
{
	double lCoordScaleX = (double)labs( m_CoordCalcOrganRect2.CenterPoint().x - m_CoordCalcOrganRect1.CenterPoint().x ) /fabs(m_pOrgan2->dMidX - m_pOrgan1->dMidX);
	double lCoordScaleY = (double)labs( m_CoordCalcOrganRect2.CenterPoint().y - m_CoordCalcOrganRect1.CenterPoint().y ) /fabs(m_pOrgan2->dMidY - m_pOrgan1->dMidY);
	((CBigGraphView*)m_pWnd)->m_pTemplateData->m_lCoordScaleX = lCoordScaleX;
	((CBigGraphView*)m_pWnd)->m_pTemplateData->m_lCoordScaleY = lCoordScaleY;
	((CBigGraphView*)m_pWnd)->m_pTemplateData->m_lOffsetX = m_CoordCalcOrganRect1.CenterPoint().x - (m_pOrgan1->dMidX)*lCoordScaleX;
	
	//CAD������ԭ�������½�
	((CBigGraphView*)m_pWnd)->m_pTemplateData->m_lOffsetY = (((CBigGraphView*)m_pWnd)->m_pTemplateData->m_ImageObj.GetHeight() - m_CoordCalcOrganRect1.CenterPoint().y) - m_pOrgan1->dMidY*lCoordScaleY;

	((CBigGraphView*)m_pWnd)->m_pTemplateData->m_bDataIsChanged = true;

}

BOOL CCoordCalc::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	CString		sStepInfo;
	CStatic *pInfoCtrl = (CStatic *)this->GetDlgItem(IDC_INFO);
	sStepInfo = g_LoadString("IDS_COORDINATEMAPSTEPINFO1","��ѡ�������Խ�Ԫ����Ȼ��ȷ����ť");
	pInfoCtrl->SetWindowTextA(sStepInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
