#pragma once
#include "dataobj.h"
#include "FovData.h"

//��׼���������
class CDatumMark :public CDataObj
{
public:
	CDatumMark(void);
public:
	~CDatumMark(void);

public:
	long	m_templateId;
	long	m_datumMarkId;
	long	type;	//0:�����ϵĻ�׼�㣬1:ƴ���ϵĻ�׼��

	long	searchLeft;
	long	searchRight;
	long	searchTop;
	long	searchBottom;
	
	long		fovId;		//������FOV ID
	CFovData	*m_pFovData;//������FOV	
};
