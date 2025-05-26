#pragma once
#include "TemplateData.h"
#include "SpellPrgGuid.h"

enum MarkWorkStep{MWS_SEARCHAREA,MWS_BODY};

// CMarkPrgGuid 对话框

class CMarkPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CMarkPrgGuid)
protected:
	CTemplateData* m_pTemplateData;
	CFovData *m_pFovData;
	CDatumMark *m_pCurrentMarkData;
	MarkWorkStep	m_CurrentWorkStep;//当前工作步骤

	CWnd		*m_pWnd;
	CSpellPrgGuid *m_pSpellGuid;
	int			m_iType;//0:基板上的基准点，1:拼板上的基准点	

public:
	CMarkPrgGuid(CWnd* pParent = NULL,int type=0);   // 标准构造函数
	virtual ~CMarkPrgGuid();

// 对话框数据
	enum { IDD = IDD_MARKPRGGUID };

	void SetSpellGuid(CSpellPrgGuid *pSpell)
	{
		m_pSpellGuid = pSpell;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
