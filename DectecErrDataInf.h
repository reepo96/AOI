#pragma once
#include "afxcmn.h"
#include "DetectStatInfo.h"
#include <vector>
using namespace std;


// CDectecErrDataInf ��ʾ�����������

class CDectecErrDataInf : public CDialog
{
	DECLARE_DYNAMIC(CDectecErrDataInf)

public:
	CDectecErrDataInf(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDectecErrDataInf();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	CListCtrl m_ErrDataListCtl;
public:
	void NotifyDetectDataChanged(void);
public:
	virtual BOOL OnInitDialog();
protected:
	void InitList(void);
	void InsObjToList(CDetectStatInfo* pDetectStatInfo);
	// ��ü��ͳ�������б�����
	void GetStatInfListANdSort(void);
};
