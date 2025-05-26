// DetectBigGraphView.cpp : ʵ���ļ�
#include "stdafx.h"
#include "AOI.h"
#include "DetectBigGraphView.h"
#include "ValidateErr.h"
#include "DetectMainView.h"
#include "OrganSelectDlg.h"
#include "Motor.h"
#include "Camera.h"
#include "PCBState.h"
#include <algorithm>
using namespace std;

#include <time.h>
#include <stdlib.h>
#include <math.h>

extern CCamera     g_camera;	//ȫ������ͷ����
extern CPCBState   g_PCBState;	//ȫ�ְ�״̬
extern CMotor      g_Motor;		//ȫ�ֵ��

#define WM_NOTIFYDATACHANGE WM_USER+100

#define WM_SHOW_IMAGE WM_USER+101

DWORD WINAPI WorkThread(LPVOID lpParam);

DWORD WINAPI ManualWorkThread(LPVOID lpParam);

DWORD WINAPI RefreshView(LPVOID lpParam);

// ��������ĺ�������(��С��������)
bool Lesser(CFovData* elem1, CFovData* elem2)
{
	return elem1->CheckOrder < elem2->CheckOrder;
}

IMPLEMENT_DYNCREATE(CDetectBigGraphView, CBaseBigGraphView)

CDetectBigGraphView::CDetectBigGraphView()
	: CBaseBigGraphView(CDetectBigGraphView::IDD)
{
	m_iCurrentZoom = 0 ;
	m_lCurrentCenterX = 0;
	m_lCurrentCenterY = 0;
	m_lSrcOriginX = 0;
	m_lSrcOriginY = 0;
	m_CurrentRect =CRect(0,0,0,0);
	m_CurrentTImgRect = CRect(0,0,0,0);
	m_pCurrentImage = NULL;

	m_DectType = T_MANUAL;
	m_pCurrentTTemplateImg = NULL;
	m_pCurrentTImg = new CImage();

	m_iCurrentPCBOffsetx = 0;
	m_iCurrentPCBOffsety = 0;
	m_dThiDa = 0.00;
	m_iOrganOffsetx = 0;
	m_iOrganOffsety = 0;

	m_bReady2Organ = false;
	m_pDetectMasterData = NULL;
	m_hManualThread = NULL;
	m_bAskWhenErr = TRUE;
	m_pCurrentDatailData = NULL;

	m_bIsUseFovOrderList = false;
	m_preMousePoint = CPoint(0,0);
}

CDetectBigGraphView::~CDetectBigGraphView()
{
	if( m_pCurrentTImg != NULL)
	{
		if(!m_pCurrentTImg->IsNull())
		{
			m_pCurrentTImg->Destroy();
		}
		delete m_pCurrentTImg;
		m_pCurrentTImg = NULL;
	}

	map<CString,CDetectStatInfo*>::iterator it = m_DStatInfoMap.begin();
	for(;it != m_DStatInfoMap.end();it++)
	{
		CDetectStatInfo *pStatInfo = (*it).second;
		delete pStatInfo;
		pStatInfo = NULL;
	}
	m_DStatInfoMap.clear();
}

void CDetectBigGraphView::DoDataExchange(CDataExchange* pDX)
{
	CBaseBigGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectBigGraphView, CBaseBigGraphView)
	ON_MESSAGE(WM_NOTIFYDATACHANGE,NotifyDetectDataChanged)
	ON_MESSAGE(WM_SHOW_IMAGE,ShowImage)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDetectBigGraphView ���

