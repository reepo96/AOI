#pragma once

#include "TemplateData.h"

// CPCBInfoDlg 对话框

class CPCBInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CPCBInfoDlg)

public:
	CPCBInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPCBInfoDlg();

// 对话框数据
	enum { IDD = IDD_PCBINFODLG };

protected:
	CTemplateData * m_pTemplateData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void SetTemplateData(CTemplateData * pTemplateData);
public:
	CString m_sPCBName;
public:
	long m_lPCBWidth;
public:
	long m_lPCBHeight;
	long m_lOldPCBHeight;
public:
	afx_msg void OnEnKillfocusPcbname();
public:
	afx_msg void OnEnKillfocusWidth();
public:
	afx_msg void OnEnKillfocusHeight();
protected:
	CString m_sLastPrgTime;
	CString m_sLastPrgStep;
	int m_iFovNum;
	int m_iOrganNum;
	int m_iMarkNum;
	CString m_sTemplateType;
public:
	afx_msg void OnEnChangeHeight();
public:
	afx_msg void OnCbnSelchangeTemptype();
public:
	afx_msg void OnEnChangeWidth();
protected:
	double m_dJiaoDuX;
	double m_dJiaoDuY;
};
