#pragma once
#include "datamgr.h"
#include "TemplateData.h"
#include <vector>
using namespace std;

/**
*����pcb�����ݵ���
**/
class CTemplateDataMgr :
	public CDataMgr
{
public:
	CTemplateDataMgr(void);
public:
	~CTemplateDataMgr(void);

protected:
	bool	m_bIsConnect;

public:
	bool GetAllTemplate(vector<CTemplateData*>& templateList);

	bool GetData(long lTemplateId,CTemplateData* pTemplateData);

	bool store(CTemplateData* pTemplateData,CString& sErrorInfo);
protected:
	// ��ȡ��׼������
	bool GetDatumMarkData(CTemplateData* pTemplateData);
	// ��ȡƴ������
	bool GetSpellPcb(CTemplateData* pTemplateData);
	// ��ȡ����������
	bool GetBarData(CTemplateData* pTemplateData);
	// ��ȡԪ������
	bool GetOrganData(CTemplateData* pTemplateData);
	// ��ȡ��������������ֵ������
	bool GetCoordCalcOrganData(CTemplateData* pTemplateData);
	// ��ȡ��������
	bool GetPadData(COrganData* pOrganData);
	// ��ȡ��������
	bool GetFootData(CPadData* pPadData);
	// ����ƴ������
	bool StoreSpellPcb(CTemplateData* pTemplateData,CString& sErrorInfo);
	// ��������������
	bool StoreBar(CTemplateData* pTemplateData,CString& sErrorInfo);
	// �����׼��
	bool StoreDatumMark(CTemplateData* pTemplateData,CString& sErrorInfo);
	// ����FOV
	bool StoreFov(CTemplateData* pTemplateData,CString& sErrorInfo);
	// ����Ԫ��
	bool StoreOrgan(CTemplateData* pTemplateData,CString& sErrorInfo);
	// �����������������ֵ������
	bool StoreCoordCalcOrganData(CTemplateData* pTemplateData,CString& sErrorInfo);
	//���溸��
	bool StorePad(COrganData* pOrganData,CPadData* pPadData,CString& sErrorInfo);
	//��������
	bool StoreFoot(CFootData* pFootData,CString& sErrorInfo);

	//ɾ������
	bool DelPad(CPadData* pPadData,CString& sErrorInfo);
	//ɾ������
	bool DelFoot(CFootData* pFootData,CString& sErrorInfo);

	//����ͼƬ
	bool StoreImage(const char *fieldName,CImage *pImage);

	//��ȡͼƬ
	bool GetImage(const char *fieldName,CImage *pImage);

public:
	// �޸�Ԫ��
	bool ModifyOrgan(COrganData* pOrganData,CString& sErrorInfo);
public:
	bool ModifyPad(CPadData* pPadData,CString& sErrorInfo);
public:
	bool ModifyFoot(CFootData* pFootData,CString& sErrorInfo);
public:
	bool DelTemplate(CTemplateData* pTemplateData,CString& sErrorInfo);
};
