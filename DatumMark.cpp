#include "StdAfx.h"
#include "DatumMark.h"

CDatumMark::CDatumMark(void)
{
	m_templateId =0;
	m_datumMarkId =0;
	type = 0;

	searchLeft=0;
	searchRight=0;
	searchTop=0;
	searchBottom=0;	

	m_pFovData = NULL;
}

CDatumMark::~CDatumMark(void)
{
	if(this->m_ImageObj.IsNull() == false)
	{
		//this->m_ImageObj.ReleaseDC();
		this->m_ImageObj.Destroy();
	}
}
