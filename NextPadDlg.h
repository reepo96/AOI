#pragma once

#include "OrganProgramDlg.h"

// CNextPadDlg �Ի���

class CNextPadDlg : public CDialog
{
	DECLARE_DYNAMIC(CNextPadDlg)

public:
	CNextPadDlg(int *piSelect,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNextPadDlg();

// �Ի�������
	enum { IDD = IDD_NEXTPAD };

protected:
	COrganProgramDlg	*m_pOrganPrgDlg;
	int					*m_piSelect;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
};
