#include "StdAfx.h"
#include "AOI.h"
#include "SettingDataMgr.h"

CSettingDataMgr::CSettingDataMgr(void)
{
}

CSettingDataMgr::~CSettingDataMgr(void)
{
}

bool CSettingDataMgr::GetColorSettingData()
{
	if(!connectDB())
		return false;

	CString	sSql = "select * from T_ColorSetting ";
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	CString sAttrName;
	long	lAttrValue;

	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("AttrName",sAttrName);
		m_adoRecordSet.GetCollect("AttrValue",lAttrValue);

		if(sAttrName == "EditAreaColor")
		{
			g_EditAreaColor = (COLORREF)lAttrValue;
		}
		else if(sAttrName == "FovColor")
		{
			g_FovColor = (COLORREF)lAttrValue;
		}
		else if(sAttrName == "MarkColor")
		{
			g_MarkColor = (COLORREF)lAttrValue;
		}
		else if(sAttrName == "OrganColor")
		{
			g_OrganColor = (COLORREF)lAttrValue;
		}
		else if(sAttrName == "BarCodeColor")
		{
			g_BarCodeColor = (COLORREF)lAttrValue;
		}
		
		m_adoRecordSet.MoveNext();
	}	

	m_adoConnection.CommitTrans();
	m_adoRecordSet.Close();
	return true;
}

bool CSettingDataMgr::UpdateColorSettingData(CString& sErrorInfo)
{
	if(!connectDB())
		return false;

	CString	sSql = "select * from T_ColorSetting ";
	
	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	CString sAttrName;
	
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("AttrName",sAttrName);

		if(sAttrName == "EditAreaColor")
		{
			m_adoRecordSet.PutCollect("AttrValue",(long)g_EditAreaColor);
		}
		else if(sAttrName == "FovColor")
		{
			m_adoRecordSet.PutCollect("AttrValue",(long)g_FovColor);
		}
		else if(sAttrName == "MarkColor")
		{
			m_adoRecordSet.PutCollect("AttrValue",(long)g_MarkColor);
		}
		else if(sAttrName == "OrganColor")
		{
			m_adoRecordSet.PutCollect("AttrValue",(long)g_OrganColor);
		}
		else if(sAttrName == "BarCodeColor")
		{
			m_adoRecordSet.PutCollect("AttrValue",(long)g_BarCodeColor);
		}

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存颜色设置数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		
		m_adoRecordSet.MoveNext();
	}	

	m_adoConnection.CommitTrans();
	m_adoRecordSet.Close();
	return true;
}