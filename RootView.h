#pragma once



// CRootView 窗体视图

class CRootView : public CFormView
{
	DECLARE_DYNCREATE(CRootView)

protected:
	CRootView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRootView();

public:
	enum { IDD = IDD_ROOTVIEW };
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
	virtual void OnInitialUpdate();
};


