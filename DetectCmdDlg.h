#pragma once
#include "afxwin.h"


// CDetectCmdDlg �Ի���

class CDetectCmdDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetectCmdDlg)

public:
	CDetectCmdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetectCmdDlg();

// �Ի�������
	enum { IDD = IDD_DETECTCMDDLG };

protected:
	CWnd	*m_pDetectBigView;

	CImage*		m_pCurrentTTemplateImg;	//��ǰ���ı�׼ͼ��
	CImage*		m_pCurrentTImg;			//��ǰ����ͼ��
	CRect m_CurrentTImgRect;//��ǰ���ͼ��Ŀ�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
