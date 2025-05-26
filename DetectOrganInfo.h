#pragma once
#include "afxcmn.h"
#include "TemplateData.h"
#include "afxwin.h"


// CDetectOrganInfo 检测时的元件信息

class CDetectOrganInfo : public CDialog
{
	DECLARE_DYNAMIC(CDetectOrganInfo)

public:
	CDetectOrganInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetectOrganInfo();

// 对话框数据
	enum { IDD = IDD_DETECTORGANINFO };

protected:
	CWnd			*m_pDBigView;
	CTemplateData	*m_pTemplateData;

	COrganData		*m_pCurrentTOrgan;	//当前检测的元件
	COrganData		*m_pSelectOrganData;

public:
	void SetTemplateData(CTemplateData *pTemplateData);
	
	void SetDBigView(CWnd *pDBigView)
	{
		m_pDBigView = pDBigView;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CListCtrl m_OrganListCtl;
public:
	void NotifyDetectDataChanged(void);
public:
	virtual BOOL OnInitDialog();
protected:	
	void InitList(void);
	void InsObjToList(COrganData* pOrganData);
public:
	afx_msg void OnNMCustomdrawOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	CButton m_btnNeedDetect;
public:
	afx_msg void OnBnClickedNeedDetect();
protected:
	BOOL m_bNeedDetect;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
