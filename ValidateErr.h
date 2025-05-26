#pragma once
#include "TemplateData.h"
#include "DetectMasterData.h"
#include "afxwin.h"
#include "TemplateDataMgr.h"
#include "ObjectImageDlg.h"
#include "DetectStatInfo.h"

// CValidateErr 验证检测错误

class CValidateErr : public CDialog
{
	DECLARE_DYNAMIC(CValidateErr)

public:
	CValidateErr(CDetectMasterData *pDetectMasterData,map<CString,CDetectStatInfo*>* pDStatInfoMap,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CValidateErr();

// 对话框数据
	enum { IDD = IDD_VALIDATEERR };

protected:
	CDetectMasterData*	m_pDetectMasterData;//检测数据

	vector<CDetectDetailData*>::iterator m_DetailIt;
	CTemplateDataMgr	m_templateDataMgr;
	map<CString,CDetectStatInfo*>*	m_pDStatInfoMap;//检测统计信息

	CObjectImageDlg*	m_pTemplateImgDlg;
	CObjectImageDlg*	m_pCurrentImgDlg;

	CImage*				m_pTempImg;
	CImage*				m_pLearnImag;	//学习的图像

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CString m_sOrganName;
	int m_iKindredValue;
	int m_iLimitValue;
	long m_lCenterX;
	long m_lCenterY;
	CString m_sErrorType;
	// 处理方法
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

