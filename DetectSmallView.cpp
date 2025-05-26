// DetectSmallView.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectSmallView.h"


// CDetectSmallView 
IMPLEMENT_DYNCREATE(CDetectSmallView, CGraphView)

CDetectSmallView::CDetectSmallView()
	: CGraphView(CDetectSmallView::IDD)
{
	m_FlashRect = CRect(0,0,0,0);
	m_iFlashRectColor = 0;

	m_dMultiple = 1;
}

CDetectSmallView::~CDetectSmallView()
{
}

void CDetectSmallView::DoDataExchange(CDataExchange* pDX)
{
	CGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectSmallView, CFormView)	
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDetectSmallView 诊断

#ifdef _DEBUG
void CDetectSmallView::AssertValid() const
{
	CGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetectSmallView::Dump(CDumpContext& dc) const
{
	CGraphView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetectSmallView 消息处理程序


// 将图像坐标下的rect转换成窗口坐标下的rect
void CDetectSmallView::ImageRect2WndRect(const CRect ImageRect, CRect& WndRect)
{
	if(m_pTemplateData->m_ImageObj.IsNull() == false)
	{
		CRect wndRs; 
		this->GetClientRect(wndRs);

		WndRect.left = m_iOriginX + ImageRect.left/m_dMultiple;
		WndRect.right = m_iOriginX + ImageRect.right/m_dMultiple;
		WndRect.top = m_iOriginY + ImageRect.top/m_dMultiple;
		WndRect.bottom = m_iOriginY + ImageRect.bottom/m_dMultiple;
	}
}


void CDetectSmallView::OnInitialUpdate()
{
	CGraphView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	this->SetTimer(1,1000,NULL);
}

void CDetectSmallView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_iFlashRectColor +=50;
	if( m_iFlashRectColor>=250)
		m_iFlashRectColor = 0;

	if( !m_FlashRect.IsRectNull())
	{
		CRect rect = m_FlashRect;
		rect.InflateRect(3,3,3,3);
		this->InvalidateRect(&rect);
	}
	
	CGraphView::OnTimer(nIDEvent);
}

void CDetectSmallView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	if(m_pTemplateData == NULL)
		return;

	if(m_pTemplateData->m_ImageObj.IsNull())
		return;
	
	CRect rs; 
	this->GetClientRect(rs);

	//计算缩小倍数
	double dWidthMultiple = (double)m_pTemplateData->m_ImageObj.GetWidth()/(rs.right-rs.left);
	double dHeightMultiple = (double)m_pTemplateData->m_ImageObj.GetHeight()/(rs.bottom-rs.top);
	m_dMultiple = (dWidthMultiple>dHeightMultiple)?dWidthMultiple:dHeightMultiple;

	//计算原点
	int iWidth = (double)m_pTemplateData->m_ImageObj.GetWidth()/m_dMultiple;
	int iHeight = (double)m_pTemplateData->m_ImageObj.GetHeight()/m_dMultiple;

	m_iOriginX = ((rs.right-rs.left) - iWidth)/2;
	m_iOriginY = ((rs.bottom-rs.top) - iHeight)/2;

	SetStretchBltMode(dc.m_hDC,HALFTONE);
	m_pTemplateData->m_ImageObj.StretchBlt(dc.m_hDC,m_iOriginX,m_iOriginY,iWidth,iHeight,SRCCOPY);//放大缩小

	//画出编辑区域
	if(m_pTemplateData->m_editAreaBottom > 0)
	{
		CBrush *pBrush = new CBrush(g_EditAreaColor);

		//取得编辑区域坐标（图像的坐标体系）
		CRect rect(m_pTemplateData->m_editAreaLeft,m_pTemplateData->m_editAreaTop,
			m_pTemplateData->m_editAreaRight,m_pTemplateData->m_editAreaBottom);		

		CRect newRect;
		ImageRect2WndRect(rect,newRect);			

		dc.FrameRect(&newRect,pBrush);
		delete pBrush;
		pBrush = NULL;
	}

	//画FOV区域
	EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
	for(;it != m_pTemplateData->m_FOVMap.end();it++)
	{
		CBrush *pBrush = new CBrush(HS_BDIAGONAL,g_FovColor);

		//取得FOV区域坐标（图像的坐标体系）
		CRect rect((*it).second->left,(*it).second->top,(*it).second->right,(*it).second->bottom);

		CRect newRect;
		ImageRect2WndRect(rect,newRect);			

		dc.FrameRect(&newRect,pBrush);

		CString sOrder;
		sOrder.Format("%d",(*it).second->CheckOrder);
		dc.TextOutA(newRect.CenterPoint().x-3,newRect.CenterPoint().y-3,sOrder);
		delete pBrush;
		pBrush = NULL;
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	//画闪动的rect
	if( !m_FlashRect.IsRectNull())
	{
		CBrush *pBrush = new CBrush(RGB(m_iFlashRectColor,0,0));
		dc.FrameRect(&m_FlashRect,pBrush);
		delete pBrush;
		pBrush = NULL;
	}
}

void CDetectSmallView::NotifyDetectDataChanged(void)
{
}
