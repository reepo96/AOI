#pragma once


// CPrgWorkSure ȷ�ϵ�ǰ�ı�̹���

class CPrgWorkSure : public CDialog
{
	DECLARE_DYNAMIC(CPrgWorkSure)

public:
	CPrgWorkSure(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPrgWorkSure();

// �Ի�������
	enum { IDD = IDD_WORKSURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CWnd *m_pBgiGraphView;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
