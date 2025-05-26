#pragma once
#include "afxwin.h"
#include "UserMgrDlg.h"

// CUserEdit �Ի���

class CUserEdit : public CDialog
{
	DECLARE_DYNAMIC(CUserEdit)

public:
	CUserEdit(CUserMgrDlg* pParent = NULL);   // ��׼���캯��
	virtual ~CUserEdit();

// �Ի�������
	enum { IDD = IDD_USEREDIT };

protected:
	CUserMgrDlg	*m_pUserMgrDlg;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
