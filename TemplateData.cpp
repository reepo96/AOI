#include "StdAfx.h"
#include "TemplateData.h"

CTemplateData::CTemplateData(void)
{
	m_bDataIsChanged = false;

	m_LastWorkStep  = NONE;

	m_templateId = -1;

	m_width = 0;
	m_height = 0;
	m_sTemplateType = "";

	bEditAreaVisible = true;
	m_editAreaLeft = 0;
	m_editAreaRight = 0;
	m_editAreaTop = 0;
	m_editAreaBottom = 0;
	
	m_lCoordScaleX= 0.00;
	m_lCoordScaleY= 0.00;
	m_dJiaoDuX = 0.00;
	m_dJiaoDuY = 0.00;
	m_lOffsetX= 0;
	m_lOffsetY= 0;
	m_lDatumLimit = 70;
	m_sLastModifyTime="";

	m_bFOVIsChange = false;
	m_bDatumMarkIsChange = false;
	m_bOrganDataIsChange = false;

	//ÃÌº”‘≠∞Â
	CSpellPCB *pSpellPCB = new CSpellPCB();
	m_SpellPcbMap.insert(::make_pair(0,pSpellPCB));

	m_pBar = NULL;

	InitializeCriticalSection(&m_Critial4FOVMap);
	InitializeCriticalSection(&m_Critial4DatumMarkMap);
	InitializeCriticalSection(&m_Critial4OrganDataMap);
	InitializeCriticalSection(&m_Critial4SpellPcbMap);
}

CTemplateData::~CTemplateData(void)
{
	::EnterCriticalSection(&m_Critial4FOVMap);
	map<long,CFovData*>::iterator fovIt = m_FOVMap.begin();
	for(;fovIt != m_FOVMap.end();fovIt++)
	{
		CFovData *pFovData = (*fovIt).second;
		delete pFovData;
		pFovData = NULL;
	}
	m_FOVMap.clear();
	::LeaveCriticalSection(&m_Critial4FOVMap);

	::EnterCriticalSection(&m_Critial4DatumMarkMap);
	map<long,CDatumMark *>::iterator datumIt = m_DatumMarkMap.begin();
	for(;datumIt != m_DatumMarkMap.end();datumIt++)
	{
		CDatumMark *pDatum = (*datumIt).second;
		delete pDatum;
		pDatum = NULL;
	}
	m_DatumMarkMap.clear();
	::LeaveCriticalSection(&m_Critial4DatumMarkMap);

	::EnterCriticalSection(&m_Critial4OrganDataMap);
	map<long,COrganData*>::iterator organIt = m_OrganDataMap.begin();
	for(;organIt != m_OrganDataMap.end();organIt++)
	{
		COrganData *pOrganData = (*organIt).second;
		delete pOrganData;
		pOrganData = NULL;
	}
	m_OrganDataMap.clear();
	::LeaveCriticalSection(&m_Critial4OrganDataMap);

	::EnterCriticalSection(&m_Critial4SpellPcbMap);
	map<int,CSpellPCB*>::iterator spellIt = m_SpellPcbMap.begin();
	for(;spellIt != m_SpellPcbMap.end();spellIt++)
	{
		CSpellPCB *pSpell = (*spellIt).second;
		delete pSpell;
		pSpell = NULL;
	}
	m_SpellPcbMap.clear();
	::LeaveCriticalSection(&m_Critial4SpellPcbMap);

	DeleteCriticalSection(&m_Critial4FOVMap);
	DeleteCriticalSection(&m_Critial4DatumMarkMap);
	DeleteCriticalSection(&m_Critial4OrganDataMap);
	DeleteCriticalSection(&m_Critial4SpellPcbMap);

	if(m_pBar != NULL)
	{
		delete m_pBar;
		m_pBar = NULL;
	}
}
