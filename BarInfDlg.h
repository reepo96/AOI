#pragma once
#include "TemplateData.h"

// CBarInfDlg 对话框

class CBarInfDlg : public CDialog
{
	DECLARE_DYNAMIC(CBarInfDlg)

public:
	CBarInfDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarInfDlg();

// 对话框数据
	enum { IDD = IDD_BARINFODLG };

protected:
	CTemplateData*	m_pTemplateData;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	long m_lSCenterX;
private:
	long m_lSCenterY;
private:
	long m_lBCenterY;
private:
	long m_lBCenterX;
private:
	long m_lSWidth;
private:
	long m_lBWidth;
private:
	long m_lSHeight;
private:
	long m_lBHeight;
public:
	void SetTemplateData(CTemplateData* pTemplateData);
	void NotifyPrgDataChanged(void);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedNew();
public:
	afx_msg void OnBnClickedModify();
public:
	afx_msg void OnBnClickedDelete();
};
