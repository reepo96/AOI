#pragma once
#include "TemplateData.h"

enum BarWorkStep{BWS_SEARCHAREA,BWS_BODY};

// CBarPrgGuid 条码编程向导

class CBarPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CBarPrgGuid)
protected:
	CTemplateData* m_pTemplateData;
	CFovData *m_pFovData;
	CBarData *m_pCurrentBarData;
	BarWorkStep	m_CurrentWorkStep;//当前工作步骤

	bool		m_bDoNext;//计算完后是否处理下一步
	CWnd		*m_pWnd;

public:
	CBarPrgGuid(bool bDoNext=true,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarPrgGuid();

// 对话框数据
	enum { IDD = IDD_BARPRGGUID };

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
public:
	afx_msg void OnBnClickedCancle();
public:
	void SetTemplateData(CTemplateData* pTemplateData);
};
