#pragma once
#include "TemplateData.h"
#include "OrganPrgGuid.h"
#include "afxwin.h"

enum OrganWorkStep{OWS_NONE,OWS_SERCHAREA,OWS_MAINBODY,OWS_PAD,OWS_FOOT};

// COrganProgramDlg Ԫ����̶Ի���

class COrganProgramDlg : public CDialog
{
	DECLARE_DYNAMIC(COrganProgramDlg)

protected:
	CWnd *m_pParrentWnd;
	
	OrganWorkStep	m_CurrentWorkStep;//��ǰ��������
	COrganPrgGuid	*m_pProgramGuid;//�򵼴���

	bool	m_bIsSingleOrgan;//�Ƿ������һ��Ԫ��
	bool	m_bDoWholeOrgan;//�Ƿ�������Ԫ������������Ԫ��������λ��ʱ��Ҫ���崦��

public:
	COrganProgramDlg(bool bIsSingleOrgan=false,COrganData* pOrganData=NULL,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrganProgramDlg();

// �Ի�������
	enum { IDD = IDD_ORGANPRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedProgram();
	afx_msg void OnBnClickedSkip();
	afx_msg void OnBnClickedInvalid();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedCancle();	
protected:
	CString m_sName;
	CString m_sFootPrint;
	double m_dCADX;
	double m_dCADY;
	CString m_sStat;

	CTemplateData * m_pTemplateData;

	map<long,CFovData*>::iterator	m_CurrentFovMapIt;//��ǰ�����m_FOVMapԪ��
	map<long,COrganData*>::iterator m_CurrentOrganMapIt;//��ǰ�����m_OrganDataMapԪ��
	COrganData						*m_pCurrentOrgan;		//��ǰ�����Ԫ��

	CPadData						*m_pCurrentPad;			//��ǰ����ĺ���
	int								m_iCurrentPadIndex;		//��ǰ�����̵�����(�ڼ�������)

protected:
	//�༭����
	int		m_iFootNum;				//������
	long	m_lFootSpaceBetween;	//���ż��
	OrganWorkStep	m_WorkStepAfterFoot;	//�༭�꺸��֮����²�����
	
public:
	CPadData* GetCurrentPad()
	{
		return m_pCurrentPad;
	}

	void SetWorkStepAfterFoot(OrganWorkStep nextStep)
	{
		m_WorkStepAfterFoot = nextStep;
	}
	void SetTemplateData(CTemplateData *pTemplateData)
	{
		m_pTemplateData = pTemplateData;
	}
public:
	// ��ʼ���
	void BeginPrg(void);
protected:
	// ���Ԫ���Ƿ���FOV��
	bool OrganInFov(const CFovData* pFovData, const COrganData* pOrganData);
	// ����Ԫ���ͺŲ�����ͬ���Ѿ���̺õ�Ԫ��
	COrganData* SearchSameOrgan(const CString sFootPrint);	
	//ȡ��Ԫ����FOV����ϵ�е����ĵ�
	CPoint GetOrganInFovCenter(const CFovData* pFovData,const COrganData* pOrganData);
public:
	long m_lCurrentX;
	long m_lCurrentY;
public:
	afx_msg void OnBnClickedOk();
	// ������ǰ����
	void EndCurrentWork(void);

	void CancelCurrentWork();
protected:
	// ȡ��һ��Ҫ��̵�Ԫ��
	COrganData* GetNextOrgan(void);
	// ����Ԫ������������
	CRect GetSerchArea(COrganData* pOrganData);	
	// ������һ��
	void NextStep(void);
	// ������������
	CRect GetBodyArea(COrganData* pOrganData);
	// �µĹ�������
	void NewWorkStep(CString sShowInfo, OrganWorkStep step, CRect workRect);
	// ȡ�ú���Rect
	CRect GetPadRect(COrganData* pOrganData,CPadData *pPadData);
public:
	// ���Ӻ�����
	void AddFootNum(int iNum);
	// ���Ӻ��ż��
	void AddFootSpaceBetween(long lSpace);
	//������һ��Ԫ��
	bool NextOrganProgram(PrgState state);	
protected:
	// ��Դ���̿���������ݵ�Ŀ�ĺ���
	void CopyPad(CPadData * srcPad, CPadData * destPad, int iRelative);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void BeginSinglePrg(void);
public:
	// ��Ƥ���Ѿ��ı�
	void NotifyTrakerChanged(void);
protected:
	// ȡ������Ԫ���ı��ͼ��
	void GetWholeOrganImage(COrganData* pOrgan);
public:
	void ModiyPrgData(void);
protected:
	CButton m_btnTurn;
public:
	afx_msg void OnBnClickedTurn();
};
