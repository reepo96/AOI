#pragma once
#include "OrganData.h"
#include<vector>
using namespace std;

//检测详细数据
class CDetectDetailData
{
public:
	CDetectDetailData(void);
public:
	~CDetectDetailData(void);
public:
	long	id;
	long	masterId;
	CString	organName;
	int		spellNo;
	long	centerX;
	long	centerY;
	int		isPass;	//是否通过，0：没有，1：通过
	CString	errType;
	int		kindred_value;	//相似值
	int		limit_value;	//阀值

	//如果检测没通过，则暂时保存下面图像
	CImage*		m_pTemplateImg;	//标准图像
	CImage*		m_pCurrentImg;	//当前图像
	CRect m_CurrentTImgRect;//当前检测图像的框

	COrganData*	m_pErrorOrgan;	//检测不通过时指向的元件
	CPadData*	m_pErrorPad;	//检测不通过时指向的焊盘
	CFootData*	m_pErrorFoot;	//检测不通过时指向的引脚
};

//检测主数据
class CDetectMasterData
{
public:
	CDetectMasterData(void);
public:
	~CDetectMasterData(void);
public:
	CString pcbName;
	long	id;
	CString	barNo;	//条码号
	CString DetectData;
	CString DetectTime;
	CString	Detecter;
	int		DetectCount;
	int		ErrorCount;

	vector<CDetectDetailData*>	m_Details;
};
