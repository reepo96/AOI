#pragma once

// CCommandDlg �Ի���

class CCommandDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommandDlg)

public:
	CCommandDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommandDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);	

// �Ի�������
	enum { IDD = IDD_COMMANDDLG };

protected:	
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void NotifyPrgDataChanged(void);	
protected:
	CWnd	*m_pBigGraphView;

	int m_iCenterX;
	int m_iCenterY;
	int m_iTrackerWidth;
	int m_iTrackerHeight;
	CString m_sStatus;
	CString m_sMouseP;
	// ��ʾ��Ϣ
	CString m_sInfo;

	HANDLE m_hEditEvent;
public:	
	afx_msg void OnBnClickedCenterxdd();
public:
	afx_msg void OnBnClickedCenterxd();
public:
	afx_msg void OnBnClickedCenteryaa();
public:
	afx_msg void OnBnClickedCenterya();
public:
	afx_msg void OnBnClickedCenterydd();
public:
	afx_msg void OnBnClickedCenteryd();
public:
	afx_msg void OnBnClickedCenterwaa();
public:
	afx_msg void OnBnClickedCenterwa();
public:
	afx_msg void OnBnClickedCenterwdd();
public:
	afx_msg void OnBnClickedCenterwd();
public:
	afx_msg void OnBnClickedCenterhaa();
public:
	afx_msg void OnBnClickedCenterha();
public:
	afx_msg void OnBnClickedCenterhdd();
public:
	afx_msg void OnBnClickedCenterhd();
public:
	afx_msg void OnEnChangeCenterx();
public:
	afx_msg void OnEnChangeCentery();
public:
	afx_msg void OnEnChangeCenterw();
public:
	afx_msg void OnEnChangeCenterh();
public:
	afx_msg void OnBnClickedCenterxaa();
public:
	afx_msg void OnBnClickedCenterxa();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	// ��������Ϣ
	void GetPrgData(void);
public:
	void SetBigGraphView(CWnd* pBigView);
protected:
	// ����X������
	void CenterXAdd(int iAdd);
	// ����Y������
	void CenterYAdd(int iAdd);
	void CenterXModify(int iNewValue);
	void CenterYModify(int iNewValue);
	void TrackerWidthAdd(int iAdd);
	void TrackerHeightAdd(int iAdd);
	void TrackerWidthModify(int iNewValue);
	void TrackerHeightModify(int iNewValue);
public:
	// ��Ƥ���Ѿ��޸�
	void NotifyTrakerChanged(void);
};
