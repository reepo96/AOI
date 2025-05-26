// MyRectTracker.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "MyRectTracker.h"


// CMyRectTracker

CMyRectTracker::CMyRectTracker()
{	
}

CMyRectTracker::CMyRectTracker(LPCRECT lpSrcRect,UINT nStyle )
{
	CRectTracker::CRectTracker(lpSrcRect,nStyle);

	ASSERT(AfxIsValidAddress(lpSrcRect, sizeof(RECT), FALSE));

	Construct();
	m_rect.CopyRect(lpSrcRect);
	m_nStyle = nStyle;
}

CMyRectTracker::~CMyRectTracker()
{
}


// CMyRectTracker 消息处理程序

void CMyRectTracker::OnChangedRect(const CRect& rectOld )
{
	::AfxMessageBox("OnChangedRect begin");
}
