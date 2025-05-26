// AOI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AOI.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "AOIDoc.h"
#include "AOIView.h"
#include "LoginDlg.h"
#include "SettingDataMgr.h"

#include "Camera.h"
#include "PCBState.h"
#include "Motor.h"

#include "YT.h"
#include "ZHOU.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAOIApp

BEGIN_MESSAGE_MAP(CAOIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CAOIApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)	
END_MESSAGE_MAP()


// CAOIApp ����

CAOIApp::CAOIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAOIApp ����

CAOIApp theApp;

CTemplateData* g_pTemplateData = NULL;

Language g_language=CHINESES;//ENGLISH;

CString	g_sUserName="";

COLORREF	g_EditAreaColor = RGB(0,0,255);
COLORREF	g_FovColor = RGB(255,0,0);
COLORREF	g_MarkColor = RGB(0,255,0);
COLORREF	g_OrganColor = RGB(255,0,0);
COLORREF	g_BarCodeColor = RGB(0,0,255);

bool g_bIsWait = false;

CCamera     g_camera;       //ȫ������ͷ����
CMotor      g_Motor;        //ȫ�ֵ��
CPCBState   g_PCBState;     //ȫ�ְ�״̬

double g_fMotorStep = 8000;
double g_fPels_MM = 69.43146;
double g_dTrackMinWidth =100.00;
int g_iPCBInTime = 6000;		//PCB����ʱ�䣨ms��
int g_iPCBOutTime = 2000;		//PCB�ͳ�ʱ�䣨ms��
int g_iMotorSpeed = 100;		//����ٶ�
int g_iPCBEntryDir = 0;		//PCB����ڣ�0-����ڣ�1-�����
int g_iMotorMovOffsetX = 5000;	//X�����˶�����
int g_iMotorMovOffsetY = 500;	//Y�����˶�����
double g_dZhouJiaoduOff = 0.002024818525809;//���Y����X��Ƕ�ƫ��

bool	g_bStopWork = false;		//ֹͣ����
bool	g_bPauseWork = false;		//��ͣ����
bool	g_bPauseDetect = false;		//��ͣ���

// CAOIApp ��ʼ��

