// OrganProgramDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganProgramDlg.h"
#include "BigGraphView.h"
#include "FootProgram.h"
#include "NextPadDlg.h"
#include <math.h>

// COrganProgramDlg �Ի���

IMPLEMENT_DYNAMIC(COrganProgramDlg, CDialog)

COrganProgramDlg::COrganProgramDlg(bool bIsSingleOrgan/*=false*/,COrganData* pOrganData/*=NULL*/,CWnd* pParent /*=NULL*/)
	: CDialog(COrganProgramDlg::IDD, pParent)
	, m_sName(_T(""))
	, m_sFootPrint(_T(""))
	, m_dCADX(0)
	, m_dCADY(0)
	, m_sStat(_T(""))
	, m_lCurrentX(0)
	, m_lCurrentY(0)
{
	m_pTemplateData = NULL;
	m_pParrentWnd = pParent;

	m_CurrentWorkStep = OWS_NONE;
	m_pProgramGuid = NULL;
	
	m_iFootNum = 0;
	m_lFootSpaceBetween = 0;

	m_bIsSingleOrgan = bIsSingleOrgan;
	m_pCurrentOrgan = pOrganData;//���������һ��Ԫ��������Ҫ����Ԫ������

	m_bDoWholeOrgan = false;

	m_iCurrentPadIndex = 1;
		
}

COrganProgramDlg::~COrganProgramDlg()
{
}

void COrganProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ORGANNAME, m_sName);
	DDX_Text(pDX, IDC_FOOTPRINT, m_sFootPrint);
	DDX_Text(pDX, IDC_CADX, m_dCADX);
	DDX_Text(pDX, IDC_CADY, m_dCADY);
	DDX_Text(pDX, IDC_STAT, m_sStat);
	DDX_Text(pDX, IDC_CURRENTX, m_lCurrentX);
	DDX_Text(pDX, IDC_CURRENTY3, m_lCurrentY);
	DDX_Control(pDX, IDC_TURN, m_btnTurn);
}


BEGIN_MESSAGE_MAP(COrganProgramDlg, CDialog)
	ON_BN_CLICKED(IDC_PROGRAM, &COrganProgramDlg::OnBnClickedProgram)
	ON_BN_CLICKED(IDC_SKIP, &COrganProgramDlg::OnBnClickedSkip)
	ON_BN_CLICKED(IDC_INVALID, &COrganProgramDlg::OnBnClickedInvalid)
	ON_BN_CLICKED(IDC_NEXT, &COrganProgramDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_CANCLE, &COrganProgramDlg::OnBnClickedCancle)
	ON_BN_CLICKED(IDC_OK, &COrganProgramDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TURN, &COrganProgramDlg::OnBnClickedTurn)
END_MESSAGE_MAP()


// COrganProgramDlg ��Ϣ�������

