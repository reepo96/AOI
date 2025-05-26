#pragma once
#include "datamgr.h"
#include "TemplateData.h"
#include <vector>
using namespace std;

/**
*管理pcb板数据的类
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
	// 获取基准点数据
	bool GetDatumMarkData(CTemplateData* pTemplateData);
	// 获取拼板数据
	bool GetSpellPcb(CTemplateData* pTemplateData);
	// 获取条形码数据
	bool GetBarData(CTemplateData* pTemplateData);
	// 获取元件数据
	bool GetOrganData(CTemplateData* pTemplateData);
	// 获取参与计算坐标相对值的数据
	bool GetCoordCalcOrganData(CTemplateData* pTemplateData);
	// 获取焊盘数据
	bool GetPadData(COrganData* pOrganData);
	// 获取焊脚数据
	bool GetFootData(CPadData* pPadData);
	// 保存拼板数据
	bool StoreSpellPcb(CTemplateData* pTemplateData,CString& sErrorInfo);
	// 保存条形码数据
	bool StoreBar(CTemplateData* pTemplateData,CString& sErrorInfo);
	// 保存基准点
	bool StoreDatumMark(CTemplateData* pTemplateData,CString& sErrorInfo);
	// 保存FOV
	bool StoreFov(CTemplateData* pTemplateData,CString& sErrorInfo);
	// 保存元件
	bool StoreOrgan(CTemplateData* pTemplateData,CString& sErrorInfo);
	// 保存参与计算坐标相对值的数据
	bool StoreCoordCalcOrganData(CTemplateData* pTemplateData,CString& sErrorInfo);
	//保存焊盘
	bool StorePad(COrganData* pOrganData,CPadData* pPadData,CString& sErrorInfo);
	//保存引脚
	bool StoreFoot(CFootData* pFootData,CString& sErrorInfo);

	//删除焊盘
	bool DelPad(CPadData* pPadData,CString& sErrorInfo);
	//删除引脚
	bool DelFoot(CFootData* pFootData,CString& sErrorInfo);

	//保存图片
	bool StoreImage(const char *fieldName,CImage *pImage);

	//获取图片
	bool GetImage(const char *fieldName,CImage *pImage);

public:
	// 修改元件
	bool ModifyOrgan(COrganData* pOrganData,CString& sErrorInfo);
public:
	bool ModifyPad(CPadData* pPadData,CString& sErrorInfo);
public:
	bool ModifyFoot(CFootData* pFootData,CString& sErrorInfo);
public:
	bool DelTemplate(CTemplateData* pTemplateData,CString& sErrorInfo);
};
