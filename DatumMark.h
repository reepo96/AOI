#pragma once
#include "dataobj.h"
#include "FovData.h"

//基准点的数据类
class CDatumMark :public CDataObj
{
public:
	CDatumMark(void);
public:
	~CDatumMark(void);

public:
	long	m_templateId;
	long	m_datumMarkId;
	long	type;	//0:基板上的基准点，1:拼板上的基准点

	long	searchLeft;
	long	searchRight;
	long	searchTop;
	long	searchBottom;
	
	long		fovId;		//所属的FOV ID
	CFovData	*m_pFovData;//所属的FOV	
};
