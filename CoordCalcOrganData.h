#pragma once
#include "dataobj.h"
#include "OrganData.h"

//参与计算坐标相对值的数据对象
class CCoordCalcOrganData :
	public CDataObj
{
public:
	CCoordCalcOrganData(void);
public:
	~CCoordCalcOrganData(void);

	long			templateId;
	COrganData*		pOrganData;
};
