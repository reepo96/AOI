#pragma once
#include "datamgr.h"
#include "UserInf.h"
#include <vector>
using namespace std;

class CUserMgr :
	public CDataMgr
{
public:
	CUserMgr(void);
public:
	~CUserMgr(void);

protected:
	bool	m_bIsConnect;

public:
	bool GetAllUsers(vector<CUserInf*>& userList);
public:
	bool AddUser(CUserInf* pUser,CString& sErrorInfo);
public:
	bool DelUser(CUserInf* pUser,CString& sErrorInfo);
public:
	bool Login(CString sUserName, CString sPassword);
};
