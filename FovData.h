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
	long type;	//0:�����ϵ�FOV��1:ƴ���ϵ�FOV
	long templateId;	
	int	 CheckOrder;	//���˳��

	//CBarData	*m_pBar;//������
};
