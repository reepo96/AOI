#include "StdAfx.h"
#include "DataMgr.h"

CDataMgr::CDataMgr(void)
{
}

CDataMgr::~CDataMgr(void)
{
}

bool CDataMgr::connectDB()
{
	CString	sFilePath="./Config/Config.ini";
	CString sServer;
	CString sDbName;
	CString sUser;
	CString sPsw;
	DWORD dwSize = 100;

	GetPrivateProfileString("DBSet","DbServer","127.0.0.1",sServer.GetBuffer(dwSize),dwSize,sFilePath);
	sServer.ReleaseBuffer();
	sServer.Replace("\\n","\n");

	GetPrivateProfileString("DBSet","DBName","AOIdb",sDbName.GetBuffer(dwSize),dwSize,sFilePath);
	sDbName.ReleaseBuffer();
	sDbName.Replace("\\n","\n");

	GetPrivateProfileString("DBSet","User","sa",sUser.GetBuffer(dwSize),dwSize,sFilePath);
	sUser.ReleaseBuffer();
	sUser.Replace("\\n","\n");

	GetPrivateProfileString("DBSet","psw","sa",sPsw.GetBuffer(dwSize),dwSize,sFilePath);
	sPsw.ReleaseBuffer();
	sPsw.Replace("\\n","\n");
	return m_adoConnection.ConnectSQLServer(sServer,sDbName,sUser,sPsw);
}