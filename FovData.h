#pragma once
#include "dataobj.h"

class CFovData :public CDataObj
{
public:
	CFovData(void);
public:
	~CFovData(void);

public:
	long fovId;
	long type;	//0:基板上的FOV，1:拼板上的FOV
	long templateId;	
	int	 CheckOrder;	//检查顺序

	//CBarData	*m_pBar;//条形码
};
