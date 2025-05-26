#pragma once
#include "OrganData.h"
#include<vector>
using namespace std;

//�����ϸ����
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
	int		isPass;	//�Ƿ�ͨ����0��û�У�1��ͨ��
	CString	errType;
	int		kindred_value;	//����ֵ
	int		limit_value;	//��ֵ

	//������ûͨ��������ʱ��������ͼ��
	CImage*		m_pTemplateImg;	//��׼ͼ��
	CImage*		m_pCurrentImg;	//��ǰͼ��
	CRect m_CurrentTImgRect;//��ǰ���ͼ��Ŀ�

	COrganData*	m_pErrorOrgan;	//��ⲻͨ��ʱָ���Ԫ��
	CPadData*	m_pErrorPad;	//��ⲻͨ��ʱָ��ĺ���
	CFootData*	m_pErrorFoot;	//��ⲻͨ��ʱָ�������
};

//���������
class CDetectMasterData
{
public:
	CDetectMasterData(void);
public:
	~CDetectMasterData(void);
public:
	CString pcbName;
	long	id;
	CString	barNo;	//�����
	CString DetectData;
	CString DetectTime;
	CString	Detecter;
	int		DetectCount;
	int		ErrorCount;

	vector<CDetectDetailData*>	m_Details;
};
