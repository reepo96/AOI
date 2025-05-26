#include "StdAfx.h"
#include "UserMgr.h"

CUserMgr::CUserMgr(void)
{
	if(!connectDB())
	{
		m_bIsConnect = false;
		::AfxMessageBox("connectDB=false");
	}
	else
	{
		m_bIsConnect = true;
	}
}

CUserMgr::~CUserMgr(void)
{
	if( m_bIsConnect)
	{
		m_adoConnection.Close();
	}
}

bool CUserMgr::GetAllUsers(vector<CUserInf*>& userList)
{
	if(!m_bIsConnect)
		return false;
	
	CString	sSql = "select * from T_UserInf ";
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	while( !m_adoRecordSet.IsEOF())
	{
		CUserInf *pUserInf= new CUserInf();

		m_adoRecordSet.GetCollect("userName",pUserInf->userName);
		m_adoRecordSet.GetCollect("password",pUserInf->password);
		m_adoRecordSet.GetCollect("enableProgram",pUserInf->enableProgram);
		m_adoRecordSet.GetCollect("enableDetect",pUserInf->enableDetect);
		m_adoRecordSet.GetCollect("enableReport",pUserInf->enableReport);
		m_adoRecordSet.GetCollect("enableSystem",pUserInf->enableSystem);

		userList.push_back(pUserInf);
		m_adoRecordSet.MoveNext();
	}	

	m_adoConnection.CommitTrans();
	m_adoRecordSet.Close();
	return true;
}
bool CUserMgr::AddUser(CUserInf* pUser,CString& sErrorInfo)
{
	if(!m_bIsConnect)
		return false;
	
	CString	sSql = "select * from T_UserInf where userName ='' ";
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	m_adoRecordSet.AddNew();

	m_adoRecordSet.PutCollect("userName",pUser->userName);
	m_adoRecordSet.PutCollect("password",pUser->password);
	m_adoRecordSet.PutCollect("enableProgram",pUser->enableProgram);
	m_adoRecordSet.PutCollect("enableDetect",pUser->enableDetect);
	m_adoRecordSet.PutCollect("enableReport",pUser->enableReport);
	m_adoRecordSet.PutCollect("enableSystem",pUser->enableSystem);

	if(FALSE==m_adoRecordSet.Update(sErrorInfo))
	{
		m_adoConnection.RollbackTrans();
		m_adoRecordSet.Close();
		return false;
	}

	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();

	return true;
}

bool CUserMgr::DelUser(CUserInf* pUser,CString& sErrorInfo)
{
	if(!m_bIsConnect)
		return false;
	
	CString	sSql = "select * from T_UserInf where userName ='" +pUser->userName+"'" ;
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	if( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();	
	
		if(FALSE==m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();

	return true;
}

CUserInf g_UserInf;

bool CUserMgr::Login(CString sUserName, CString sPassword)
{
	if(!m_bIsConnect)
		return false;
	
	CString	sSql = "select * from T_UserInf where userName ='" +sUserName+"' and password='"+ sPassword+"'";
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();
	
	if(m_adoRecordSet.GetRecordCount()>0)
	{
		m_adoRecordSet.GetCollect("userName",g_UserInf.userName);
		m_adoRecordSet.GetCollect("password",g_UserInf.password);
		m_adoRecordSet.GetCollect("enableProgram",g_UserInf.enableProgram);
		m_adoRecordSet.GetCollect("enableDetect",g_UserInf.enableDetect);
		m_adoRecordSet.GetCollect("enableReport",g_UserInf.enableReport);
		m_adoRecordSet.GetCollect("enableSystem",g_UserInf.enableSystem);

		m_adoRecordSet.Close();
		return true;
	}
	else
	{
		m_adoRecordSet.Close();
		return false;
	}
}
