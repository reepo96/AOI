// SetSystemCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SetSystemCfg.h"

extern double g_fMotorStep;
extern double g_fPels_MM;

// SetSystemCfg 对话框

IMPLEMENT_DYNAMIC(SetSystemCfg, CDialog)

SetSystemCfg::SetSystemCfg(CWnd* pParent /*=NULL*/)
	: CDialog(SetSystemCfg::IDD, pParent)
	, m_fMotorStep(0)
	, m_fPels_MM(0)
    , m_dTrackMinWidth(0)
	, m_iPCBInTime(0)
	, m_iPCBOutTime(0)
	, m_iMotorSpeed(0)
	, m_iPCBEntryDir(0)
	, m_iMotorMovOffsetX(0)
	, m_iMotorMovOffsetY(0)
	, m_dZhouJiaoduOff(0)
{
	char szSetValue[32]={0};
	DWORD dwSize = 31;
	GetPrivateProfileString("SystemCfg","MotorStep","8000",szSetValue,dwSize,"./Config/Config.ini");
	m_fMotorStep = ::atof(szSetValue);

	GetPrivateProfileString("SystemCfg","PELS_MM","69.43146",szSetValue,dwSize,"./Config/Config.ini");
	m_fPels_MM = ::atof(szSetValue);

    GetPrivateProfileString("SystemCfg","TrackMinWidth","100.00",szSetValue,dwSize,"./Config/Config.ini");
	m_dTrackMinWidth = ::atof(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBInTime","6000",szSetValue,dwSize,"./Config/Config.ini");
	m_iPCBInTime = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBOutTime","2000",szSetValue,dwSize,"./Config/Config.ini");
	m_iPCBOutTime = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorSpeed","100",szSetValue,dwSize,"./Config/Config.ini");
	m_iMotorSpeed = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBEntryDir","0",szSetValue,dwSize,"./Config/Config.ini");
	m_iPCBEntryDir = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorMoveOffsetX","5000",szSetValue,dwSize,"./Config/Config.ini");
	m_iMotorMovOffsetX = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorMoveOffsetY","500",szSetValue,dwSize,"./Config/Config.ini");
	m_iMotorMovOffsetY = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","ZhouJiaoduOff","0.002024818525809",szSetValue,dwSize,"./Config/Config.ini");
	m_dZhouJiaoduOff = ::atof(szSetValue);
}

SetSystemCfg::~SetSystemCfg()
{
}

void SetSystemCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MOTOR_STEP, m_fMotorStep);
	DDX_Text(pDX, IDC_PELS_MM, m_fPels_MM);
	DDX_Text(pDX, IDC_TRACK_MINWIDTH, m_dTrackMinWidth);
	DDX_Text(pDX, IDC_PCB_IN_TIME, m_iPCBInTime);
	DDX_Text(pDX, IDC_PCB_OUT_TIME, m_iPCBOutTime);
	DDX_Text(pDX, IDC_MOTOR_SPEED, m_iMotorSpeed);
	DDX_CBIndex(pDX, IDC_PCB_ENTRY, m_iPCBEntryDir);
	DDX_Text(pDX, IDC_MOTOR_MOVOFFSET_X, m_iMotorMovOffsetX);
	DDX_Text(pDX, IDC_MOTOR_MOVOFFSET_Y, m_iMotorMovOffsetY);
	DDX_Text(pDX, IDC_ZHOU_JIAODU_OFFSET, m_dZhouJiaoduOff);
}


BEGIN_MESSAGE_MAP(SetSystemCfg, CDialog)
	ON_BN_CLICKED(IDOK, &SetSystemCfg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DEFAULT, &SetSystemCfg::OnBnClickedDefault)
	ON_BN_CLICKED(IDC_CANCEL, &SetSystemCfg::OnBnClickedCancel)
END_MESSAGE_MAP()


// SetSystemCfg 消息处理程序

