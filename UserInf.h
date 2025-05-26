#pragma once

class CUserInf
{
public:
	CUserInf(void);
public:
	~CUserInf(void);

public:
	CString	userName;
	CString	password;
	int		enableProgram;	//编程权限：0：没有，1：有
	int		enableDetect;	//检测权限
	int		enableReport;	//报表权限
	int		enableSystem;	//系统配置权限
};
