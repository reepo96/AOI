#pragma once
#include "afxcmn.h"
#include "TemplateData.h"

// CSpellInfDlg 对话框

class CSpellInfDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpellInfDlg)

public:
	CSpellInfDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSpellInfDlg();

// 对话框数据
	enum { IDD = IDD_SPELLINFODLG };

protected:
	CTemplateData*	m_pTemplateData;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CListCtrl m_SpellListCtrl;
public:
	void SetTemplateData(CTemplateData* pTemplateData);
protected:
	void InitList(void);
public:
	void NotifyPrgDataChanged(void);
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedDelete();
};
