#pragma once

#include "TemplateData.h"


// CMyTreeView 窗体视图

class CMyTreeView : public CFormView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyTreeView();

	CTreeCtrl* m_pTree;

	CTemplateData	*m_pTemplateData;	//模板数据

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void ReSelectCurrent(void);
};


