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
	int		enableProgram;	//���Ȩ�ޣ�0��û�У�1����
	int		enableDetect;	//���Ȩ��
	int		enableReport;	//����Ȩ��
	int		enableSystem;	//ϵͳ����Ȩ��
};
