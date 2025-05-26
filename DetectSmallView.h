#pragma once
#include "GraphView.h"

// CDetectSmallView �Ի���

class CDetectSmallView : public CGraphView
{
	DECLARE_DYNCREATE(CDetectSmallView)

protected:
	CRect		m_FlashRect;	//������rect
	int			m_iFlashRectColor;
	double		m_dMultiple;
	int			m_iOriginX;//ԭ��x
	int			m_iOriginY;//ԭ��y

protected:
	CDetectSmallView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	// ��ͼ�������µ�rectת���ɴ��������µ�rect
	void ImageRect2WndRect(const CRect ImageRect, CRect& WndRect);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
public:
	void NotifyDetectDataChanged(void);
};
