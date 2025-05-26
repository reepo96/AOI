#pragma once
#include "DatumMark.h"

//拼板的数据类
class CSpellPCB
{
public:
	CSpellPCB(void);
public:
	~CSpellPCB(void);

public:
	long	m_templateId;
	long	no;
	long	id;

	long	lDatumMarkId1;	//拼板第一个基准点id
	long	lDatumMarkId2;	//拼板第二个基准点id

	CDatumMark	*pDatumMark1;//拼板第一个基准点对象
	CDatumMark	*pDatumMark2;//拼板第二个基准点对象
};
