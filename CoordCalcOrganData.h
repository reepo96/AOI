#pragma once
#include "dataobj.h"
#include "OrganData.h"

//��������������ֵ�����ݶ���
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
