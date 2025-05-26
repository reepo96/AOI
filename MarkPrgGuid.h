#pragma once
#include "TemplateData.h"
#include "SpellPrgGuid.h"

enum MarkWorkStep{MWS_SEARCHAREA,MWS_BODY};

// CMarkPrgGuid �Ի���

class CMarkPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CMarkPrgGuid)
protected:
	CTemplateData* m_pTemplateData;
	CFovData *m_pFovData;
	CDatumMark *m_pCurrentMarkData;
	MarkWorkStep	m_CurrentWorkStep;//��ǰ��������

	CWnd		*m_pWnd;
	CSpellPrgGuid *m_pSpellGuid;
	int			m_iType;//0:�����ϵĻ�׼�㣬1:ƴ���ϵĻ�׼��	

public:
	CMarkPrgGuid(CWnd* pParent = NULL,int type=0);   // ��׼���캯��
	virtual ~CMarkPrgGuid();

// �Ի�������
	enum { IDD = IDD_MARKPRGGUID };

	void SetSpellGuid(CSpellPrgGuid *pSpell)
	{
		m_pSpellGuid = pSpell;
	}

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
	afx_msg void OnBnClickedCancle();
	void SetTemplateData(CTemplateData* pTemplateData);
};