#ifdef _DEBUG
void CDetectBigGraphView::AssertValid() const
{
	CBaseBigGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetectBigGraphView::Dump(CDumpContext& dc) const
{
	CBaseBigGraphView::Dump(dc);
}
#endif
#endif //_DEBUG

// ��FOV mapת��list����С��������list
void CDetectBigGraphView::GetFovListANdSort(void)
{
	if(m_bIsUseFovOrderList)
		return;

	m_FOVOrderList.clear();

	//����
	EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	map<long,CFovData*>::iterator mapIt=m_pTemplateData->m_FOVMap.begin();
	for(;mapIt != m_pTemplateData->m_FOVMap.end();mapIt++)
	{
		m_FOVOrderList.push_back( (*mapIt).second );
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	if(m_FOVOrderList.size()<=1)
		return;

	//����
	sort(m_FOVOrderList.begin(),m_FOVOrderList.end(),Lesser);
}

void CDetectBigGraphView::Init(void)
{
	GetFovListANdSort();
}

// CDetectBigGraphView ��Ϣ�������
void CDetectBigGraphView::SetTemplateData(CTemplateData * pTemplateData)
{
	CBaseBigGraphView::SetTemplateData(pTemplateData);

	m_pCurrentImage = NULL;
	m_pCurrentTTemplateImg = NULL;
	m_pCurrentTImg = new CImage();

	m_pDetectMasterData = NULL;	
	m_pCurrentDatailData = NULL;

	if(pTemplateData == NULL)
		return;

	Init();
}

void CDetectBigGraphView::OnZoomin()
{
	if(m_iCurrentZoom<5)
	{
		m_iCurrentZoom++;

		// ��image����Ŵ���С��ŵ���ʱCImage������
		ZoomImageToTemp();
	}
	else
		return;

	Invalidate();//ʹ������������Ч������OnPainˢ�´���
}

void CDetectBigGraphView::OnZoomout()
{
	if(m_iCurrentZoom>-4)
	{
		m_iCurrentZoom--;

		// ��image����Ŵ���С��ŵ���ʱCImage������
		ZoomImageToTemp();
	}
	else
		return;

	Invalidate();
}

//���Ի�׼��
bool CDetectBigGraphView::DetectDatumMark(CDatumMark *pBaseDatum[2],CDatumMark *pDatum[2])
{
	CString	sInfo;
	CImage	*pFovImag = NULL;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	long Xi[2] = {0}; //ģ���׼���x����
	long Yi[2] = {0}; //ģ���׼���y����

	long Xw[2] = {0}; //��ǰ���׼���x����
	long Yw[2] = {0}; //��ǰ���׼���y����

	if(pBaseDatum[0] == NULL || pBaseDatum[1] == NULL || pDatum[0] == NULL || pDatum[1] == NULL)
	{
		return false;
	}

	for(int i=0;i<2;i++)
	{
		CFovData *pFov = pDatum[i]->m_pFovData;
		//����fovͼ�񵽵�ǰ��ʾͼ����
		pFovImag = new CImage(); //modify at 2008-04-01
		LoadFovImage(CRect(pFov->left,pFov->top,pFov->right,pFov->bottom),
					pFovImag);

		::EnterCriticalSection(&m_Critial4CurrentImage);
		if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
		{
			m_pCurrentImage->Destroy();
			delete m_pCurrentImage;
			m_pCurrentImage = NULL;
		}
		m_pCurrentImage = pFovImag;
		::LeaveCriticalSection(&m_Critial4CurrentImage);

		//m_pCurrentImage�Ŵ���С��ŵ���ʱCImage������
		ZoomImageToTemp();

		this->Invalidate();//��ʾ

		Sleep(100);

		//��õ�ǰ��׼��ͼ��
		CImage* pMarkCurrentImage = new CImage();
		GetFactImage(CRect(pDatum[i]->searchLeft,pDatum[i]->searchTop,
			pDatum[i]->searchRight,pDatum[i]->searchBottom),pMarkCurrentImage);

		if(pMarkCurrentImage == NULL || pMarkCurrentImage->IsNull() || pDatum[i]->m_ImageObj.IsNull())
			continue;

		m_CurrentRect = CRect(pDatum[i]->searchLeft,pDatum[i]->searchTop,pDatum[i]->searchRight,pDatum[i]->searchBottom);
		m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
		m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;
		this->Invalidate();//��ʾ

		int iKindred_Value=100;//����ֵ
		iKindred_Value = match( &(pDatum[i]->m_ImageObj),pMarkCurrentImage,&scoredata,80);//�Ƚ�����ͼ��

#ifdef _MYUTEST
		if( pBaseDatum[i] ==  pDatum[i])
		{
			scoredata.Bx = pBaseDatum[i]->m_pFovData->left*PCBMULTIPLE + pBaseDatum[i]->left
				- pFov->left*PCBMULTIPLE - pDatum[i]->searchLeft;
			scoredata.By = pBaseDatum[i]->m_pFovData->top*PCBMULTIPLE + pBaseDatum[i]->top
				- pFov->top*PCBMULTIPLE - pDatum[i]->searchTop;
		}
		else
		{
			if(pFov->GetMidP().x == 93 && pFov->GetMidP().y == 69)
			{
				scoredata.Bx = 81;
				scoredata.By = 55;
			}
			else if(pFov->GetMidP().x == 395 && pFov->GetMidP().y == 263)
			{
				scoredata.Bx = 73;
				scoredata.By = 53;
			}
			else if(pFov->GetMidP().x == 574 && pFov->GetMidP().y == 68)
			{
				scoredata.Bx = 65;
				scoredata.By = 44;
			}
			else if(pFov->GetMidP().x == 875 && pFov->GetMidP().y == 260)
			{
				scoredata.Bx = 62;
				scoredata.By = 55;
			}
			else if(pFov->GetMidP().x == 573 && pFov->GetMidP().y == 399)
			{
				scoredata.Bx = 77;
				scoredata.By = 47;
			}
			else if(pFov->GetMidP().x == 872 && pFov->GetMidP().y == 593)
			{
				scoredata.Bx = 84;
				scoredata.By = 52;
			}
		}
#endif

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + pDatum[i]->m_ImageObj.GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + pDatum[i]->m_ImageObj.GetHeight();

		//�������ֵС�ڷ�ֵ
		if(iKindred_Value < m_pTemplateData->m_lDatumLimit)
		{
			CImage* pTmpMarkTImage = new CImage();
			pTmpMarkTImage->Create(pDatum[i]->m_ImageObj.GetWidth(),
				pDatum[i]->m_ImageObj.GetHeight(),
				pDatum[i]->m_ImageObj.GetBPP());

			pDatum[i]->m_ImageObj.BitBlt(pTmpMarkTImage->GetDC(),0,0,SRCCOPY);
			pTmpMarkTImage->ReleaseDC();

			//����
			CBrush *pBrush = new CBrush(RGB(255,0,0));
			CRgn rg;
			rg.CreateRectRgnIndirect(&m_CurrentTImgRect);
			CDC dc;
			dc.Attach(pMarkCurrentImage->GetDC());
			dc.FrameRgn(&rg,pBrush,2,2);
			dc.Detach();
			pMarkCurrentImage->ReleaseDC();
			delete pBrush;
			pBrush = NULL;

			::PostMessage(this->m_hWnd,WM_SHOW_IMAGE,1,(LPARAM)pTmpMarkTImage);
			::PostMessage(this->m_hWnd,WM_SHOW_IMAGE,2,(LPARAM)pMarkCurrentImage);
			pMarkCurrentImage = NULL;
			
			CString sKindVal =g_LoadString("IDS_KINDRED_VALUE_S","����ֵ");
			sKindVal.Format("%s %d, ",sKindVal,iKindred_Value);
			CString sMsg = ::g_LoadString("IDS_MARK_NOMATCH","��׼���ⲻͨ�����������PCB��Ȼ�󰴡�ȷ�����������ð壬���򰴡�ȡ������ȡ�����˰�");
			sInfo = sKindVal + sMsg;

			if(IDCANCEL == ::AfxMessageBox(sInfo,MB_OKCANCEL))
			{
				m_bReady2Organ = false;
				return false;
			}
			else
			{
				m_bReady2Organ = false;
				return DetectDatumMark(pBaseDatum,pDatum);
			}
		}

		if(!pMarkCurrentImage->IsNull())
			pMarkCurrentImage->Destroy();
		delete pMarkCurrentImage;
		pMarkCurrentImage = NULL;

		//���ɹ�������㵱ǰ����ģ��֮���ƫ��
		int iFactOffsetx = scoredata.Bx;//x����ʵ��ƫ��
		int iFactOffsety = scoredata.By;//y����ʵ��ƫ��

		//�õ�ģ���i����׼���x,y����
		Xi[i] = pBaseDatum[i]->m_pFovData->left*PCBMULTIPLE + pBaseDatum[i]->left;
		Yi[i] = pBaseDatum[i]->m_pFovData->top*PCBMULTIPLE + pBaseDatum[i]->top;

		//�õ���ǰ���i����׼���x,y����
		Xw[i] = pFov->left*PCBMULTIPLE + (pDatum[i]->searchLeft + iFactOffsetx);
		Yw[i] = pFov->top*PCBMULTIPLE + (pDatum[i]->searchTop + iFactOffsety);
	}

	//˳�򱣳�һ��
	if( ((Xi[0] < Xi[1]) && (Xw[0] > Xw[1])) ||
		((Xi[0] > Xi[1]) && (Xw[0] < Xw[1]))
		)
	{
		long lTmp = Xw[0];
		Xw[0] = Xw[1];
		Xw[1] = lTmp;

		lTmp = Yw[0];
		Yw[0] = Yw[1];
		Yw[1] = lTmp;
	}

	double thiDa_i = 0.00;//ģ��Ƕ�
	double thiDa_w = 0.00;//��ǰ��Ƕ�
	m_dThiDa = 0.00;
	m_dS = 1;

	//����Ƕ�
	//��ʽ��S=(Yw2-Yw1)/[(Xi2-Xi1)*sinQ+(Yi2-Yi1)*cosQ]
	//Q=atan{[(Xi2-Xi1)*(Yw2-Yw1) - (Xw2-Xw1)(Yi2-Yi1)]/[(Xw2-Xw1)*(Xi2-Xi1) + (Yi2-Yi1)*(Yw2-Yw1)]}
	m_dThiDa = atan( double( (Xi[1]-Xi[0])*(Yw[1]-Yw[0]) - (Yi[1]-Yi[0])*(Xw[1]-Xw[0]))/double((Xw[1]- Xw[0])*(Xi[1]-Xi[0]) +(Yi[1]-Yi[0])*(Yw[1]-Yw[0])) );
		m_dS = double(Yw[1]-Yw[0])/double((Xi[1]-Xi[0])*sin(m_dThiDa) +(Yi[1]-Yi[0])*cos(m_dThiDa));

	//����λ����
	//m_dS = 1;//double(Xw[1]-Xw[0])/double( (Xi[1]-Xi[0])* cos(m_dThiDa) -(Yi[1]-Yi[0])*sin(m_dThiDa) );
	m_iCurrentPCBOffsetx = Xw[0] - m_dS*Xi[0] * cos(m_dThiDa) + m_dS*Yi[0] * sin(m_dThiDa);
	m_iCurrentPCBOffsety = Yw[0] - m_dS*Xi[0] * sin(m_dThiDa) - m_dS*Yi[0] * cos(m_dThiDa);

	return true;
}

// ��ʼ���PCB
bool CDetectBigGraphView::BeginDetect(void)
{
	CString	sInfo;
	CImage	*pFovImag = NULL;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//�һ�׼��
	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	m_iCurrentPCBOffsetx = 0;
	m_iCurrentPCBOffsety = 0;

	CDatumMark *pDatum[2] = {0};
	int i = 0;

	map<long,CDatumMark *>::iterator markIt=m_pTemplateData->m_DatumMarkMap.begin();
	for(;markIt!=m_pTemplateData->m_DatumMarkMap.end();markIt++)
	{
		if((*markIt).second->type != 0)
			continue;

		pDatum[i] = (*markIt).second;
		i++;
		if(i >= 2)
			break;
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	if(!DetectDatumMark(pDatum,pDatum))
		return false;

	m_bReady2Organ = true;

	//����������
	//CDetectMasterData	dtMasterData;
	if( m_pDetectMasterData != NULL)
	{
		if(m_DetectBaseInfo.lStartId == 0)
		{
			m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
			m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData +" " +m_pDetectMasterData->DetectTime;
		}

		m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
		m_DetectBaseInfo.iDetPcbNum ++;
		if(m_pDetectMasterData->ErrorCount>0)
		{
			m_DetectBaseInfo.iErrPcbNum++;
		}
		m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
		m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

		delete m_pDetectMasterData;
		m_pDetectMasterData =NULL;
	}

	m_pDetectMasterData = new CDetectMasterData();
	m_pDetectMasterData->pcbName = m_pTemplateData->m_templateName;

	CTime t = CTime::GetCurrentTime();
	m_pDetectMasterData->DetectData = t.Format("%Y-%m-%d");
	m_pDetectMasterData->DetectTime = t.Format("%H:%M");
	m_pDetectMasterData->Detecter = g_sUserName;

	//������
	if(m_pTemplateData->m_pBar !=NULL)
	{
		CFovData *pFov = m_pTemplateData->m_pBar->m_pFovData;
		if( pFov != NULL)
		{

			//����fovͼ�񵽵�ǰ��ʾͼ����
			pFovImag = new CImage();

			LoadFovImage(CRect(pFov->left,pFov->top,pFov->right,pFov->bottom),pFovImag);
			::EnterCriticalSection(&m_Critial4CurrentImage);

			if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
			{
				m_pCurrentImage->Destroy();
				delete m_pCurrentImage;
				m_pCurrentImage = NULL;
			}
			m_pCurrentImage = pFovImag;

			::LeaveCriticalSection(&m_Critial4CurrentImage);

			//����������ͼ��
			/*CImage* pBarImage;
			GetFactImage(CRect(m_pTemplateData->m_pBar->barSerchLeft,m_pTemplateData->m_pBar->barSerchTop,
				m_pTemplateData->m_pBar->barSerchRight,m_pTemplateData->m_pBar->barSerchBottom),
				pBarImage);*/
			//TODO
			//�����������ͼ��������������
		}
	}

	return true;
}

/* ���Ԫ���Ƿ���FOV��
�ڷ���true,���򷵻�false
*/
bool CDetectBigGraphView::OrganInFov(const CFovData*  pFovData, const COrganData* pOrganData)
{
	long	lOrganX;//Ԫ������X��
	long	lOrganY;//Ԫ������Y��

	lOrganX = pOrganData->dMidX * m_pTemplateData->m_lCoordScaleX + m_pTemplateData->m_lOffsetX;
	
	//CAD����ԭ���������½�
	lOrganY = m_pTemplateData->m_ImageObj.GetHeight() - (pOrganData->dMidY * m_pTemplateData->m_lCoordScaleY + m_pTemplateData->m_lOffsetY);

	CRect fovRect(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);

	if(true == fovRect.PtInRect( CPoint(lOrganX,lOrganY) ))
	{
		int iImagWidth = pFovData->m_ImageObj.GetWidth();
		int iImagHeight = pFovData->m_ImageObj.GetHeight();

		if( (pOrganData->right < iImagWidth) && ( pOrganData->bottom < iImagHeight) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

/* ���Ԫ��
*pTemplatOrgan��Ϊģ��Ԫ��
*SerchRect:Ԫ��������Χ
*���ͨ������true
*/
bool CDetectBigGraphView::DetectOrgan(COrganData* pTemplatOrgan,CRect SerchRect,int& kindred_value)
{
	if( pTemplatOrgan == NULL )
		return true;

	bool	bDetectResult=false;

	//���Ԫ��������Χ�ڵ�ͼ��
	//CImage* pOrganSerchtImage = new CImage();
	GetFactImage(SerchRect ,m_pCurrentTImg);
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//�ȼ��Ԫ������
	CRect organRect(pTemplatOrgan->left,pTemplatOrgan->top,pTemplatOrgan->right,pTemplatOrgan->bottom);

	//GetFactImage(rect,m_pCurrentTImg);//��ǰ����ͼ��

	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	int iOrgOffsetx = 0;	//Ԫ��ʵ��λ����ģ���x����ƫ��
	int iOrgOffsety = 0;	//Ԫ��ʵ��λ����ģ���y����ƫ��

	int count = 0;
	while(1)
	{
		count++;

		//�����������б�׼ͼƬ
		vector<CImage*>::const_iterator mainBodyIt = pTemplatOrgan->m_MainBodyImageList.begin();
		for(;mainBodyIt != pTemplatOrgan->m_MainBodyImageList.end();mainBodyIt++)
		{
			while(g_bPauseDetect)
			{
				Sleep(30);
			}

			m_pCurrentTTemplateImg = (*mainBodyIt); //��׼ͼ��
			if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
				m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
				continue;

			::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

			kindred_value = match( (*mainBodyIt),m_pCurrentTImg,&scoredata);//�Ƚ�����ͼ��

			m_CurrentTImgRect.left = scoredata.Bx;
			m_CurrentTImgRect.top = scoredata.By;
			m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*mainBodyIt)->GetWidth();
			m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*mainBodyIt)->GetHeight();

			//���Ԫ��������������ʵ�ʵ�ƫ��
			int iFactOffsetx = scoredata.Bx;//x����ʵ��ƫ��
			int iFactOffsety = scoredata.By;//y����ʵ��ƫ��

			if(kindred_value >= pTemplatOrgan->iLimit)
			{
				//Ԫ�����ɹ�������Ԫ��������������ʵ�ʵ�ƫ��
				int iTOffsetx = pTemplatOrgan->left - SerchRect.left;	//x����ģ��ƫ��
				int iTOffsety = pTemplatOrgan->top - SerchRect.top;		//y����ģ��ƫ��

				iOrgOffsetx = iFactOffsetx - iTOffsetx;	//Ԫ��ʵ��λ����ģ���x����ƫ��
				iOrgOffsety = iFactOffsety - iTOffsety;	//Ԫ��ʵ��λ����ģ���y����ƫ��

				bDetectResult = true;
				break;
			}
		}

		if(bDetectResult==false) //����ļ��û��ͨ��
		{
			if(count == 1 && pTemplatOrgan->iIsPolar == 0)
			{
				//���Ԫ�����Ǽ��ԣ���תͼ����ٴβ���
				Flip(m_pCurrentTImg,3);
				continue;
			}
			else
			{
				m_pCurrentDatailData->m_pErrorOrgan = pTemplatOrgan;
				return false;
			}
		}
		else
		{
			break;
		}
	}

	//��麸�̺ͺ���
	for(int i=0;i<4;i++)
	{
		if( !DetectPadAndFoot(pTemplatOrgan,pTemplatOrgan->m_pPadData[i],iOrgOffsetx,iOrgOffsety,kindred_value) )
		{
			return false;
		}
	}
	return true;
}

/* ��⺸�̺ͺ���
*pTemplatOrgan��Ϊģ��Ԫ��
*pTemplatPad��Ϊģ�庸��
*iOrgOffsetx:Ԫ��ʵ��λ����ģ���x����ƫ��
*iOrgOffsety:Ԫ��ʵ��λ����ģ���y����ƫ��
*���ͨ������true
*/
bool CDetectBigGraphView::DetectPadAndFoot(COrganData* pTemplatOrgan, CPadData* pTemplatPad,int iOrgOffsetx,int iOrgOffsety,int& kindred_value)
{
	if( pTemplatPad == NULL)
		return true;

	bool	bDetectResult=false;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//��麸��
	CRect rect(pTemplatPad->left+iOrgOffsetx,pTemplatPad->top+iOrgOffsety,pTemplatPad->right+iOrgOffsetx,
		pTemplatPad->bottom+iOrgOffsety);
	rect.InflateRect(12,12);	//�ٽ�������������12������

	GetFactImage(rect,m_pCurrentTImg);//��ǰ����ͼ��
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//�������б�׼ͼƬ
	vector<CImage*>::const_iterator padIt = pTemplatPad->m_ImageList.begin();
	for(;padIt != pTemplatPad->m_ImageList.end();padIt++)
	{
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		m_pCurrentTTemplateImg = (*padIt); //��׼ͼ��
		if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
			m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
			continue;

		::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

		kindred_value = match( (*padIt),m_pCurrentTImg,&scoredata);//�Ƚ�����ͼ��

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*padIt)->GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*padIt)->GetHeight();

		//��麸��������������ʵ�ʵ�ƫ��
		int iFactOffsetx = scoredata.Bx;//x����ʵ��ƫ��
		int iFactOffsety = scoredata.By;//y����ʵ��ƫ��

		if(kindred_value >= pTemplatOrgan->iLimit)
		{
			bDetectResult = true;
			break;
		}

		//ȡ��ͼ��
		/*::EnterCriticalSection(&m_Critial4CurrentImage);

		CImage *pTempImage = m_pCurrentTImg;
		m_pCurrentTImg = new CImage();//�����µĿռ�����µ�ͼ��
		if(pTempImage != NULL && !pTempImage->IsNull())
		{
			CRect padRect(pTemplatPad->left,pTemplatPad->top,pTemplatPad->right,pTemplatPad->bottom);
			int iImageWidth = padRect.Width();
			int iImageHeight = padRect.Height();

			m_pCurrentTImg->Create(iImageWidth,iImageHeight,pTempImage->GetBPP());
			pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,iImageWidth,iImageHeight,iFactOffsetx,iFactOffsety,SRCCOPY);
			m_pCurrentTImg->ReleaseDC();

			pTempImage->Destroy();
			delete pTempImage;
			pTempImage = NULL;
		}

		::LeaveCriticalSection(&m_Critial4CurrentImage);*/

	}

	//����ļ��û��ͨ��
	if(bDetectResult==false)
	{
		m_pCurrentDatailData->m_pErrorPad = pTemplatPad;
		return false;
	}

	//������к���
	vector<CFootData*>::const_iterator footIt = pTemplatPad->m_footList.begin();
	for(;footIt!=pTemplatPad->m_footList.end();footIt++)
	{
		if(false == DetectFoot(pTemplatOrgan,(*footIt),iOrgOffsetx,iOrgOffsety,kindred_value) )
			return false;
	}

	return true;
}

/* ��⺸��
*pTemplatOrgan��Ϊģ��Ԫ��
*pTemplateFoot��Ϊģ�庸��
*iOrgOffsetx:Ԫ��ʵ��λ����ģ���x����ƫ��
*iOrgOffsety:Ԫ��ʵ��λ����ģ���y����ƫ��
*kindred_value:����ֵ
*���ͨ������true
*/
bool CDetectBigGraphView::DetectFoot(COrganData* pTemplatOrgan , CFootData* pTemplateFoot,int iOrgOffsetx,int iOrgOffsety,int& kindred_value)
{
	if( pTemplateFoot == NULL)
		return true;

	bool	bDetectResult=false;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//��麸��
	CRect rect(pTemplateFoot->left+iOrgOffsetx,pTemplateFoot->top+iOrgOffsety,pTemplateFoot->right+iOrgOffsetx,
		pTemplateFoot->bottom+iOrgOffsety);
	rect.InflateRect(6,6);	//�ٽ�������������6������

	GetFactImage(rect,m_pCurrentTImg);//��ǰ����ͼ��
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//�������б�׼ͼƬ
	vector<CImage*>::const_iterator footIt = pTemplateFoot->m_ImageList.begin();
	for(;footIt != pTemplateFoot->m_ImageList.end();footIt++)
	{
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		m_pCurrentTTemplateImg = (*footIt); //��׼ͼ��
		if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
			m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
			continue;

		::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

		kindred_value = match( (*footIt),m_pCurrentTImg,&scoredata);//�Ƚ�����ͼ��

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*footIt)->GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*footIt)->GetHeight();

		//��麸��������������ʵ�ʵ�ƫ��
		int iFactOffsetx = scoredata.Bx;//x����ʵ��ƫ��
		int iFactOffsety = scoredata.By;//y����ʵ��ƫ��

		if(kindred_value >= pTemplatOrgan->iLimit)
		{
			bDetectResult = true;
			break;
		}

	}

	if(bDetectResult == false)
	{
		m_pCurrentDatailData->m_pErrorFoot = pTemplateFoot;
	}

	return bDetectResult;
}

