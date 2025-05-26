// BigGraphView.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "AOI.h"
#include "BaseBigGraphView.h"
#include <math.h>
#include "Motor.h"
#include "Camera.h"
#include "PCBState.h"

extern CCamera     g_camera;       //ȫ������ͷ����
extern CPCBState   g_PCBState;     //ȫ�ְ�״̬
extern CMotor      g_Motor;        //ȫ�ֵ��

extern double g_fMotorStep;
extern double g_fPels_MM;

// CBaseBigGraphView

IMPLEMENT_DYNCREATE(CBaseBigGraphView, CGraphView)

CBaseBigGraphView::CBaseBigGraphView()
	: CGraphView(CBaseBigGraphView::IDD)
{
	init();
}

CBaseBigGraphView::CBaseBigGraphView(UINT nIDTemplate)
	: CGraphView(nIDTemplate)
{
	init();
}

CBaseBigGraphView::~CBaseBigGraphView()
{
	if( !m_TempImage.IsNull())
	{
		//m_TempImage.ReleaseDC();
		m_TempImage.Destroy();
	}

	::DeleteCriticalSection(&m_Critial4CurrentImage);
}

void CBaseBigGraphView::init()
{
	m_iCurrentZoom = 0;	//û�зŴ���С
	m_lCurrentCenterX = 0;
	m_lCurrentCenterY = 0;
	m_lSrcOriginX = 0;
	m_lSrcOriginY = 0;
	
	m_iOldPCBCurrentZoom = 0;
	m_lOldPCBCurrentCenterX = 0;
	m_lOldPCBCurrentCenterY = 0;
	m_lOldPCBSrcOriginX = 0;
	m_lOldPCBSrcOriginY = 0;

	m_iOldFOVCurrentZoom = 0;
	m_lOldFOVCurrentCenterX = 0;
	m_lOldFOVCurrentCenterY = 0;
	m_lOldFOVSrcOriginX = 0;
	m_lOldFOVSrcOriginY = 0;

	::InitializeCriticalSection(&m_Critial4CurrentImage);
	
	m_pCurrentImage = NULL;
	
}

void CBaseBigGraphView::DoDataExchange(CDataExchange* pDX)
{
	CGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBaseBigGraphView, CGraphView)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBaseBigGraphView ���

