#pragma once
#include "datamgr.h"
#include "DetectMasterData.h"
#include <vector>
using namespace std;

//测试数据管理类
class CDetectDataMgr :
	public CDataMgr
{
public:
	CDetectDataMgr(void);
public:
	~CDetectDataMgr(void);

protected:
	bool	m_bIsConnect;
public:
	bool Store(CDetectMasterData* pDetectMasterData,CString& sErrorInfo);
protected:
	bool StoreDetail(CDetectMasterData* pDetectMasterData,CString& sErrorInfo);
public:
	bool GetDetectDataByWhere(CString sWhere, vector<CDetectMasterData*>& masterDataList);
protected:
	bool GetDetail(CDetectMasterData* pDetectMasterData);
};
