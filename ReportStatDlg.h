#pragma once
#include "DetectMasterData.h"
#include <vector>
#include <map>
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;

// CReportStatDlg 报表统计

class CReportStatDlg : public CDialog
{
	DECLARE_DYNAMIC(CReportStatDlg)

public:
	CReportStatDlg(vector<CDetectMasterData*>& MasterDataList,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportStatDlg();

// 对话框数据
	enum { IDD = IDD_REPORTSTAT };

protected:
	vector<CDetectMasterData*> m_MasterDataList;

	map<CString,int>	m_OrganAndErrCount;		//按元件统计错误数
	map<CString,int>	m_ErrTypeAndErrCount;	//按错误类型统计错误数	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	void Init(void);
	CListCtrl m_OrganStatList;
	CListCtrl m_ETypeStatList;
public:
	afx_msg void OnBnClickedClose();
protected:
	// 元件总数
	int m_iOrganTotal;
	// 错误总数
	int m_iErrTotal;
	CString m_sStatType;
	CString m_sImageType;
	CComboBox m_StatTypeCtl;
	CComboBox m_ImageTypeCtl;
public:
	afx_msg void OnPaint();
public:
	CStatic m_ShowErea;
protected:
	// 得到元件最大错误数
	int GetMaxOrganErrCount(void);
	// 得到错误类型的最大错误数
	int GetMaxErrTypeCount(void);
public:
	afx_msg void OnCbnSelchangeStattype();
public:
	afx_msg void OnCbnSelchangeImagtype();
protected:
	// 在饼图上标注信息
	void WritPieInfo(CRect rct1, CRect rct2, double dDegree,CString sInfo);
};
