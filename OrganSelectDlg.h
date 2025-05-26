#pragma once
#include "TemplateData.h"
#include "afxcmn.h"

// COrganSelectDlg �Ի���

class COrganSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(COrganSelectDlg)

protected:
	CTemplateData * m_pTemplateData;
	COrganData	**m_pSelectOrgan;
	PrgState	*m_pSelectState;//Ҫѡ���ض�״̬��Ԫ�������ΪNULL����ȫ��
	int			m_iSelectStateNum;

public:
	COrganSelectDlg(CTemplateData * pTemplateData,COrganData **pOrgan,CWnd* pParent = NULL,PrgState *pSelectState=NULL,int iSelectStateNum=0);   // ��׼���캯��
	virtual ~COrganSelectDlg();	

// �Ի�������
	enum { IDD = IDD_ORGANSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CListCtrl m_OrganList;

	int InsObjToList(COrganData* pOrganData);	
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();

	// ��ʼ���б�ؼ�
	void InitList(void);	
};
