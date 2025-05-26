#pragma once


// CWaitDlg 对话框

class CWaitDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitDlg)

public:
	CWaitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaitDlg();

// 对话框数据
	enum { IDD = IDD_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
