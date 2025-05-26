#pragma once
#include "afxcmn.h"
#include "UserMgr.h"

// CUserMgrDlg �Ի���

class CUserMgrDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserMgrDlg)

public:
	CUserMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserMgrDlg();

// �Ի�������
	enum { IDD = IDD_USERMGR };

protected:
	vector<CUserInf*>	m_UserList;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedModify();
public:
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnBnClickedClose();
protected:
	CListCtrl m_UserListCtl;
public:
	void NewUser(CUserInf* pUser);
protected:
	void InitList(void);
};
