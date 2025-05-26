#pragma once

#include "OrganProgramDlg.h"

// CNextPadDlg 对话框

class CNextPadDlg : public CDialog
{
	DECLARE_DYNAMIC(CNextPadDlg)

public:
	CNextPadDlg(int *piSelect,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNextPadDlg();

// 对话框数据
	enum { IDD = IDD_NEXTPAD };

protected:
	COrganProgramDlg	*m_pOrganPrgDlg;
	int					*m_piSelect;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
