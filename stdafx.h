// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#include <atlimage.h>



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdb.h>

#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4805)


#define IN_EVT_FRONT_DOOR  0X200  //前门信号
#define IN_EVT_BACK_DOOR  0X400  //后门信号
#define IN_EVT_LEFT_ENTRY  0X800  //左入口信号
#define IN_EVT_RIGHT_ENTRY  0X1000  //右入口信号
#define IN_EVT_YELLOW_BUTTON  0X2000  //黄色按钮信号
#define IN_EVT_GREEN_BUTTON  0X4000  //绿色按钮信号
#define IN_EVT_KEY_BUTTON  0X8000  //钥匙钮信号

#define OUT_EVT_CONVEY_MOTOR    (~0X08)     //输送带电机
#define OUT_EVT_BLUE_LAMP       (~0X20)     //LED兰
#define OUT_EVT_GREEN_LAMP       (~0X40)     //LED绿
#define OUT_EVT_RED_LAMP          (~0X80)     //LED红
#define OUT_EVT_FAN               (~0X100)     //散热风扇
#define OUT_EVT_STICK_BOARD_UP          (~0X400)     //顶板气缸-升高
#define OUT_EVT_STICK_BOARD_DOWN          (~0X800)     //顶板气缸-落下
#define OUT_EVT_STOP_BOARD_UP          (~0X1000)     //板停止气缸-升高
#define OUT_EVT_STOP_BOARD_DOWN          (~0X2000)     //板停止气缸-落下
#define OUT_EVT_PRESS_BOARD_CLAMP          (~0X4000)     //板压紧气缸-压紧
#define OUT_EVT_PRESS_BOARD_LOOSE          (~0X8000)     //板压紧气缸-松

#define OUT_ALL_STOP    0XFFFF      //全部停止






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


