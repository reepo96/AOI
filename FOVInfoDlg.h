#pragma once
#include "TemplateData.h"
#include "afxcmn.h"

// CFOVInfoDlg �Ի���

class CFOVInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFOVInfoDlg)

public:
	CFOVInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFOVInfoDlg();

// �Ի�������
	enum { IDD = IDD_FOVINFODLG };

protected:
	CTemplateData * m_pTemplateData;
	CFovData*		m_pSelectFovData;

	int m_iSelFovExcOrder;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);
	
	afx_msg void OnBnClickedNew();
public:
	virtual BOOL OnInitDialog();
protected:
	CListCtrl m_FovListCtrl;
	// ��ʼ���б�ؼ�
	void InitList(void);
	int InsObjToList(CFovData* pFovData);
public:
	afx_msg void OnLvnItemchangedFovlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeExcorder();
	void NotifyPrgDataChanged(void);
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
public:
	afx_msg void OnNMRclickFovlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedScandfov();
};