#ifdef _DEBUG
void CBaseBigGraphView::AssertValid() const
{
	CGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CBaseBigGraphView::Dump(CDumpContext& dc) const
{
	CGraphView::Dump(dc);
}
#endif
#endif //_DEBUG

void CBaseBigGraphView::SetTemplateData(CTemplateData * pTemplateData)
{
	CGraphView::SetTemplateData(pTemplateData);
}

// CBaseBigGraphView ��Ϣ�������

void CBaseBigGraphView::OnZoomin()
{
	if(m_iCurrentZoom<5)
	{
		m_iCurrentZoom++;

		// ��image����Ŵ���С��ŵ���ʱCImage������
		ZoomImageToTemp();
	}
	else
		return;

	//draw();
	Invalidate();//ʹ������������Ч������OnPainˢ�´���

}

void CBaseBigGraphView::OnZoomout()
{
	if(m_iCurrentZoom>-4)
	{
		m_iCurrentZoom--;

		// ��image����Ŵ���С��ŵ���ʱCImage������
		ZoomImageToTemp();
	}
	else
		return;

	//draw();
	Invalidate();
}

void CBaseBigGraphView::OnOriginal()
{
	m_iCurrentZoom = 0;

	// ��image����Ŵ���С��ŵ���ʱCImage������
	ZoomImageToTemp();
	Invalidate();
}

/**
*�����Դ����ͼƬ��Ŀ��ʱ����Դ���ĸ��㿪ʼ����
*CenterPoint��Դ��Ŀ������ĵ�
*SrcWH��Դ����ͼ�񣩵Ŀ����߶���ɵ�CPoint
*DestWH��Ŀ�꣨�����ڣ��Ŀ����߶���ɵ�CPoint
*����Դ������ԭ��
**/
CPoint CBaseBigGraphView::ComputerSrcOrigin(CPoint CenterPoint, CPoint SrcWH, CPoint DestWH)
{
	int iOriginX=0,iOriginY=0;

	iOriginX = CenterPoint.x - DestWH.x/2;//����ԭ��Ϊͼ��ǰ���ļ�һ�봰�ڿ��
	iOriginX = iOriginX<0? 0:iOriginX;//���������С��0����ȡ0

	//������ڸ��Ƿ�Χ����ͼ�����ұߣ�������ԭ��ʹ������ͼ����ұ߶���
	if( (iOriginX+DestWH.x) > SrcWH.x)
	{
		iOriginX = (SrcWH.x - DestWH.x)>0?(SrcWH.x - DestWH.x):0;
	}

	iOriginY = CenterPoint.y - DestWH.y/2;
	iOriginY = iOriginY<0? 0:iOriginY;
	if( (iOriginY+DestWH.y) > SrcWH.y)
	{
		iOriginY = (SrcWH.y - DestWH.y)>0?(SrcWH.y - DestWH.y):0;
	}

	return CPoint(iOriginX,iOriginY);
}

/**
*����ԭʼ�Ļ����ϷŴ���Сrect
*rect:����rectû�зŴ���Сʱ��ԭʼ��С
*return:���ص�ǰ�Ŵ����µ�rect�Ĵ�С
**/
void CBaseBigGraphView::ZoomRectOnOrigin(CRect& rect)
{
	rect.top = (1+m_iCurrentZoom*ZOOMSTEP)*rect.top;
	rect.left = (1+m_iCurrentZoom*ZOOMSTEP)*rect.left;
	rect.right = (1+m_iCurrentZoom*ZOOMSTEP)*rect.right;
	rect.bottom = (1+m_iCurrentZoom*ZOOMSTEP)*rect.bottom;	
}

/**
*���ڵ�ǰrect�����ϷŴ�rect
*rect:����rect��ǰ��С��rect
*return:���طŴ���rect�Ĵ�С
**/
void CBaseBigGraphView::ZoomInRectOnCurrent(CRect& rect)
{
	rect.top = (1+ZOOMSTEP)*rect.top;
	rect.left = (1+ZOOMSTEP)*rect.left;
	rect.right = (1+ZOOMSTEP)*rect.right;
	rect.bottom = (1+ZOOMSTEP)*rect.bottom;
}

/**
*���ڵ�ǰrect��������Сrect
*rect:����rect��ǰ��С��rect
*return:������С���rect�Ĵ�С
**/
void CBaseBigGraphView::ZoomOutRectOnCurrent(CRect& rect)
{
	rect.top = (1-ZOOMSTEP)*rect.top;
	rect.left = (1-ZOOMSTEP)*rect.left;
	rect.right = (1-ZOOMSTEP)*rect.right;
	rect.bottom = (1-ZOOMSTEP)*rect.bottom;
}

void CBaseBigGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGraphView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBaseBigGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CGraphView::OnLButtonDblClk(nFlags, point);
}
	
//���ݵ�ǰ��rect��ô�rect��ԭ��С������µ�rect
void CBaseBigGraphView::GetOriginRect(const CRect currentRect, CRect& OriginRect)
{
	OriginRect.left = currentRect.left/(1+m_iCurrentZoom*ZOOMSTEP);
	OriginRect.right = currentRect.right/(1+m_iCurrentZoom*ZOOMSTEP);
	OriginRect.top = currentRect.top/(1+m_iCurrentZoom*ZOOMSTEP);
	OriginRect.bottom = currentRect.bottom/(1+m_iCurrentZoom*ZOOMSTEP);
}

