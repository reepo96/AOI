#pragma once

#include "ado.h"

/**
*���ݹ�����
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
