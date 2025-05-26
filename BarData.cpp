#include "StdAfx.h"
#include "BarData.h"

CBarData::CBarData(void)
{
	bSerchVisible = true;
	barSerchLeft=0;		//条形码搜索区左边
	barSerchRight=0;	//条形码搜索区右边
	barSerchTop=0;		//条形码搜索区上边
	barSerchBottom=0;	//条形码搜索区下边

	fovId = -1000;
	m_pFovData = NULL;
}

CBarData::~CBarData(void)
{
}
