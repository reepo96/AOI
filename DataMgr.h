#pragma once

#include "ado.h"

/**
*数据管理类
**/
class CDataMgr
{
public:
	CDataMgr(void);
public:
	~CDataMgr(void);

protected:
	CAdoConnection m_adoConnection;
	CAdoRecordSet  m_adoRecordSet;

protected:
	bool connectDB();
};
