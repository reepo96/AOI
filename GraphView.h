#pragma once

#include "TemplateData.h"
#include "FactOrganImage.h"

// CGraphView 显示图形的视图

class CGraphView : public CFormView
{
	DECLARE_DYNCREATE(CGraphView)

protected:
	CGraphView();           // 动态创建所使用的受保护的构造函数
	CGraphView(UINT nIDTemplate);  
	virtual ~CGraphView();

protected:
	CTemplateData	*m_pTemplateData;	//模板数据

	CRectTracker *m_pRectTracker;	//选择矩形框

	CFactOrganImage	*m_pFactOranImageDlg;	//显示实际图像的对话框

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	enum { IDD = IDD_GRAPHVIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual void OnDraw(CDC* /*pDC*/);

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);
protected:
	// 整数倍缩小
	void ZoomOut(CImage* pSrcImage, CImage* pDestImage, int iMultiple);
};


