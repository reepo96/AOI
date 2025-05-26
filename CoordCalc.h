#pragma once
#include "TemplateData.h"

enum CalcWorkStep{CWS_COORDINATEMAPINI,CWS_COORDINATEMAPSTEP1,CWS_COORDINATEMAPSTEP2};
// CCoordCalc �����������ֵ

class CCoordCalc : public CDialog
{
	DECLARE_DYNAMIC(CCoordCalc)

protected:
	CTemplateData * m_pTemplateData;
	CalcWorkStep	m_CurrentWorkStep;//��ǰ��������

	COrganData	*m_pOrgan1;	//Ԫ��1
	COrganData	*m_pOrgan2;	//Ԫ��2
	CRect		m_CoordCalcOrganRect1;
	CRect		m_CoordCalcOrganRect2;

	bool		m_bDoNext;//��������Ƿ�����һ��

	CWnd *m_pWnd;

public:
	CCoordCalc(bool bDoNext=true,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCoordCalc();

// �Ի�������
	enum { IDD = IDD_COORDCALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelctiorgan1();
public:
	CString m_sOrganName1;
public:
	CString m_sOrganName2;
protected:
	double m_dCADX1;
	double m_dCADY1;
	double m_dCADY2;
	double m_dCADX2;
	long m_lCurrentX1;
	long m_lCurrentX2;
	long m_lCurrentY1;
	long m_lCurrentY2;
	long m_lRelativeValue;
public:
	afx_msg void OnBnClickedSelctiorgan2();
public:
	void SetTemplateData(CTemplateData * pTemplateData);
public:
	// ����Ԫ��1��ǰ��������
	void UpdateOrgan1(CRect rect);
public:
	// ����Ԫ��2��ǰ��������
	void UpdateOrgan2(CRect rect);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancle();
protected:
	void EndCurrentWork(void);
	void NextStep(void);
	bool CoordCalcStep1(void);
	bool CoordCalcStep2(void);
	void CaclRelativValue(void);
public:
	virtual BOOL OnInitDialog();
};