// ��image����Ŵ���С��ŵ���ʱCImage������
void CBaseBigGraphView::ZoomImageToTemp(void)
{
	if(m_pCurrentImage == NULL || m_pCurrentImage->IsNull())
	{
		return;
	}

	::EnterCriticalSection(&m_Critial4CurrentImage);

	long lOldWidth = 0;
	if( !m_TempImage.IsNull())
	{
		lOldWidth = m_TempImage.GetWidth();
		m_TempImage.Destroy();
	}

	//����Ŵ���С�󻭲��Ĵ�С
	long lNewWidth = (1+m_iCurrentZoom*ZOOMSTEP)*m_pCurrentImage->GetWidth();
	long lNewHeight = (1+m_iCurrentZoom*ZOOMSTEP)*m_pCurrentImage->GetHeight();

	if(!m_TempImage.Create(lNewWidth,lNewHeight,m_pCurrentImage->GetBPP(),0))
	{
		::AfxMessageBox("Create image object fail");
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return;
	}

	HDC hTempImageDC = m_TempImage.GetDC();
	SetStretchBltMode(hTempImageDC,HALFTONE);
	m_pCurrentImage->StretchBlt(hTempImageDC,0,0,lNewWidth,lNewHeight,SRCCOPY);
	m_TempImage.ReleaseDC();

	//������Ƥ�������λ��
	if(lOldWidth>0)
	{
		double dMultiple = (double)lNewWidth/(double)lOldWidth;
		
		if(m_pRectTracker!=NULL)
		{
			//����Ƥ��ת����ͼ������ϵ��
			m_pRectTracker->m_rect.left += m_lSrcOriginX;
			m_pRectTracker->m_rect.right += m_lSrcOriginX;
			m_pRectTracker->m_rect.top += m_lSrcOriginY;
			m_pRectTracker->m_rect.bottom += m_lSrcOriginY;
		}

		//����λ�ñ䶯
		m_lCurrentCenterX *= dMultiple;
		m_lCurrentCenterY *= dMultiple;

		//����Դ�µ�ԭ��
		CRect rc;
		GetClientRect(rc);

		CPoint srcP=ComputerSrcOrigin( CPoint(m_lCurrentCenterX,m_lCurrentCenterY),
			CPoint(m_TempImage.GetWidth(),m_TempImage.GetHeight()),
			CPoint(rc.right-rc.left,rc.bottom-rc.top));
		m_lSrcOriginX = srcP.x;
		m_lSrcOriginY = srcP.y;

		if(m_pRectTracker!=NULL)
		{
			//��Ƥ���С�䶯
			m_pRectTracker->m_rect.bottom *= dMultiple;
			m_pRectTracker->m_rect.left *= dMultiple;
			m_pRectTracker->m_rect.right *= dMultiple;
			m_pRectTracker->m_rect.top *= dMultiple;

			//����Ƥ��ת������������ϵ��
			m_pRectTracker->m_rect.left -= m_lSrcOriginX;
			m_pRectTracker->m_rect.right -= m_lSrcOriginX;
			m_pRectTracker->m_rect.top -= m_lSrcOriginY;
			m_pRectTracker->m_rect.bottom -= m_lSrcOriginY;
		}
	}

	::LeaveCriticalSection(&m_Critial4CurrentImage);
	
}

//�ƶ����
void CBaseBigGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	CGraphView::OnMouseMove(nFlags, point);
}

BOOL CBaseBigGraphView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CGraphView::OnSetCursor(pWnd, nHitTest, message);
}

void CBaseBigGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGraphView::OnLButtonDown(nFlags, point);
}

void CBaseBigGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CGraphView::OnLButtonUp(nFlags, point);
}


BOOL CBaseBigGraphView::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;

	return CGraphView::OnEraseBkgnd(pDC);
}

/* ������ͼ����X��
*iAdjustNum������������ֵΪ�����ƣ�Ϊ������
*return�������ɹ�����true��û�е�������false
*/
bool CBaseBigGraphView::AdjustCenterX(int iAdjustNum)
{
	if(m_TempImage.IsNull())
		return false;

	CRect rc;
	GetClientRect(rc);
	::EnterCriticalSection(&m_Critial4CurrentImage);

	//long lNewCurrentCenterX = (1+m_iCurrentZoom*ZOOMSTEP)*(m_lCurrentCenterX +iAdjustNum);
	long lNewCurrentCenterX = m_lCurrentCenterX +iAdjustNum;
	
	//ͼ������Ͻ��Ѿ��ʹ��ڵ�����غϣ����ĵ㲻����������
	if(iAdjustNum<0 && (lNewCurrentCenterX - rc.Width()/2) <= 0)
	{
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return false;
	}
	else if(iAdjustNum>0 && (m_TempImage.GetWidth()- lNewCurrentCenterX) <= (rc.Width()/2) )
	{//ͼ������½��Ѿ��ʹ��ڵ��ұ��غϣ����ĵ㲻����������
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return false;
	}
	else
	{
		m_lCurrentCenterX += iAdjustNum;
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return true;
	}
	
}

