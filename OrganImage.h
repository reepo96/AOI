#pragma once
#include "OrganData.h"
#include "afxwin.h"

// COrganImage ��ʾԪ��ͼ��

class COrganImage : public CDialog
{
	DECLARE_DYNAMIC(COrganImage)

public:
	COrganImage(COrganData *pOrgan,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COrganImage();

// �Ի�������
	enum { IDD = IDD_ORGANIMAGE };

protected:
	COrganData	*m_pOrgan;
	CImage		*m_pBodyImage;
	int			m_iImageIndex;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
