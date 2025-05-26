// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <atlimage.h>



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdb.h>

#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4805)


#define IN_EVT_FRONT_DOOR  0X200  //ǰ���ź�
#define IN_EVT_BACK_DOOR  0X400  //�����ź�
#define IN_EVT_LEFT_ENTRY  0X800  //������ź�
#define IN_EVT_RIGHT_ENTRY  0X1000  //������ź�
#define IN_EVT_YELLOW_BUTTON  0X2000  //��ɫ��ť�ź�
#define IN_EVT_GREEN_BUTTON  0X4000  //��ɫ��ť�ź�
#define IN_EVT_KEY_BUTTON  0X8000  //Կ��ť�ź�

#define OUT_EVT_CONVEY_MOTOR    (~0X08)     //���ʹ����
#define OUT_EVT_BLUE_LAMP       (~0X20)     //LED��
#define OUT_EVT_GREEN_LAMP       (~0X40)     //LED��
#define OUT_EVT_RED_LAMP          (~0X80)     //LED��
#define OUT_EVT_FAN               (~0X100)     //ɢ�ȷ���
#define OUT_EVT_STICK_BOARD_UP          (~0X400)     //��������-����
#define OUT_EVT_STICK_BOARD_DOWN          (~0X800)     //��������-����
#define OUT_EVT_STOP_BOARD_UP          (~0X1000)     //��ֹͣ����-����
#define OUT_EVT_STOP_BOARD_DOWN          (~0X2000)     //��ֹͣ����-����
#define OUT_EVT_PRESS_BOARD_CLAMP          (~0X4000)     //��ѹ������-ѹ��
#define OUT_EVT_PRESS_BOARD_LOOSE          (~0X8000)     //��ѹ������-��

#define OUT_ALL_STOP    0XFFFF      //ȫ��ֹͣ






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


