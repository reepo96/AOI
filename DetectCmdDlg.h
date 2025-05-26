#pragma once
#include "afxwin.h"


// CDetectCmdDlg 对话框

class CDetectCmdDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetectCmdDlg)

public:
	CDetectCmdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetectCmdDlg();

// 对话框数据
	enum { IDD = IDD_DETECTCMDDLG };

protected:
	CWnd	*m_pDetectBigView;

	CImage*		m_pCurrentTTemplateImg;	//当前检测的标准图像
	CImage*		m_pCurrentTImg;			//当前检测的图像
	CRect m_CurrentTImgRect;//当前检测图像的框

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetDetectBigView(CWnd *pDetectBigView)
	{
		m_pDetectBigView = pDetectBigView;
	}
	
public:
	afx_msg void OnBnClickedManual();
public:
	afx_msg void OnBnClickedAuto();
public:
	afx_msg void OnBnClickedStop();
public:
	void NotifyDetectDataChanged(void);
public:
	afx_msg void OnPaint();
protected:
	CStatic m_TemplateImgCtl;
	CStatic m_CurrentImgCtl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedDetectOrgan();	
protected:
	CButton m_btnManual;
	CButton m_btnAuto;
	CButton m_btnDetectOrgan;
	CButton m_btnStop;
};