//�趨Ϊ�Զ�ģʽ
void CDetectBigGraphView::TAuto()
{
	m_DectType = T_AUTO;
	m_bStopWork = false;

	DWORD dwThreadId;
	HANDLE hThread=::CreateThread(NULL,0,WorkThread,(LPVOID)this,0,&dwThreadId);//�����߳�	
}

//�趨Ϊ�ֶ�ģʽ
void CDetectBigGraphView::TManual()
{
	TStop();
	m_DectType = T_MANUAL;
}

//ֹͣ���
void CDetectBigGraphView::TStop()
{
	m_DectType = T_MANUAL;
	m_bStopWork = true;
}


//x��ص�ԭ��
bool CDetectBigGraphView::TOriginx()
{
    g_Motor.GoHome(1,100,10);
	return true;
}

//y��ص�ԭ��
bool CDetectBigGraphView::TOriginy()
{
    g_Motor.GoHome(2,100,10);
	return true;
}

//Ԫ�����
void CDetectBigGraphView::TOrgan()
{
	if( m_bReady2Organ == false || m_pDetectMasterData == NULL)
	{
		if(false == BeginDetect())
			return;
	}

    COrganData *pOrgan=NULL;
	PrgState selectState[]={PS_COMPLETE};

	if(m_DectType == T_MANUAL)
	{
		DWORD dwThreadId;
		DWORD dwExitCode;
		
		if( m_hManualThread != NULL)
		{
			::GetExitCodeThread(m_hManualThread,&dwExitCode);
			if(STILL_ACTIVE == dwExitCode)
				return;
		}

        COrganSelectDlg selectOrgan(m_pTemplateData,&pOrgan,this,&selectState[0],1);

	    if(IDOK!=selectOrgan.DoModal() || pOrgan == NULL)
	    {
		    return;
	    }

	    CFovData *pCurrentFov = pOrgan->m_pFovData;

	    CRect fovRect(pCurrentFov->left,pCurrentFov->top,pCurrentFov->right,pCurrentFov->bottom);

		fovRect.left = pCurrentFov->left*PCBMULTIPLE*cos(m_dThiDa)*m_dS
		- pCurrentFov->top*PCBMULTIPLE*sin(m_dThiDa)*m_dS +m_iCurrentPCBOffsetx;
		fovRect.top = pCurrentFov->left*PCBMULTIPLE* sin(m_dThiDa)*m_dS
			+ pCurrentFov->top*PCBMULTIPLE* cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

		m_iOrganOffsetx = fovRect.left%PCBMULTIPLE;
		m_iOrganOffsety = fovRect.top%PCBMULTIPLE;
		fovRect.left = fovRect.left/PCBMULTIPLE;
		fovRect.top = fovRect.top/PCBMULTIPLE;

		fovRect.right = fovRect.left + (pCurrentFov->right - pCurrentFov->left);
		fovRect.bottom = fovRect.top + (pCurrentFov->bottom - pCurrentFov->top);

	    CImage	*pFovImag = new CImage();
	    //CImage* pOrganSerchtImage = new CImage();

	    //����fovͼ�񵽵�ǰ��ʾͼ����
	    LoadFovImage(fovRect,pFovImag);

        if(!pFovImag->IsNull())
        {
			::EnterCriticalSection(&m_Critial4CurrentImage);

            if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
            {
                m_pCurrentImage->Destroy();
                delete m_pCurrentImage;
                m_pCurrentImage = NULL;
            }
	        m_pCurrentImage = pFovImag;

			::LeaveCriticalSection(&m_Critial4CurrentImage);
        }
        else
        {
            return;
        }

	    //m_pCurrentImage�Ŵ���С��ŵ���ʱCImage������
	    ZoomImageToTemp();
        m_pCurrentTOrgan = pOrgan;

        m_hManualThread=::CreateThread(NULL,0,ManualWorkThread,(LPVOID)this,0,&dwThreadId);//�����߳�

		//this->ManulDetectOrgan();
	}
	else
	{
		m_bIsUseFovOrderList = true;
		this->AutoDetectOrgan();
		m_bIsUseFovOrderList = false;
	}
}

//����PCB��
bool CDetectBigGraphView::TPcbdown()
{
	m_bReady2Organ = false;

	//��������
	CString sErrInfo;

    g_PCBState.Down();

	if(m_DectType == T_MANUAL && m_pDetectMasterData != NULL)
	{

		if(false==m_DetectDataMgr.Store(m_pDetectMasterData,sErrInfo))
		{
			return true;
		}

		if(m_DetectBaseInfo.lStartId == 0)
		{
			m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
			m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData  +" " + m_pDetectMasterData->DetectTime;
		}

		m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
		m_DetectBaseInfo.iDetPcbNum ++;
		if(m_pDetectMasterData->ErrorCount>0)
		{
			m_DetectBaseInfo.iErrPcbNum++;
		}
		m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
		m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

		delete m_pDetectMasterData;
		m_pDetectMasterData = NULL;

		m_bReady2Organ = false;
	}

	return true;

}


//�Զ����Ĺ����߳�
DWORD WINAPI WorkThread(LPVOID lpParam)
{
	CDetectBigGraphView *pBigView=(CDetectBigGraphView *)lpParam;
	while(1)
	{
		if(pBigView->m_bStopWork)
			break;

        if(!g_PCBState.Up(false))
        {
            Sleep(50);
			continue;
        }

		if(pBigView->m_bStopWork)
			break;

		pBigView->TOrgan();

		if(pBigView->m_bStopWork)
			break;

		pBigView->TPcbdown();

        g_PCBState.Loose();

        g_PCBState.SendOut();
	}

	return 0;
}

