// SmallGraView.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SmallGraView.h"


// CSmallGraView

IMPLEMENT_DYNCREATE(CSmallGraView, CGraphView)

CSmallGraView::CSmallGraView()
	: CGraphView(CSmallGraView::IDD)
{
	m_pBigGraphView = NULL;
	m_FlashRect = CRect(0,0,0,0);
	m_iFlashRectColor = 0;
	m_dMultiple = 0.0;

	m_iOriginX = 0;//原点x
	m_iOriginY = 0;//原点y
}

CSmallGraView::~CSmallGraView()
{
}

void CSmallGraView::DoDataExchange(CDataExchange* pDX)
{
	CGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSmallGraView, CFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSmallGraView 诊断

#ifdef _DEBUG
void CSmallGraView::AssertValid() const
{
	CGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CSmallGraView::Dump(CDumpContext& dc) const
{
	CGraphView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSmallGraView 消息处理程序


void CSmallGraView::OnDraw(CDC* /*pDC*/)
{
	//draw();
}

void CSmallGraView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect wndRc;

	EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
	for(;it!=m_pTemplateData->m_FOVMap.end();it++)
	{
		CRect imageRc((*it).second->left,(*it).second->top,(*it).second->right,(*it).second->bottom);
		ImageRect2WndRect( imageRc,wndRc);

		if(wndRc.PtInRect(point) && m_pBigGraphView !=NULL)	//点在FOV框内
		{
			//切换到FOV图片
			m_pBigGraphView->SetCurrentShowFov((*it).second);
			m_pBigGraphView->SetCurrentImage( &((*it).second->m_ImageObj),FOVIMAGE);
			
			m_FlashRect = wndRc;//闪动
			this->Invalidate();

			LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
			return;
		}
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	m_FlashRect = CRect(0,0,0,0);
	m_pBigGraphView->SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	this->Invalidate();

	CGraphView::OnLButtonDown(nFlags, point);
}

// 将图像坐标下的rect转换成窗口坐标下的rect
void CSmallGraView::ImageRect2WndRect(const CRect ImageRect, CRect& WndRect)
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

void CSmallGraView::SetBigGraphView(CBigGraphView * pBigGraphView)
{
	m_pBigGraphView = pBigGraphView;
}

void CSmallGraView::OnInitialUpdate()
{
	CGraphView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	this->SetTimer(1,1000,NULL);
}

void CSmallGraView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*m_iFlashRectColor +=50;
	if( m_iFlashRectColor>=250)
		m_iFlashRectColor = 0;

	if( !m_FlashRect.IsRectNull())
	{
		CRect rect = m_FlashRect;
		rect.InflateRect(3,3,3,3);
		this->InvalidateRect(&rect);
	}*/
	
	CGraphView::OnTimer(nIDEvent);
}

void CSmallGraView::OnPaint()
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
		CBrush *pBrush = new CBrush(g_FovColor);

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

	//画基准点
	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	map<long,CDatumMark*>::iterator MarkIt = m_pTemplateData->m_DatumMarkMap.begin();
	for(;MarkIt != m_pTemplateData->m_DatumMarkMap.end();MarkIt++)
	{
		//取得FOV区域坐标
		CRect fovRect((*MarkIt).second->m_pFovData->left,(*MarkIt).second->m_pFovData->top,
			(*MarkIt).second->m_pFovData->right,(*MarkIt).second->m_pFovData->bottom);
		
		//将基准点的坐标转换到缩略图模式
		double dWidthRate = (double)fovRect.Width()/(double)((*MarkIt).second->m_pFovData->m_ImageObj.GetWidth());
		double dHeightRate = (double)fovRect.Height()/(double)((*MarkIt).second->m_pFovData->m_ImageObj.GetHeight());

		CRect markRect(fovRect.left+(*MarkIt).second->left*dWidthRate,
			fovRect.top+(*MarkIt).second->top*dHeightRate,
			fovRect.left+(*MarkIt).second->right*dWidthRate,
			fovRect.top+(*MarkIt).second->bottom*dHeightRate);

		CRect markSearchRT(fovRect.left+(*MarkIt).second->searchLeft*dWidthRate,
			fovRect.top+(*MarkIt).second->searchTop*dHeightRate,
			fovRect.left+(*MarkIt).second->searchRight*dWidthRate,
			fovRect.top+(*MarkIt).second->searchBottom*dHeightRate);

		CBrush *pBrush = new CBrush(g_MarkColor);
		CRect bodyRect,newSearchRT;
		ImageRect2WndRect(markRect,bodyRect);
		ImageRect2WndRect(markSearchRT,newSearchRT);

		dc.FrameRect(&bodyRect,pBrush);
		dc.FrameRect(&newSearchRT,pBrush);
		delete pBrush;
		pBrush = NULL;
	}
	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	//突出选中的FOV
	if(m_FlashRect.Width()>0 && m_FlashRect.Height()>0)
	{
		CDC	maskDC;
		CBitmap mbm;
		maskDC.CreateCompatibleDC(&dc);
		mbm.CreateCompatibleBitmap(&dc, m_FlashRect.Width(), m_FlashRect.Height());
		maskDC.SelectObject(&mbm);
		maskDC.FillSolidRect(CRect(0,0,m_FlashRect.Width(),m_FlashRect.Height()),RGB(0x7F, 0x7F, 0x7F));
		dc.BitBlt(m_FlashRect.left,m_FlashRect.top,m_FlashRect.Width(),m_FlashRect.Height(),&maskDC,0,0,SRCAND);

		//CBrush *pOldBrush = (CBrush *)dc.SelectStockObject(NULL_BRUSH);
		//dc.Rectangle(&m_FlashRect);
		//dc.SelectObject(pOldBrush);
	}	
}

void CSmallGraView::NotifyPrgDataChanged(void)
{
	this->Invalidate();
}

void CSmallGraView::SetFlashRect(CFovData* pFovData)
{
	CRect wndRc;

	CRect imageRc(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);
	ImageRect2WndRect( imageRc,wndRc);
	m_FlashRect = wndRc;//闪动
	this->Invalidate();
}
