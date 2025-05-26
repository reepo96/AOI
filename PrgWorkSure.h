#pragma once


// CPrgWorkSure 确认当前的编程工作

class CPrgWorkSure : public CDialog
{
	DECLARE_DYNAMIC(CPrgWorkSure)

public:
	CPrgWorkSure(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrgWorkSure();

// 对话框数据
	enum { IDD = IDD_WORKSURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CWnd *m_pBgiGraphView;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
