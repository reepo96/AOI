#pragma once
#include "afxcmn.h"
#include "TemplateData.h"

// CTemplateSelDlg 对话框

class CTemplateSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTemplateSelDlg)

public:
	CTemplateSelDlg(long* pTemplateID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTemplateSelDlg();

// 对话框数据
	enum { IDD = IDD_TEMPLATESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CListCtrl m_TemplateListCtrl;
	long	*m_pTemplateID;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
protected:
	void InsObjToList(CTemplateData* pTemplateData);
public:
	afx_msg void OnBnClickedDelete();
};