void SetSystemCfg::OnBnClickedOk()
{
	this->UpdateData(TRUE);
	char szValue[32]={0};

	g_fMotorStep = m_fMotorStep;
	g_fPels_MM = m_fPels_MM;
    g_dTrackMinWidth = m_dTrackMinWidth;
	g_iPCBInTime = m_iPCBInTime;
	g_iPCBOutTime = m_iPCBOutTime;
	g_iMotorSpeed = m_iMotorSpeed;
	g_iPCBEntryDir = m_iPCBEntryDir;
	g_iMotorMovOffsetX = m_iMotorMovOffsetX;
	g_iMotorMovOffsetY = m_iMotorMovOffsetY;
	g_dZhouJiaoduOff = m_dZhouJiaoduOff;
	
	sprintf(szValue,"%f",m_fMotorStep);
	::WritePrivateProfileString("SystemCfg","MotorStep",szValue,"./Config/Config.ini");

	sprintf(szValue,"%f",m_fPels_MM);
	::WritePrivateProfileString("SystemCfg","PELS_MM",szValue,"./Config/Config.ini");

    sprintf(szValue,"%f",m_dTrackMinWidth);
	::WritePrivateProfileString("SystemCfg","TrackMinWidth",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBInTime);
	::WritePrivateProfileString("SystemCfg","PCBInTime",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBOutTime);
	::WritePrivateProfileString("SystemCfg","PCBOutTime",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorSpeed);
	::WritePrivateProfileString("SystemCfg","MotorSpeed",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBEntryDir);
	::WritePrivateProfileString("SystemCfg","PCBEntryDir",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorMovOffsetX);
	::WritePrivateProfileString("SystemCfg","MotorMoveOffsetX",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorMovOffsetY);
	::WritePrivateProfileString("SystemCfg","MotorMoveOffsetY",szValue,"./Config/Config.ini");

	sprintf(szValue,"%20.13e",m_dZhouJiaoduOff);
	::WritePrivateProfileString("SystemCfg","ZhouJiaoduOff",szValue,"./Config/Config.ini");
	
	OnOK();
}

void SetSystemCfg::OnBnClickedDefault()
{
	m_fMotorStep = 8000;
	m_fPels_MM = 68.270000;
    m_dTrackMinWidth = 114.50;
	m_iPCBInTime = 6000;
	m_iPCBOutTime = 3000;
	m_iMotorSpeed = 100;
	m_iPCBEntryDir = 0;
	m_iMotorMovOffsetX = 5000;
	m_iMotorMovOffsetY = 500;
	m_dZhouJiaoduOff = 0.002024818525809;

	g_fMotorStep = m_fMotorStep;
	g_fPels_MM = m_fPels_MM;
    g_dTrackMinWidth = m_dTrackMinWidth;
	g_iPCBInTime = m_iPCBInTime;
	g_iPCBOutTime = m_iPCBOutTime;
	g_iMotorSpeed = m_iMotorSpeed;
	g_iPCBEntryDir = m_iPCBEntryDir;
	g_iMotorMovOffsetX = m_iMotorMovOffsetX;
	g_iMotorMovOffsetY = m_iMotorMovOffsetY;
	g_dZhouJiaoduOff = m_dZhouJiaoduOff;

	char szValue[32]={0};
	
	sprintf(szValue,"%f",m_fMotorStep);
	::WritePrivateProfileString("SystemCfg","MotorStep",szValue,"./Config/Config.ini");

	sprintf(szValue,"%f",m_fPels_MM);
	::WritePrivateProfileString("SystemCfg","PELS_MM",szValue,"./Config/Config.ini");

    sprintf(szValue,"%f",m_dTrackMinWidth);
	::WritePrivateProfileString("SystemCfg","TrackMinWidth",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBInTime);
	::WritePrivateProfileString("SystemCfg","PCBInTime",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBOutTime);
	::WritePrivateProfileString("SystemCfg","PCBOutTime",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorSpeed);
	::WritePrivateProfileString("SystemCfg","MotorSpeed",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iPCBEntryDir);
	::WritePrivateProfileString("SystemCfg","PCBEntryDir",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorMovOffsetX);
	::WritePrivateProfileString("SystemCfg","MotorMoveOffsetX",szValue,"./Config/Config.ini");

	sprintf(szValue,"%d",m_iMotorMovOffsetY);
	::WritePrivateProfileString("SystemCfg","MotorMoveOffsetY",szValue,"./Config/Config.ini");

	sprintf(szValue,"%f",m_dZhouJiaoduOff);
	::WritePrivateProfileString("SystemCfg","ZhouJiaoduOff",szValue,"./Config/Config.ini");


	UpdateData(FALSE);

}

BOOL SetSystemCfg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void SetSystemCfg::OnBnClickedCancel()
{
	OnCancel();
}
