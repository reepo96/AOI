#include "StdAfx.h"
#include "DetectDataMgr.h"

CDetectDataMgr::CDetectDataMgr(void)
{
	if(!connectDB())
	{
		m_bIsConnect = false;
	}
	else
	{
		m_bIsConnect = true;
	}
}

CDetectDataMgr::~CDetectDataMgr(void)
{
	if( m_bIsConnect)
	{
		m_adoConnection.Close();
	}
}

bool CDetectDataMgr::Store(CDetectMasterData* pDetectMasterData,CString& sErrorInfo)
{
	if( !m_bIsConnect)
		return false;

	CString	sSql;

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();

	m_adoConnection.BeginTrans();

	long	lMaxMasterId=0;
	sSql = "select ISNULL( max(id),0 ) as MaxId from T_DetectMaster";
	m_adoRecordSet.Open(sSql);
	if(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("MaxId",lMaxMasterId);
	}
	m_adoRecordSet.Close();

	sSql.Format("select * from T_DetectMaster where id=%d",pDetectMasterData->id);
	m_adoRecordSet.Open(sSql);
	m_adoRecordSet.AddNew();

	pDetectMasterData->id = lMaxMasterId+1;

	m_adoRecordSet.PutCollect("id",pDetectMasterData->id);
	m_adoRecordSet.PutCollect("pcbName",pDetectMasterData->pcbName);
	m_adoRecordSet.PutCollect("barNo",pDetectMasterData->barNo);
	m_adoRecordSet.PutCollect("DetectData",pDetectMasterData->DetectData);
	m_adoRecordSet.PutCollect("DetectTime",pDetectMasterData->DetectTime);
	m_adoRecordSet.PutCollect("Detecter",pDetectMasterData->Detecter);
	m_adoRecordSet.PutCollect("DetectCount",pDetectMasterData->DetectCount);
	m_adoRecordSet.PutCollect("ErrorCount",pDetectMasterData->ErrorCount);

	if(FALSE == m_adoRecordSet.Update(sErrorInfo))
	{
		m_adoConnection.RollbackTrans();
		m_adoRecordSet.Close();
		return false;
	}
	m_adoRecordSet.Close();

	return StoreDetail(pDetectMasterData,sErrorInfo);
}

bool CDetectDataMgr::StoreDetail(CDetectMasterData* pDetectMasterData,CString& sErrorInfo)
{
	if(pDetectMasterData->m_Details.size() <= 0)
		return true;

	CString	sSql;
	sSql.Format("select * from T_DetectDetail where id=0");
	m_adoRecordSet.Open(sSql);
	
	vector<CDetectDetailData*>::iterator it = pDetectMasterData->m_Details.begin();
	for(;it !=pDetectMasterData->m_Details.end();it++)
	{
		CDetectDetailData* pDetail= (*it);
		pDetail->masterId = pDetectMasterData->id;

		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("id",pDetail->id);
		m_adoRecordSet.PutCollect("masterId",pDetail->masterId);
		m_adoRecordSet.PutCollect("organName",pDetail->organName);
		m_adoRecordSet.PutCollect("centerX",pDetail->centerX);
		m_adoRecordSet.PutCollect("centerY",pDetail->centerY);
		m_adoRecordSet.PutCollect("isPass",pDetail->isPass);
		m_adoRecordSet.PutCollect("errType",pDetail->errType);
		m_adoRecordSet.PutCollect("kindred_value",pDetail->kindred_value);
		m_adoRecordSet.PutCollect("limit_value",pDetail->limit_value);
	}

	if(FALSE == m_adoRecordSet.Update(sErrorInfo))
	{
		m_adoConnection.RollbackTrans();
		m_adoRecordSet.Close();
		return false;
	}
	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();

	return true;
}

bool CDetectDataMgr::GetDetectDataByWhere(CString sWhere, vector<CDetectMasterData*>& masterDataList)
{
	if( !m_bIsConnect)
		return false;

	CString	sSql="select * from T_DetectMaster ";

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoConnection.BeginTrans();

	sSql += sWhere;
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		CDetectMasterData	*pMasterData=new CDetectMasterData();
		m_adoRecordSet.GetCollect("id",pMasterData->id);
		m_adoRecordSet.GetCollect("pcbName",pMasterData->pcbName);
		m_adoRecordSet.GetCollect("barNo",pMasterData->barNo);
		m_adoRecordSet.GetCollect("DetectData",pMasterData->DetectData);
		m_adoRecordSet.GetCollect("DetectTime",pMasterData->DetectTime);
		m_adoRecordSet.GetCollect("Detecter",pMasterData->Detecter);
		m_adoRecordSet.GetCollect("DetectCount",pMasterData->DetectCount);
		m_adoRecordSet.GetCollect("ErrorCount",pMasterData->ErrorCount);

		masterDataList.push_back(pMasterData);

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	vector<CDetectMasterData*>::iterator it = masterDataList.begin();
	for(;it!=masterDataList.end();it++)
	{
		GetDetail((*it));
	}
	return true;
}

bool CDetectDataMgr::GetDetail(CDetectMasterData* pDetectMasterData)
{
	CString	sSql="";

	sSql.Format("select * from T_DetectDetail where masterId=%d",pDetectMasterData->id);

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoConnection.BeginTrans();

	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		CDetectDetailData* pDetail=new CDetectDetailData();

		m_adoRecordSet.GetCollect("id",pDetail->id);
		m_adoRecordSet.GetCollect("masterId",pDetail->masterId);
		m_adoRecordSet.GetCollect("organName",pDetail->organName);
		m_adoRecordSet.GetCollect("centerX",pDetail->centerX);
		m_adoRecordSet.GetCollect("centerY",pDetail->centerY);
		m_adoRecordSet.GetCollect("isPass",pDetail->isPass);
		m_adoRecordSet.GetCollect("errType",pDetail->errType);
		m_adoRecordSet.GetCollect("kindred_value",pDetail->kindred_value);
		m_adoRecordSet.GetCollect("limit_value",pDetail->limit_value);

		pDetectMasterData->m_Details.push_back(pDetail);

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	return true;
}
