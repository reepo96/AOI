#pragma once
#include "dataobj.h"
#include <vector>
using namespace std;

//引脚数据
class CFootData :
	public CDataObj
{
public:
	CFootData(void);
public:
	~CFootData(void);

public:
	long	padId;
	long	footId;

	vector<CImage*>	m_ImageList;	//引脚图像(应只有一个，但可能存在多个标准图像)
	
};

//焊盘数据
class CPadData :
	public CDataObj
{
public:
	CPadData(void);
public:
	~CPadData(void);

public:
	long	organId;
	long	padId;	

	vector<CImage*>	m_ImageList;	//焊盘图像(应只有一个，但可能存在多个标准图像)

	vector<CFootData*>	m_footList;

protected:	

public:
	void CopyPrgData(const CPadData *pSrcPadData,const long lCurrentCenterX,const long lCurrentCenterY);
	void Draw(DRAWCONTEXT DrawContext);
public:
	void MoveX(long lMoveStep);
	void MoveY(long lMoveStep);

	// 增加焊脚数
	void AddFootNum(int iNum);
	//翻转90度
	void Turn();
public:
	// 增加焊脚间间隔
	void AddFootSpaceBetween(long lSpace);
};
