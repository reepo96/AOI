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

//			none,	加载PCB  编辑区域, FOV区域,	加载FOV		基准点,	  坐标映射,		元件,条码,拼板
enum WorkStep{NONE,LOADPCB, EDITAREA, EDITFOVAREA,LOADFOV,DATUMMARK, COORDINATEMAP, ORGAN,BAR,SPELLPCB,ANGLE_OFFSET};

/**
*PCB板的数据类
**/
class CTemplateData :public CDataObj
{
public:
	CTemplateData(void);
public:
	~CTemplateData(void);

public:
	bool	m_bDataIsChanged;	//数据是否已经被修改
	
	WorkStep	m_LastWorkStep;	//向导最后完成的工作步骤

	long	m_templateId;
	CStringA	m_templateName;
	long	m_width;		//板长
	long	m_height;		//板高
	CString	m_sTemplateType;//模板类型 B:bottom, T:top

	bool	bEditAreaVisible;	//编辑区域是否显示
	long	m_editAreaLeft;		//板编辑区域左边
	long	m_editAreaRight;	//板编辑区域右边
	long	m_editAreaTop;		//板编辑区域上边
	long	m_editAreaBottom;	//板编辑区域下边
		
	double	m_lCoordScaleX;		//坐标系在X轴上的比例(象素/mm)
	double	m_lCoordScaleY;		//坐标系在Y轴上的比例(象素/mm)
	double	m_dJiaoDuX;			//模板与电机x轴角度偏差
	double	m_dJiaoDuY;			//模板与电机y轴角度偏差
	long	m_lOffsetX;			//元件在X轴上的偏移(理论值+此值=实际值)
	long	m_lOffsetY;			//元件在Y轴上的偏移(理论值+此值=实际值)
	long	m_lDatumLimit;		//基准点相似阀值
	CString m_sLastModifyTime;

	map<long,CFovData*>	m_FOVMap;		//FOV map
	bool			m_bFOVIsChange; //FOV数据是否已修改

	map<long,CDatumMark *>	m_DatumMarkMap;		//基准点的map
	bool			m_bDatumMarkIsChange;		//基准点数据是否已修改

	map<long,COrganData*>	m_OrganDataMap;		//元件map
	bool			m_bOrganDataIsChange;		//元件数据是否已修改

	vector<CCoordCalcOrganData*>	m_CoordCalcOrganDataList;	//参与计算坐标相对值的数据对象列表

	map<int,CSpellPCB*>	m_SpellPcbMap;		//拼板

	CBarData	*m_pBar;

	CRITICAL_SECTION	m_Critial4FOVMap;
	CRITICAL_SECTION	m_Critial4DatumMarkMap;
	CRITICAL_SECTION	m_Critial4OrganDataMap;
	CRITICAL_SECTION	m_Critial4SpellPcbMap;

public:	
};
