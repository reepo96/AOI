#pragma once



// CProgramView ������ͼ

class CProgramView : public CFormView
{
	DECLARE_DYNCREATE(CProgramView)

protected:
	CProgramView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CProgramView();

public:
	enum { IDD = IDD_PROGRAMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


