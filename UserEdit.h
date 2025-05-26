#pragma once
#include "afxwin.h"
#include "UserMgrDlg.h"

// CUserEdit 对话框

class CUserEdit : public CDialog
{
	DECLARE_DYNAMIC(CUserEdit)

public:
	CUserEdit(CUserMgrDlg* pParent = NULL);   // 标准构造函数
	virtual ~CUserEdit();

// 对话框数据
	enum { IDD = IDD_USEREDIT };

protected:
	CUserMgrDlg	*m_pUserMgrDlg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CString m_sUserName;
	CString m_sPassword;
	CString m_sConfirm;
	CButton m_ckPrg;
	CButton m_ckDetect;
	CButton m_ckReport;
	CButton m_ckSystem;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	virtual BOOL OnInitDialog();
};
