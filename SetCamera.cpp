// SetCamera.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SetCamera.h"
#include "Camera.h"
#include "CameraSetDataMgr.h"

extern CCamera     g_camera;

// CSetCamera 对话框

IMPLEMENT_DYNAMIC(CSetCamera, CDialog)

CSetCamera::CSetCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCamera::IDD, pParent)
	, m_iHorBlankVal(0)
	, m_iVerBlankVal(0)
	, m_iBlkLevCalRedVal(0)
	, m_iBlkLevCalGreen1Val(0)
	, m_iBlkLevCalGreen2Val(0)
	, m_iBlkLevCalBlueVal(0)
	, m_iBrightNessVal(0)
	, m_iGainRedVal(0)
	, m_iGainGreen1Val(0)
	, m_iGainGreen2Val(0)
	, m_iGainBlueVal(0)
	, m_iGainCameraVal(0)
	, m_iCamMode(0)
	, m_iCamAdcBits(0)
	, m_bEnableMonitor(FALSE)
	, m_lExposure(0)
	, m_iShuterUnit(0)
    , m_iSnapMode(0)
    , m_iTriggerPolarity(0)
    , m_iStrobePolarity(0)
    , m_lShutterSpeed(0)
    , m_bBlkLevCal(FALSE)
{
    CCameraSetDataMgr cameraCfg;
    cameraCfg.GetData();

    m_iHorBlankVal = cameraCfg.HorBlankVal;
    m_iVerBlankVal = cameraCfg.VerBlankVal;
    m_iSnapMode           = cameraCfg.SnapMode          ;
    m_iTriggerPolarity    = cameraCfg.TriggerPolarity   ;
    m_iStrobePolarity     = cameraCfg.StrobePolarity    ;
    m_iSnapSpeed          = cameraCfg.SnapSpeed         ;
    m_iEnableVideoMirror  = cameraCfg.EnableVideoMirror ;
    m_iCamAdcBits         = cameraCfg.CamAdcBits        ;
    m_bBlkLevCal          = cameraCfg.BlkLevCal         ;
    m_iBlkLevCalRedVal    = cameraCfg.BlkLevCalRedVal   ;
    m_iBlkLevCalGreen1Val = cameraCfg.BlkLevCalGreen1Val;
    m_iBlkLevCalGreen2Val = cameraCfg.BlkLevCalGreen2Val;
    m_iBlkLevCalBlueVal   = cameraCfg.BlkLevCalBlueVal  ;
    m_iBrightNessVal      = cameraCfg.BrightNessVal     ;
    m_iGainRedVal         = cameraCfg.GainRedVal        ;
    m_iGainGreen1Val      = cameraCfg.GainGreen1Val     ;
    m_iGainGreen2Val      = cameraCfg.GainGreen2Val     ;
    m_iGainBlueVal        = cameraCfg.GainBlueVal       ;
    m_iGainCameraVal	   = cameraCfg.GainCameraVal	;
    m_lExposure           = cameraCfg.Exposure          ;
    m_lShutterSpeed       = cameraCfg.ShutterSpeed      ;
    m_iShuterUnit         = cameraCfg.ShuterUnit        ;

	m_bEnableMonitor = m_iEnableVideoMirror;
}

CSetCamera::~CSetCamera()
{
}

void CSetCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOR_BLANK, m_sldHorBlank);
	DDX_Control(pDX, IDC_VER_BLANK, m_sldVerBlank);
	DDX_Text(pDX, IDC_HOR_BLANK_VAL, m_iHorBlankVal);
	DDX_Text(pDX, IDC_VER_BLANK_VAL, m_iVerBlankVal);
	DDX_Control(pDX, IDC_BLKLEVEL_CAL_RED, m_sldBlkLevelCalRed);
	DDX_Control(pDX, IDC_BLKLEVEL_CAL_GREEN1, m_sldBlkLevelCalGreen1);
	DDX_Control(pDX, IDC_BLKLEVEL_CAL_GREEN2, m_sldBlkLevelCalGreen2);
	DDX_Control(pDX, IDC_BLKLEVEL_CAL_BLUE, m_sldBlkLevelCalBlue);
	DDX_Control(pDX, IDC_CAM_BRIGHTNESS, m_sldBrightNess);
	DDX_Control(pDX, IDC_GAIN_RED, m_sldGainRed);
	DDX_Control(pDX, IDC_GAIN_GREEN1, m_sldGainGreen1);
	DDX_Control(pDX, IDC_GAIN_GREEN2, m_sldGainGreen2);
	DDX_Control(pDX, IDC_GAIN_BLUE, m_sldGainBlue);
	DDX_Control(pDX, IDC_GAIN_CAMERA, m_sldGainCamera);
	DDX_Text(pDX, IDC_BLKLEV_CAL_RED_VAL, m_iBlkLevCalRedVal);
	DDX_Text(pDX, IDC_BLKLEV_CAL_GREEN1_VAL, m_iBlkLevCalGreen1Val);
	DDX_Text(pDX, IDC_BLKLEV_CAL_GREEN2_VAL, m_iBlkLevCalGreen2Val);
	DDX_Text(pDX, IDC_BLKLEV_CAL_BLUE_VAL2, m_iBlkLevCalBlueVal);
	DDX_Text(pDX, IDC_BRIGHTNESS_VAL, m_iBrightNessVal);
	DDX_Text(pDX, IDC_GAIN_RED_VAL, m_iGainRedVal);
	DDX_Text(pDX, IDC_GAIN_GREEN1_VAL, m_iGainGreen1Val);
	DDX_Text(pDX, IDC_GAIN_GREEN2_VAL, m_iGainGreen2Val);
	DDX_Text(pDX, IDC_GAIN_BLUE_VAL, m_iGainBlueVal);
	DDX_Text(pDX, IDC_GAIN_CAMERA_VAL, m_iGainCameraVal);
	DDX_Control(pDX, IDC_CAM_BLKLEVEL_CAL, m_ckBlkLevelEnable);
	DDX_CBIndex(pDX, IDC_CAM_MODE, m_iCamMode);
	DDX_CBIndex(pDX, IDC_CAM_ADC_BITS, m_iCamAdcBits);
	DDX_Check(pDX, IDC_CAM_ENABLE_MONITOR, m_bEnableMonitor);
	DDX_Text(pDX, IDC_EXPOSURE, m_lExposure);
	DDX_CBIndex(pDX, IDC_SHUTER_UNIT, m_iShuterUnit);
	DDX_CBIndex(pDX, IDC_CAM_SNAP_MODE, m_iSnapMode);
	DDX_CBIndex(pDX, IDC_CAM_TRIG_POLARITY, m_iTriggerPolarity);
	DDX_CBIndex(pDX, IDC_CAM_STROBE_POLARITY2, m_iStrobePolarity);
	DDX_Text(pDX, IDC_SHUTER_SPEED, m_lShutterSpeed);
	DDX_Check(pDX, IDC_CAM_BLKLEVEL_CAL, m_bBlkLevCal);
	DDX_Control(pDX, IDC_CAM_SNAP_MODE, m_CamSnapModeCtrl);
	DDX_Control(pDX, IDC_CAM_TRIG_POLARITY, m_TrigerPolarityCtrl);
	DDX_Control(pDX, IDC_CAM_STROBE_POLARITY2, m_StrobePolarityCtrl);
	DDX_Control(pDX, IDC_SHUTER_UNIT, m_ShuterUnitCtrl);
	DDX_Control(pDX, IDC_CAM_MODE, m_CamModeCtrl);
}


BEGIN_MESSAGE_MAP(CSetCamera, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CSetCamera::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CAM_BLKLEVEL_CAL, &CSetCamera::OnBnClickedCamBlklevelCal)
    ON_CBN_SELCHANGE(IDC_CAM_SNAP_MODE, &CSetCamera::OnCbnSelchangeCamSnapMode)
	ON_BN_CLICKED(IDC_DEFAULT, &CSetCamera::OnBnClickedDefault)
END_MESSAGE_MAP()


// CSetCamera 消息处理程序

