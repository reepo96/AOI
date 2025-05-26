#include "StdAfx.h"
#include "SpellPCB.h"

CSpellPCB::CSpellPCB(void)
{
	m_templateId = 0;
	id = 0;
	no = 0;
	lDatumMarkId1 = 0;
	lDatumMarkId2 = 0;

	pDatumMark1 = NULL;
	pDatumMark2 = NULL;
}

CSpellPCB::~CSpellPCB(void)
{
}
