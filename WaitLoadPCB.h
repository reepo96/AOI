#pragma once


// CWaitLoadPCB 对话框

class CWaitLoadPCB : public CDialog
{
	DECLARE_DYNAMIC(CWaitLoadPCB)

public:
	CWaitLoadPCB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaitLoadPCB();

// 对话框数据
	enum { IDD = IDD_WAIT_LOADPCB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStop();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
