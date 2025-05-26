#pragma once

#include "TemplateData.h"


// CMyTreeView ������ͼ

class CMyTreeView : public CFormView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyTreeView();

	CTreeCtrl* m_pTree;

	CTemplateData	*m_pTemplateData;	//ģ������

public:
	enum { IDD = IDD_MYTREEVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void SetTemplateData(CTemplateData *pTemplateData)
	{
		m_pTemplateData = pTemplateData;
	}

	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void ReSelectCurrent(void);
};