/* ������ͼ����Y��
*iAdjustNum������������ֵΪ�����ƣ�Ϊ������
*return�������ɹ�����true��û�е�������false
*/
bool CBaseBigGraphView::AdjustCenterY(int iAdjustNum)
{
	if(m_TempImage.IsNull())
		return false;

	CRect rc;
	GetClientRect(rc);
	
	::EnterCriticalSection(&m_Critial4CurrentImage);

	//long lNewCurrentCenterY = (1+m_iCurrentZoom*ZOOMSTEP)*(m_lCurrentCenterY+iAdjustNum);
	long lNewCurrentCenterY = m_lCurrentCenterY+iAdjustNum;

	//ͼ������Ͻ��Ѿ��ʹ��ڵ��ϱ��غϣ����ĵ㲻����������
	if( iAdjustNum<0 &&(lNewCurrentCenterY - rc.Height()/2) <= 0)
	{
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return false;
	}
	else if( iAdjustNum>0 && ( m_TempImage.GetHeight() - lNewCurrentCenterY) <= (rc.Height()/2) )
	{//ͼ������½��Ѿ��ʹ��ڵ��±��غϣ����ĵ㲻����������
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return false;
	}
	else
	{
		m_lCurrentCenterY += iAdjustNum;
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return true;
	}	
}


