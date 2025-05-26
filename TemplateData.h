#pragma once

#include "DataObj.h"
#include "DatumMark.h"
#include "OrganData.h"
#include "FovData.h"
#include "BarData.h"
#include "SpellPCB.h"
#include "CoordCalcOrganData.h"
#include <vector>
#include <map>
using namespace std;

//			none,	����PCB  �༭����, FOV����,	����FOV		��׼��,	  ����ӳ��,		Ԫ��,����,ƴ��
enum WorkStep{NONE,LOADPCB, EDITAREA, EDITFOVAREA,LOADFOV,DATUMMARK, COORDINATEMAP, ORGAN,BAR,SPELLPCB,ANGLE_OFFSET};

/**
*PCB���������
**/
class CTemplateData :public CDataObj
{
public:
	CTemplateData(void);
public:
	~CTemplateData(void);

public:
	bool	m_bDataIsChanged;	//�����Ƿ��Ѿ����޸�
	
	WorkStep	m_LastWorkStep;	//�������ɵĹ�������

	long	m_templateId;
	CStringA	m_templateName;
	long	m_width;		//�峤
	long	m_height;		//���
	CString	m_sTemplateType;//ģ������ B:bottom, T:top

	bool	bEditAreaVisible;	//�༭�����Ƿ���ʾ
	long	m_editAreaLeft;		//��༭�������
	long	m_editAreaRight;	//��༭�����ұ�
	long	m_editAreaTop;		//��༭�����ϱ�
	long	m_editAreaBottom;	//��༭�����±�
		
	double	m_lCoordScaleX;		//����ϵ��X���ϵı���(����/mm)
	double	m_lCoordScaleY;		//����ϵ��Y���ϵı���(����/mm)
	double	m_dJiaoDuX;			//ģ������x��Ƕ�ƫ��
	double	m_dJiaoDuY;			//ģ������y��Ƕ�ƫ��
	long	m_lOffsetX;			//Ԫ����X���ϵ�ƫ��(����ֵ+��ֵ=ʵ��ֵ)
	long	m_lOffsetY;			//Ԫ����Y���ϵ�ƫ��(����ֵ+��ֵ=ʵ��ֵ)
	long	m_lDatumLimit;		//��׼�����Ʒ�ֵ
	CString m_sLastModifyTime;

	map<long,CFovData*>	m_FOVMap;		//FOV map
	bool			m_bFOVIsChange; //FOV�����Ƿ����޸�

	map<long,CDatumMark *>	m_DatumMarkMap;		//��׼���map
	bool			m_bDatumMarkIsChange;		//��׼�������Ƿ����޸�

	map<long,COrganData*>	m_OrganDataMap;		//Ԫ��map
	bool			m_bOrganDataIsChange;		//Ԫ�������Ƿ����޸�

	vector<CCoordCalcOrganData*>	m_CoordCalcOrganDataList;	//��������������ֵ�����ݶ����б�

	map<int,CSpellPCB*>	m_SpellPcbMap;		//ƴ��

	CBarData	*m_pBar;

	CRITICAL_SECTION	m_Critial4FOVMap;
	CRITICAL_SECTION	m_Critial4DatumMarkMap;
	CRITICAL_SECTION	m_Critial4OrganDataMap;
	CRITICAL_SECTION	m_Critial4SpellPcbMap;

public:	
};