BOOL CAOIApp::InitInstance()
{
	CoInitialize(NULL);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
#ifndef _MYUTEST
	GT_ExInpt(&g_uDevInEvent);
	if(g_uDevInEvent&IN_EVT_KEY_BUTTON)//û�в���Կ��
	{
		return TRUE;
	}
#endif

	//�����������
	CString	sFilePath="./Config/Config.ini";
	CString sLanguage;	
	DWORD dwSize = 100;
	GetPrivateProfileString("LanguageSet","Language","Chinese",sLanguage.GetBuffer(dwSize),dwSize,sFilePath);
	sLanguage.ReleaseBuffer();
	sLanguage.Replace("\\n","\n");
	if(sLanguage == "Chinese")
	{
		g_language=CHINESES;
	}
	else
	{
		g_language=ENGLISH;
	}

	char szSetValue[32]={0};	
	dwSize = 31;
	GetPrivateProfileString("SystemCfg","MotorStep","8000",szSetValue,dwSize,sFilePath);
	g_fMotorStep = ::atof(szSetValue);

	GetPrivateProfileString("SystemCfg","PELS_MM","69.43146",szSetValue,dwSize,sFilePath);
	g_fPels_MM = ::atof(szSetValue);

    GetPrivateProfileString("SystemCfg","TrackMinWidth","100.00",szSetValue,dwSize,sFilePath);
	g_dTrackMinWidth = ::atof(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBInTime","6000",szSetValue,dwSize,sFilePath);
	g_iPCBInTime = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBOutTime","2000",szSetValue,dwSize,sFilePath);
	g_iPCBOutTime = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorSpeed","100",szSetValue,dwSize,sFilePath);
	g_iMotorSpeed = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","PCBEntryDir","0",szSetValue,dwSize,sFilePath);
	g_iPCBEntryDir = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorMoveOffsetX","5000",szSetValue,dwSize,sFilePath);
	g_iMotorMovOffsetX = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","MotorMoveOffsetY","500",szSetValue,dwSize,sFilePath);
	g_iMotorMovOffsetY = ::atoi(szSetValue);

	GetPrivateProfileString("SystemCfg","ZhouJiaoduOff","0.002024818525809",szSetValue,dwSize,sFilePath);
	g_dZhouJiaoduOff = ::atof(szSetValue);

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������

	UINT nResourcID;
	if(g_language == ENGLISH)
	{
		nResourcID = IDR_AOITYPE_E;
	}
	else
	{
		nResourcID = IDR_AOITYPE;
	}

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(nResourcID,
		RUNTIME_CLASS(CAOIDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CAOIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	//��¼
	CLoginDlg loginDlg;

	if(IDOK!=loginDlg.DoModal())
	{
		return TRUE;
	}

	CSettingDataMgr SettingMgr;
	SettingMgr.GetColorSettingData();


	// ������ MDI ��ܴ���
	UINT nMainFrameID;
	if(g_language == ENGLISH)
	{
		nMainFrameID = IDR_MAINFRAME_E;
	}
	else
	{
		nMainFrameID = IDR_MAINFRAME;
	}
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(nMainFrameID))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���	

	//�����û�Ȩ�����ò˵�	
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(1,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(4,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(6,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);

	if(g_UserInf.enableDetect == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(4,MF_BYPOSITION|MF_ENABLED);
	}
	if(g_UserInf.enableProgram == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(1,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_ENABLED);		
	}
	if(g_UserInf.enableSystem == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(5,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(6,MF_BYPOSITION|MF_ENABLED);
	}

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);	
	pMainFrame->UpdateWindow();

	//*******************************************************
	/*GT_HookCommand(CommandHandle);
	::AfxMessageBox("���Ե��-��ʼ����ʼ");
	YT ytTest1;
	ytTest1.YTInitiel();
	ZHOU zhouTest1;
	zhouTest1.AxisInitial(1,0,7,3);//X
	zhouTest1.AxisInitial(2,0,7,3);//Y
	zhouTest1.AxisInitial(3,0,7,3);//M������
	//zhouTest.SetAtlPos(1,-1000000);//���ó�ʼʵ��λ��
	//zhouTest.SetAtlPos(2,400000);
	//zhouTest.AxisRunS(1,-400000,100,1,0.001);//����Ŀ��λ��
	//zhouTest.AxisRunS(2,200000,100,1,0.001);//
	//zhouTest.AxisRunS(3,-100000,100,1,0.001);
	::AfxMessageBox("���Ե��-��ʼ������");*/
	return TRUE;

}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CAOIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CAOIApp ��Ϣ�������


int CAOIApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CoUninitialize();

	return CWinApp::ExitInstance();
}

/********************************************************************* 
* ˵��:	���ݱ�ʶ szID��ѡ���������ļ��м����ַ���
*sID:�ַ�����ʶ
*sDefault:û���ҵ��ַ����������
*********************************************************************/
CString g_LoadString(CString sID,CString sDefault)
{
	CString	sFilePath;
	CString sValue;
	DWORD dwSize = 1000;

	if(g_language == ENGLISH)
	{
		sFilePath = "./Language/Multi_en.ini";
	}
	else
	{
		sFilePath = "./Language/Multi_ch.ini";
	}

	GetPrivateProfileString("String",sID,sDefault,
		sValue.GetBuffer(dwSize),dwSize,sFilePath);
	sValue.ReleaseBuffer();

	sValue.Replace("\\n","\n");	//�滻�ػ��з���

	return sValue;
}

/********************************************************************* 
* ˵��:�Զ����ضԻ�������ؼ����ַ���
*********************************************************************/
void g_SetDialogStrings(CWnd *pDlg)
{
	CString szSection = "String";
	CString szKey,szText;

	CString szDefault = "";
	DWORD dwSize = 1000;
	char* pData = (char*)malloc(dwSize);

	CString	sFilePath;
	if(g_language == ENGLISH)
	{
		sFilePath = "./Language/Multi_en.ini";
	}
	else
	{
		sFilePath = "./Language/Multi_ch.ini";
	}

	//���Ի������
	pDlg->GetWindowTextA(szKey);	
	if(GetPrivateProfileString(szSection,szKey,szDefault,
		pData,dwSize,sFilePath) != 0)
	{
		pDlg->SetWindowText(pData);
	}

	//д������ӿؼ��ı�������
	CWnd* pWnd = pDlg->GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		pWnd->GetWindowTextA(szKey);
		if(GetPrivateProfileString(szSection,szKey,szDefault,
			pData,dwSize,sFilePath) != 0)
		{
			pWnd->SetWindowText(pData);			
		}

		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	//�ͷ��ڴ�
	free(pData);
	pData=NULL;

}

//ʵ���ڶԻ������ͼ�ϴ�����ͼ
CWnd* CreateNewView(CCreateContext* pContext, CWnd *pParent, CRect& rect, int wID)
{
	CWnd* pWnd = NULL;

	if (pContext != NULL)
	{
		if (pContext->m_pNewViewClass != NULL)
		{
			pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
			if (pWnd == NULL)
			{
				TRACE1("Error: Dynamic create of view %Fs failed\n", pContext->m_pNewViewClass->m_lpszClassName);
				return NULL;
			}

			ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CWnd)));

			if (!pWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, pParent, wID, pContext))
			{
				TRACE0("Error: couldn't create view \n");
				return NULL;
			}
			// send initial notification message
			//pWnd->SendMessage(WM_INITIALUPDATE);
		}
	}

	return pWnd;

}
void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CAOIApp::OnLogin()
{
	CLoginDlg loginDlg;

	if(IDOK!=loginDlg.DoModal())
	{
		return;
	}

	this->OnFileNew();

	//�����û�Ȩ�����ò˵�	
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(1,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(4,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	::AfxGetMainWnd()->GetMenu()->EnableMenuItem(6,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);

	if(g_UserInf.enableDetect == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(3,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(4,MF_BYPOSITION|MF_ENABLED);
	}
	if(g_UserInf.enableProgram == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(1,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(2,MF_BYPOSITION|MF_ENABLED);		
	}
	if(g_UserInf.enableSystem == 1)
	{
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(5,MF_BYPOSITION|MF_ENABLED);
		::AfxGetMainWnd()->GetMenu()->EnableMenuItem(6,MF_BYPOSITION|MF_ENABLED);
	}
	m_pMainWnd->ShowWindow(SW_MINIMIZE);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
}

/**********************************************************************
�̸��˶�������ָ�����
**********************************************************************/
void __stdcall CommandHandle(char *command,short error)
{
	CString sError;
	switch(error)
	{
	case -1:
		sError="\a\nCommunication Error !"; break;
	case 0:
		//sError="ָ��ִ��������";
		break;
	case 1:
		sError="\a\nCommand Error !"; break;
	case 2:
		sError="\a\nRadius or chord is 0 !"; break;
	case 3:
		sError="\a\nLength is 0 or overflow !"; break;
	case 4:
		sError="\a\nVelocity or acceleration is less then 0 !";
		break;
	case 5:
		sError="\a\nChord is greater than diameter !"; break;
	case 7:
		sError="\a\nParameter error !"; break;
	default:
		sError.Format ("%d",error);sError="\a\nError Code ="+sError; break;
	}
	if(error!=0)
	{
		sError=command+sError;
		::AfxMessageBox(sError);
	}
}