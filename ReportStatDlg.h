#pragma once
#include "DetectMasterData.h"
#include <vector>
#include <map>
#include "afxcmn.h"
#include "afxwin.h"
using namespace std;

// CReportStatDlg ����ͳ��

class CReportStatDlg : public CDialog
{
	DECLARE_DYNAMIC(CReportStatDlg)

public:
	CReportStatDlg(vector<CDetectMasterData*>& MasterDataList,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReportStatDlg();

// �Ի�������
	enum { IDD = IDD_REPORTSTAT };

protected:
	vector<CDetectMasterData*> m_MasterDataList;

	map<CString,int>	m_OrganAndErrCount;		//��Ԫ��ͳ�ƴ�����
	map<CString,int>	m_ErrTypeAndErrCount;	//����������ͳ�ƴ�����	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// Ԫ������
	int m_iOrganTotal;
	// ��������
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
	// �õ�Ԫ����������
	int GetMaxOrganErrCount(void);
	// �õ��������͵���������
	int GetMaxErrTypeCount(void);
public:
	afx_msg void OnCbnSelchangeStattype();
public:
	afx_msg void OnCbnSelchangeImagtype();
protected:
	// �ڱ�ͼ�ϱ�ע��Ϣ
	void WritPieInfo(CRect rct1, CRect rct2, double dDegree,CString sInfo);
};
