#pragma once

#include "GraphView.h"
#include "BigGraphView.h"

// CSmallGraView ������ͼ

class CSmallGraView : public CGraphView
{
	DECLARE_DYNCREATE(CSmallGraView)

protected:
	CBigGraphView	*m_pBigGraphView;

	CRect		m_FlashRect;	//������rect
	int			m_iFlashRectColor;
	double		m_dMultiple;

	int			m_iOriginX;//ԭ��x
	int			m_iOriginY;//ԭ��y

protected:
	CSmallGraView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSmallGraView();

public:
	enum { IDD = IDD_SMALLGRAPHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	// ��ͼ�������µ�rectת���ɴ��������µ�rect
	void ImageRect2WndRect(const CRect ImageRect, CRect& WndRect);
public:
	void SetBigGraphView(CBigGraphView * pBigGraphView);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnPaint();
public:
	void NotifyPrgDataChanged(void);
public:
	void SetFlashRect(CFovData* pFovData);
};


