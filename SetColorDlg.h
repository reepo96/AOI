#pragma once
#include "afxwin.h"


// CSetColorDlg 对话框

class CSetColorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetColorDlg)

public:
	CSetColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetColorDlg();	

// 对话框数据
	enum { IDD = IDD_SETCOLORDLG };

protected:

	COLORREF	m_EditAreaColor;
	COLORREF	m_FovColor;
	COLORREF	m_MarkColor;
	COLORREF	m_OrganColor;
	COLORREF	m_BarCodeColor;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CButton m_btnEditAreaColor;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnBnClickedColorEdita();
public:
	afx_msg void OnBnClickedColorFov();
public:
	afx_msg void OnBnClickedColorMark();
public:
	afx_msg void OnBnClickedColorOrgan();
public:
	afx_msg void OnBnClickedColorBarcode();
public:
	afx_msg void OnBnClickedSave();
public:
	afx_msg void OnBnClickedCancel();
};
