// SmallGraphView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "SmallGraphView.h"


// CSmallGraphView �Ի���

IMPLEMENT_DYNAMIC(CSmallGraphView, CDialog)

CSmallGraphView::CSmallGraphView(CWnd* pParent /*=NULL*/)
	: CDialog(CSmallGraphView::IDD, pParent)
{

}

CSmallGraphView::~CSmallGraphView()
{
}

void CSmallGraphView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSmallGraphView, CDialog)
END_MESSAGE_MAP()


// CSmallGraphView ��Ϣ�������
// SmallGraphView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "SmallGraphView.h"


// CSmallGraphView

IMPLEMENT_DYNCREATE(CSmallGraphView, CFormView)

CSmallGraphView::CSmallGraphView()
	: CFormView(CSmallGraphView::IDD)
{

}

CSmallGraphView::~CSmallGraphView()
{
}

void CSmallGraphView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSmallGraphView, CFormView)
END_MESSAGE_MAP()


// CSmallGraphView ���

#ifdef _DEBUG
void CSmallGraphView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSmallGraphView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSmallGraphView ��Ϣ�������
