#pragma once
#include "dataobj.h"
#include <vector>
using namespace std;

//��������
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

	vector<CImage*>	m_ImageList;	//����ͼ��(Ӧֻ��һ���������ܴ��ڶ����׼ͼ��)
	
};

//��������
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

	vector<CImage*>	m_ImageList;	//����ͼ��(Ӧֻ��һ���������ܴ��ڶ����׼ͼ��)

	vector<CFootData*>	m_footList;

protected:	

public:
	void CopyPrgData(const CPadData *pSrcPadData,const long lCurrentCenterX,const long lCurrentCenterY);
	void Draw(DRAWCONTEXT DrawContext);
public:
	void MoveX(long lMoveStep);
	void MoveY(long lMoveStep);

	// ���Ӻ�����
	void AddFootNum(int iNum);
	//��ת90��
	void Turn();
public:
	// ���Ӻ��ż���
	void AddFootSpaceBetween(long lSpace);
};
