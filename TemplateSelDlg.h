#pragma once
#include "afxcmn.h"
#include "TemplateData.h"

// CTemplateSelDlg �Ի���

class CTemplateSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTemplateSelDlg)

public:
	CTemplateSelDlg(long* pTemplateID,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTemplateSelDlg();

// �Ի�������
	enum { IDD = IDD_TEMPLATESELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CListCtrl m_TemplateListCtrl;
	long	*m_pTemplateID;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	virtual BOOL OnInitDialog();
protected:
	void InsObjToList(CTemplateData* pTemplateData);
public:
	afx_msg void OnBnClickedDelete();
};
