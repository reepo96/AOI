#pragma once
#include "TemplateData.h"
#include "DetectMasterData.h"
#include "afxwin.h"
#include "TemplateDataMgr.h"
#include "ObjectImageDlg.h"
#include "DetectStatInfo.h"

// CValidateErr ��֤������

class CValidateErr : public CDialog
{
	DECLARE_DYNAMIC(CValidateErr)

public:
	CValidateErr(CDetectMasterData *pDetectMasterData,map<CString,CDetectStatInfo*>* pDStatInfoMap,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CValidateErr();

// �Ի�������
	enum { IDD = IDD_VALIDATEERR };

protected:
	CDetectMasterData*	m_pDetectMasterData;//�������

	vector<CDetectDetailData*>::iterator m_DetailIt;
	CTemplateDataMgr	m_templateDataMgr;
	map<CString,CDetectStatInfo*>*	m_pDStatInfoMap;//���ͳ����Ϣ

	CObjectImageDlg*	m_pTemplateImgDlg;
	CObjectImageDlg*	m_pCurrentImgDlg;

	CImage*				m_pTempImg;
	CImage*				m_pLearnImag;	//ѧϰ��ͼ��

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CString m_sOrganName;
	int m_iKindredValue;
	int m_iLimitValue;
	long m_lCenterX;
	long m_lCenterY;
	CString m_sErrorType;
	// ������
	CString m_sDoWay;
public:
	afx_msg void OnCbnSelchangeErrortype();
protected:
	CComboBox m_DoWayCtrl;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
protected:
	CComboBox m_ErrorTypeCtrl;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnCbnSelchangeDoWay();
public:
	void SetLearnImage(CImage* pImage);
protected:
	int m_iSpellNo;
};

