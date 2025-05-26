#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSetCamera 对话框

class CSetCamera : public CDialog
{
	DECLARE_DYNAMIC(CSetCamera)

public:
	CSetCamera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetCamera();

// 对话框数据
	enum { IDD = IDD_SET_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CSliderCtrl m_sldHorBlank;
	CSliderCtrl m_sldVerBlank;
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
protected:
	long m_iHorBlankVal;
	long m_iVerBlankVal;
public:
	afx_msg void OnBnClickedOk();
protected:
	CSliderCtrl m_sldBlkLevelCalRed;
	CSliderCtrl m_sldBlkLevelCalGreen1;
	CSliderCtrl m_sldBlkLevelCalGreen2;
	CSliderCtrl m_sldBlkLevelCalBlue;
	CSliderCtrl m_sldBrightNess;
	CSliderCtrl m_sldGainRed;
	CSliderCtrl m_sldGainGreen1;
	CSliderCtrl m_sldGainGreen2;
	CSliderCtrl m_sldGainBlue;
	CSliderCtrl m_sldGainCamera;    
	long m_iBlkLevCalRedVal;
	long m_iBlkLevCalGreen1Val;
	long m_iBlkLevCalGreen2Val;
	long m_iBlkLevCalBlueVal;
	long m_iBrightNessVal;
	long m_iGainRedVal;
	long m_iGainGreen1Val;
	long m_iGainGreen2Val;
	long m_iGainBlueVal;
	long m_iGainCameraVal;
	CButton m_ckBlkLevelEnable;
	int m_iCamMode;
	int m_iCamAdcBits;
public:
	afx_msg void OnBnClickedCamBlklevelCal();

    afx_msg void OnCbnSelchangeCamSnapMode();
	
protected:
	BOOL m_bEnableMonitor;
	long m_lExposure;
	int m_iShuterUnit;
    int m_iSnapMode;
    long m_iSnapSpeed;
    long m_iEnableVideoMirror;
    int m_iTriggerPolarity;
    int m_iStrobePolarity;
    long m_lShutterSpeed;
    BOOL m_bBlkLevCal;
    CComboBox m_CamSnapModeCtrl;
    CComboBox m_TrigerPolarityCtrl;
    CComboBox m_StrobePolarityCtrl;
    CComboBox m_ShuterUnitCtrl;
public:
	afx_msg void OnBnClickedDefault();
protected:
	CComboBox m_CamModeCtrl;
};
