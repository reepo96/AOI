#pragma once



// CRootView ������ͼ

class CRootView : public CFormView
{
	DECLARE_DYNCREATE(CRootView)

protected:
	CRootView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


