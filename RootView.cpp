// RootView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "RootView.h"


// CRootView

IMPLEMENT_DYNCREATE(CRootView, CFormView)

CRootView::CRootView()
	: CFormView(CRootView::IDD)
{

}

CRootView::~CRootView()
{
}

void CRootView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRootView, CFormView)
END_MESSAGE_MAP()


// CRootView ���

#ifdef _DEBUG
void CRootView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRootView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRootView ��Ϣ�������

void CRootView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
}
