// AOI.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)	
END_MESSAGE_MAP()


// CAOIApp 构造

CAOIApp::CAOIApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAOIApp 对象

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

CCamera     g_camera;       //全局摄像头对象
CMotor      g_Motor;        //全局电机
CPCBState   g_PCBState;     //全局板状态

double g_fMotorStep = 8000;
double g_fPels_MM = 69.43146;
double g_dTrackMinWidth =100.00;
int g_iPCBInTime = 6000;		//PCB进入时间（ms）
int g_iPCBOutTime = 2000;		//PCB送出时间（ms）
int g_iMotorSpeed = 100;		//电机速度
int g_iPCBEntryDir = 0;		//PCB板入口：0-右入口，1-左入口
int g_iMotorMovOffsetX = 5000;	//X轴电机运动补偿
int g_iMotorMovOffsetY = 500;	//Y轴电机运动补偿
double g_dZhouJiaoduOff = 0.002024818525809;//电机Y轴与X轴角度偏差

bool	g_bStopWork = false;		//停止工作
bool	g_bPauseWork = false;		//暂停工作
bool	g_bPauseDetect = false;		//暂停检测

// CAOIApp 初始化

BOOL CAOIApp::InitInstance()
{
	CoInitialize(NULL);

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
#ifndef _MYUTEST
	GT_ExInpt(&g_uDevInEvent);
	if(g_uDevInEvent&IN_EVT_KEY_BUTTON)//没有插入钥匙
	{
		return TRUE;
	}
#endif

	//获得语言配置
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

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接

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
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CAOIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	//登录
	CLoginDlg loginDlg;

	if(IDOK!=loginDlg.DoModal())
	{
		return TRUE;
	}

	CSettingDataMgr SettingMgr;
	SettingMgr.GetColorSettingData();


	// 创建主 MDI 框架窗口
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

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生


	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新	

	//根据用户权限设置菜单	
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
	::AfxMessageBox("测试电机-初始化开始");
	YT ytTest1;
	ytTest1.YTInitiel();
	ZHOU zhouTest1;
	zhouTest1.AxisInitial(1,0,7,3);//X
	zhouTest1.AxisInitial(2,0,7,3);//Y
	zhouTest1.AxisInitial(3,0,7,3);//M轨道宽度
	//zhouTest.SetAtlPos(1,-1000000);//设置初始实际位置
	//zhouTest.SetAtlPos(2,400000);
	//zhouTest.AxisRunS(1,-400000,100,1,0.001);//设置目标位置
	//zhouTest.AxisRunS(2,200000,100,1,0.001);//
	//zhouTest.AxisRunS(3,-100000,100,1,0.001);
	::AfxMessageBox("测试电机-初始化结束");*/
	return TRUE;

}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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

// 用于运行对话框的应用程序命令
void CAOIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CAOIApp 消息处理程序


int CAOIApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CoUninitialize();

	return CWinApp::ExitInstance();
}

/********************************************************************* 
* 说明:	根据标识 szID到选定的语言文件中加载字符串
*sID:字符串标识
*sDefault:没有找到字符串的替代串
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

	sValue.Replace("\\n","\n");	//替换回换行符号

	return sValue;
}

/********************************************************************* 
* 说明:自动加载对话框各个控件的字符串
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

	//读对话框标题
	pDlg->GetWindowTextA(szKey);	
	if(GetPrivateProfileString(szSection,szKey,szDefault,
		pData,dwSize,sFilePath) != 0)
	{
		pDlg->SetWindowText(pData);
	}

	//写入各个子控件的标题文字
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

	//释放内存
	free(pData);
	pData=NULL;

}

//实现在对话框或视图上创建视图
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
	// TODO: 在此添加控件通知处理程序代码
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

	//根据用户权限设置菜单	
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
固高运动控制器指令处理函数
**********************************************************************/
void __stdcall CommandHandle(char *command,short error)
{
	CString sError;
	switch(error)
	{
	case -1:
		sError="\a\nCommunication Error !"; break;
	case 0:
		//sError="指令执行正常！";
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