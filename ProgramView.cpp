// ProgramView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "ProgramView.h"


// CProgramView

IMPLEMENT_DYNCREATE(CProgramView, CFormView)

CProgramView::CProgramView()
	: CFormView(CProgramView::IDD)
{

}

CProgramView::~CProgramView()
{
}

void CProgramView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProgramView, CFormView)
END_MESSAGE_MAP()


// CProgramView ���

#ifdef _DEBUG
void CProgramView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProgramView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProgramView ��Ϣ�������
