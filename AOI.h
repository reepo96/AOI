// AOI.h : AOI 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "public.h"
#include "UserInf.h"
#include "gep.h"
#include "TemplateData.h"

#include "./CameraInc/HVDAILT.h"
#include "./CameraInc/HVDef.h"
#include "./CameraInc/HVUtil.h"
#include "./CameraInc/Raw2Rgb.h"

#include "Camera.h"
#include "PCBState.h"
#include "Motor.h"

// CAOIApp:
// 有关此类的实现，请参阅 AOI.cpp
//

class CAOIApp : public CWinApp
{
protected:
	
public:
	CAOIApp();


// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
public:
	afx_msg void OnLogin();
};

extern CAOIApp theApp;

extern CTemplateData* g_pTemplateData;

extern Language g_language;

extern CString	g_sUserName;

extern CUserInf g_UserInf;

extern COLORREF	g_EditAreaColor;
extern COLORREF	g_FovColor;
extern COLORREF	g_MarkColor;
extern COLORREF	g_OrganColor;
extern COLORREF	g_BarCodeColor;

extern bool		g_bIsWait;		//是否在等待中

extern HHV      g_hCameraHandle;    //摄像头句柄

extern CCamera     g_camera;       //全局摄像头对象
extern CPCBState   g_PCBState;     //全局板状态
extern CMotor      g_Motor;        //全局电机

extern unsigned short g_uDevInEvent;     //设备输入事件
extern double g_fMotorStep ;    //电机步长(步/mm)
extern double g_fPels_MM ;      //像素个数/mm
extern double g_dTrackMinWidth ;  //轨道最小宽度(mm)
extern int g_iPCBInTime;		//PCB进入时间（ms）
extern int g_iPCBOutTime;		//PCB送出时间（ms）
extern int g_iMotorSpeed;		//电机速度
extern int g_iPCBEntryDir;		//PCB板入口：0-右入口，1-左入口
extern int g_iMotorMovOffsetX;	//X轴电机运动补偿
extern int g_iMotorMovOffsetY;	//Y轴电机运动补偿
extern double g_dZhouJiaoduOff;	//电机Y轴与X轴角度偏差

extern bool	g_bStopWork;		//停止工作
extern bool	g_bPauseWork;		//暂停工作
extern bool	g_bPauseDetect;		//暂停检测

/********************************************************************* 
 * 说明:根据标识 szID到选定的语言文件中加载字符串
*********************************************************************/
CString g_LoadString(CString sID,CString sDefault);

/********************************************************************* 
 * 说明:自动加载对话框各个控件的字符串
*********************************************************************/
void g_SetDialogStrings(CWnd *pDlg);

//实现在对话框或视图上创建视图
CWnd* CreateNewView(CCreateContext* pContext, CWnd *pParent, CRect& rect, int wID);
/**********************************************************************
固高运动控制器指令处理函数
**********************************************************************/
void __stdcall CommandHandle(char *command,short error);