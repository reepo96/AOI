// MainFrm.h : CMainFrame 类的接口
//


#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CMemoryState   CM1,CM2,CM3;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
public:
	afx_msg void OnUpdateNewTemplate(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateOpenTemplate(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateSaveTemplate(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgAddfov(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgGuide(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgImportcad(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgMark(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgOrgan(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgScanfov(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgScanpcb(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdatePrgSpellpcb(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateTAuto(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateTManual(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateTStop(CCmdUI *pCmdUI);
public:
	afx_msg void OnLogin();
public:
	afx_msg void OnUpdateLogin(CCmdUI *pCmdUI);
};