BOOL CSetCamera::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);
	
	//分辨率
    switch(g_camera.GetResolutionMode())
	{
	case RES_MODE0:
		m_iCamMode = 0;
		m_CamModeCtrl.SetCurSel(0);
		break;
	case RES_MODE1:
		m_iCamMode = 1;
		m_CamModeCtrl.SetCurSel(1);
		break;
	case RES_MODE2:
		m_iCamMode = 2;
		m_CamModeCtrl.SetCurSel(2);
		break;
	case RES_MODE3:
		m_iCamMode = 3;
		m_CamModeCtrl.SetCurSel(3);
		break;
	default:
		m_iCamMode = 0;
		m_CamModeCtrl.SetCurSel(0);
		break;
	}

	CWnd *pBuddyWnd = NULL;

	CWnd *pWndHorBlankVal = GetDlgItem(IDC_HOR_BLANK_VAL);
	m_sldHorBlank.SetBuddy(pWndHorBlankVal,FALSE);
    m_sldHorBlank.SetRange(g_camera.GetHMinBlkSize(),g_camera.GetHMaxBlkSize());	
	m_sldHorBlank.SetPos(m_iHorBlankVal);

	CWnd *pWndVerBlankVal = GetDlgItem(IDC_VER_BLANK_VAL);
	m_sldVerBlank.SetBuddy(pWndVerBlankVal,FALSE);
    m_sldVerBlank.SetRange(g_camera.GetVMinBlkSize(),g_camera.GetVMaxBlkSize());
	m_sldVerBlank.SetPos(m_iVerBlankVal);

	pBuddyWnd = GetDlgItem(IDC_BLKLEV_CAL_RED_VAL);
	m_sldBlkLevelCalRed.SetBuddy(pBuddyWnd,FALSE);
    m_sldBlkLevelCalRed.SetRange(-255,255);
	m_sldBlkLevelCalRed.SetPos(m_iBlkLevCalRedVal);

	pBuddyWnd = GetDlgItem(IDC_BLKLEV_CAL_GREEN1_VAL);
	m_sldBlkLevelCalGreen1.SetBuddy(pBuddyWnd,FALSE);
    m_sldBlkLevelCalGreen1.SetRange(-255,255);
	m_sldBlkLevelCalGreen1.SetPos(m_iBlkLevCalGreen1Val);
	
	pBuddyWnd = GetDlgItem(IDC_BLKLEV_CAL_GREEN2_VAL);
	m_sldBlkLevelCalGreen2.SetBuddy(pBuddyWnd,FALSE);
    m_sldBlkLevelCalGreen2.SetRange(-255,255);
	m_sldBlkLevelCalGreen2.SetPos(m_iBlkLevCalGreen2Val);

	pBuddyWnd = GetDlgItem(IDC_BLKLEV_CAL_BLUE_VAL2);
	m_sldBlkLevelCalBlue.SetBuddy(pBuddyWnd,FALSE);
    m_sldBlkLevelCalBlue.SetRange(-255,255);
	m_sldBlkLevelCalBlue.SetPos(m_iBlkLevCalBlueVal);

	pBuddyWnd = GetDlgItem(IDC_BRIGHTNESS_VAL);
	m_sldBrightNess.SetBuddy(pBuddyWnd,FALSE);
    m_sldBrightNess.SetRange(0,255);
	m_sldBrightNess.SetPos(m_iBrightNessVal);

    pBuddyWnd = GetDlgItem(IDC_GAIN_RED_VAL);
	m_sldGainRed.SetBuddy(pBuddyWnd,FALSE);
    m_sldGainRed.SetRange(0,127);
	m_sldGainRed.SetPos(m_iGainRedVal);

	pBuddyWnd = GetDlgItem(IDC_GAIN_GREEN1_VAL);
	m_sldGainGreen1.SetBuddy(pBuddyWnd,FALSE);
    m_sldGainGreen1.SetRange(0,127);
	m_sldGainGreen1.SetPos(m_iGainGreen1Val);

	pBuddyWnd = GetDlgItem(IDC_GAIN_GREEN2_VAL);
	m_sldGainGreen2.SetBuddy(pBuddyWnd,FALSE);
    m_sldGainGreen2.SetRange(0,127);
	m_sldGainGreen2.SetPos(m_iGainGreen2Val);

	pBuddyWnd = GetDlgItem(IDC_GAIN_BLUE_VAL);
	m_sldGainBlue.SetBuddy(pBuddyWnd,FALSE);
    m_sldGainBlue.SetRange(0,127);
	m_sldGainBlue.SetPos(m_iGainBlueVal);

	pBuddyWnd = GetDlgItem(IDC_GAIN_CAMERA_VAL);
	m_sldGainCamera.SetBuddy(pBuddyWnd,FALSE);
    m_sldGainCamera.SetRange(0,1023);
	m_sldGainCamera.SetPos(m_iGainCameraVal);


	if(m_bBlkLevCal)
	{
		m_ckBlkLevelEnable.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckBlkLevelEnable.SetCheck(BST_UNCHECKED);
	}

    if(m_iSnapMode == 1)
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(FALSE);
    }

    if(m_iSnapSpeed == 0)
    {
        ((CButton*)GetDlgItem(IDC_NORMAL_SPEED))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_HIGH_SPEED))->SetCheck(BST_UNCHECKED);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_NORMAL_SPEED))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_HIGH_SPEED))->SetCheck(BST_CHECKED);
    }

    if(m_iEnableVideoMirror == 0)
    {
        ((CButton*)GetDlgItem(IDC_CAM_ENABLE_MONITOR))->SetCheck(BST_UNCHECKED);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_CAM_ENABLE_MONITOR))->SetCheck(BST_CHECKED);
    }

	if(m_bBlkLevCal)
	{
		m_sldBlkLevelCalRed.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen1.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen2.EnableWindow(TRUE);
		m_sldBlkLevelCalBlue.EnableWindow(TRUE);
	}
	else
	{
		m_sldBlkLevelCalRed.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen1.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen2.EnableWindow(FALSE);
		m_sldBlkLevelCalBlue.EnableWindow(FALSE);
	}

    CString sSnapMode = g_LoadString("IDS_CONTINUE_MODE","连续方式");
    m_CamSnapModeCtrl.InsertString(-1,sSnapMode);

    sSnapMode = g_LoadString("IDS_OUT_TRIGER_MODE","外触发方式");
    m_CamSnapModeCtrl.InsertString(-1,sSnapMode);

    sSnapMode = g_LoadString("IDS_BRIM_TRIGER","边缘外触发");
    m_CamSnapModeCtrl.InsertString(-1,sSnapMode);

    sSnapMode = g_LoadString("IDS_PULSE_TRIGER","电平外触发");
    m_CamSnapModeCtrl.InsertString(-1,sSnapMode);

	switch(m_iSnapMode)
	{
	case 0:
		m_CamSnapModeCtrl.SetCurSel(0);
		break;
	case 1:
		m_CamSnapModeCtrl.SetCurSel(1);
		break;
	case 2:
		m_CamSnapModeCtrl.SetCurSel(2);
		break;
	case 3:
		m_CamSnapModeCtrl.SetCurSel(3);
		break;
	default:
		m_CamSnapModeCtrl.SetCurSel(0);
		break;
	}

    CString sTrigerPolarity = g_LoadString("IDS_DOWN_BRIM_VALID","下降沿或低电平有效");
    m_TrigerPolarityCtrl.InsertString(-1,sTrigerPolarity);

    sTrigerPolarity = g_LoadString("IDS_UP_BRIM_VALID","上升沿或高电平有效");
    m_TrigerPolarityCtrl.InsertString(-1,sTrigerPolarity);

	if(m_iTriggerPolarity == 0)
	{
		m_TrigerPolarityCtrl.SetCurSel(0);
	}
	else
	{
		m_TrigerPolarityCtrl.SetCurSel(1);
	}

    sTrigerPolarity = g_LoadString("IDS_DOWN_BRIM_VALID","下降沿或低电平有效");
    m_StrobePolarityCtrl.InsertString(-1,sTrigerPolarity);

    sTrigerPolarity = g_LoadString("IDS_UP_BRIM_VALID","上升沿或高电平有效");
    m_StrobePolarityCtrl.InsertString(-1,sTrigerPolarity);

	if(m_iStrobePolarity == 0)
	{
		m_StrobePolarityCtrl.SetCurSel(0);
	}
	else
	{
		m_StrobePolarityCtrl.SetCurSel(1);
	}

    CString sTimeUnit = g_LoadString("IDS_TIME_UNIT_MICS","微秒");
    m_ShuterUnitCtrl.InsertString(-1,sTimeUnit);

    sTimeUnit = g_LoadString("IDS_TIME_UNIT_MS","毫秒");
    m_ShuterUnitCtrl.InsertString(-1,sTimeUnit); 

    sTimeUnit = g_LoadString("IDS_TIME_UNIT_S","秒");
    m_ShuterUnitCtrl.InsertString(-1,sTimeUnit); 

	switch(m_iShuterUnit)
	{
	case 0:
		m_ShuterUnitCtrl.SetCurSel(0);
		break;
	case 1:
		m_ShuterUnitCtrl.SetCurSel(1);
		break;
	case 2:
		m_ShuterUnitCtrl.SetCurSel(2);
		break;
	default:
		m_ShuterUnitCtrl.SetCurSel(0);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetCamera::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int position = pSlider->GetPos();

	CString sText;
	sText.Format("%d",position);

	CWnd *pWnd =pSlider->GetBuddy(FALSE);
	pWnd->SetWindowTextA(sText);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSetCamera::OnBnClickedOk()
{
	UpdateData();

	CButton *pButton = NULL;

	//分辨率
	switch(m_iCamMode)
	{
	case 0:
        g_camera.SetResolutionMode(RES_MODE0);
		break;
	case 1:
		g_camera.SetResolutionMode(RES_MODE1);
		break;
	case 2:
		g_camera.SetResolutionMode(RES_MODE2);
		break;
	case 3:
		g_camera.SetResolutionMode(RES_MODE3);
		break;
	}

	//消隐
    g_camera.SetBlanking(m_iHorBlankVal,m_iVerBlankVal);

	//图像采集方式
    switch(m_iSnapMode)
    {
	case 0:
        g_camera.SetSetSnapMode(CONTINUATION);
		break;
	case 1:
		g_camera.SetSetSnapMode(TRIGGER);
		break;
	case 2:
		g_camera.SetSetSnapMode(TRIGGER_EDGE);
		break;
	case 3:
		g_camera.SetSetSnapMode(TRIGGER_LEVEL);
		break;
	}	

	//外触发信号极性
    if(m_iSnapMode == 1)
    {
        if(m_iTriggerPolarity == 0)
        {
            g_camera.SetTriggerPolarity(LOW);
        }
        else
        {
            g_camera.SetTriggerPolarity(HIGH);
        }
    }

	//闪光灯信号极性
    if(m_iStrobePolarity == 0)
    {
        g_camera.SetStrobePolarity(LOW);
    }
    else
    {
        g_camera.SetStrobePolarity(HIGH);
    }

	//图像采集速度
	pButton = (CButton*)GetDlgItem(IDC_NORMAL_SPEED);
	if(pButton->GetCheck() == BST_CHECKED)
	{
        g_camera.SetSnapSpeed(::NORMAL_SPEED);
        m_iSnapSpeed = 0;
	}
	else
	{
		g_camera.SetSnapSpeed(HIGH_SPEED);
        m_iSnapSpeed = 1;
	}

	//镜像方向及是否允许
    pButton = (CButton*)GetDlgItem(IDC_CAM_ENABLE_MONITOR);
	if(pButton->GetCheck() == BST_CHECKED)
	{
        g_camera.EnableVideoMirror(VERT_DIR,TRUE);
        m_iEnableVideoMirror = 1;
	}
	else
	{
		g_camera.EnableVideoMirror(VERT_DIR,FALSE);
        m_iEnableVideoMirror = 0;
	}

	//模拟数字转换级别
	switch(m_iCamAdcBits)
	{
	case 0:
        g_camera.ADCControl(::ADC_BITS,::ADC_LEVEL0);
		break;
	case 1:
		g_camera.ADCControl(::ADC_BITS,::ADC_LEVEL1);
		break;
	case 2:
		g_camera.ADCControl(::ADC_BITS,::ADC_LEVEL2);
		break;
	case 3:
		g_camera.ADCControl(::ADC_BITS,::ADC_LEVEL3);
		break;
	}

	//允许黑电平调节
	g_camera.ADCControl(::ADC_BLKLEVEL_CAL,m_bBlkLevCal);
	
	//黑电平调节
	g_camera.ADCControl(::ADC_BLKLEVEL_CAL_REDCHANNEL,m_iBlkLevCalRedVal);
	g_camera.ADCControl(::ADC_BLKLEVEL_CAL_GREENCHANNEL1,m_iBlkLevCalGreen1Val);
	g_camera.ADCControl(::ADC_BLKLEVEL_CAL_GREENCHANNEL2,m_iBlkLevCalGreen2Val);
	g_camera.ADCControl(::ADC_BLKLEVEL_CAL_BLUECHANNEL,m_iBlkLevCalBlueVal);

	//亮度
	//HVADCControl(g_hCameraHandle,::,);

	//增益调节
    g_camera.AGCControl(RED_CHANNEL,m_iGainRedVal);
    g_camera.AGCControl(GREEN_CHANNEL1,m_iGainGreen1Val);
    g_camera.AGCControl(GREEN_CHANNEL2,m_iGainGreen2Val);
    g_camera.AGCControl(BLUE_CHANNEL,m_iGainBlueVal);

	//快门设置
    //g_camera.AECControl(::AEC_EXPOSURE_TIME,m_lExposure);

	//快门单位、速度
	/*switch(m_iShuterUnit)
	{
	case 0:
        g_camera.AECControl();*/

    //保存设置
    CCameraSetDataMgr cameraCfg;
    
    cameraCfg.HorBlankVal		 =   m_iHorBlankVal         ;
    cameraCfg.VerBlankVal		 =   m_iVerBlankVal         ;
    cameraCfg.SnapMode           =   m_iSnapMode            ;
    cameraCfg.TriggerPolarity    =   m_iTriggerPolarity     ;
    cameraCfg.StrobePolarity     =   m_iStrobePolarity      ;
    cameraCfg.SnapSpeed          =   m_iSnapSpeed           ;
    cameraCfg.EnableVideoMirror  =   m_iEnableVideoMirror   ;
    cameraCfg.CamAdcBits         =   m_iCamAdcBits          ;
    cameraCfg.BlkLevCal          =   m_bBlkLevCal           ;
    cameraCfg.BlkLevCalRedVal    =   m_iBlkLevCalRedVal     ;
    cameraCfg.BlkLevCalGreen1Val =   m_iBlkLevCalGreen1Val  ;
    cameraCfg.BlkLevCalGreen2Val =   m_iBlkLevCalGreen2Val  ;
    cameraCfg.BlkLevCalBlueVal   =   m_iBlkLevCalBlueVal    ;
    cameraCfg.BrightNessVal      =   m_iBrightNessVal       ;
    cameraCfg.GainRedVal         =   m_iGainRedVal          ;
    cameraCfg.GainGreen1Val      =   m_iGainGreen1Val       ;
    cameraCfg.GainGreen2Val      =   m_iGainGreen2Val       ;
    cameraCfg.GainBlueVal        =   m_iGainBlueVal         ;
    cameraCfg.GainCameraVal	     =   m_iGainCameraVal	    ;
    cameraCfg.Exposure           =   m_lExposure            ;
    cameraCfg.ShutterSpeed       =   m_lShutterSpeed        ;
    cameraCfg.ShuterUnit         =   m_iShuterUnit          ; 

    cameraCfg.SaveData();

	OnOK();
}

void CSetCamera::OnBnClickedCamBlklevelCal()
{
	int iCheck = m_ckBlkLevelEnable.GetCheck();
	if(iCheck == BST_CHECKED)
	{
		m_sldBlkLevelCalRed.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen1.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen2.EnableWindow(TRUE);
		m_sldBlkLevelCalBlue.EnableWindow(TRUE);
	}
	else
	{
		m_sldBlkLevelCalRed.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen1.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen2.EnableWindow(FALSE);
		m_sldBlkLevelCalBlue.EnableWindow(FALSE);
	}
}

void CSetCamera::OnCbnSelchangeCamSnapMode()
{
    UpdateData();

    if(m_iSnapMode == 1)
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(FALSE);
    }
}

