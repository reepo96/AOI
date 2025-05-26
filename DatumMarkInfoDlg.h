#pragma once
#include "TemplateData.h"
#include "afxcmn.h"

// CDatumMarkInfoDlg 对话框

class CDatumMarkInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CDatumMarkInfoDlg)

public:
	CDatumMarkInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDatumMarkInfoDlg();

// 对话框数据
	enum { IDD = IDD_DATUMMARKINFODLG };

protected:
	CTemplateData * m_pTemplateData;
	CDatumMark*		m_pSelectDatumMark;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CListCtrl m_MarkListCtrl;
	void InitList(void);
	int InsObjToList(CDatumMark* pDatumMark);
public:
	void SetTemplateData(CTemplateData * pTemplateData);
	afx_msg void OnLvnItemchangedDatummarklist(NMHDR *pNMHDR, LRESULT *pResult);
	void NotifyPrgDataChanged(void);
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedModify();
public:
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnBnClickedShowimage();
public:
	afx_msg void OnNMRclickDatummarklist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnShowmarkimage();
protected:
	long m_lLimit;
public:
	afx_msg void OnEnChangeLimit();
};
