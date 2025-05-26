// AOIView.cpp : CAOIView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAOIView 构造/析构

CAOIView::CAOIView()
{
	// TODO: 在此处添加构造代码

}

CAOIView::~CAOIView()
{
}

BOOL CAOIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAOIView 绘制

void CAOIView::OnDraw(CDC* /*pDC*/)
{
	CAOIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CAOIView 打印

BOOL CAOIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAOIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAOIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CAOIView 诊断

#ifdef _DEBUG
void CAOIView::AssertValid() const
{
	CView::AssertValid();
}

void CAOIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAOIDoc* CAOIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAOIDoc)));
	return (CAOIDoc*)m_pDocument;
}
#endif //_DEBUG


// CAOIView 消息处理程序