//�ֶ����Ĺ����߳�
DWORD WINAPI ManualWorkThread(LPVOID lpParam)
{
	CDetectBigGraphView *pBigView=(CDetectBigGraphView *)lpParam;
	pBigView->ManulDetectOrgan();
	return 0;
}

LRESULT CDetectBigGraphView::NotifyDetectDataChanged(WPARAM wParam, LPARAM lParam)
{
	//CDetectMainView *pParrent=(CDetectMainView*)this->GetParent();

	CDetectMainView *pParrent = (CDetectMainView*)m_pParrent;
	if( pParrent && pParrent->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pParrent->NotifyDetectDataChanged();
	}
	return 0;
}

LRESULT CDetectBigGraphView::ShowImage(WPARAM wParam, LPARAM lParam)
{
	CImage *pImage = (CImage*)lParam;
	CString sTitle = "";
	switch(wParam)
	{
	case 1:
		sTitle = "��׼���׼ͼ��";
		break;
	case 2:
		sTitle = "��׼�㵱ǰͼ��";
		break;
	default:
		break;
	}
	CObjectImageDlg *pShowImageDlg = new CObjectImageDlg(pImage,sTitle,this);
	pShowImageDlg->Create(IDD_OBJIMAGE,this);
	pShowImageDlg->ShowWindow(SW_SHOW);

	return 0;
}

