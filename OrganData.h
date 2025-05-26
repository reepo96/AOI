#pragma once
#include "dataobj.h"
#include "PadData.h"
#include <vector>
#include "FovData.h"
using namespace std;

enum PrgState{PS_NEW,PS_COMPLETE,PS_SKIP,PS_INVALID};

//Ԫ��������
class COrganData :
	public CDataObj
{
public:
	COrganData(void);
public:
	~COrganData(void);

public:
	bool bDataChanged;	//�����Ƿ�ı�
	bool bImagChanged;	//ͼ���Ƿ�ı�
	bool bPadChanged;	//�����Ƿ�ı�

public:
	long	templateId;
	long	organId;

	//��Χ
	long	RangeLeft;
	long	RangeRight;
	long	RrangeTop;
	long	RangeBottom;

	//����	
	vector<CImage*>	m_MainBodyImageList;	//����ͼ��(Ӧֻ��һ���������ܴ��ڶ����׼ͼ��)

	CString		sName;	
	CString		sFootPrint;	//��װ
	double		dMidX;		//����X
	double		dMidY;		//����Y
	double		dPadX;		//��һ����X
	double		dPadY;		//��һ����Y
	double		dRotation;		//�Ƕ�
	int			iIsPolar;	//�Ƿ��Ǽ��ԣ�0����1����
	bool		bNeedDetect;//�Ƿ���Ҫ��⣨���ʱ��ʱ�趨��������⣩
	
	PrgState	PState;	//���״̬

	//vector<CPadData*>	m_PadList;	
	CPadData	*m_pPadData[4];	//��һ����
	//CPadData	*m_pPadData2;	//�ڶ�����
	//CPadData	*m_pPadData3;	//��������
	//CPadData	*m_pPadData4;	//���ĺ���

	CPadData	*m_pDelPadData[4];//��ɾ���ĺ���

	int		iLimit;	//��ֵ

	long		fovId;		//������FOV ID
	CFovData	*m_pFovData;//������FOV

public:
	void CopyPrgData(const COrganData *pSrcOrganData,const long lCurrentCenterX,const long lCurrentCenterY);
	void Draw(DRAWCONTEXT DrawContext);
protected:
	void CopyPadData(CPadData** pDestPad, const COrganData* pSrcOrganData, const CPadData* pSrcPad,const long  lCurrentCenterX, const long lCurrentCenterY);
public:
	// ɾ���������
	void DelPrgData(void);
public:
	// ��ʱ����ת90��
	void Turn(void);
};
