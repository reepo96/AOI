#pragma once
#include "DatumMark.h"

//ƴ���������
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

	long	lDatumMarkId1;	//ƴ���һ����׼��id
	long	lDatumMarkId2;	//ƴ��ڶ�����׼��id

	CDatumMark	*pDatumMark1;//ƴ���һ����׼�����
	CDatumMark	*pDatumMark2;//ƴ��ڶ�����׼�����
};