// �Ƚ�����ͼ��֮������ƶ�
//pImag1:ģ��ͼ��
//pImag2��Ҫ����ͼ��
//�������ƶ�
int CDetectBigGraphView::match(CImage *pImag1, CImage *pImag2,struct SCOREDATA* pScoreData,int ar)
{
#ifndef _MYUTEST
	if(pImag2->IsNull() || pImag1->IsNull())
	{
		return 0;
	}

	SCOREDATA scoreData = ::Match(pImag2,pImag1,ar);

	if(pScoreData != NULL)
	{
		memcpy(pScoreData,&scoreData,sizeof(scoreData));
	}

	int iScore = min(scoreData.scoreR,scoreData.scoreG);
	return  min(iScore,scoreData.scoreB);

#else
	memset(pScoreData,0,sizeof(struct SCOREDATA));
	pScoreData->scoreB=pScoreData->scoreG=pScoreData->scoreR = 40;
	Sleep(100);
	return 100;
#endif
}

void CDetectBigGraphView::OnInitialUpdate()
{
	CBaseBigGraphView::OnInitialUpdate();

	m_pParrent = this->GetParent();
}

void CDetectBigGraphView::OnPaint()
{

	CPaintDC dc(this); // device context for painting

	if(m_pTemplateData == NULL)
		return;

	if(m_pCurrentImage == NULL || m_pCurrentImage->IsNull())
	{
		//return CBaseBigGraphView::OnPaint();
		return;
	}

	CRect rc;
	GetClientRect(rc);

	//�������
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(0,0,1023,639);

	ZoomImageToTemp();
    //�����������ĵ��ԭ��
	ComputerCenterAndOrigin(m_lCurrentCenterX, m_lCurrentCenterY);	

	m_TempImage.BitBlt(dc,0,0,m_TempImage.GetWidth(),m_TempImage.GetHeight(),m_lSrcOriginX,m_lSrcOriginY,SRCCOPY);

    if(!m_CurrentRect.IsRectEmpty() && !m_CurrentRect.IsRectNull())
	{
		CBrush *pBrush = new CBrush(RGB(255,0,0));
		CRect rect =m_CurrentRect;
        ZoomRectOnOrigin(rect);

		rect.left -= m_lSrcOriginX;
		rect.right -= m_lSrcOriginX;
		rect.top -= m_lSrcOriginY;
		rect.bottom -= m_lSrcOriginY;

		dc.FrameRect(&rect,pBrush);
		delete pBrush;
		pBrush = NULL;
	}

}

