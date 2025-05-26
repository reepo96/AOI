#pragma once
#include "afxcmn.h"
#include "DetectStatInfo.h"
#include <vector>
using namespace std;

// CDetectBassInfo 对话框

class CDetectBassInfo : public CDialog
{
	DECLARE_DYNAMIC(CDetectBassInfo)

public:
	CDetectBassInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDetectBassInfo();

// 对话框数据
	enum { IDD = IDD_DETECTBASEINFO };

protected:
	CWnd	*m_pDBigView;	
	vector<CDetectStatInfo*>	m_StatInfList;

public:
	void SetDBigView(CWnd *pDBigView)
	{
		m_pDBigView = pDBigView;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void NotifyDetectDataChanged(void);
	virtual BOOL OnInitDialog();
	
protected:
	long m_lCurrentId;
	CString m_sCurrentBarCode;
	int m_iPcbNum;
	int m_iErrPcbNum;
	int m_iOrganNum;
	int m_iErrOganNum;
	CString m_sStarTime;
	long m_lStartId;
	BOOL m_bAskWhenErr;
public:
	afx_msg void OnBnClickedAskwhenerr();
};