//������һ��Ԫ��
bool COrganProgramDlg::NextOrganProgram(PrgState state)
{
	m_pCurrentOrgan->PState = state; 
	if(m_bIsSingleOrgan)//��������һ��Ԫ��
		return false;

	m_CurrentOrganMapIt++;	//��һ��Ԫ��
	m_pCurrentOrgan = NULL;
	
	if(NULL ==GetNextOrgan())//ȡ��һ��Ԫ��
	{
		CString sInfo = ::g_LoadString("IDS_ALLORGAN_PRG_END","����Ԫ��������");
		::AfxMessageBox(sInfo);

		if(m_bIsSingleOrgan)
			((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);
		else
			((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(true);

		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();

		return false;
	}

	if( m_bDoWholeOrgan == false)
	{
		OnBnClickedProgram();
	}
	return true;
}

void COrganProgramDlg::OnBnClickedOk()
{
	if( m_pCurrentOrgan == NULL)
	{
		::AfxMessageBox("No Organ");
		return;
	}

	if( m_pCurrentOrgan->GetMidP().x==0 || 
		m_pCurrentOrgan->GetMidP().y==0 )
	{
		CString sInfo =::g_LoadString("IDS_INVALID_PRGDATA","��Ч�ı��");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_bDoWholeOrgan && m_pCurrentOrgan!= NULL)
	{
		GetWholeOrganImage(m_pCurrentOrgan);
	}

	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
	}
	
	m_bDoWholeOrgan = false;
	m_btnTurn.EnableWindow(FALSE);

	((CBigGraphView*)m_pParrentWnd)->NotifyPrgDataChanged();
	((CBigGraphView*)m_pParrentWnd)->Invalidate();

	if(!m_bIsSingleOrgan)
	{
		NextOrganProgram(PS_COMPLETE);
	}
	else
	{
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
	}

}

//��ʼ���
void COrganProgramDlg::OnBnClickedProgram()
{
	if(m_pCurrentOrgan == NULL)
	{
		CString sInfo = g_LoadString("IDS_NOPRGORGAN","��ǰû�пɱ�̵�Ԫ��");
		::AfxMessageBox(sInfo);
		return;
	}
	else if(m_pCurrentOrgan->m_pFovData == NULL && m_pCurrentOrgan->dMidX!=0 && m_pCurrentOrgan->dMidY!=0)
	{
		CString sInfo = g_LoadString("IDS_ORGAN_NOIN_FOV","��ǰԪ�������κ�FOV��");
		::AfxMessageBox(sInfo);
		return;
	}
	else if(m_pCurrentOrgan->dMidX==0 || m_pCurrentOrgan->dMidY==0)
	{
		CString sInfo = g_LoadString("IDS_ORGARNOCADVAL","Ԫ����CAD����ֵΪ0���޷��Զ���λ��Ԫ��λ�ã����Ƿ�Ҫ�ֶ���λ��Ԫ����");
		if(IDYES==::AfxMessageBox(sInfo,MB_YESNO) )
		{
			if(((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType != FOVIMAGE)
			{
				sInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","���л���FOVģʽ��");
				::AfxMessageBox(sInfo);
				return;
			}

			m_bDoWholeOrgan = false;
			m_btnTurn.EnableWindow(FALSE);
			((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

			CString sStepInfo = g_LoadString("IDS_ORGAN_SERAREA_STEP_M","Ԫ����̵�һ�����༭Ԫ��������\n�밴��CTRL����������ѡ��Ԫ����������Ȼ�󰴡�ȷ������ť");
			
			NewWorkStep(sStepInfo,OWS_SERCHAREA,CRect(0,0,0,0));//��ʼ�����������

			((CBigGraphView*)m_pParrentWnd)->m_bCanDrawRectTracker = true;

			return;
		}
		else
		{
			return;
		}
	}

	m_bDoWholeOrgan = false;
	m_btnTurn.EnableWindow(FALSE);

	CString sStepInfo = g_LoadString("IDS_ORGAN_SERAREA_STEP","Ԫ����̵�һ��������Ԫ��������\n���������Ԫ����������Ȼ�󰴡�ȷ������ť");
	
	CRect rect = GetSerchArea( m_pCurrentOrgan );	

	NewWorkStep(sStepInfo,OWS_SERCHAREA,rect);//��ʼ�����������
}

void COrganProgramDlg::OnBnClickedSkip()
{
	NextOrganProgram(PS_SKIP);	
}

void COrganProgramDlg::OnBnClickedInvalid()
{
	NextOrganProgram(PS_INVALID);	
}

void COrganProgramDlg::OnBnClickedNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void COrganProgramDlg::OnBnClickedCancle()
{
	((CBigGraphView*)m_pParrentWnd)->CancelCurrentWork();
	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

void COrganProgramDlg::CancelCurrentWork()
{
	m_CurrentWorkStep = OWS_NONE;
	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}	
}

// ׼�����
void COrganProgramDlg::BeginPrg(void)
{
	if( m_pTemplateData == NULL)
		return;

	m_CurrentFovMapIt = m_pTemplateData->m_FOVMap.begin();
	m_CurrentOrganMapIt = m_pTemplateData->m_OrganDataMap.begin();

	//ȡ��һ��Ԫ��
	if(NULL ==GetNextOrgan())
	{
		CString sInfo = ::g_LoadString("IDS_ALLORGAN_PRG_END","����Ԫ��������");
		::AfxMessageBox(sInfo);
	}
}

//ȡ��Ԫ����FOV����ϵ�е����ĵ�
CPoint COrganProgramDlg::GetOrganInFovCenter(const CFovData* pFovData,const COrganData* pOrganData)
{
	//pcb����ϵ�е�λ��
	long lOrganInPcbX = pOrganData->dMidX * (m_pTemplateData->m_lCoordScaleX) + m_pTemplateData->m_lOffsetX;
	//pcb����ϵ�е�λ��(CAD����ԭ�������½�)
	long lOrganInPcbY = m_pTemplateData->m_ImageObj.GetHeight() - 
		(pOrganData->dMidY * (m_pTemplateData->m_lCoordScaleY) + m_pTemplateData->m_lOffsetY);

	//��FOV�еĵ�
	long lOrganInFovX = lOrganInPcbX - pFovData->left;
	long lOrganInFovY = lOrganInPcbY - pFovData->top;

	//�Ŵ�
	lOrganInFovX = lOrganInFovX * ( double(pFovData->m_ImageObj.GetWidth())/double(pFovData->right - pFovData->left) );
	lOrganInFovY = lOrganInFovY * ( double(pFovData->m_ImageObj.GetHeight())/double(pFovData->bottom - pFovData->top) );

	return CPoint(lOrganInFovX,lOrganInFovY);
}

/* ���Ԫ���Ƿ���FOV��
�ڷ���true,���򷵻�false
*/
bool COrganProgramDlg::OrganInFov(const CFovData* pFovData, const COrganData* pOrganData)
{
	long	lOrganX;//Ԫ������X��
	long	lOrganY;//Ԫ������Y��

	lOrganX = pOrganData->dMidX * m_pTemplateData->m_lCoordScaleX + m_pTemplateData->m_lOffsetX;
	
	//CAD����ԭ���������½�
	lOrganY = m_pTemplateData->m_ImageObj.GetHeight() - (pOrganData->dMidY * m_pTemplateData->m_lCoordScaleY + m_pTemplateData->m_lOffsetY);

	CRect fovRect(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);

	return (bool)fovRect.PtInRect( CPoint(lOrganX,lOrganY) );
}

// ����Ԫ���ͺŲ�����ͬ���Ѿ���̺õ�Ԫ��
COrganData* COrganProgramDlg::SearchSameOrgan(const CString sFootPrint)
{
	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.begin();
	for(;it != m_pTemplateData->m_OrganDataMap.end();it++)
	{
		//��ͬ���ͺţ��Ҵ�Ԫ���Ѿ�������
		if( (*it).second->sFootPrint == sFootPrint && (*it).second->PState == PS_COMPLETE )
		{
			return (*it).second;
		}
	}
	return NULL;
}


// ȡ��һ��Ҫ��̵�Ԫ��
COrganData* COrganProgramDlg::GetNextOrgan(void)
{
	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	double dMultiple = (1+iCurrentZoom*ZOOMSTEP);

	for(;m_CurrentFovMapIt != m_pTemplateData->m_FOVMap.end();m_CurrentFovMapIt++)
	{
		
		for(;m_CurrentOrganMapIt!=m_pTemplateData->m_OrganDataMap.end(); m_CurrentOrganMapIt++)
		{
			//���Ԫ���Ƿ���FOV��
			if( OrganInFov( (*m_CurrentFovMapIt).second,(*m_CurrentOrganMapIt).second ) )
			{
				if( (*m_CurrentFovMapIt).second->m_ImageObj.IsNull())
				{
					CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","���ȼ���FOVͼ��");
					::AfxMessageBox(sInfo);					
					return NULL;
				}

				m_bDoWholeOrgan = false;

				//û�б�̹��Ļ���֮ǰ��Ч�ģ���ȡ��
				if( (*m_CurrentOrganMapIt).second->PState == PS_NEW ||
					(*m_CurrentOrganMapIt).second->PState == PS_INVALID )
				{
					m_sName  = (*m_CurrentOrganMapIt).second->sName;
					m_sFootPrint = (*m_CurrentOrganMapIt).second->sFootPrint;
					m_dCADX = (*m_CurrentOrganMapIt).second->dMidX;
					m_dCADY = (*m_CurrentOrganMapIt).second->dMidY;					

					//ȡ�õ�ǰԪ�����ģ�����뵱ǰFOV������ϵ��						
					CPoint point = GetOrganInFovCenter((*m_CurrentFovMapIt).second ,(*m_CurrentOrganMapIt).second);	
					m_lCurrentX = point.x;
					m_lCurrentY = point.y;

					this->UpdateData(FALSE);

					//�л���FOVͼƬ
					((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov((*m_CurrentFovMapIt).second);
					((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &((*m_CurrentFovMapIt).second->m_ImageObj),FOVIMAGE);

					m_pCurrentOrgan = (*m_CurrentOrganMapIt).second;
					((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

					//���ô�Ԫ��������FOV
					m_pCurrentOrgan->fovId = (*m_CurrentFovMapIt).second->fovId;
					m_pCurrentOrgan->m_pFovData = (*m_CurrentFovMapIt).second;

					//������ͬ�ͺ����Ѿ���̺õ�Ԫ��
					m_btnTurn.EnableWindow(FALSE);
					COrganData* pSameOrgan = SearchSameOrgan((*m_CurrentOrganMapIt).second->sFootPrint);
					if( pSameOrgan != NULL)
					{
						//�����������
						(*m_CurrentOrganMapIt).second->CopyPrgData(pSameOrgan,m_lCurrentX,m_lCurrentY);

						m_bDoWholeOrgan = true;
						m_btnTurn.EnableWindow(TRUE);
						m_CurrentWorkStep = OWS_MAINBODY;//���ΪOWS_FOOT����˺��޸��������Ƥ��ʱ�����޸����ŵ�����

						CString sInfo =::g_LoadString("IDS_FIND_SAME_ORGAN","�ҵ��뵱ǰԪ����ͬ�ͺŵ�Ԫ���������������ı�����ݣ����λ��ƫ�ƣ������Ԫ������λ��Ȼ��ȷ������������Ԫ����̣������Ҫ���µ������밴����̡���ť����������");
						::AfxMessageBox(sInfo);

						//ȡԪ������������
						CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);
						((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//�Ŵ���С����ǰ�ı���

						((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);
						long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
						long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

						//��Ƥ���С�䶯
						/*workRect.left *= dMultiple;
						workRect.right *=dMultiple;
						workRect.top *=dMultiple;
						workRect.bottom *=dMultiple;*/
									
						//�л�����������ϵ
						workRect.left -=lSrcOriginX;
						workRect.right -=lSrcOriginX;
						workRect.top -=lSrcOriginY;
						workRect.bottom -=lSrcOriginY;						
											
						((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
						((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

					}
					else
					{
						//modify at 2008-04-22
						//m_lCurrentX *= dMultiple;
						//m_lCurrentY *= dMultiple;
						((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(m_lCurrentX, m_lCurrentY);
					}
					
					((CBigGraphView*)m_pParrentWnd)->Invalidate();

					return m_pCurrentOrgan;
				}
			}
		}//end for2

		//��ǰFOV������ϣ���Ҫ������һ��FOV��Ԫ����Ҫ��ͷ�ҹ�
		m_CurrentOrganMapIt = m_pTemplateData->m_OrganDataMap.begin();
	}//end for1

	return NULL;
}

// ����Ԫ������������
CRect COrganProgramDlg::GetSerchArea(COrganData* pOrganData)
{
	if( (pOrganData->RangeLeft + pOrganData->RangeRight) >0 &&
		(pOrganData->RangeBottom + pOrganData->RrangeTop) >0 )
	{	
		//�����Ѿ����ڣ����Ʋ�����
		CRect rect(pOrganData->RangeLeft,pOrganData->RrangeTop,pOrganData->RangeRight,pOrganData->RangeBottom);
		//pOrganData->RangeLeft = pOrganData->RangeRight =0;
		//pOrganData->RrangeTop = pOrganData->RangeBottom = 0;
		
		return rect;
	} 
	else
	{
		//���㺸��������Ԫ�����ĵľ���
		long lWidth= FOVMULTIPLE*fabs(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		if( lWidth == 0)
		{
			lWidth = FOVMULTIPLE*fabs(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;
		}
		if( lWidth == 0)
			lWidth = 150;

		lWidth += 15;
		
		CRect rect;
		rect.left = m_lCurrentX -lWidth;
		rect.right = m_lCurrentX +lWidth;
		rect.top = m_lCurrentY -lWidth;
		rect.bottom = m_lCurrentY +lWidth;

		return rect;
	}	
}

// ������ǰ����
void COrganProgramDlg::EndCurrentWork(void)
{
	CString sMsgInfo ;
	
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	if( ((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType != FOVIMAGE )
	{
		sMsgInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","���л���FOVģʽ��");
		AfxMessageBox(sMsgInfo);
		return;
	}

	if(pRectTracker == NULL)
	{
		sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
		AfxMessageBox(sMsgInfo);
		return;
	}

	m_pCurrentOrgan->fovId = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov->fovId;
	m_pCurrentOrgan->m_pFovData = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;

	switch(m_CurrentWorkStep)
	{
	case OWS_SERCHAREA:	
		
		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","��ȷ����Ԫ�����������Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

			m_pCurrentOrgan->RangeBottom = originRect.bottom;
			m_pCurrentOrgan->RangeLeft = originRect.left;
			m_pCurrentOrgan->RangeRight = originRect.right;
			m_pCurrentOrgan->RrangeTop = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;

			//Ԫ��û��CAD����ֵ
			if(m_pCurrentOrgan->dMidX==0 || m_pCurrentOrgan->dMidY==0)
			{
				CFovData *pCurrentShowFov = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;

				m_pCurrentOrgan->fovId = pCurrentShowFov->fovId;
				m_pCurrentOrgan->m_pFovData = pCurrentShowFov;

				m_lCurrentX = originRect.CenterPoint().x;
				m_lCurrentY = originRect.CenterPoint().y;

				long lMidX = originRect.CenterPoint().x/FOVMULTIPLE + pCurrentShowFov->left;
				long lMidY = m_pTemplateData->m_ImageObj.GetHeight() - (originRect.CenterPoint().y/FOVMULTIPLE + pCurrentShowFov->top);

				//����CAD����ֵ
				m_pCurrentOrgan->dMidX = (double)(lMidX - m_pTemplateData->m_lOffsetX)/(double)m_pTemplateData->m_lCoordScaleX;
				m_pCurrentOrgan->dMidY = (double)(lMidY - m_pTemplateData->m_lOffsetY)/(double)m_pTemplateData->m_lCoordScaleY;

				m_pCurrentOrgan->dPadX = m_pCurrentOrgan->dMidX -2.0;
				m_pCurrentOrgan->dPadY= m_pCurrentOrgan->dMidY;
			}
			
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();

		}
		else
		{
			return;
		}
		NextStep();
		break;

	case OWS_MAINBODY:	//
		
		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_BODY","��ȷ����Ԫ�������Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

			//ȡ��Ԫ�������ͼ��
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(originRect,pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}

			vector<CImage*>::iterator bodyIt=m_pCurrentOrgan->m_MainBodyImageList.begin();
			for(;bodyIt!=m_pCurrentOrgan->m_MainBodyImageList.end();bodyIt++)
			{
				CImage* pOldImage = (*bodyIt);
				pOldImage->Destroy();
				pOldImage = NULL;
			}
			m_pCurrentOrgan->m_MainBodyImageList.clear();

			m_pCurrentOrgan->m_MainBodyImageList.push_back(pImage);

			m_pCurrentOrgan->bottom = originRect.bottom;
			m_pCurrentOrgan->left = originRect.left;
			m_pCurrentOrgan->right = originRect.right;
			m_pCurrentOrgan->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bImagChanged = true;
				
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
			
		}
		else
		{
			return;
		}
		NextStep();
		break;
	case OWS_PAD:
		
		sMsgInfo = g_LoadString("IDS_SURE_PAD","��ȷ���˺����Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT
			//ȡ��Ԫ�����̵�ͼ��
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(originRect,pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}

			vector<CImage*>::iterator PadIt=m_pCurrentPad->m_ImageList.begin();
			for(;PadIt!=m_pCurrentPad->m_ImageList.end();PadIt++)
			{
				CImage* pOldImage = (*PadIt);
				pOldImage->Destroy();
				pOldImage = NULL;
			}
			m_pCurrentPad->m_ImageList.clear();

			m_pCurrentPad->m_ImageList.push_back(pImage);

			m_pCurrentPad->bottom = originRect.bottom;
			m_pCurrentPad->left = originRect.left;
			m_pCurrentPad->right = originRect.right;
			m_pCurrentPad->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;			
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bPadChanged = true;
				
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
			
		}
		else
		{
			return;
		}
		NextStep();
		break;
	case OWS_FOOT:
		
		sMsgInfo = g_LoadString("IDS_SURE_FOOT","��ȷ���������Ѿ���̺�����");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
			CRect originRect;

			//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������(����ͼ���봰��(0,0)���غϵĵ�)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

			//ȡ��һ������
			vector<CFootData*>::iterator it = m_pCurrentPad->m_footList.begin();
			if(it != m_pCurrentPad->m_footList.end() )
			{
				(*it)->bottom = originRect.bottom;
				(*it)->left = originRect.left;
				(*it)->right = originRect.right;
				(*it)->top = originRect.top;

				(*it)->bVisible = true;
			}

			//ȡ����ͼ��
			for(; it!=m_pCurrentPad->m_footList.end(); it++)
			{
				CRect rect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
				CImage* pImage=new CImage();
				((CBigGraphView*)m_pParrentWnd)->GetFactImage(rect,pImage);
				if(pImage->IsNull())
				{
					delete pImage;
					pImage = NULL;
					return;
				}

				vector<CImage*>::iterator FootImagIt=(*it)->m_ImageList.begin();
				for(;FootImagIt!=(*it)->m_ImageList.end();FootImagIt++)
				{
					CImage* pOldImage = (*FootImagIt);
					pOldImage->Destroy();
					pOldImage = NULL;
				}
				(*it)->m_ImageList.clear();

				(*it)->m_ImageList.push_back(pImage);
			}

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bPadChanged = true;

			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();

			if( m_WorkStepAfterFoot == OWS_PAD)
			{
				//������һ�����̼�����
				int iSelect=0;
				CNextPadDlg nextPad(&iSelect,this);
				if( IDOK == nextPad.DoModal())
				{
					CPadData *pNewPadData=NULL;
					if(m_iCurrentPadIndex == 4)
					{
						NextOrganProgram(PS_COMPLETE);
						m_iCurrentPadIndex = 1;
						return;
					}
					else
					{
						m_iCurrentPadIndex++;
						if( m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] != NULL)
							delete m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
						m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] = new CPadData();
						pNewPadData = m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
					}
	
					CopyPad(m_pCurrentPad, pNewPadData, iSelect);
					m_pCurrentPad = pNewPadData;
					m_CurrentWorkStep = OWS_MAINBODY;//OWS_PAD;
					((CBigGraphView*)m_pParrentWnd)->Invalidate();
					NextStep();
				} //end if( IDOK == nextPad.DoModal())
			}//end if( m_WorkStepAfterFoot == OWS_PAD)
			else
			{
				m_iCurrentPadIndex = 1;
				if(false==NextOrganProgram(PS_COMPLETE))
				{
					/*if(m_bIsSingleOrgan)
						((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);
					else
						((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);

					this->DestroyWindow();*/
				}
			}
		}
		else
		{
			return;
		}		
		break;
	
	default:
		break;
	}
	
	((CBigGraphView*)m_pParrentWnd)->NotifyPrgDataChanged();
}

// ������һ��
void COrganProgramDlg::NextStep(void)
{
	CRect bodyRect;
	CString sStepInfo;
	CRect rect;
	CFootProgram *pFootPrgDlg=NULL;

	switch(m_CurrentWorkStep)
	{
	case OWS_SERCHAREA:		
		sStepInfo = g_LoadString("IDS_ORGAN_BODY_STEP","Ԫ����̵ڶ���������Ԫ��������\n�����Ԫ����������Ȼ�󰴡�ȷ������ť");
	
		bodyRect = GetBodyArea(m_pCurrentOrgan);
		NewWorkStep(sStepInfo,OWS_MAINBODY,bodyRect);//��ʼ�����������
		break;
	case OWS_MAINBODY:		
		sStepInfo = g_LoadString("IDS_ORGAN_PAD_STEP","Ԫ����̵�����������Ԫ��������\n�����Ԫ����������Ȼ�󰴡�ȷ������ť");
	
		if(m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1]==NULL)
		{
			m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] = new CPadData();
		}
		m_pCurrentPad = m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
		//m_iCurrentPadIndex = 1;	//��һ����

		rect = GetPadRect( m_pCurrentOrgan,m_pCurrentPad);
		NewWorkStep(sStepInfo,OWS_PAD,rect);//��ʼ�����������
		break;
	case OWS_PAD:		
		sStepInfo = g_LoadString("IDS_ORGAN_FOOT_STEP","Ԫ����̵��Ĳ�������Ԫ������\n�����Ԫ�����ţ�Ȼ�󰴡�ȷ������ť");
	
		//��һ��������Ƥ��
		if( m_pCurrentPad != NULL)
		{
			vector<CFootData*>::iterator footIt = m_pCurrentPad->m_footList.begin();
			if(footIt != m_pCurrentPad->m_footList.end())
			{
				CRect rc((*footIt)->left,(*footIt)->top,(*footIt)->right,(*footIt)->bottom);
				(*footIt)->bVisible = false;

				((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(rc);//�Ŵ󵽵�ǰ�ı���	
	
				//�������ĵ��ԭ��
				((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(rc.left + rc.Width()/2, rc.top + rc.Height()/2);	
				long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
				long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

				CRect trackerRect(rc.left-lSrcOriginX,rc.top-lSrcOriginY,rc.right-lSrcOriginX,rc.bottom-lSrcOriginY);
				
				((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&trackerRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
				((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
			}
			//ˢ����Ļ	
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
		}

		if(m_pProgramGuid != NULL && m_pProgramGuid->m_hWnd && IsWindow(m_pProgramGuid->m_hWnd))
		{
			m_pProgramGuid->DestroyWindow();
			delete m_pProgramGuid;
			m_pProgramGuid = NULL;
		}

		pFootPrgDlg = new CFootProgram(this);
		pFootPrgDlg->Create(IDD_FOOTPRG,this);
		pFootPrgDlg ->ShowWindow(SW_SHOW);

		m_CurrentWorkStep = OWS_FOOT;
		break;
	case OWS_FOOT:		
		sStepInfo = g_LoadString("IDS_ORGAN_PAD_STEP","Ԫ����̵�����������Ԫ��������\n�����Ԫ����������Ȼ�󰴡�ȷ������ť");
	
		rect = CRect(m_pCurrentPad->left,m_pCurrentPad->top,m_pCurrentPad->right,m_pCurrentPad->bottom);
		NewWorkStep(sStepInfo,OWS_PAD,rect);//��ʼ�����������
		break;
	}
}

// ������������
CRect COrganProgramDlg::GetBodyArea(COrganData* pOrganData)
{
	if( (pOrganData->left + pOrganData->right) >0 &&
		(pOrganData->bottom + pOrganData->top) >0 )
	{	
		//�����Ѿ����ڣ����Ʋ�����
		CRect rect(pOrganData->left,pOrganData->top,pOrganData->right,pOrganData->bottom);
		pOrganData->left = pOrganData->right =0;
		pOrganData->top = pOrganData->bottom = 0;

		return rect;
	}
	else
	{
		//���㺸��������Ԫ�����ĵľ���
		long lWidth= FOVMULTIPLE*fabs(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		if( lWidth == 0)
		{
			lWidth = FOVMULTIPLE*fabs(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;
		}
		if( lWidth == 0)
			lWidth = 150;

		lWidth -= 10;
		CRect rect;
		rect.left = ((m_lCurrentX -lWidth) > pOrganData->RangeLeft)?(m_lCurrentX -lWidth):(pOrganData->RangeLeft+10);
		rect.right = ((m_lCurrentX +lWidth) < pOrganData->RangeRight)?(m_lCurrentX +lWidth):(pOrganData->RangeRight-10);
		rect.top = ((m_lCurrentY -lWidth) > pOrganData->RrangeTop)?(m_lCurrentY -lWidth):(pOrganData->RrangeTop+10);
		rect.bottom = ((m_lCurrentY +lWidth) < pOrganData->RangeBottom)?(m_lCurrentY +lWidth):(pOrganData->RangeBottom -10);

		return rect;
	}
}

// �µĹ�������
void COrganProgramDlg::NewWorkStep(CString sShowInfo, OrganWorkStep step, CRect workRect)
{
	if(m_pProgramGuid == NULL || m_pProgramGuid->m_hWnd==NULL || !IsWindow(m_pProgramGuid->m_hWnd))
	{
		m_pProgramGuid = new COrganPrgGuid(this);
		m_pProgramGuid->Create(IDD_ORGANPRGSTEP,this);
	}		
	
	CStatic *pInfoCtrl = (CStatic *)m_pProgramGuid->GetDlgItem(IDC_INFO);
	pInfoCtrl->SetWindowTextA(sShowInfo);
	m_pProgramGuid->ShowWindow(SW_SHOW);

	m_CurrentWorkStep = step;

	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
	}	

	((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//�Ŵ󵽵�ǰ�ı���

	//�������ĵ��ԭ��
	((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
	long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;
				
	//�л�����������ϵ
	workRect.left -=lSrcOriginX;
	workRect.right -=lSrcOriginX;
	workRect.top -=lSrcOriginY;
	workRect.bottom -=lSrcOriginY;
		
	((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;

	//ˢ����Ļ	
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

// ȡ�ú���Rect
CRect COrganProgramDlg::GetPadRect(COrganData* pOrganData,CPadData *pPadData)
{
	//CPadData *pPadData=pOrganData->m_pPadData[0];
	
	if( pPadData!=NULL && (pPadData->left + pPadData->right) >0 &&
		(pPadData->bottom + pPadData->top) >0 )
	{	
		//�����Ѿ����ڣ����Ʋ�����
		CRect rect(pPadData->left,pPadData->top,pPadData->right,pPadData->bottom);
		pPadData->left = pPadData->right =0;
		pPadData->top = pPadData->bottom = 0;

		return rect;
	}
	else
	{
		//���㺸��������Ԫ�����ĵľ���
		long lPadMidX= m_lCurrentX + FOVMULTIPLE*(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		long lPadMidY= m_lCurrentY + FOVMULTIPLE*(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;

		long lWidth = labs( labs(lPadMidX - m_lCurrentX) - (pOrganData->right - pOrganData->left)/2 );
		long lHeight = labs( labs(lPadMidY - m_lCurrentY) - (pOrganData->bottom - pOrganData->top)/2 );
		
		CRect rect;
		rect.left = lPadMidX -lWidth;
		rect.right = lPadMidX +lWidth;
		rect.top = lPadMidY -lHeight;
		rect.bottom = lPadMidY +lHeight;

		return rect;
	}
}

// ���Ӻ�����
void COrganProgramDlg::AddFootNum(int iNum)
{
	m_pCurrentPad->AddFootNum(iNum);
	
	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	//ȡ��һ������������Ƥ��
	vector<CFootData*>::iterator it=m_pCurrentPad->m_footList.begin();
	if(it != m_pCurrentPad->m_footList.end())
	{
		CRect rect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);

		//��Ҫ����(*it)->left=(*it)->top=(*it)->right=(*it)->bottom=0;
		(*it)->bVisible = false;	//������Ϊ������
		
		((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(rect);//�Ŵ󵽵�ǰ����
		
		//�л�����������ϵ
		rect.left -=lSrcOriginX;
		rect.right -=lSrcOriginX;
		rect.top -=lSrcOriginY;
		rect.bottom -=lSrcOriginY;

		if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
		{
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker=NULL;
		}

		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);		
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
	
}

/*
*���Ӻ��ż���
*lSpace:���
*/
void COrganProgramDlg::AddFootSpaceBetween(long lSpace)
{
	m_pCurrentPad->AddFootSpaceBetween(lSpace);
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

/* ��Դ���̿���������ݵ�Ŀ�ĺ���
*srcPad��Դ
*destPad��Ŀ��
*iRelative��Ŀ����Դ�Ĺ�ϵ��1��Ŀ����Դ�Ķ��棬2������ߣ�3�����ұ�
**/
void COrganProgramDlg::CopyPad(CPadData * srcPad, CPadData * destPad, int iRelative)
{
	long	lMoveStep=0;
	long lDestPadMidX = 0;
	long lDestPadMidY = 0;
	long lDestFootMidX=0;
	long lDestFootMidY=0;
	
	switch(iRelative)
	{
	case 1://����
		if( srcPad->GetHeight() >= srcPad->GetWidth() )
		{
			destPad->CopyRect(srcPad);//����Դ��rect
			lMoveStep = 2*(m_pCurrentOrgan->GetMidP().x - srcPad->GetMidP().x);
			destPad->MoveX(lMoveStep);	//��X��ƽ��

			//��������
			vector<CFootData*>::iterator it = srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				CFootData *pFootData = new CFootData();
				pFootData->CopyRect( (*it) );//����Դ��rect
				pFootData->MoveX(lMoveStep);//��X��ƽ��

				destPad->m_footList.push_back(pFootData);
			}			
		}
		else
		{
			destPad->CopyRect(srcPad);//����Դ��rect
			lMoveStep = 2*(m_pCurrentOrgan->GetMidP().y - srcPad->GetMidP().y);
			destPad->MoveY(lMoveStep);	//��Y��ƽ��

			//��������
			vector<CFootData*>::iterator it = srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				CFootData *pFootData = new CFootData();
				pFootData->CopyRect( (*it) );//����Դ��rect
				pFootData->MoveY(lMoveStep);//��X��ƽ��

				destPad->m_footList.push_back(pFootData);
			}
		}
		break;
	case 2:
	case 3:
		if( srcPad->GetHeight() >= srcPad->GetWidth() ) //Դ������Ԫ����������
		{
			lDestPadMidX = m_pCurrentOrgan->GetMidP().x;
			
			//������Ԫ���ϱ�
			if( (iRelative == 2 && (srcPad->GetMidP().x < m_pCurrentOrgan->GetMidP().x) ) ||
				(iRelative == 3 && (srcPad->GetMidP().x > m_pCurrentOrgan->GetMidP().x) ) )
			{
				lDestPadMidY = m_pCurrentOrgan->GetMidP().y - m_pCurrentOrgan->GetHeight()/2 -srcPad->GetWidth()/2;
			}
			else //������Ԫ���±�
			{
				lDestPadMidY = m_pCurrentOrgan->GetMidP().y + m_pCurrentOrgan->GetHeight()/2 +srcPad->GetWidth()/2;
			}

			destPad->CopyPrgData(srcPad,lDestPadMidX,lDestPadMidY); //��������
			destPad->Turn();	//��ת

			//��������
			/*lDestFootMidY = lDestPadMidY;

			vector<CFootData*>::iterator it =srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				lDestFootMidX = lDestPadMidX - (srcPad->GetMidP().y - (*it)->GetMidP().y);
				CFootData *pFootData = new CFootData();
				pFootData->CopyPrgData( (*it) ,lDestFootMidX,lDestFootMidY); //����
				pFootData->Turn(); //��ת
				destPad->m_footList.push_back(pFootData);
			}			*/
		}
		else //Դ������Ԫ����������
		{
			lDestPadMidY = m_pCurrentOrgan->GetMidP().y;
			
			//������Ԫ���ұ�
			if( (iRelative == 2 && (srcPad->GetMidP().y < m_pCurrentOrgan->GetMidP().y) ) ||
				(iRelative == 3 && (srcPad->GetMidP().y > m_pCurrentOrgan->GetMidP().y) ) )
			{
				lDestPadMidX = m_pCurrentOrgan->GetMidP().x + m_pCurrentOrgan->GetWidth()/2 +srcPad->GetHeight()/2;
			}
			else //������Ԫ�����
			{
				lDestPadMidX = m_pCurrentOrgan->GetMidP().x - m_pCurrentOrgan->GetWidth()/2 -srcPad->GetHeight()/2;
			}

			destPad->CopyPrgData(srcPad,lDestPadMidX,lDestPadMidY); //��������
			destPad->Turn();	//��ת

			//��������
			/*lDestFootMidX = lDestPadMidX;

			vector<CFootData*>::iterator it =srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				lDestFootMidY = lDestPadMidY - (srcPad->GetMidP().x - (*it)->GetMidP().x);
				CFootData *pFootData = new CFootData();
				pFootData->CopyPrgData( (*it) ,lDestFootMidX,lDestFootMidY); //����
				pFootData->Turn(); //��ת
				destPad->m_footList.push_back(pFootData);
			}*/
		}
		break;
	default:
		break;
	}

	//ȡ��Ԫ�����̵�ͼ��
	CImage* pImage=new CImage();
	((CBigGraphView*)m_pParrentWnd)->GetFactImage(CRect(destPad->left,destPad->top,destPad->right,destPad->bottom),pImage);
	if(pImage->IsNull())
	{
		delete pImage;
		pImage = NULL;
		return;
	}
	destPad->m_ImageList.push_back(pImage);

	//��һ��������Ƥ��
	/*long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	vector<CFootData*>::iterator footIt = destPad->m_footList.begin();
	if(footIt != destPad->m_footList.end())
	{
		CRect rc((*footIt)->left-lSrcOriginX,(*footIt)->top-lSrcOriginY,
			(*footIt)->right-lSrcOriginX,(*footIt)->bottom-lSrcOriginY);
		(*footIt)->bVisible = false;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&rc,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
	}*/
	//ˢ����Ļ	
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}


BOOL COrganProgramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);
	
	this->SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void COrganProgramDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}

//��һԪ�����
void COrganProgramDlg::BeginSinglePrg(void)
{
	if(m_pCurrentOrgan == NULL)
		return;

	m_sName  = m_pCurrentOrgan->sName;
	m_sFootPrint = m_pCurrentOrgan->sFootPrint;
	m_dCADX = m_pCurrentOrgan->dMidX;
	m_dCADY = m_pCurrentOrgan->dMidY;

	this->UpdateData(FALSE);

	CFovData*	pBelongFovData = NULL;	//Ԫ��������FOV
	if(m_pCurrentOrgan->m_pFovData != NULL)
	{
		pBelongFovData = m_pCurrentOrgan->m_pFovData;
	}
	else
	{
		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
		for(;it != m_pTemplateData->m_FOVMap.end();it++)
		{
			//���Ԫ���Ƿ���FOV��
			if( OrganInFov( (*it).second,m_pCurrentOrgan ) )
			{
				pBelongFovData = (*it).second;
				break;
			}
		}
	}

	if( pBelongFovData == NULL)
		return;
	
	if(pBelongFovData->m_ImageObj.IsNull())
	{
		CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","���ȼ���FOVͼ��");
		::AfxMessageBox(sInfo);
		return;
	}

	//���ô�Ԫ��������FOV
	m_pCurrentOrgan->fovId = pBelongFovData->fovId;
	m_pCurrentOrgan->m_pFovData = pBelongFovData;

	//ȡ�õ�ǰԪ�����ģ�����뵱ǰFOV������ϵ��						
	CPoint point = GetOrganInFovCenter(pBelongFovData ,m_pCurrentOrgan);	
	m_lCurrentX = point.x;
	m_lCurrentY = point.y;

	this->UpdateData(FALSE);

	//�л���FOVͼƬ
	((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov(pBelongFovData);
	((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &(pBelongFovData->m_ImageObj),FOVIMAGE);			

	((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	double dMultiple = (1+iCurrentZoom*ZOOMSTEP);
	
	//modify at 2008-04-22
	//((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_pCurrentOrgan->GetMidP().x*(1+iCurrentZoom*ZOOMSTEP);
	//((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_pCurrentOrgan->GetMidP().y*(1+iCurrentZoom*ZOOMSTEP);
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_lCurrentX*dMultiple;
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_lCurrentY*dMultiple;
		
	
	if(m_pCurrentOrgan != NULL)
	{
		//������ͬ�ͺ����Ѿ���̺õ�Ԫ��		
		COrganData* pSameOrgan = SearchSameOrgan(m_pCurrentOrgan->sFootPrint);
		if( pSameOrgan != NULL)
		{
			//�����������
			m_pCurrentOrgan->CopyPrgData(pSameOrgan,m_lCurrentX,m_lCurrentY);

			m_bDoWholeOrgan = true;
			m_btnTurn.EnableWindow(TRUE);
			m_CurrentWorkStep = OWS_MAINBODY;//���ΪOWS_FOOT����˺��޸��������Ƥ��ʱ�����޸����ŵ�����

			CString sInfo =::g_LoadString("IDS_FIND_SAME_ORGAN","�ҵ��뵱ǰԪ����ͬ�ͺŵ�Ԫ���������������ı�����ݣ����λ��ƫ�ƣ������Ԫ������λ��Ȼ��ȷ������������Ԫ����̣������Ҫ���µ������밴����̡���ť����������");
			::AfxMessageBox(sInfo);

			//ȡԪ������������
			CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

			((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//�Ŵ���С����ǰ�ı���

			((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
			long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
			long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

			//��Ƥ���С�䶯
			/*workRect.left *= dMultiple;
			workRect.right *=dMultiple;
			workRect.top *=dMultiple;
			workRect.bottom *=dMultiple;*/

			//�л�����������ϵ
			workRect.left -=lSrcOriginX;
			workRect.right -=lSrcOriginX;
			workRect.top -=lSrcOriginY;
			workRect.bottom -=lSrcOriginY;						

			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
		}
		else
		{
			//modify at 2008-04-22
			//m_lCurrentX *= dMultiple;
			//m_lCurrentY *= dMultiple;
			//((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(m_lCurrentX, m_lCurrentY);

			OnBnClickedProgram();
		}
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

// ��Ƥ���Ѿ��ı�
void COrganProgramDlg::NotifyTrakerChanged(void)
{
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;

	if( pRectTracker == NULL)
		return;

	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;
	
	CRect CurrentZoomRect;//��ǰ�Ŵ����µ�RECT
	CRect originRect;	
	long lMidOffsetX=0;
	long lMidOffsetY=0;
	
	CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
	CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
	CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
	CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;
		
	((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//ȡԭʼ��С��RECT

	if(m_pCurrentOrgan != NULL && m_pParrentWnd != NULL && 
		((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType == FOVIMAGE)
	{
		m_pCurrentOrgan->fovId = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov->fovId;
		m_pCurrentOrgan->m_pFovData = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}

	if(m_CurrentWorkStep == OWS_FOOT && pRectTracker != NULL)
	{
		//��Ƥ��ı䣬��������Ҳ���Ÿı�		
		CDataObj dataObj;
		dataObj.left = originRect.left;
		dataObj.right = originRect.right;
		dataObj.top = originRect.top;
		dataObj.bottom = originRect.bottom;
		
		vector<CFootData*>::iterator it =m_pCurrentPad->m_footList.begin();
		
		//ȡ���ĵ��ƫ�ƣ���Ƥ������һ���ţ�		
		if(it != m_pCurrentPad->m_footList.end())
		{
			CRect oldRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
			if( oldRect == originRect)
				return;
				
			lMidOffsetX = originRect.CenterPoint().x - (*it)->GetMidP().x;
			lMidOffsetY = originRect.CenterPoint().y - (*it)->GetMidP().y;
		}
		else
		{
			return;
		}
		
		for(;it != m_pCurrentPad->m_footList.end();it++)
		{
			//�Ŵ��뵱ǰ��Ƥ��һ����
			(*it)->CopyPrgData(&dataObj,(*it)->GetMidP().x,(*it)->GetMidP().y);	
			
			//ƽ��
			(*it)->MoveX(lMidOffsetX);
			(*it)->MoveY(lMidOffsetY);			
		}
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
	else if(m_bDoWholeOrgan == true)//Ԫ���������ƶ�
	{
		CRect oldBodyRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,
			m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

		lMidOffsetX = originRect.CenterPoint().x - oldBodyRect.CenterPoint().x;
		lMidOffsetY = originRect.CenterPoint().y - oldBodyRect.CenterPoint().y;

		//��������
		m_pCurrentOrgan->RangeLeft += lMidOffsetX;
		m_pCurrentOrgan->RrangeTop += lMidOffsetY;
		m_pCurrentOrgan->RangeRight += lMidOffsetX;
		m_pCurrentOrgan->RangeBottom += lMidOffsetY;

		//����
		m_pCurrentOrgan->left = originRect.left;
		m_pCurrentOrgan->top = originRect.top;
		m_pCurrentOrgan->right = originRect.right;
		m_pCurrentOrgan->bottom = originRect.bottom;

		//����
		for(int i=0;i<4;i++)
		{
			if( m_pCurrentOrgan->m_pPadData[i] != NULL)
			{
				m_pCurrentOrgan->m_pPadData[i]->MoveX(lMidOffsetX);
				m_pCurrentOrgan->m_pPadData[i]->MoveY(lMidOffsetY);

				//����
				/*vector<CFootData*>::iterator it =m_pCurrentOrgan->m_pPadData[i]->m_footList.begin();
				for(;it != m_pCurrentOrgan->m_pPadData[i]->m_footList.end();it++)
				{
					(*it)->MoveX(lMidOffsetX);
					(*it)->MoveY(lMidOffsetY);
				}*/
			}//end if
		}//end for

		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
}

// ȡ������Ԫ���ı��ͼ��
void COrganProgramDlg::GetWholeOrganImage(COrganData* pOrgan)
{
	if(pOrgan == NULL)
		return;

	m_pTemplateData->m_bDataIsChanged = true;
	m_pTemplateData->m_bOrganDataIsChange = true;
	pOrgan->bDataChanged = true;
	pOrgan->bImagChanged = true;
	pOrgan->bPadChanged = true;
	pOrgan->PState = PS_COMPLETE;

	//ȡ��Ԫ�������ͼ��
	CImage* pImage=new CImage();
	((CBigGraphView*)m_pParrentWnd)->GetFactImage(pOrgan->getRect(),pImage);
	if(pImage->IsNull())
	{
		delete pImage;
		pImage = NULL;
		return;
	}

	m_pCurrentOrgan->m_MainBodyImageList.push_back(pImage);

	//ȡ��Ԫ�����̵�ͼ��
	for(int i=0;i<4;i++)
	{
		if(pOrgan->m_pPadData[i] != NULL)
		{
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(pOrgan->m_pPadData[i]->getRect(),pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}
			pOrgan->m_pPadData[i]->m_ImageList.push_back(pImage);

			//ȡ����ͼ��
			vector<CFootData*>::iterator it = pOrgan->m_pPadData[i]->m_footList.begin();
			for(; it!=pOrgan->m_pPadData[i]->m_footList.end(); it++)
			{
				CImage* pImage=new CImage();
				((CBigGraphView*)m_pParrentWnd)->GetFactImage((*it)->getRect(),pImage);
				if(pImage->IsNull())
				{
					delete pImage;
					pImage = NULL;
					return;
				}
				(*it)->m_ImageList.push_back(pImage);
			}
		}
	}	
}

void COrganProgramDlg::ModiyPrgData(void)
{
	if(m_pCurrentOrgan == NULL)
		return;

	CFovData*	pBelongFovData = m_pCurrentOrgan->m_pFovData;	//Ԫ��������FOV
	if( pBelongFovData == NULL)
	{
		return;
	}
		
	if(pBelongFovData->m_ImageObj.IsNull())
	{
		CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","���ȼ���FOVͼ��");
		::AfxMessageBox(sInfo);
		return;
	}

	m_sName  = m_pCurrentOrgan->sName;
	m_sFootPrint = m_pCurrentOrgan->sFootPrint;
	m_dCADX = m_pCurrentOrgan->dMidX;
	m_dCADY = m_pCurrentOrgan->dMidY;
	
	//ȡ�õ�ǰԪ�����ģ�����뵱ǰFOV������ϵ��						
	CPoint point = GetOrganInFovCenter(pBelongFovData ,m_pCurrentOrgan);	
	m_lCurrentX = point.x;
	m_lCurrentY = point.y;

	this->UpdateData(FALSE);

	//�л���FOVͼƬ
	((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov(pBelongFovData);
	((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &(pBelongFovData->m_ImageObj),FOVIMAGE);			

	((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_pCurrentOrgan->GetMidP().x*(1+iCurrentZoom*ZOOMSTEP);
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_pCurrentOrgan->GetMidP().y*(1+iCurrentZoom*ZOOMSTEP);			
		
	
	if(m_pCurrentOrgan != NULL)
	{
		OnBnClickedProgram();
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

void COrganProgramDlg::OnBnClickedTurn()
{
	if(m_pCurrentOrgan != NULL)
	{
		m_pCurrentOrgan->Turn();
		
		//���»���Ƥ��
		if(((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
		{
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
		}

		//ȡԪ������������
		CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

		((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//�Ŵ󵽵�ǰ�ı���

		((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
		long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
		long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

		//�л�����������ϵ
		workRect.left -=lSrcOriginX;
		workRect.right -=lSrcOriginX;
		workRect.top -=lSrcOriginY;
		workRect.bottom -=lSrcOriginY;						

		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
}
