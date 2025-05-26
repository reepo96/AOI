// AOIView.h : CAOIView ��Ľӿ�
//


#pragma once


class CAOIView : public CView
{
protected: // �������л�����
	CAOIView();
	DECLARE_DYNCREATE(CAOIView)

// ����
public:
	CAOIDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CAOIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AOIView.cpp �еĵ��԰汾
inline CAOIDoc* CAOIView::GetDocument() const
   { return reinterpret_cast<CAOIDoc*>(m_pDocument); }
#endif

