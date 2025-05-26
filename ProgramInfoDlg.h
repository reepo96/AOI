#pragma once
#include "afxcmn.h"
#include "PCBInfoDlg.h"
#include "TemplateData.h"
#include "FOVInfoDlg.h"
#include "DatumMarkInfoDlg.h"
#include "OrganInfoDlg.h"
#include "SpellInfDlg.h"
#include "BarInfDlg.h"


// CProgramInfoDlg 对话框

//编程时显示元件等信息的对话框
class CProgramInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgramInfoDlg)

public:
	CProgramInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProgramInfoDlg();

// 对话框数据
	enum { IDD = IDD_PROGRAMINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CPCBInfoDlg	m_PcbInfoDlg;
	CFOVInfoDlg m_FOVInfoDlg;
	CDatumMarkInfoDlg	m_DatumMarkInfoDlg;
	COrganInfoDlg		m_OrganInfoDlg;
	CSpellInfDlg		m_SpellInfDlg;
	CBarInfDlg			m_BarInfDlg;

	CBigGraphView * m_pBigGraphView;

	CTemplateData *m_pTemplateData;

public:
	CTabCtrl m_TabCtrl;
public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void SetTemplateData(CTemplateData * pTemplateData);
public:
	void SetBigGraphView(CBigGraphView * pBigGraphView);
public:
	void PrgImportcad(void);
public:
	void NotifyPrgDataChanged(void);
};
