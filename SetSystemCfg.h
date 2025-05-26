#pragma once


// SetSystemCfg �Ի���

class SetSystemCfg : public CDialog
{
	DECLARE_DYNAMIC(SetSystemCfg)

public:
	SetSystemCfg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetSystemCfg();

// �Ի�������
	enum { IDD = IDD_SYSTEM_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	double m_fMotorStep;
	double m_fPels_MM;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedDefault();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedCancel();
protected:
    double m_dTrackMinWidth;
	int m_iPCBInTime;
	int m_iPCBOutTime;
	int m_iMotorSpeed;
	int m_iPCBEntryDir;
	// X�����˶�����
	int m_iMotorMovOffsetX;
	// Y�����˶�����
	int m_iMotorMovOffsetY;
	double m_dZhouJiaoduOff;
};
