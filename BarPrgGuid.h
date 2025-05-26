#pragma once
#include "TemplateData.h"

enum BarWorkStep{BWS_SEARCHAREA,BWS_BODY};

// CBarPrgGuid ��������

class CBarPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CBarPrgGuid)
protected:
	CTemplateData* m_pTemplateData;
	CFovData *m_pFovData;
	CBarData *m_pCurrentBarData;
	BarWorkStep	m_CurrentWorkStep;//��ǰ��������

	bool		m_bDoNext;//��������Ƿ�����һ��
	CWnd		*m_pWnd;

public:
	CBarPrgGuid(bool bDoNext=true,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBarPrgGuid();

// �Ի�������
	enum { IDD = IDD_BARPRGGUID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	void EndCurrentWork(void);
	void NextStep(void);

public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
public:
	void SetTemplateData(CTemplateData* pTemplateData);
};
