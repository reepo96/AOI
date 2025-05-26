#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "DetectMasterData.h"
#include <vector>
#include "afxcmn.h"
using namespace std;


// CReportView 窗体视图

class CReportView : public CFormView
{
	DECLARE_DYNCREATE(CReportView)

protected:
	CReportView();           // 动态创建所使用的受保护的构造函数
	virtual ~CReportView();

public:
	enum { IDD = IDD_REPORTVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	vector<CDetectMasterData*> m_MasterDataList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:	
	CTime m_tStartDate;
	CTime m_tEndDate;
	CTime m_tStartTime;
	CTime m_tEndTime;
	CString m_sPCBName;
	CString m_sTester;
	CString m_sRelative;
	int m_iErrorCount;
	int m_iStartId;
	int m_iEndId;
	CString m_sStartBarNo;
	CString m_sEndBarNo;
	CComboBox m_PcbNameCtl;
	CComboBox m_TesterCtl;	
	CButton m_DateChecker;
	CButton m_TimeChecker;
	CDateTimeCtrl m_StartDateCtrl;
	CDateTimeCtrl m_EndDateCtrl;
	CDateTimeCtrl m_StartTimeCtrl;
	CDateTimeCtrl m_EndTimeCtrl;
public:
	afx_msg void OnBnClickedDateCheck();
public:
	afx_msg void OnBnClickedTimeCheck();
public:
	afx_msg void OnBnClickedQuery();
protected:
	CListCtrl m_MasterListCtl;
	CListCtrl m_DetailListCtl;
	void InitList(void);
	void InsObjToList(CDetectMasterData* pDetectMasterData);
public:
	afx_msg void OnLvnItemchangedMasterlist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedStat();
};