// ����fov��Χ����fovͼ��(������ͷ�м���)
void CBaseBigGraphView::LoadFovImage(CRect fovRect, CImage* pDestImage,bool bNeedCompensate)
{
	if(!pDestImage)
		return;
	if( !pDestImage->IsNull())
	{
		//pDestImage->ReleaseDC();
		pDestImage->Destroy();
	}
	
#ifndef _MYUTEST

	int iWinWidth = 0;      //������ڿ��
    int iWinHeight = 0;     //������ڸ߶�
	 
	//��Ŀ���
    int iBoardWidth = 0;
    int iBoardHeight = 0;

	double	dLenOfPels = 0.00;	//ÿ�����صĳ��ȣ���λ���ף�

	iBoardWidth = m_pTemplateData->m_width * g_fPels_MM;
	iBoardHeight = m_pTemplateData->m_height * g_fPels_MM;
	dLenOfPels = 1.00/g_fPels_MM;
    
    HV_RESOLUTION mode = g_camera.GetResolutionMode();
    switch(mode)
    {
    case RES_MODE0:
        iWinWidth = 2048;
        iWinHeight = 1536;
        break;
    case RES_MODE1:
		//iBoardWidth = m_pTemplateData->m_width * MM_PELS_1024;
        //iBoardHeight = m_pTemplateData->m_height * MM_PELS_1024;
		//dLenOfPels = 1.00/(double)MM_PELS_1024;
        iWinWidth = 1024;
        iWinHeight = 768;
        break;
    case RES_MODE2:
        iWinWidth = 640;
        iWinHeight = 480;
        break;
    case RES_MODE3:
        iWinWidth = 512;
        iWinHeight = 384;
        break;
    default:
        ::AfxMessageBox("Unknow the camera mode");
        return;
    }

    if(!g_camera.SetOutputWindow(0, 0,iWinWidth,iWinHeight))
    {
        ::AfxMessageBox("Set camera outputwindows error");
        return;
    }

	//����x��Ŀ��λ��
	int iPosX = (dLenOfPels * fovRect.left*FOVMULTIPLE)* g_fMotorStep;
	//����y��Ŀ��λ��
	int iPosY = dLenOfPels * (iBoardHeight - fovRect.bottom*FOVMULTIPLE)* g_fMotorStep;

	//����ʵ��Ҫ�ƶ���λ��
	//long lNewPosX=0,lNewPosY=0;
	//g_Motor.ComputerFactPos(iPosX,iPosY,&lNewPosX,&lNewPosY);
	int iCompensatePosX =0,iCompensatePosY =0; //�ƶ�λ�ò���
	if(bNeedCompensate)
	{
		iCompensatePosY = iPosX * sin(m_pTemplateData->m_dJiaoDuX);
		iCompensatePosX = iPosY * sin(m_pTemplateData->m_dJiaoDuY);
	}

	iPosX -= iCompensatePosX;
	iPosY -= iCompensatePosY;

	g_Motor.AxisRunS(1,iPosX,g_iMotorSpeed,1.00,0.001);
	Sleep(100);
	g_Motor.AxisRunS(2,iPosY,g_iMotorSpeed,1.00,0.001);
	Sleep(100);

	g_Motor.WaitStop(1);
	g_Motor.WaitStop(2);

	//����
	GT_ExOpt(OUT_EVT_BLUE_LAMP&OUT_EVT_GREEN_LAMP&OUT_EVT_RED_LAMP);

	::Sleep(500);

	if(!pDestImage->Create(iWinWidth,iWinHeight,24,0))
	{
		::AfxMessageBox("Create image object fail");
		return;
	}

	HDC hdc = pDestImage->GetDC();
	SetStretchBltMode(hdc,HALFTONE);

	CImage tempImage[1];
	if(!g_camera.GetImage(tempImage,1,iWinWidth,iWinHeight))
	{
		::AfxMessageBox("Get image from camera error");
		pDestImage->ReleaseDC();
		GT_ExOpt(OUT_ALL_STOP);//���
		return;
	}

	tempImage[0].StretchBlt(hdc,0,0,iWinWidth,iWinHeight,SRCCOPY);

	pDestImage->ReleaseDC();

	GT_ExOpt(OUT_ALL_STOP);//���
	
#else
	CImage* pTmpImage = new CImage();

	CImage* pSaveCurrentImage = m_pCurrentImage;
	m_pCurrentImage = new CImage();
	m_pCurrentImage->Load("../pcb.bmp");

	int iHeight = 768;
	int iFovHeight = 128;
	int iFovWidth = 170;
	double lNewPosX = fovRect.left * ::cos(0.007) - (iHeight - fovRect.top - iFovHeight)*sin(0.007);
	double lNewPosY = fovRect.top + (iHeight - fovRect.bottom) * (1-cos(0.007)) + fovRect.left * ::sin(0.007);

	double iCompensatePosX =0.00,iCompensatePosY =0.00; //�ƶ�λ�ò���
	if(bNeedCompensate)
	{
		//iCompensatePosY = fovRect.top * cos(0.007) + fovRect.left * ::sin(0.007) - fovRect.top;
		//iCompensatePosX = fovRect.left * ::cos(0.007) -(iHeight - fovRect.top - iFovHeight) * sin(0.007) - fovRect.left;
		iCompensatePosY = fovRect.left * sin(m_pTemplateData->m_dJiaoDuX);
		iCompensatePosX = (iHeight - fovRect.top - iFovHeight) * sin(m_pTemplateData->m_dJiaoDuY);
	}

	lNewPosX -= iCompensatePosX;
	lNewPosY += iCompensatePosY;

	CRect newFovRect((int)lNewPosX,(int)lNewPosY,(int)lNewPosX+iFovWidth+2,(int)lNewPosY+iFovHeight+2);

	GetFactImage(newFovRect,pTmpImage);

	m_pCurrentImage->Destroy();
	delete m_pCurrentImage;
	m_pCurrentImage = NULL;
	m_pCurrentImage = pSaveCurrentImage;

	CImage* pTmpImage2 = new CImage();
	pTmpImage2->Create((iFovWidth+2)*FOVMULTIPLE,(iFovHeight+2)*FOVMULTIPLE,pTmpImage->GetBPP(),0);

	HDC hImageDC = pTmpImage2->GetDC();
	SetStretchBltMode(hImageDC,HALFTONE);
	pTmpImage->StretchBlt(hImageDC,0,0,(iFovWidth+2)*FOVMULTIPLE,(iFovHeight+2)*FOVMULTIPLE,SRCCOPY);
	pTmpImage2->ReleaseDC();

	pTmpImage->Destroy();
	delete pTmpImage;
	pTmpImage = NULL;

	long lNewWidth = fovRect.Width()*FOVMULTIPLE;
	long lNewHeight = fovRect.Height()*FOVMULTIPLE;
	if(!pDestImage->Create(lNewWidth,lNewHeight,pTmpImage2->GetBPP(),0))
	{
		::AfxMessageBox("Create image object fail");
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		pTmpImage->Destroy();
		return;
	}

	double dt = lNewPosY - (double)((int)lNewPosY);

	hImageDC = pDestImage->GetDC();
	pTmpImage2->BitBlt(hImageDC,0,0,lNewWidth,lNewHeight,
		(lNewPosX - (double)((int)lNewPosX))*FOVMULTIPLE,(lNewPosY - (double)((int)lNewPosY))*FOVMULTIPLE, SRCCOPY);
	pDestImage->ReleaseDC();

	pTmpImage2->Destroy();
	delete pTmpImage2;
	pTmpImage2 = NULL;

#endif

}