// �Զ����Ԫ��
void CDetectBigGraphView::AutoDetectOrgan(void)
{
	m_bHasError  = false;
	bool bCanLoadFovImag = false;

	CDatumMark *pDatum[2] = {0};

	CImage	*pFovImag = NULL;
	//CImage* pOrganSerchtImage = new CImage();

	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	CDatumMark *pBaseDatum[2] = {0};
	int count = 0;

	map<long,CDatumMark *>::iterator markIt=m_pTemplateData->m_DatumMarkMap.begin();
	for(;markIt!=m_pTemplateData->m_DatumMarkMap.end();markIt++)
	{
		if((*markIt).second->type != 0)
			continue;

		pBaseDatum[count] = (*markIt).second;
		count++;
		if(count >= 2)
			break;
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	if(m_pTemplateData->m_SpellPcbMap.find(0) == m_pTemplateData->m_SpellPcbMap.end())
	{
		CSpellPCB *pSpellPCB = new CSpellPCB();
		m_pTemplateData->m_SpellPcbMap.insert(::make_pair(0,pSpellPCB));
	}
	
	map<int,CSpellPCB*>::iterator spellIt = m_pTemplateData->m_SpellPcbMap.begin();
	for(int i=0;i < m_pTemplateData->m_SpellPcbMap.size();i++)
	{
		CSpellPCB	*pSpellPCB = NULL;

		if(i == 0)//��һ�α�����ԭ��
		{
			pSpellPCB = m_pTemplateData->m_SpellPcbMap[0];
			if( (*spellIt).first == 0)
			{
				spellIt++;
			}
		}
		else
		{
			if( (*spellIt).first == 0)
			{
				spellIt++;
			}
			pSpellPCB = (*spellIt).second;
			spellIt++;
		}

		if(pSpellPCB == NULL)
			continue;

		if(pSpellPCB->pDatumMark1 != NULL && pSpellPCB->pDatumMark2 != NULL)
		{
			pDatum[0] = pSpellPCB->pDatumMark1;
			pDatum[1] = pSpellPCB->pDatumMark2;

			if(!DetectDatumMark(pBaseDatum,pDatum))
				continue;
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		vector<CFovData*>::iterator it = m_FOVOrderList.begin();
		for(;it!=m_FOVOrderList.end();it++) //ѭ��������е�FOV
		{
			if(m_bStopWork || g_bStopWork)
			{
				return;
			}

			while(g_bPauseDetect)
			{
				Sleep(30);
			}

			//���ǻ������ϵ�FOV
			if( (*it)->type != 0)
			{
				continue;
			}

			CRect tempFovRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);

			CRect fovRect1(0,0,0,0);
			
			//У��ԭ��FOV
			fovRect1.left = (tempFovRect.left*PCBMULTIPLE) * cos(m_dThiDa)*m_dS
				- (tempFovRect.top*PCBMULTIPLE) * sin(m_dThiDa)*m_dS +m_iCurrentPCBOffsetx;
			fovRect1.top = (tempFovRect.left*PCBMULTIPLE) * sin(m_dThiDa)*m_dS
				+ (tempFovRect.top*PCBMULTIPLE) * cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

			m_iOrganOffsetx = fovRect1.left%PCBMULTIPLE;
			m_iOrganOffsety = fovRect1.top%PCBMULTIPLE;
			fovRect1.left = fovRect1.left/PCBMULTIPLE;
			fovRect1.top = fovRect1.top/PCBMULTIPLE;

			fovRect1.right = fovRect1.left + (tempFovRect.right - tempFovRect.left);
			fovRect1.bottom = fovRect1.top + (tempFovRect.bottom - tempFovRect.top);

			bCanLoadFovImag = false;

			EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
			map<long,COrganData*>::iterator organTmpIt=m_pTemplateData->m_OrganDataMap.begin();
			for(;organTmpIt != m_pTemplateData->m_OrganDataMap.end();organTmpIt++)
			{
				if( (*it)->fovId == (*organTmpIt).second->fovId && (*organTmpIt).second->PState == PS_COMPLETE)
				{
					bCanLoadFovImag = true;
					break;
				}
			}
			::LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

			/**����fovͼ�񵽵�ǰ��ʾͼ���� --->>>>*/
			if( bCanLoadFovImag )
			{
				pFovImag = new CImage();
				LoadFovImage(fovRect1,pFovImag,true);

				::EnterCriticalSection(&m_Critial4CurrentImage);

				if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
				{
					m_pCurrentImage->Destroy();
					delete m_pCurrentImage;
					m_pCurrentImage = NULL;
				}
				m_pCurrentImage = pFovImag;
				m_CurrentRect = CRect(0,0,0,0);

				::LeaveCriticalSection(&m_Critial4CurrentImage);

				//m_pCurrentImage�Ŵ���С��ŵ���ʱCImage������
				ZoomImageToTemp();
				this->Invalidate();//��ʾ
			}
			else
			{
				continue;
			}
			/**end ����fovͼ�񵽵�ǰ��ʾͼ����<<<<----*/
			
			//�������е�Ԫ��
			EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
			map<long,COrganData*>::iterator organMapIt=m_pTemplateData->m_OrganDataMap.begin();
			for(;organMapIt != m_pTemplateData->m_OrganDataMap.end();organMapIt++)
			{
				LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
				if(m_bStopWork || g_bStopWork)
					return;

				while(g_bPauseDetect)
				{
					Sleep(30);
				}

				EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
				if((*organMapIt).second->PState == PS_SKIP || (*organMapIt).second->PState == PS_INVALID 
					|| (*organMapIt).second->bNeedDetect == false)
				{
					continue; //���Դ�Ԫ��
				}
				else if( (*it)->fovId == (*organMapIt).second->fovId && // OrganInFov( (*it),(*organMapIt).second ) &&
					(*organMapIt).second->PState == PS_COMPLETE)
				{//Ԫ���ڴ�fov��Χ�ڲ����Ѿ�������

					m_pDetectMasterData->DetectCount++;//Ԫ������

					m_pCurrentTOrgan = (*organMapIt).second;

					CDetectDetailData	*pDetectDatailData = new CDetectDetailData();//��¼�������
					m_pCurrentDatailData = pDetectDatailData;

					m_pDetectMasterData->m_Details.push_back(pDetectDatailData);
					pDetectDatailData->organName = m_pCurrentTOrgan->sName;
					pDetectDatailData->spellNo = pSpellPCB->no;
					pDetectDatailData->centerX = m_pCurrentTOrgan->GetMidP().x;
					pDetectDatailData->centerY = m_pCurrentTOrgan->GetMidP().y;
					pDetectDatailData->isPass = 1;
					pDetectDatailData->limit_value = m_pCurrentTOrgan->iLimit;

					CRect serchRect(m_pCurrentTOrgan->RangeLeft+m_iOrganOffsetx,m_pCurrentTOrgan->RrangeTop+m_iOrganOffsety,
						m_pCurrentTOrgan->RangeRight+m_iOrganOffsetx,m_pCurrentTOrgan->RangeBottom+m_iOrganOffsety);

					//serchRect.left = m_pCurrentTOrgan->RangeLeft * cos(m_dThiDa) - m_pCurrentTOrgan->RrangeTop * sin(m_dThiDa) +m_iCurrentPCBOffsetx;
					//serchRect.top = m_pCurrentTOrgan->RangeLeft * sin(m_dThiDa) + m_pCurrentTOrgan->RrangeTop * cos(m_dThiDa) +m_iCurrentPCBOffsety;
					//serchRect.right = serchRect.left + (m_pCurrentTOrgan->RangeRight - m_pCurrentTOrgan->RangeLeft);
					//serchRect.bottom = serchRect.top + (m_pCurrentTOrgan->RangeBottom - m_pCurrentTOrgan->RrangeTop);

					m_CurrentRect = serchRect;
					m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
					m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;
					this->Invalidate();//��ʾ

					LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
					bool bResult=DetectOrgan( m_pCurrentTOrgan,serchRect,pDetectDatailData->kindred_value);//���Ԫ��
					EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

					if(bResult == false)
					{
						m_bHasError = true;
						pDetectDatailData->isPass = 0;
						pDetectDatailData->m_pTemplateImg = m_pCurrentTTemplateImg;

						::EnterCriticalSection(&m_Critial4CurrentImage);

						CImage *pTempImage = m_pCurrentTImg;
						pDetectDatailData->m_pCurrentImg = m_pCurrentTImg;//���浱ǰͼ��
						pDetectDatailData->m_CurrentTImgRect = m_CurrentTImgRect;
						m_pCurrentTImg = new CImage();//�����µĿռ�����µ�ͼ��
						if(pTempImage != NULL && !pTempImage->IsNull())
						{
							m_pCurrentTImg->Create(pTempImage->GetWidth(),pTempImage->GetHeight(),pTempImage->GetBPP());
							pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,pTempImage->GetWidth(),pTempImage->GetHeight(),0,0,SRCCOPY);
							m_pCurrentTImg->ReleaseDC();
						}

						::LeaveCriticalSection(&m_Critial4CurrentImage);
					}

					#ifdef _MYUTEST
					::Sleep(30);
					#endif

					//pOrganSerchtImage->Destroy();
				}
			}//end �������е�Ԫ��
			LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		}//end ѭ��������е�FOV
		EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	}//end ѭ��������е�ƴ��
	LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	this->Invalidate();
    //RefreshUITread();

	if(m_bHasError)//��Ԫ�����û��ͨ��
	{
		//��֤����
		if(m_bAskWhenErr)
		{
			CValidateErr validateErr(m_pDetectMasterData,&m_DStatInfoMap,this);
			validateErr.DoModal();
		}
		else
		{
			AutoAdjustErr();
		}
	}

	if(m_pDetectMasterData == NULL)
		return;

	//��������
	CString sErrInfo;
	
	if(false==m_DetectDataMgr.Store(m_pDetectMasterData,sErrInfo))
	{
		sErrInfo ="����ʧ��\n"+sErrInfo;
		::AfxMessageBox(sErrInfo);
		return;
	}

	if(m_DetectBaseInfo.lStartId == 0)
	{
		m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
		m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData  +" " + m_pDetectMasterData->DetectTime;
	}

	m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
	m_DetectBaseInfo.iDetPcbNum ++;
	if(m_pDetectMasterData->ErrorCount>0)
	{
		m_DetectBaseInfo.iErrPcbNum++;
	}
	m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
	m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

	m_bReady2Organ = false;
	delete m_pDetectMasterData;
	m_pDetectMasterData = NULL;
}

