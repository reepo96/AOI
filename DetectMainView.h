#pragma once
#include "DetectCmdDlg.h"
#include "DetectSmallView.h"
#include "DetectBigGraphView.h"
#include "DetectInfoDlg.h"
#include "TemplateData.h"


// CDetectMainView 窗体视图

class CDetectMainView : public CFormView
{
	DECLARE_DYNCREATE(CDetectMainView)

protected:
	CDetectMainView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDetectMainView();

protected:
	CDetectBigGraphView		*m_pDBigGraphView;		//显示主图像的对话框
	CDetectSmallView		*m_pDSmallgView;	//显示缩略图像的对话框
	CDetectCmdDlg			 m_DCommandDlg;		//放置命令按钮及状态的对话框
	CDetectInfoDlg			 m_DetectInfoDlg;	//显示编程信息的对话框	

	CTemplateData	*m_pTemplateData;	//模板数据

	CRect	m_DBigGraphRs;
	CRect	m_DSmallGraphRs;

public:
	enum { IDD = IDD_DETECTMAINVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();
	
	void SetTemplateData(CTemplateData * pTemplateData);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	 void TAuto();

	 void TManual();

	 void TStop();

	 void TAcceptpcb();

	 void TClarmp();

	 void TOriginx();

	 void TOriginy();

	 void TPcbup();

	 void TOrgan();

	 void TPcbdown();

	 void TLoosenpcb();

	 void TSendpcb();
public:
	void NotifyDetectDataChanged(void);
};


