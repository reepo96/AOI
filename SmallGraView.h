#pragma once

#include "GraphView.h"
#include "BigGraphView.h"

// CSmallGraView 窗体视图

class CSmallGraView : public CGraphView
{
	DECLARE_DYNCREATE(CSmallGraView)

protected:
	CBigGraphView	*m_pBigGraphView;

	CRect		m_FlashRect;	//闪动的rect
	int			m_iFlashRectColor;
	double		m_dMultiple;

	int			m_iOriginX;//原点x
	int			m_iOriginY;//原点y

protected:
	CSmallGraView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	// 将图像坐标下的rect转换成窗口坐标下的rect
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


