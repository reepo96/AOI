#pragma once
#include "afxwin.h"


// CMotorCtrl 对话框

class CMotorCtrl : public CDialog
{
	DECLARE_DYNAMIC(CMotorCtrl)

public:
	CMotorCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMotorCtrl();

// 对话框数据
	enum { IDD = IDD_MOTOR_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
    int m_iAxis;
	long m_lPos;
public:
	afx_msg void OnBnClickedGohome();
public:
	afx_msg void OnBnClickedMove();
public:
	afx_msg void OnBnClickedClose();
public:
    afx_msg void OnCbnSelchangeAxis();
public:
	virtual BOOL OnInitDialog();
protected:
	CComboBox m_axisCtrl;
};
