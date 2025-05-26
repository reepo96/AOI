#pragma once
#include "dataobj.h"
#include "FovData.h"

//条形码数据
class CBarData :
	public CDataObj
{
public:
	CBarData(void);
public:
	~CBarData(void);

public:
	long	m_templateId;
	long	m_barId;

	bool	bSerchVisible;
	long	barSerchLeft;		//条形码搜索区左边
	long	barSerchRight;	//条形码搜索区右边
	long	barSerchTop;		//条形码搜索区上边
	long	barSerchBottom;	//条形码搜索区下边

	long		fovId;		//所属的FOV ID
	CFovData	*m_pFovData;//所属的FOV
};
