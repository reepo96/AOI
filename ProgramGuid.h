#pragma once

// CProgramGuid �����

class CProgramGuid : public CDialog
{
	DECLARE_DYNAMIC(CProgramGuid)

public:
	CProgramGuid(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProgramGuid();

// �Ի�������
	enum { IDD = IDD_PRGSTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
