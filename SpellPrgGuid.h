#pragma once
#include "TemplateData.h"

enum SpellWorkStep{SWS_SPELLPSTEP1,SWS_SPELLPSTEP2,SWS_SPELLPSTEP3};
// CSpellPrgGuid ƴ������

class CSpellPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CSpellPrgGuid)

protected:
	CTemplateData * m_pTemplateData;
	SpellWorkStep	m_CurrentWorkStep;//��ǰ��������

	CWnd *m_pWnd;

	CRect m_rect1;
	CRect m_rect2;
	CFovData *m_pFov1;
	CFovData *m_pFov2;

	CSpellPCB *m_pSpell;

public:
	CSpellPrgGuid(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpellPrgGuid();

// �Ի�������
	enum { IDD = IDD_SPELLPRGGUID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetTemplateData(CTemplateData * pTemplateData);
	void EndCurrentWork(void);
	void NextStep(void);

	//��׼�������
	void MarkPrgComplete(CDatumMark *pMark);

protected:
	void NewFov(CFovData **ppFov);

	bool GetFovPrgData(CFovData *pFov);

public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
};
