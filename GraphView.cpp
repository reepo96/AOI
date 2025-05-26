// GraphView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "GraphView.h"
#include "MainView.h"

// CGraphView

IMPLEMENT_DYNCREATE(CGraphView, CFormView)

CGraphView::CGraphView()
	: CFormView(CGraphView::IDD)
{
	m_pRectTracker = NULL;
	m_pFactOranImageDlg = NULL;
}
CGraphView::CGraphView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	m_pRectTracker = NULL;
	m_pFactOranImageDlg = NULL;	
}

CGraphView::~CGraphView()
{
}


void CGraphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGraphView, CFormView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CGraphView ���

#ifdef _DEBUG
void CGraphView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGraphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGraphView ��Ϣ�������
void CGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFormView::OnLButtonDown(nFlags, point);
}

BOOL CGraphView::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
		{
			return FALSE;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CGraphView::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;
}

/* ��������С
*pSrcImage:ԭͼ��
*pDestImage:Ŀ��ͼ��
*iMultiple:��С����
*/
void CGraphView::ZoomOut(CImage* pSrcImage, CImage* pDestImage, int iMultiple)
{
	if(!pDestImage->IsNull())
	{
		//pDestImage->Detach();
		pDestImage->Destroy();
	}
	
	if(!pDestImage->Create(pSrcImage->GetWidth()/iMultiple,pSrcImage->GetHeight()/iMultiple,pSrcImage->GetBPP(),0))
	{
		::AfxMessageBox("Create image object fail");
		return;
	}

	SetStretchBltMode(pDestImage->GetDC(),HALFTONE);
	pDestImage->ReleaseDC();
	pSrcImage->StretchBlt(pDestImage->GetDC(),0,0,pSrcImage->GetWidth()/iMultiple,pSrcImage->GetHeight()/iMultiple,SRCCOPY);	
	pDestImage->ReleaseDC();

	/*//ȡ����ɫ��
	if(pSrcImage->IsDIBSection())
	{
		int MaxColors = pSrcImage->GetMaxColorTableEntries();
		if(MaxColors>0)
		{
			RGBQUAD* ColorTable;
			ColorTable = new RGBQUAD[MaxColors];
			pSrcImage->GetColorTable(1,MaxColors,ColorTable);
			pDestImage->SetColorTable(0,MaxColors,ColorTable);
			delete []ColorTable;
		}
	}

	for(i=0;i<pDestImage->GetHeight();i++)
	{
		//���е�ַ
		destBuf = (BYTE*)pDestImage->GetPixelAddress(0,i);
		srcBuf = (BYTE*)pSrcImage->GetPixelAddress(0,i*iMultiple);
		for(j=0;j<pDestImage->GetWidth();j++)
		{
			//������������
			::memcpy(destBuf,srcBuf,pSrcImage->GetBPP()/8);
			destBuf += pSrcImage->GetBPP()/8;
			srcBuf += iMultiple*(pSrcImage->GetBPP()/8);
		}
	}*/
	
}
