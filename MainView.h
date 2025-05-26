#pragma once

#include "CommandDlg.h"
#include "SmallGraView.h"
#include "BigGraphView.h"
#include "ProgramInfoDlg.h"
#include "MyRectTracker.h"
#include "TemplateData.h"
#include "PrgWorkSure.h"

// CMainView ������ͼ

class CMainView : public CFormView
{
	DECLARE_DYNCREATE(CMainView)

protected:
	CMainView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMainView();

protected:
	CBigGraphView	*m_pBigGraphView;		//��ʾ��ͼ��ĶԻ���
	CSmallGraView	*m_pSmallgGraphView;	//��ʾ����ͼ��ĶԻ���
	CCommandDlg m_commandDlg;		//�������ť��״̬�ĶԻ���
	CProgramInfoDlg m_proInfoDlg;	//��ʾ�����Ϣ�ĶԻ���	

	CTemplateData	*m_pTemplateData;	//ģ������

	CRect	m_BigGraphRs;
	CRect	m_SmallGraphRs;

	CPrgWorkSure	*m_pPrgWorkSureDlg;//ȷ�ϱ�̵ĶԻ���

public:
	enum { IDD = IDD_MAINVIEW };
	CRectTracker *m_pRectTracker;	//ѡ����ο�

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
public:
	void ShowBitmap(void);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void SetTemplateData(CTemplateData * pTemplateData);	
protected:
	
public:
	afx_msg void OnZoomin();
public:
	afx_msg void OnZoomout();
public:
	afx_msg void OnOriginal();
public:
	afx_msg void OnCollect();
public:
	void Editarea(void);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	void NewDatumMark(void);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	// �����
	void PrgGuide(void);
public:
	void PrgScanpcb(void);

	void PrgAddfov(void);

	void PrgScanfov(void);

	//ɨ���޸Ĺ���FOV
	void PrgScanModifyFov(void);

	void PrgImportcad(void);

	void PrgCoordscale(void);

	void PrgOrgan(void);

	void PrgBarcode(void);

	void PrgSpellpcb(void);

	// ֪ͨ��������Ѿ��ı�
	void NotifyPrgDataChanged(void);

	void ModifyFov(CFovData* pFovData);

	void InvalidBigGraphView(void);

	void ModifyDatumMark(CDatumMark* pDatumMark);

	void ModifyOrgan(COrganData * pOrgan);

	void DelOrgan(COrganData * pOrgan);
public:
	// ֪ͨ��Ƥ���Ѿ��ı�
	void NotifyTrakerChanged(void);
public:
	void SetCurrentOrgan(COrganData* pOrganData);
public:
	void SetCurrentImage(CImage * pImage, ImageType type);
public:
	void SetCurrentShowFov(CFovData* pCurrentShowFov);
public:
	void ColorChanged(void);
public:
	void DelSpellPrg(int no);
public:
	// ��FOV��Ϣ�Ի���任FOV
	void ChangeFovFromInfDlg(CFovData* pFovData);
public:
	void PrgAngleoffset(void);
};


