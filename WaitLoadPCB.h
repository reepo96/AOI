#pragma once


// CWaitLoadPCB �Ի���

class CWaitLoadPCB : public CDialog
{
	DECLARE_DYNAMIC(CWaitLoadPCB)

public:
	CWaitLoadPCB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaitLoadPCB();

// �Ի�������
	enum { IDD = IDD_WAIT_LOADPCB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStop();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
