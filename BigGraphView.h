#pragma once

#include "BaseBigGraphView.h"
#include "TemplateData.h"
#include "DatumMark.h"
#include "ProgramGuid.h"
#include "CoordCalc.h"
#include "OrganProgramDlg.h"
#include "BarPrgGuid.h"
#include "SpellPrgGuid.h"
#include "MarkPrgGuid.h"
#include "AngleOffsetPrgGuid.h"

//			   PCB��ͼ, FOV����ͼ
enum ImageType{PCBIMAGE,FOVIMAGE}; //��ǰ��ʾͼƬ������

// CBigGraphView ������ͼ

class CBigGraphView : public CBaseBigGraphView
{
	DECLARE_DYNCREATE(CBigGraphView)

	friend class CCoordCalc;
	friend class COrganProgramDlg;
	friend class CBarPrgGuid;
	friend class CSpellPrgGuid;
	friend class CCommandDlg;
	friend class CMarkPrgGuid;
	friend class CAngleOffsetPrgGuid;

protected:
	CBigGraphView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBigGraphView();
	
	CFovData		*m_pCurrentFov;	//��ǰ�༭��FOV	
	CDatumMark	*m_pCurrentDatumMark;	//��ǰ�༭�Ļ�׼��
	CBarPrgGuid	*m_pBarPrgGuid;
	CSpellPrgGuid *m_pSpellPrgGuid;
	CMarkPrgGuid *m_pMarkPrgGuid;
	CAngleOffsetPrgGuid *m_pAngleOffsetPrgGuid;

protected:
	ImageType	m_CurrentImageType;	//��ǰ��ʾͼƬ������	

protected:
	WorkStep	m_CurrentWorkStep;	//��ǰ��������
	CFovData		*m_pCurrentShowFov;	//��ǰ��ǰ��ʾ�ĵ�FOV�������ǰ������ʾFOVͼ��Ļ���
public:
	WorkStep GetCurrentWorkStep()
	{
		return m_CurrentWorkStep;
	}
	void SetCurrentShowFov(CFovData *pCurrentShowFov)
	{
		m_pCurrentShowFov = pCurrentShowFov;
		if(m_pOrganProgramDlg != NULL && m_pOrganProgramDlg->m_hWnd!=NULL && ::IsWindow(m_pOrganProgramDlg->m_hWnd))
			m_pOrganProgramDlg->NotifyTrakerChanged();
	}

//�����������ֵ��س�Ա����
protected:
	COrganData	*m_pCoordCalcOrgan1;//��������������ֵ��Ԫ��1
	COrganData	*m_pCoordCalcOrgan2;//��������������ֵ��Ԫ��2
	CRect		m_CoordCalcOrganRect1;
	CRect		m_CoordCalcOrganRect2;
	CCoordCalc	*m_pCoordCalcDlg;

//Ԫ�����
	COrganData	*m_pCurrentOrganData;//��ǰ��̵�Ԫ��
	COrganProgramDlg *m_pOrganProgramDlg;

public:
	void	SetCurrentImage(CImage *pImage,ImageType type);

	bool	m_bCanDrawRectTracker;	//�ܷ�������϶�������Ƥ��
protected:
	CPoint	m_MousePoint;
	HCURSOR	m_hCurrentCursor;
	CPoint	m_preMousePoint;	//���ǰһ��λ��

	int		m_iNewRectTrackStep;//�½���Ƥ��Ĳ��裨��������
	CPoint	m_NewRectTrackOriginP;//����Ƥ���ԭ��
	
	CProgramGuid *m_pProgramGuid;	//����򵼴���

public:
	enum { IDD = IDD_BIGGRAPHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);
	void draw(CDC* pInvalidDC=NULL);
protected:
	virtual void OnDraw(CDC* pDC);

	void	init();
public:
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnOriginal();
	void Editarea(void);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	void EndCurrentWork(bool bDoNext=true);
	void CancelCurrentWork();
	void NextStep();
	void NewCurrentWork();
	// �´���FOV
	void NewFOV(void);
public:
	// �޸�FOV
	void ModifyFov(CFovData *pFov);
	// �����µĻ�׼��
	bool NewDatumMark(void);
	// �޸Ļ�׼��
	void ModifyDatumMark(CDatumMark * pDatumMark);	
	// ��������Ԫ������CAD����ϵ�뱾ϵͳ����ϵ֮������ֵ
	long CaclRelativValue();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnPaint();
protected:
	// �����������ֵ����1
	//bool CoordCalcStep1(void);
	// �����������ֵ����2
	//bool CoordCalcStep2(void);
public:
	// �����
	void PrgGuide(void);
	// ����ģ��ͼ��(�����߳�)
	bool LoadTemplateImage(void);
	// ����ģ��ͼ��
	bool LoadTemplateImageWork(void);

	// �������е�FOVͼ��
	void LoadAllFov(void);

	// �����޸Ĺ���FOVͼ��
	void LoadModifyFov(void);

	//׼�������������ֵ
	bool BeginCoordCalc(bool bDoNext=true);
	// Ԫ�����
	void PrgOrgan(bool bIsSingleOrgan,COrganData* pOrganData);
protected:
	// ֪ͨ�����ڱ�������Ѿ��ı�
	void NotifyPrgDataChanged(void);
public:
	// ��������
	void PrgBar(bool bDoNextStep);
	// ƴ��
	void PrgSpellPcb(void);
protected:
	// ֪ͨ����������Ƥ���Ѿ��ı�
	void NotifyTrakerChanged(void);
public:
	void SetCurrentOrgan(COrganData* pCurrentOrganData);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	void RefreshUITread();
public:
	// �޸�Ԫ���������
	void ModifyOrganPrg(COrganData* pOrgan);
public:
	void PrgAngleoffset(void);
};


