#pragma once
#include "OrganData.h"

// COrganEdit �Ի���

class COrganEdit : public CDialog
{
	DECLARE_DYNAMIC(COrganEdit)

public:
	COrganEdit(COrganData **pOrganData,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrganEdit();

// �Ի�������
	enum { IDD = IDD_ORGANEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	COrganData	*m_pOrganData;

	CString m_sName;
	CString m_sFootPrint;
	double m_dMidX;
	double m_dMidY;
	int m_iLimit;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
protected:
	double m_dPadX;
	double m_dPadY;
public:
	virtual BOOL OnInitDialog();
};
