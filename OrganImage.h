#pragma once
#include "OrganData.h"
#include "afxwin.h"

// COrganImage 显示元件图像

class COrganImage : public CDialog
{
	DECLARE_DYNAMIC(COrganImage)

public:
	COrganImage(COrganData *pOrgan,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrganImage();

// 对话框数据
	enum { IDD = IDD_ORGANIMAGE };

protected:
	COrganData	*m_pOrgan;
	CImage		*m_pBodyImage;
	int			m_iImageIndex;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CStatic m_ImageCtrl;
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedPre();
public:
	afx_msg void OnBnClickedNext();
public:
	afx_msg void OnBnClickedClose();
public:
	afx_msg void OnBnClickedDelete();
protected:
	CString m_sImageNumInf;
};
