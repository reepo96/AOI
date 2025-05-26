#include "StdAfx.h"
#include "DetectMasterData.h"

CDetectDetailData::CDetectDetailData(void)
{
	id=0;
	organName="";
	spellNo = 0;
	centerX=0;
	centerY=0;
	isPass=1;	//�Ƿ�ͨ����0��û�У�1��ͨ��
	errType="";
	kindred_value=100;	//����ֵ
	limit_value=100;	//��ֵ

	m_pErrorOrgan =NULL;	//��ⲻͨ��ʱָ���Ԫ��
	m_pErrorPad =NULL;	//��ⲻͨ��ʱָ��ĺ���
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
