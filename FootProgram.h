#pragma once


// CFootProgram 对话框

#include "OrganProgramDlg.h"

class CFootProgram : public CDialog
{
	DECLARE_DYNAMIC(CFootProgram)

public:
	CFootProgram(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFootProgram();

// 对话框数据
	enum { IDD = IDD_FOOTPRG };

protected:
	COrganProgramDlg	*m_pOrganPrgDlg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CMemoryState   CM1,CM2,CM3;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFootnumAdd();
public:
	afx_msg void OnBnClickedFootnumDown();
public:
	afx_msg void OnBnClickedSpaceAddM();
public:
	afx_msg void OnBnClickedSpaceAdd();
public:
	afx_msg void OnBnClickedSpaceDown();
public:
	afx_msg void OnBnClickedSpaceDownM();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
protected:
	int m_iFootNum;
};
