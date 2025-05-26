#include "StdAfx.h"
#include "UserInf.h"

CUserInf::CUserInf(void)
{
	CString	userName ="";
	CString	password ="";
	int		enableProgram =1;	//编程权限：0：没有，1：有
	int		enableDetect =1;	//检测权限
	int		enableReport =1;	//报表权限
	int		enableSystem =1;	//系统配置权限
}

CUserInf::~CUserInf(void)
{
}
