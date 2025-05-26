#pragma once
#include "afxcmn.h"
#include "DectecErrDataInf.h"
#include "DetectOrganInfo.h"
#include "TemplateData.h"
#include "DetectBigGraphView.h"
#include "DetectBassInfo.h"

// CDetectInfoDlg PCB���ʱ��ʾ�����Ϣ�ĶԻ���

class CDetectInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetectInfoDlg)

public:
	CDetectInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDetectInfoDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// �Ի�������
	enum { IDD = IDD_DETECTINFODLG };

protected:
	CDectecErrDataInf	m_DErrDataInfo;
	CDetectOrganInfo	m_DOrganInfo;
	CDetectBassInfo		m_DBassInfo;

	CTemplateData		*m_pTemplateData;
	CDetectBigGraphView	*m_pDBigView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);
	void SetDBigGraphView(CDetectBigGraphView * pBigGraphView);
	void NotifyDetectDataChanged(void);
protected:
	CTabCtrl m_TabCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
