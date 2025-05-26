#include "StdAfx.h"
#include "DetectMasterData.h"

CDetectDetailData::CDetectDetailData(void)
{
	id=0;
	organName="";
	spellNo = 0;
	centerX=0;
	centerY=0;
	isPass=1;	//是否通过，0：没有，1：通过
	errType="";
	kindred_value=100;	//相似值
	limit_value=100;	//阀值

	m_pErrorOrgan =NULL;	//检测不通过时指向的元件
	m_pErrorPad =NULL;	//检测不通过时指向的焊盘
	m_pErrorFoot =NULL;
}

CDetectDetailData::~CDetectDetailData(void)
{
}



CDetectMasterData::CDetectMasterData(void)
{
	pcbName="";
	id=0;
	barNo="";
	DetectData="";
	DetectTime="";
	Detecter="";
	DetectCount=0;
	ErrorCount=0;
}

CDetectMasterData::~CDetectMasterData(void)
{
	vector<CDetectDetailData*>::iterator it= m_Details.begin();
	for(;it!=m_Details.end();it++)
	{
		CDetectDetailData *pDetailData=(*it);
		delete pDetailData;
		pDetailData = NULL;
	}

	m_Details.clear();
}
