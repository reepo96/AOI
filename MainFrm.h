// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	CMemoryState   CM1,CM2,CM3;

// ���ɵ���Ϣӳ�亯��
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


