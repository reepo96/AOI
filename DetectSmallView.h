#pragma once
#include "GraphView.h"

// CDetectSmallView 对话框

class CDetectSmallView : public CGraphView
{
	DECLARE_DYNCREATE(CDetectSmallView)

protected:
	CRect		m_FlashRect;	//闪动的rect
	int			m_iFlashRectColor;
	double		m_dMultiple;
	int			m_iOriginX;//原点x
	int			m_iOriginY;//原点y

protected:
	CDetectSmallView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDetectSmallView();

public:
	enum { IDD = IDD_DETECTSMALLVIEW };
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
	// 将图像坐标下的rect转换成窗口坐标下的rect
	void ImageRect2WndRect(const CRect ImageRect, CRect& WndRect);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
public:
	void NotifyDetectDataChanged(void);
};
