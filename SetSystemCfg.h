#pragma once


// SetSystemCfg 对话框

class SetSystemCfg : public CDialog
{
	DECLARE_DYNAMIC(SetSystemCfg)

public:
	SetSystemCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetSystemCfg();

// 对话框数据
	enum { IDD = IDD_SYSTEM_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	// X轴电机运动补偿
	int m_iMotorMovOffsetX;
	// Y轴电机运动补偿
	int m_iMotorMovOffsetY;
	double m_dZhouJiaoduOff;
};
