#pragma once


// CAngleOffsetPrgGuid 对话框

class CAngleOffsetPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CAngleOffsetPrgGuid)

public:
	enum PrgWorkStep{PWS_STEP1,PWS_STEP2,PWS_STEP3,PWS_STEP4,PWS_STEP5,PWS_STEP6};

protected:
	CTemplateData * m_pTemplateData;
	PrgWorkStep	m_CurrentWorkStep;//当前工作步骤

	CWnd *m_pWnd;

	CRect m_rect1;
	CRect m_rect2;
	CFovData *m_pFov1;
	CFovData *m_pFov2;

public:
	CAngleOffsetPrgGuid(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAngleOffsetPrgGuid();

// 对话框数据
	enum { IDD = IDD_ANGLEOFFSETPRGGUID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	void EndCurrentWork(void);
	void NextStep(void);

	// 计算角度
	//type=1计算x轴,type=2计算y轴
	void CaclAngel(int type);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancle();
	virtual BOOL OnInitDialog();
	void SetTemplateData(CTemplateData * pTemplateData);
};