// ����rect���ԭͼ��rect�����ͼ��
//imageΪ���ص�ͼ��
void CBaseBigGraphView::GetFactImage(const CRect rect, CImage* pImage)
{
	if(m_pCurrentImage == NULL || m_pCurrentImage->IsNull() || pImage == NULL)
		return;

	if( !pImage->IsNull())
	{
		pImage->Destroy();
	}

	::EnterCriticalSection(&m_Critial4CurrentImage);

	if(!pImage->Create(rect.Width(),rect.Height(),m_pCurrentImage->GetBPP()))
	{
		::AfxMessageBox("Create image object fail");
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return;
	}

	//ȡ����ɫ��
	if(m_pCurrentImage->IsDIBSection())
	{
		int MaxColors = m_pCurrentImage->GetMaxColorTableEntries();
		if(MaxColors>0)
		{
			RGBQUAD* ColorTable;
			ColorTable = new RGBQUAD[MaxColors];
			m_pCurrentImage->GetColorTable(1,MaxColors,ColorTable);
			m_TempImage.SetColorTable(0,MaxColors,ColorTable);
			delete []ColorTable;
		}
	}

	if(pImage->IsNull())
	{
		::LeaveCriticalSection(&m_Critial4CurrentImage);
		return;
	}

	m_pCurrentImage->BitBlt(pImage->GetDC(),0,0,rect.Width(),rect.Height(),rect.left,rect.top,SRCCOPY);
	pImage->ReleaseDC();

	::LeaveCriticalSection(&m_Critial4CurrentImage);
}

/* �������õ����ĵ�������������ĵ��ԭ��(��ĳ�Ա����)
*lNewCenterX:�����õ�x�����ĵ�(��Ҫ����ʵ�ʼ������������ĵ�)
*lNewCenterY:ͬ��
*/
void CBaseBigGraphView::ComputerCenterAndOrigin(long lNewCenterX, long lNewCenterY)
{
	CRect rc;
	GetClientRect(rc);

	::EnterCriticalSection(&m_Critial4CurrentImage);

	m_lCurrentCenterX = lNewCenterX;
	m_lCurrentCenterY = lNewCenterY;
	if(m_lCurrentCenterX < rc.Width()/2)
		m_lCurrentCenterX = rc.Width()/2;
	if(m_lCurrentCenterY < rc.Height()/2)
		m_lCurrentCenterY = rc.Height()/2;

	//ͼ������½��Ѿ��ʹ��ڵ����½��غϣ����ĵ㲻���������ƻ�����
	if( (m_TempImage.GetWidth()- m_lCurrentCenterX) < (rc.Width()/2) )
	{
		m_lCurrentCenterX = m_TempImage.GetWidth() - rc.Width()/2;
		//m_lCurrentCenterX = lNewCurrentCenterX/(1+m_iCurrentZoom*ZOOMSTEP);
	}
	if( ( m_TempImage.GetHeight() - m_lCurrentCenterY) < (rc.Height()/2) )
	{
		m_lCurrentCenterY = m_TempImage.GetHeight() - rc.Height()/2;
		//m_lCurrentCenterY = lNewCurrentCenterY/(1+m_iCurrentZoom*ZOOMSTEP);
	}

	//����Դ��ԭ��
	CPoint srcP=ComputerSrcOrigin( CPoint(m_lCurrentCenterX,m_lCurrentCenterY),
		CPoint(m_TempImage.GetWidth(),m_TempImage.GetHeight()),
		CPoint(rc.right-rc.left,rc.bottom-rc.top));
	m_lSrcOriginX = srcP.x;
	m_lSrcOriginY = srcP.y;

	::LeaveCriticalSection(&m_Critial4CurrentImage);
}

void CBaseBigGraphView::GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam)
{
	if (pImg->IsNull()) return;

	ppImgParam->nWidth   = pImg->GetWidth();
	ppImgParam->nHeight   = pImg->GetHeight();
	ppImgParam->nBitCount  = pImg->GetBPP();
	ppImgParam->nBytesPerLine   = (pImg->GetWidth()*pImg->GetBPP()+31)/32*4;
	ppImgParam->nBytesPerPixel   = pImg->GetBPP()/8;
	if (pImg->GetBPP()<=8) 
		ppImgParam->nNumColors= 1 << pImg->GetBPP();
	else 
		ppImgParam->nNumColors= 0;
	ppImgParam->nSize  = ppImgParam->nBytesPerLine*ppImgParam->nHeight;
}

