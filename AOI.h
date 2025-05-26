// AOI.h : AOI Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
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
// �йش����ʵ�֣������ AOI.cpp
//

class CAOIApp : public CWinApp
{
protected:
	
public:
	CAOIApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
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

extern bool		g_bIsWait;		//�Ƿ��ڵȴ���

extern HHV      g_hCameraHandle;    //����ͷ���

extern CCamera     g_camera;       //ȫ������ͷ����
extern CPCBState   g_PCBState;     //ȫ�ְ�״̬
extern CMotor      g_Motor;        //ȫ�ֵ��

extern unsigned short g_uDevInEvent;     //�豸�����¼�
extern double g_fMotorStep ;    //�������(��/mm)
extern double g_fPels_MM ;      //���ظ���/mm
extern double g_dTrackMinWidth ;  //�����С���(mm)
extern int g_iPCBInTime;		//PCB����ʱ�䣨ms��
extern int g_iPCBOutTime;		//PCB�ͳ�ʱ�䣨ms��
extern int g_iMotorSpeed;		//����ٶ�
extern int g_iPCBEntryDir;		//PCB����ڣ�0-����ڣ�1-�����
extern int g_iMotorMovOffsetX;	//X�����˶�����
extern int g_iMotorMovOffsetY;	//Y�����˶�����
extern double g_dZhouJiaoduOff;	//���Y����X��Ƕ�ƫ��

extern bool	g_bStopWork;		//ֹͣ����
extern bool	g_bPauseWork;		//��ͣ����
extern bool	g_bPauseDetect;		//��ͣ���

/********************************************************************* 
 * ˵��:���ݱ�ʶ szID��ѡ���������ļ��м����ַ���
*********************************************************************/
CString g_LoadString(CString sID,CString sDefault);

/********************************************************************* 
 * ˵��:�Զ����ضԻ�������ؼ����ַ���
*********************************************************************/
void g_SetDialogStrings(CWnd *pDlg);

//ʵ���ڶԻ������ͼ�ϴ�����ͼ
CWnd* CreateNewView(CCreateContext* pContext, CWnd *pParent, CRect& rect, int wID);
/**********************************************************************
�̸��˶�������ָ�����
**********************************************************************/
void __stdcall CommandHandle(char *command,short error);