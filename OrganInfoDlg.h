#pragma once
#include "afxcmn.h"
#include "TemplateData.h"
#include "BigGraphView.h"

// COrganInfoDlg 对话框

class COrganInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(COrganInfoDlg)

public:
	COrganInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrganInfoDlg();

// 对话框数据
	enum { IDD = IDD_ORGANINFODLG };

protected:
	CTemplateData * m_pTemplateData;

	CBigGraphView	*m_pBigGraphView;	//指向画图窗口

	COrganData		*m_pSelectOrganData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_OrganList;
public:
	virtual BOOL OnInitDialog();
public:
	void SetTemplateData(CTemplateData * pTemplateData);
	void NotifyPrgDataChanged(void);
protected:
	// 初始化列表控件
	void InitList(void);
public:
	int InsObjToList(COrganData* pOrganData);
public:
	afx_msg void OnBnClickedImport();
protected:
	// 分析一行数据，得到一个元件
	void parse(char* pLine);
public:
	afx_msg void OnBnClickedCalcrelative();
public:
	void SetBigGraphView(CBigGraphView * pBigGraphView);
public:
	afx_msg void OnBnClickedImage();
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedModify();
public:
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnHdnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnEnChangeLimit();
protected:
	int m_iSelectOrganLimit;
public:
	afx_msg void OnNMCustomdrawOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	bool OrganIsInFov(const CFovData* pFovData, const COrganData* pOrganData);
public:
	afx_msg void OnNMRclickOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedProgram();
public:
	afx_msg void OnBnClickedDelprg();
public:
	afx_msg void OnCbnSelchangeIspolar();
public:
	CString m_sIsPolar;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
