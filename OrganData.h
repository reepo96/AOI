#pragma once
#include "dataobj.h"
#include "PadData.h"
#include <vector>
#include "FovData.h"
using namespace std;

enum PrgState{PS_NEW,PS_COMPLETE,PS_SKIP,PS_INVALID};

//元件数据类
class COrganData :
	public CDataObj
{
public:
	COrganData(void);
public:
	~COrganData(void);

public:
	bool bDataChanged;	//数据是否改变
	bool bImagChanged;	//图像是否改变
	bool bPadChanged;	//焊盘是否改变

public:
	long	templateId;
	long	organId;

	//范围
	long	RangeLeft;
	long	RangeRight;
	long	RrangeTop;
	long	RangeBottom;

	//主体	
	vector<CImage*>	m_MainBodyImageList;	//主体图像(应只有一个，但可能存在多个标准图像)

	CString		sName;	
	CString		sFootPrint;	//封装
	double		dMidX;		//中心X
	double		dMidY;		//中心Y
	double		dPadX;		//第一焊盘X
	double		dPadY;		//第一焊盘Y
	double		dRotation;		//角度
	int			iIsPolar;	//是否是极性，0：否，1：是
	bool		bNeedDetect;//是否需要检测（检测时临时设定，不需入库）
	
	PrgState	PState;	//编程状态

	//vector<CPadData*>	m_PadList;	
	CPadData	*m_pPadData[4];	//第一焊盘
	//CPadData	*m_pPadData2;	//第二焊盘
	//CPadData	*m_pPadData3;	//第三焊盘
	//CPadData	*m_pPadData4;	//第四焊盘

	CPadData	*m_pDelPadData[4];//被删除的焊盘

	int		iLimit;	//阀值

	long		fovId;		//所属的FOV ID
	CFovData	*m_pFovData;//所属的FOV

public:
	void CopyPrgData(const COrganData *pSrcOrganData,const long lCurrentCenterX,const long lCurrentCenterY);
	void Draw(DRAWCONTEXT DrawContext);
protected:
	void CopyPadData(CPadData** pDestPad, const COrganData* pSrcOrganData, const CPadData* pSrcPad,const long  lCurrentCenterX, const long lCurrentCenterY);
public:
	// 删除编程数据
	void DelPrgData(void);
public:
	// 逆时针旋转90度
	void Turn(void);
};
