// AOIView.cpp : CAOIView ���ʵ��
//

#include "stdafx.h"
#include "AOI.h"

#include "AOIDoc.h"
#include "AOIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAOIView

IMPLEMENT_DYNCREATE(CAOIView, CView)

BEGIN_MESSAGE_MAP(CAOIView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAOIView ����/����

CAOIView::CAOIView()
{
	// TODO: �ڴ˴���ӹ������

}

CAOIView::~CAOIView()
{
}

BOOL CAOIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAOIView ����

void CAOIView::OnDraw(CDC* /*pDC*/)
{
	CAOIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAOIView ��ӡ

BOOL CAOIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAOIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAOIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CAOIView ���

#ifdef _DEBUG
void CAOIView::AssertValid() const
{
	CView::AssertValid();
}

void CAOIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAOIDoc* CAOIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAOIDoc)));
	return (CAOIDoc*)m_pDocument;
}
#endif //_DEBUG


// CAOIView ��Ϣ�������
