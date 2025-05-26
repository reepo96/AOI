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

//			   PCB板图, FOV区域图
enum ImageType{PCBIMAGE,FOVIMAGE}; //当前显示图片的类型

// CBigGraphView 窗体视图

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
	CBigGraphView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBigGraphView();
	
	CFovData		*m_pCurrentFov;	//当前编辑的FOV	
	CDatumMark	*m_pCurrentDatumMark;	//当前编辑的基准点
	CBarPrgGuid	*m_pBarPrgGuid;
	CSpellPrgGuid *m_pSpellPrgGuid;
	CMarkPrgGuid *m_pMarkPrgGuid;
	CAngleOffsetPrgGuid *m_pAngleOffsetPrgGuid;

protected:
	ImageType	m_CurrentImageType;	//当前显示图片的类型	

protected:
	WorkStep	m_CurrentWorkStep;	//当前工作步骤
	CFovData		*m_pCurrentShowFov;	//当前当前显示的的FOV（如果当前是在显示FOV图像的话）
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

//计算坐标相对值相关成员变量
protected:
	COrganData	*m_pCoordCalcOrgan1;//参与计算坐标相对值的元件1
	COrganData	*m_pCoordCalcOrgan2;//参与计算坐标相对值的元件2
	CRect		m_CoordCalcOrganRect1;
	CRect		m_CoordCalcOrganRect2;
	CCoordCalc	*m_pCoordCalcDlg;

//元件编程
	COrganData	*m_pCurrentOrganData;//当前编程的元件
	COrganProgramDlg *m_pOrganProgramDlg;

public:
	void	SetCurrentImage(CImage *pImage,ImageType type);

	bool	m_bCanDrawRectTracker;	//能否用鼠标拖动创建橡皮框
protected:
	CPoint	m_MousePoint;
	HCURSOR	m_hCurrentCursor;
	CPoint	m_preMousePoint;	//鼠标前一个位置

	int		m_iNewRectTrackStep;//新建橡皮框的步骤（分两步）
	CPoint	m_NewRectTrackOriginP;//新橡皮框的原点
	
	CProgramGuid *m_pProgramGuid;	//编程向导窗口

public:
	enum { IDD = IDD_BIGGRAPHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// 新创建FOV
	void NewFOV(void);
public:
	// 修改FOV
	void ModifyFov(CFovData *pFov);
	// 创建新的基准点
	bool NewDatumMark(void);
	// 修改基准点
	void ModifyDatumMark(CDatumMark * pDatumMark);	
	// 根据两个元件计算CAD坐标系与本系统坐标系之间的相对值
	long CaclRelativValue();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnPaint();
protected:
	// 计算坐标相对值步骤1
	//bool CoordCalcStep1(void);
	// 计算坐标相对值步骤2
	//bool CoordCalcStep2(void);
public:
	// 编程向导
	void PrgGuide(void);
	// 加载模板图像(启动线程)
	bool LoadTemplateImage(void);
	// 加载模板图像
	bool LoadTemplateImageWork(void);

	// 加载所有的FOV图像
	void LoadAllFov(void);

	// 加载修改过的FOV图像
	void LoadModifyFov(void);

	//准备计算坐标相对值
	bool BeginCoordCalc(bool bDoNext=true);
	// 元件编程
	void PrgOrgan(bool bIsSingleOrgan,COrganData* pOrganData);
protected:
	// 通知各窗口编程数据已经改变
	void NotifyPrgDataChanged(void);
public:
	// 条形码编程
	void PrgBar(bool bDoNextStep);
	// 拼板
	void PrgSpellPcb(void);
protected:
	// 通知其它窗口橡皮框已经改变
	void NotifyTrakerChanged(void);
public:
	void SetCurrentOrgan(COrganData* pCurrentOrganData);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	void RefreshUITread();
public:
	// 修改元件编程数据
	void ModifyOrganPrg(COrganData* pOrgan);
public:
	void PrgAngleoffset(void);
};


