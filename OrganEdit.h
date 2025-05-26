#pragma once
#include "OrganData.h"

// COrganEdit 对话框

class COrganEdit : public CDialog
{
	DECLARE_DYNAMIC(COrganEdit)

public:
	COrganEdit(COrganData **pOrganData,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrganEdit();

// 对话框数据
	enum { IDD = IDD_ORGANEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
