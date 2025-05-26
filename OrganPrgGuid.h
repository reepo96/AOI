#pragma once


// COrganPrgGuid 元件编程向导

class COrganPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(COrganPrgGuid)

public:
	COrganPrgGuid(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrganPrgGuid();

// 对话框数据
	enum { IDD = IDD_ORGANPRGSTEP };

protected:
	CWnd	*m_pWnd;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
