#pragma once


// CAngleOffsetPrgGuid �Ի���

class CAngleOffsetPrgGuid : public CDialog
{
	DECLARE_DYNAMIC(CAngleOffsetPrgGuid)

public:
	enum PrgWorkStep{PWS_STEP1,PWS_STEP2,PWS_STEP3,PWS_STEP4,PWS_STEP5,PWS_STEP6};

protected:
	CTemplateData * m_pTemplateData;
	PrgWorkStep	m_CurrentWorkStep;//��ǰ��������

	CWnd *m_pWnd;

	CRect m_rect1;
	CRect m_rect2;
	CFovData *m_pFov1;
	CFovData *m_pFov2;

public:
	CAngleOffsetPrgGuid(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAngleOffsetPrgGuid();

// �Ի�������
	enum { IDD = IDD_ANGLEOFFSETPRGGUID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	void EndCurrentWork(void);
	void NextStep(void);

	// ����Ƕ�
	//type=1����x��,type=2����y��
	void CaclAngel(int type);

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancle();
	virtual BOOL OnInitDialog();
	void SetTemplateData(CTemplateData * pTemplateData);
};
