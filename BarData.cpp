#include "StdAfx.h"
#include "BarData.h"

CBarData::CBarData(void)
{
	bSerchVisible = true;
	barSerchLeft=0;		//���������������
	barSerchRight=0;	//�������������ұ�
	barSerchTop=0;		//�������������ϱ�
	barSerchBottom=0;	//�������������±�

	fovId = -1000;
	m_pFovData = NULL;
}

CBarData::~CBarData(void)
{
}
