#pragma once
#include "TemplateData.h"
#include "afxcmn.h"

// COrganSelectDlg 对话框

class COrganSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(COrganSelectDlg)

protected:
	CTemplateData * m_pTemplateData;
	COrganData	**m_pSelectOrgan;
	PrgState	*m_pSelectState;//要选择特定状态的元件，如果为NULL，则全部
	int			m_iSelectStateNum;

public:
	COrganSelectDlg(CTemplateData * pTemplateData,COrganData **pOrgan,CWnd* pParent = NULL,PrgState *pSelectState=NULL,int iSelectStateNum=0);   // 标准构造函数
	virtual ~COrganSelectDlg();	

// 对话框数据
	enum { IDD = IDD_ORGANSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CListCtrl m_OrganList;

	int InsObjToList(COrganData* pOrganData);	
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();

	// 初始化列表控件
	void InitList(void);	
};