// �ֶ����Ԫ��
void CDetectBigGraphView::ManulDetectOrgan(void)
{
	m_bHasError  = false;

	m_pDetectMasterData->DetectCount++;//Ԫ������

	CDetectDetailData	*pDetectDatailData = new CDetectDetailData();//��¼�������
    /*if(m_pCurrentDatailData != NULL)
    {
        delete m_pCurrentDatailData;
        m_pCurrentDatailData = NULL;
    }*/
	m_pCurrentDatailData = pDetectDatailData;

	CDetectMasterData	TmpMasterData;
	TmpMasterData.m_Details.push_back(pDetectDatailData);
	m_pDetectMasterData->m_Details.push_back(pDetectDatailData);
	pDetectDatailData->organName = m_pCurrentTOrgan->sName;
	pDetectDatailData->centerX = m_pCurrentTOrgan->GetMidP().x;
	pDetectDatailData->centerY = m_pCurrentTOrgan->GetMidP().y;
	pDetectDatailData->isPass = 1;
	pDetectDatailData->limit_value = m_pCurrentTOrgan->iLimit;

	CRect serchRect(m_pCurrentTOrgan->RangeLeft+m_iOrganOffsetx,m_pCurrentTOrgan->RrangeTop+m_iOrganOffsety,
		m_pCurrentTOrgan->RangeRight+m_iOrganOffsetx,m_pCurrentTOrgan->RangeBottom+m_iOrganOffsety);
	//CRect serchRect;

	CFovData *pFov = m_pCurrentTOrgan->m_pFovData;

	/*serchRect.left = (pFov->left*PCBMULTIPLE + m_pCurrentTOrgan->RangeLeft) * cos(m_dThiDa) 
		- (pFov->top*PCBMULTIPLE + m_pCurrentTOrgan->RrangeTop) * sin(m_dThiDa) +m_iCurrentPCBOffsetx;
	serchRect.top = (pFov->left*PCBMULTIPLE + m_pCurrentTOrgan->RangeLeft) * sin(m_dThiDa)*m_dS
		+ (pFov->top*PCBMULTIPLE + m_pCurrentTOrgan->RrangeTop) * cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

	serchRect.left -= pFov->left*PCBMULTIPLE;
	serchRect.top -= pFov->top*PCBMULTIPLE;

	serchRect.right = serchRect.left + (m_pCurrentTOrgan->RangeRight - m_pCurrentTOrgan->RangeLeft);
	serchRect.bottom = serchRect.top + (m_pCurrentTOrgan->RangeBottom - m_pCurrentTOrgan->RrangeTop);*/

	m_CurrentRect = CRect(serchRect.left,serchRect.top,serchRect.right,serchRect.bottom);
	m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
	m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;

	m_CurrentRect2= CRect(m_pCurrentTOrgan->RangeLeft,m_pCurrentTOrgan->RrangeTop,m_pCurrentTOrgan->RangeRight,m_pCurrentTOrgan->RangeBottom);

	this->Invalidate();//��ʾ

	bool bResult=DetectOrgan( m_pCurrentTOrgan,serchRect,pDetectDatailData->kindred_value);//���Ԫ��
	if(bResult == false)
	{
		m_bHasError = true;
		pDetectDatailData->isPass = 0;
		pDetectDatailData->m_pTemplateImg = m_pCurrentTTemplateImg;

		::EnterCriticalSection(&m_Critial4CurrentImage);

		CImage *pTempImage = m_pCurrentTImg;
		pDetectDatailData->m_pCurrentImg = m_pCurrentTImg;//���浱ǰͼ��
		pDetectDatailData->m_CurrentTImgRect = m_CurrentTImgRect;
		m_pCurrentTImg = new CImage();//�����µĿռ�����µ�ͼ��
		if(pTempImage != NULL && !pTempImage->IsNull())
		{
			m_pCurrentTImg->Create(pTempImage->GetWidth(),pTempImage->GetHeight(),pTempImage->GetBPP());
			pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,pTempImage->GetWidth(),pTempImage->GetHeight(),0,0,SRCCOPY);
			m_pCurrentTImg->ReleaseDC();
		}

		::LeaveCriticalSection(&m_Critial4CurrentImage);
	}

	this->Invalidate();

	if(m_bHasError)//��Ԫ�����û��ͨ��
	{
		//��֤����
		if(m_bAskWhenErr)
		{
			CValidateErr validateErr(&TmpMasterData,&m_DStatInfoMap,this);
			validateErr.DoModal();
			m_pDetectMasterData->ErrorCount += TmpMasterData.ErrorCount;
		}
		else
		{
			AutoAdjustErr();
		}

	}
	else
	{
		CString sInfo = g_LoadString("IDS_DETECT_COMPLETE","������");
		::AfxMessageBox(sInfo);
	}

	TmpMasterData.m_Details.clear();
}

// �Զ��жϴ���
void CDetectBigGraphView::AutoAdjustErr(void)
{
	bool	bIsError = true;//Ԫ���Ƿ��Ǵ����
	CString	sErrType ;	//��������

	vector<CDetectDetailData*>::iterator DetailIt = m_pDetectMasterData->m_Details.begin();
	for(;DetailIt != m_pDetectMasterData->m_Details.end();DetailIt++)
	{
		CDetectDetailData* pDetailData = (*DetailIt);
		if(pDetailData->isPass == 0)
		{
			//TODO:����pDetailData->kindred_value,pDetailData->m_pTemplateImg,pDetailData->m_pCurrentImg
			//�ж�Ԫ���Ƿ������������

			//just for test:�������������Ϊ"��λ"��ʵ����Ҫ���˴���ɾ��
			bIsError = true;
			sErrType = "��λ";

			if(bIsError)
			{
				pDetailData->errType = sErrType;
				m_pDetectMasterData->ErrorCount++;

				//ͳ����Ϣ
				map<CString,CDetectStatInfo*>::iterator it = m_DStatInfoMap.find(pDetailData->organName);
				if(it != m_DStatInfoMap.end())
				{
					(*it).second->lErrorCount +=1;
				}
				else
				{
					CDetectStatInfo* pStatInfo = new CDetectStatInfo();
					pStatInfo->sOrganName = pDetailData->organName;
					pStatInfo->lErrorCount = 1;
					m_DStatInfoMap.insert(::make_pair(pStatInfo->sOrganName,pStatInfo));
				}
			}//end if(bIsError)
		}//end if(pDetailData->isPass == 0)
	}//end for
}

void CDetectBigGraphView::RefreshUITread()
{
    CreateThread(NULL,0,RefreshView,(LPVOID)this,0,NULL);//�����߳�
}

DWORD WINAPI RefreshView(LPVOID lpParam)
{
    CDetectBigGraphView *pView = (CDetectBigGraphView*)lpParam;
    pView->Invalidate();
    return 0;
}
BOOL CDetectBigGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta<0)
	{
		this->OnZoomout();
	}
	else
	{
		this->OnZoomin();
	}

	return CBaseBigGraphView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDetectBigGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	bool bLBDown=false;	//�������Ƿ���
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		bLBDown = true;
	}
	else
	{
		m_preMousePoint = CPoint(0,0);
		return;
	}

	if(m_TempImage.IsNull())
		return;

	if(m_preMousePoint.x>0)
		AdjustCenterX(m_preMousePoint.x - point.x);
	if(m_preMousePoint.y>0)
		AdjustCenterY(m_preMousePoint.y - point.y);

	m_preMousePoint = point;

	CBaseBigGraphView::OnMouseMove(nFlags, point);
}

void CDetectBigGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	Invalidate();

	CBaseBigGraphView::OnLButtonUp(nFlags, point);
}

void CDetectBigGraphView::PcbStateChange(void)
{
	m_bReady2Organ = false;
}
