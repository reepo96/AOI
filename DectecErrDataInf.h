#pragma once
#include "afxcmn.h"
#include "DetectStatInfo.h"
#include <vector>
using namespace std;


// CDectecErrDataInf 显示检测错误的数据

class CDectecErrDataInf : public CDialog
{
	DECLARE_DYNAMIC(CDectecErrDataInf)

public:
	CDectecErrDataInf(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDectecErrDataInf();

// 对话框数据
	enum { IDD = IDD_DETECTERRDATAINF };

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
	CListCtrl m_ErrDataListCtl;
public:
	void NotifyDetectDataChanged(void);
public:
	virtual BOOL OnInitDialog();
protected:
	void InitList(void);
	void InsObjToList(CDetectStatInfo* pDetectStatInfo);
	// 获得检测统计数据列表并排序
	void GetStatInfListANdSort(void);
};
