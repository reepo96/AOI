// ChildFrm.h : CChildFrame ��Ľӿ�
//


#pragma once

#include "MyViewSplitter.h"
#include "TemplateData.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:

private:
	MyViewSplitter m_wndSplitter;

	CTemplateData	*m_pTemplateData;	//ģ������

// ����
public:

// ��д
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

// ʵ��
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNewTemplate();
public:
	afx_msg void OnSaveTemplate();
public:
	afx_msg void OnOpenTemplate();
public:
	afx_msg void OnPrgGuide();
public:
	afx_msg void OnPrgScanpcb();
public:
	afx_msg void OnPrgEditarea();
public:
	afx_msg void OnPrgMark();
public:
	afx_msg void OnPrgAddfov();
public:
	afx_msg void OnPrgScanfov();
public:
	afx_msg void OnPrgImportcad();
public:
	afx_msg void OnPrgCoordscale();
public:
	afx_msg void OnPrgOrgan();
public:
	afx_msg void OnPrgBarcode();
public:
	afx_msg void OnPrgSpellpcb();
public:
	afx_msg void OnTAuto();
public:
	afx_msg void OnTManual();
public:
	afx_msg void OnTStop();
public:
	afx_msg void OnTAcceptpcb();
public:
	afx_msg void OnTClarmp();
public:
	afx_msg void OnTOriginx();
public:
	afx_msg void OnTOriginy();
public:
	afx_msg void OnTPcbup();
public:
	afx_msg void OnTOrgan();
public:
	afx_msg void OnTPcbdown();
public:
	afx_msg void OnTLoosenpcb();
public:
	afx_msg void OnTSendpcb();
public:
	afx_msg void OnUsrmgr();
public:
	afx_msg void OnZoomin();
public:
	afx_msg void OnZoomout();
public:
	afx_msg void OnOriginal();
public:
	afx_msg void OnSetcolor();
public:
	afx_msg void OnDeltemplate();
public:
	afx_msg void OnTestDJ();
public:
	afx_msg void OnCameraSet();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnSystem();
public:
	afx_msg void OnRemoveSet();
public:
	afx_msg void OnLightOff();
public:
	afx_msg void OnLightOn();
public:
	afx_msg void OnPrgAngleoffset();
};
