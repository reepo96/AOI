#pragma once

#include "TemplateData.h"
#include "FactOrganImage.h"

// CGraphView ��ʾͼ�ε���ͼ

class CGraphView : public CFormView
{
	DECLARE_DYNCREATE(CGraphView)

protected:
	CGraphView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	CGraphView(UINT nIDTemplate);  
	virtual ~CGraphView();

protected:
	CTemplateData	*m_pTemplateData;	//ģ������

	CRectTracker *m_pRectTracker;	//ѡ����ο�

	CFactOrganImage	*m_pFactOranImageDlg;	//��ʾʵ��ͼ��ĶԻ���

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//	virtual void OnDraw(CDC* /*pDC*/);

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);
protected:
	// ��������С
	void ZoomOut(CImage* pSrcImage, CImage* pDestImage, int iMultiple);
};


