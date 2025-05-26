#pragma once

#include "CommandDlg.h"
#include "SmallGraView.h"
#include "BigGraphView.h"
#include "ProgramInfoDlg.h"
#include "MyRectTracker.h"
#include "TemplateData.h"
#include "PrgWorkSure.h"

// CMainView 窗体视图

class CMainView : public CFormView
{
	DECLARE_DYNCREATE(CMainView)

protected:
	CMainView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainView();

protected:
	CBigGraphView	*m_pBigGraphView;		//显示主图像的对话框
	CSmallGraView	*m_pSmallgGraphView;	//显示缩略图像的对话框
	CCommandDlg m_commandDlg;		//放置命令按钮及状态的对话框
	CProgramInfoDlg m_proInfoDlg;	//显示编程信息的对话框	

	CTemplateData	*m_pTemplateData;	//模板数据

	CRect	m_BigGraphRs;
	CRect	m_SmallGraphRs;

	CPrgWorkSure	*m_pPrgWorkSureDlg;//确认编程的对话框

public:
	enum { IDD = IDD_MAINVIEW };
	CRectTracker *m_pRectTracker;	//选择矩形框

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 编程向导
	void PrgGuide(void);
public:
	void PrgScanpcb(void);

	void PrgAddfov(void);

	void PrgScanfov(void);

	//扫描修改过的FOV
	void PrgScanModifyFov(void);

	void PrgImportcad(void);

	void PrgCoordscale(void);

	void PrgOrgan(void);

	void PrgBarcode(void);

	void PrgSpellpcb(void);

	// 通知编程数据已经改变
	void NotifyPrgDataChanged(void);

	void ModifyFov(CFovData* pFovData);

	void InvalidBigGraphView(void);

	void ModifyDatumMark(CDatumMark* pDatumMark);

	void ModifyOrgan(COrganData * pOrgan);

	void DelOrgan(COrganData * pOrgan);
public:
	// 通知橡皮框已经改变
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
	// 从FOV信息对话框变换FOV
	void ChangeFovFromInfDlg(CFovData* pFovData);
public:
	void PrgAngleoffset(void);
};


