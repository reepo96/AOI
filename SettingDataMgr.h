#pragma once
#include "datamgr.h"

class CSettingDataMgr :
	public CDataMgr
{
public:
	CSettingDataMgr(void);
public:
	~CSettingDataMgr(void);

	bool GetColorSettingData();
	bool UpdateColorSettingData(CString& sErrorInfo);
};
