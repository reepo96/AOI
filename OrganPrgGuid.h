#pragma once


// COrganPrgGuid Ԫ�������

class COrganPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(COrganPrgGuid)

public:
	COrganPrgGuid(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrganPrgGuid();

// �Ի�������
	enum { IDD = IDD_ORGANPRGSTEP };

protected:
	CWnd	*m_pWnd;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