int CBaseBigGraphView::InImage(CImage *pImg,int x,int y)
{
	struct	IMAGEPARAMENT  P;

	GetImageParament(pImg,&P);
 	if ((x<0)||(y<0)||(x>=P.nWidth)||(y>=P.nHeight))  return 0;
	else  return 1;
}

void CBaseBigGraphView::GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf)
{
	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
	int		i,dw,dh,x1,y1,alpha,delta,Dxb,dwb;

	GetImageParament(pImg,&P);
	if (P.nBitCount<8) return;
	x1=x;
	y1=y;
	alpha=delta=0;
	if (x<0)
	{ 
		alpha=-x;
		x1=0;
	}
	if (y<0)
	{ 
		delta=-y;
		y1=0;
	}

	if (!InImage(pImg,x1,y1)) return;
	dw=min(Dx,(int) P.nWidth-x1);
	dh=min(Dy,(int) P.nHeight-y1);
	dw -= alpha;
	dh -= delta;

	Dxb = Dx*P.nBytesPerPixel;
	dwb = dw*P.nBytesPerPixel;
	lp = (BYTE*) pImg->GetPixelAddress(x1,y1);
	buf += (delta*Dx+alpha)*P.nBytesPerPixel;
	for (i=0;i<dh;i++)
	{
		memcpy(buf,lp,dwb);
		buf += Dxb;	
		lp -= P.nBytesPerLine;
	}
}

void  CBaseBigGraphView::SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf)
{
	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
	int		i,dw,dh,x1,y1,alpha,delta,Dxb,dwb;
 
 	GetImageParament(pImg,&P);
	if (P.nBitCount<8) return;
	x1=x;
	y1=y;
	alpha=delta=0;
	if (x<0) { 
		alpha=-x;    x1=0;
	}
 	if (y<0) { 
		delta=-y;    y1=0;
	}
	if (!InImage(pImg,x1,y1)) return;
  	dw=min(Dx,(int) P.nWidth-x1);
	dh=min(Dy,(int) P.nHeight-y1);
	dw -= alpha;
	dh -= delta;

 	Dxb = Dx*P.nBytesPerPixel;
	dwb = dw*P.nBytesPerPixel;
 	lp = (BYTE*) pImg->GetPixelAddress(x1,y1);
	buf += (delta*Dx+alpha)*P.nBytesPerPixel;
	for (i=0;i<dh;i++) {
		memcpy(lp,buf,dwb);
 		buf += Dxb;
		lp -= P.nBytesPerLine;
	}
}

void CBaseBigGraphView::Flip(CImage *pImg,int flag)
{
	if(pImg == NULL || pImg->IsNull())
		return;

	int		i,j,k,m;
	DWORD	dd;
	BYTE	*buff1,*buff2;
 	struct	IMAGEPARAMENT P;

  	GetImageParament(pImg,&P);

 	buff1=(BYTE*) malloc(P.nBytesPerLine);
	buff2=(BYTE*) malloc(P.nBytesPerLine);
	if ((flag&0x1)==1)
	{
		m=(P.nWidth-1)*P.nBytesPerPixel;
		for (i=0;i<P.nHeight;i++)
		{
			GetRectValue(pImg,0,i,P.nWidth,1,buff1);
			for (j=0,k=0;j<P.nWidth/2;j++,k+=P.nBytesPerPixel)
			{
 				memcpy(&dd,&buff1[k],P.nBytesPerPixel);
				memcpy(&buff1[k],&buff1[m-k],P.nBytesPerPixel);
				memcpy(&buff1[m-k],&dd,P.nBytesPerPixel);
			}
 			SetRectValue(pImg,0,i,P.nWidth,1,buff1);
		}
	}

	if ((flag&0x2)==2)
	{
		for (i=0;i<P.nHeight/2;i++)
		{
			GetRectValue(pImg,0,i    ,P.nWidth,1,buff1);
			GetRectValue(pImg,0,P.nHeight-1-i,P.nWidth,1,buff2);
			SetRectValue(pImg,0,i    ,P.nWidth,1,buff2);
			SetRectValue(pImg,0,P.nHeight-1-i,P.nWidth,1,buff1);
		}
	}
	free(buff1);
	free(buff2);
}