void CSetCamera::OnBnClickedDefault()
{
	m_iCamMode = 0;
	m_iHorBlankVal = 0;
    m_iVerBlankVal = 0;
    m_iSnapMode           = 0          ;
    m_iTriggerPolarity    = 1   ;
    m_iStrobePolarity     = 1    ;
    m_iSnapSpeed          = 0         ;
    m_iEnableVideoMirror  = 0 ;
    m_iCamAdcBits         = 2        ;
    m_bBlkLevCal          = 0         ;
    m_iBlkLevCalRedVal    = 255   ;
    m_iBlkLevCalGreen1Val = 255;
    m_iBlkLevCalGreen2Val = 255;
    m_iBlkLevCalBlueVal   = 255  ;
    m_iBrightNessVal      = 0     ;
    m_iGainRedVal         = 72        ;
    m_iGainGreen1Val      = 72     ;
    m_iGainGreen2Val      = 72     ;
    m_iGainBlueVal        = 72       ;
    m_iGainCameraVal	   = 72	;
    m_lExposure           = 0          ;
    m_lShutterSpeed       = 107      ;
    m_iShuterUnit         = 1        ;



	//分辨率
    switch(m_iCamMode)
	{
	case RES_MODE0:		
		m_CamModeCtrl.SetCurSel(0);
		break;
	case RES_MODE1:		
		m_CamModeCtrl.SetCurSel(1);
		break;
	case RES_MODE2:		
		m_CamModeCtrl.SetCurSel(2);
		break;
	case RES_MODE3:		
		m_CamModeCtrl.SetCurSel(3);
		break;
	default:		
		m_CamModeCtrl.SetCurSel(0);
		break;
	}

	
	m_sldHorBlank.SetPos(m_iHorBlankVal);	
	m_sldVerBlank.SetPos(m_iVerBlankVal);	
	m_sldBlkLevelCalRed.SetPos(m_iBlkLevCalRedVal);	
	m_sldBlkLevelCalGreen1.SetPos(m_iBlkLevCalGreen1Val);	
	m_sldBlkLevelCalGreen2.SetPos(m_iBlkLevCalGreen2Val);
	m_sldBlkLevelCalBlue.SetPos(m_iBlkLevCalBlueVal);
	m_sldBrightNess.SetPos(m_iBrightNessVal);
	m_sldGainRed.SetPos(m_iGainRedVal);
	m_sldGainGreen1.SetPos(m_iGainGreen1Val);
	m_sldGainGreen2.SetPos(m_iGainGreen2Val);
	m_sldGainBlue.SetPos(m_iGainBlueVal);
	m_sldGainCamera.SetPos(m_iGainCameraVal);


	if(m_bBlkLevCal)
	{
		m_ckBlkLevelEnable.SetCheck(BST_CHECKED);
	}
	else
	{
		m_ckBlkLevelEnable.SetCheck(BST_UNCHECKED);
	}

    if(m_iSnapMode == 1)
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_CAM_TRIG_POLARITY)->EnableWindow(FALSE);
    }

    if(m_iSnapSpeed == 0)
    {
        ((CButton*)GetDlgItem(IDC_NORMAL_SPEED))->SetCheck(BST_CHECKED);
        ((CButton*)GetDlgItem(IDC_HIGH_SPEED))->SetCheck(BST_UNCHECKED);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_NORMAL_SPEED))->SetCheck(BST_UNCHECKED);
        ((CButton*)GetDlgItem(IDC_HIGH_SPEED))->SetCheck(BST_CHECKED);
    }

    if(m_iEnableVideoMirror == 0)
    {
        ((CButton*)GetDlgItem(IDC_CAM_ENABLE_MONITOR))->SetCheck(BST_UNCHECKED);
    }
    else
    {
        ((CButton*)GetDlgItem(IDC_CAM_ENABLE_MONITOR))->SetCheck(BST_CHECKED);
    }

	if(m_bBlkLevCal)
	{
		m_sldBlkLevelCalRed.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen1.EnableWindow(TRUE);
		m_sldBlkLevelCalGreen2.EnableWindow(TRUE);
		m_sldBlkLevelCalBlue.EnableWindow(TRUE);
	}
	else
	{
		m_sldBlkLevelCalRed.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen1.EnableWindow(FALSE);
		m_sldBlkLevelCalGreen2.EnableWindow(FALSE);
		m_sldBlkLevelCalBlue.EnableWindow(FALSE);
	}
    
	switch(m_iSnapMode)
	{
	case 0:
		m_CamSnapModeCtrl.SetCurSel(0);
		break;
	case 1:
		m_CamSnapModeCtrl.SetCurSel(1);
		break;
	case 2:
		m_CamSnapModeCtrl.SetCurSel(2);
		break;
	case 3:
		m_CamSnapModeCtrl.SetCurSel(3);
		break;
	default:
		m_CamSnapModeCtrl.SetCurSel(0);
		break;
	}
    
	if(m_iTriggerPolarity == 0)
	{
		m_TrigerPolarityCtrl.SetCurSel(0);
	}
	else
	{
		m_TrigerPolarityCtrl.SetCurSel(1);
	}   

	if(m_iStrobePolarity == 0)
	{
		m_StrobePolarityCtrl.SetCurSel(0);
	}
	else
	{
		m_StrobePolarityCtrl.SetCurSel(1);
	}

    switch(m_iShuterUnit)
	{
	case 0:
		m_ShuterUnitCtrl.SetCurSel(0);
		break;
	case 1:
		m_ShuterUnitCtrl.SetCurSel(1);
		break;
	case 2:
		m_ShuterUnitCtrl.SetCurSel(2);
		break;
	default:
		m_ShuterUnitCtrl.SetCurSel(0);
		break;
	}


	this->UpdateData(FALSE);

	//return OnBnClickedOk();
}
