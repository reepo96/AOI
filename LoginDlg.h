#pragma once


// CLoginDlg �Ի���

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	int m_iCount;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CString m_sUserName;
	CString m_sPassword;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	virtual BOOL OnInitDialog();
};
