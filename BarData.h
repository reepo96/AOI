#pragma once
#include "dataobj.h"
#include "FovData.h"

//����������
class CBarData :
	public CDataObj
{
public:
	CBarData(void);
public:
	~CBarData(void);

public:
	long	m_templateId;
	long	m_barId;

	bool	bSerchVisible;
	long	barSerchLeft;		//���������������
	long	barSerchRight;	//�������������ұ�
	long	barSerchTop;		//�������������ϱ�
	long	barSerchBottom;	//�������������±�

	long		fovId;		//������FOV ID
	CFovData	*m_pFovData;//������FOV
};
