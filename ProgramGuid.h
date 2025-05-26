#pragma once

// CProgramGuid 编程向导

class CProgramGuid : public CDialog
{
	DECLARE_DYNAMIC(CProgramGuid)

public:
	CProgramGuid(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgramGuid();

// 对话框数据
	enum { IDD = IDD_PRGSTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CWnd *m_pBgiGraphView;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedNext();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
