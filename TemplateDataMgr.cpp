#include "StdAfx.h"
#include "AOI.h"
#include "TemplateDataMgr.h"
#include "ado.h"

CTemplateDataMgr::CTemplateDataMgr(void)
{
	if(!connectDB())
	{
		m_bIsConnect = false;
	}
	else
	{
		m_bIsConnect = true;
	}
}

CTemplateDataMgr::~CTemplateDataMgr(void)
{
	if( m_bIsConnect)
	{
		m_adoConnection.Close();
	}
}

//从数据库中查询数据
bool CTemplateDataMgr::GetData(long lTemplateId,CTemplateData* pTemplateData)
{
	pTemplateData->m_templateId = lTemplateId;

	//if(!connectDB())
	if( !m_bIsConnect)
		return false;

	int iWorkStep=0;
	//CBitmap Bitmap;
	HBITMAP hBitmap=NULL;
	CString	sSql;
	sSql.Format("select * from T_Template where templateId=%d",lTemplateId);

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	m_adoRecordSet.GetCollect("templateId",pTemplateData->m_templateId);
	m_adoRecordSet.GetCollect("templateName",pTemplateData->m_templateName);
	m_adoRecordSet.GetCollect("width",pTemplateData->m_width);
	m_adoRecordSet.GetCollect("height",pTemplateData->m_height);
	m_adoRecordSet.GetCollect("templateType",pTemplateData->m_sTemplateType);
	m_adoRecordSet.GetCollect("editAreaLeft",pTemplateData->m_editAreaLeft);
	m_adoRecordSet.GetCollect("editAreaRight",pTemplateData->m_editAreaRight);
	m_adoRecordSet.GetCollect("editAreaTop",pTemplateData->m_editAreaTop);
	m_adoRecordSet.GetCollect("editAreaBottom",pTemplateData->m_editAreaBottom);

	m_adoRecordSet.GetCollect("lastWorkStep",iWorkStep);
	pTemplateData->m_LastWorkStep =(WorkStep)iWorkStep;

	/*m_adoRecordSet.GetCollect("barSerchLeft",pTemplateData->m_barSerchLeft);
	m_adoRecordSet.GetCollect("barSerchRight",pTemplateData->m_barSerchRight);
	m_adoRecordSet.GetCollect("barSerchTop",pTemplateData->m_barSerchTop);
	m_adoRecordSet.GetCollect("barSerchBottom",pTemplateData->m_barSerchBottom);

	m_adoRecordSet.GetCollect("barBodyLeft",pTemplateData->m_barBodyLeft);
	m_adoRecordSet.GetCollect("barBodyRight",pTemplateData->m_barBodyRight);
	m_adoRecordSet.GetCollect("barBodyTop",pTemplateData->m_barBodyTop);
	m_adoRecordSet.GetCollect("barBodyottom",pTemplateData->m_barBodyottom);*/

	m_adoRecordSet.GetCollect("coordScaleX",pTemplateData->m_lCoordScaleX);
	m_adoRecordSet.GetCollect("coordScaleY",pTemplateData->m_lCoordScaleY);
	m_adoRecordSet.GetCollect("jiaoduX",pTemplateData->m_dJiaoDuX);
	m_adoRecordSet.GetCollect("jiaoduY",pTemplateData->m_dJiaoDuY);
	m_adoRecordSet.GetCollect("offsetX",pTemplateData->m_lOffsetX);
	m_adoRecordSet.GetCollect("offsetY",pTemplateData->m_lOffsetY);
	m_adoRecordSet.GetCollect("datumLimit",pTemplateData->m_lDatumLimit);
	m_adoRecordSet.GetCollect("lastModifyTime",pTemplateData->m_sLastModifyTime);

	//m_adoRecordSet.GetChunk("image",hBitmap);
	//if(hBitmap != NULL)
	//	pTemplateData->m_ImageObj.Attach(hBitmap);

	GetImage("image",&(pTemplateData->m_ImageObj));

	m_adoConnection.CommitTrans();
	m_adoRecordSet.Close();

	//取FOV
	hBitmap = NULL;
	sSql.Format("select * from T_FOV where templateId=%d",lTemplateId);
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();
	while(!m_adoRecordSet.IsEOF())
	{
		CFovData *pFov =new CFovData();
		m_adoRecordSet.GetCollect("FOVID",pFov->fovId);
		m_adoRecordSet.GetCollect("type",pFov->type);
		m_adoRecordSet.GetCollect("templateId",pFov->templateId);
		m_adoRecordSet.GetCollect("FOVLeft",pFov->left);
		m_adoRecordSet.GetCollect("FOVRight",pFov->right);
		m_adoRecordSet.GetCollect("FOVTop",pFov->top);
		m_adoRecordSet.GetCollect("FOVBottom",pFov->bottom);
		m_adoRecordSet.GetCollect("CheckOrder",pFov->CheckOrder);
		//m_adoRecordSet.GetChunk("ImageObj",hBitmap);
		//if(hBitmap != NULL)
		//	pFov->m_ImageObj.Attach(hBitmap);

		GetImage("ImageObj",&(pFov->m_ImageObj));

		hBitmap=NULL;
		pTemplateData->m_FOVMap.insert(::make_pair(pFov->fovId,pFov));

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	GetBarData(pTemplateData);

	//取基准点数据
	GetDatumMarkData(pTemplateData);

	GetSpellPcb(pTemplateData);

	//取基元件数据
	GetOrganData(pTemplateData);

	// 获取参与计算坐标相对值的数据
	GetCoordCalcOrganData(pTemplateData);

	m_adoConnection.CommitTrans();
	//m_adoConnection.Close();

	pTemplateData->m_bDataIsChanged = false;

	return true;
}

bool CTemplateDataMgr::store(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	if(!pTemplateData->m_bDataIsChanged)
		return true;

	//if(!connectDB())
	if( !m_bIsConnect)
	{
		sErrorInfo = "No connect to database";
		return false;
	}

	CString	sSql;
	long	lOldTemplateId = pTemplateData->m_templateId;//保存原来的templateId

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();

	m_adoConnection.BeginTrans();

	if(pTemplateData->m_templateId == -1) //add new
	{
		m_adoRecordSet.Open("select * from T_Template");
		m_adoRecordSet.AddNew();
	}
	else
	{
		sSql.Format("select * from T_Template where templateId=%d",pTemplateData->m_templateId);		
		m_adoRecordSet.Open(sSql);		
	}

	m_adoRecordSet.PutCollect("templateName",pTemplateData->m_templateName);
	m_adoRecordSet.PutCollect("width",pTemplateData->m_width);
	m_adoRecordSet.PutCollect("height",pTemplateData->m_height);
	m_adoRecordSet.PutCollect("templateType",pTemplateData->m_sTemplateType);
	m_adoRecordSet.PutCollect("editAreaLeft",pTemplateData->m_editAreaLeft);
	m_adoRecordSet.PutCollect("editAreaRight",pTemplateData->m_editAreaRight);
	m_adoRecordSet.PutCollect("editAreaTop",pTemplateData->m_editAreaTop);
	m_adoRecordSet.PutCollect("editAreaBottom",pTemplateData->m_editAreaBottom);

	m_adoRecordSet.PutCollect("lastWorkStep",(int)pTemplateData->m_LastWorkStep);

	/*m_adoRecordSet.PutCollect("barSerchLeft",pTemplateData->m_barSerchLeft);
	m_adoRecordSet.PutCollect("barSerchRight",pTemplateData->m_barSerchRight);
	m_adoRecordSet.PutCollect("barSerchTop",pTemplateData->m_barSerchTop);
	m_adoRecordSet.PutCollect("barSerchBottom",pTemplateData->m_barSerchBottom);

	m_adoRecordSet.PutCollect("barBodyLeft",pTemplateData->m_barBodyLeft);
	m_adoRecordSet.PutCollect("barBodyRight",pTemplateData->m_barBodyRight);
	m_adoRecordSet.PutCollect("barBodyTop",pTemplateData->m_barBodyTop);
	m_adoRecordSet.PutCollect("barBodyottom",pTemplateData->m_barBodyottom);*/

	m_adoRecordSet.PutCollect("coordScaleX",pTemplateData->m_lCoordScaleX);
	m_adoRecordSet.PutCollect("coordScaleY",pTemplateData->m_lCoordScaleY);
	m_adoRecordSet.PutCollect("jiaoduX",pTemplateData->m_dJiaoDuX);
	m_adoRecordSet.PutCollect("jiaoduY",pTemplateData->m_dJiaoDuY);
	m_adoRecordSet.PutCollect("offsetX",pTemplateData->m_lOffsetX);
	m_adoRecordSet.PutCollect("offsetY",pTemplateData->m_lOffsetY);
	m_adoRecordSet.PutCollect("datumLimit",pTemplateData->m_lDatumLimit);

	CTime t = CTime::GetCurrentTime();
	pTemplateData->m_sLastModifyTime = t.Format("%Y-%m-%d %H:%M");	
	m_adoRecordSet.PutCollect("lastModifyTime",pTemplateData->m_sLastModifyTime);

	//UINT uiDatalen = ( ((m_ImageObj.GetBPP()*m_ImageObj.GetWidth())+31)/32*4 ) *m_ImageObj.GetHeight();
	//m_adoRecordSet.AppendChunk("image",m_ImageObj.GetBits(),2000);//uiDatalen);

	if( !pTemplateData->m_ImageObj.IsNull() )
	{
		//pTemplateData->m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("image","c:\\aoi_tmpbitmap.bmp");

		StoreImage("image",&(pTemplateData->m_ImageObj));
	}
	else
	{
		m_adoRecordSet.PutCollect("image",0);
	}
	//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

	if(FALSE == m_adoRecordSet.Update(sErrorInfo))
	{
		sErrorInfo = "保存模板数据失败原因：" + sErrorInfo;
		m_adoConnection.RollbackTrans();
		m_adoRecordSet.Close();
		m_adoConnection.Close();
		return false;
	}
	m_adoRecordSet.Close();

	//如果是新增的模板，保存后则还要将模板id查询出来
	if(pTemplateData->m_templateId == -1)
	{
		sSql = "select max(templateId) as MaxTemplateId from T_Template";
		m_adoRecordSet.Open(sSql);

		m_adoRecordSet.GetCollect("MaxTemplateId",pTemplateData->m_templateId);
		m_adoRecordSet.Close();
	}

	//保存FOV
	if(false == StoreFov(pTemplateData,sErrorInfo))
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	if(false == StoreBar(pTemplateData,sErrorInfo) )
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	//保存基准点
	if(false == StoreDatumMark(pTemplateData,sErrorInfo) )
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	if(false == StoreSpellPcb(pTemplateData,sErrorInfo) )
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	//保存元件
	if(false == StoreOrgan(pTemplateData,sErrorInfo) )
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	//保存参与计算坐标相对值的数据
	if(false == StoreCoordCalcOrganData(pTemplateData,sErrorInfo) )
	{
		m_adoConnection.Close();
		pTemplateData->m_templateId = lOldTemplateId;
		return false;
	}

	m_adoConnection.CommitTrans();	
	//m_adoConnection.Close();

	pTemplateData->m_bDataIsChanged = false;

	return true;
}

// 获取拼板数据
bool CTemplateDataMgr::GetSpellPcb(CTemplateData* pTemplateData)
{
	CString	sSql;
	sSql.Format("select * from T_SpellPcb where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);
	int	count = 0;
	while(!m_adoRecordSet.IsEOF())
	{
		CSpellPCB *pSpellPCB = new CSpellPCB();
		pSpellPCB->m_templateId = pTemplateData->m_templateId;
		m_adoRecordSet.GetCollect("id",pSpellPCB->id);
		m_adoRecordSet.GetCollect("no",pSpellPCB->no);
		m_adoRecordSet.GetCollect("DatumMarkId1",pSpellPCB->lDatumMarkId1);
		m_adoRecordSet.GetCollect("DatumMarkId2",pSpellPCB->lDatumMarkId2);

		map<long,CDatumMark *>::iterator it= pTemplateData->m_DatumMarkMap.find(pSpellPCB->lDatumMarkId1);
		if( it != pTemplateData->m_DatumMarkMap.end())
		{
			pSpellPCB->pDatumMark1 = (*it).second;
		}

		it= pTemplateData->m_DatumMarkMap.find(pSpellPCB->lDatumMarkId2);
		if( it != pTemplateData->m_DatumMarkMap.end())
		{
			pSpellPCB->pDatumMark2 = (*it).second;
		}

		count++;
		pTemplateData->m_SpellPcbMap.insert(pTemplateData->m_SpellPcbMap.begin(),::make_pair(count,pSpellPCB));

		m_adoRecordSet.MoveNext();
	}

	m_adoRecordSet.Close();

	return true;
}

// 获取条形码数据
bool CTemplateDataMgr::GetBarData(CTemplateData* pTemplateData)
{
	CString	sSql;
	sSql.Format("select * from T_Bar where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);
	if(!m_adoRecordSet.IsEOF())
	{
		CBarData *pBar=new CBarData();

		m_adoRecordSet.GetCollect("barId",pBar->m_barId);
		m_adoRecordSet.GetCollect("bodyLeft",pBar->left);
		m_adoRecordSet.GetCollect("bodyRight",pBar->right);
		m_adoRecordSet.GetCollect("bodyTop",pBar->top);
		m_adoRecordSet.GetCollect("bodyBottom",pBar->bottom);
		m_adoRecordSet.GetCollect("barSerchLeft",pBar->barSerchLeft);
		m_adoRecordSet.GetCollect("barSerchRight",pBar->barSerchRight);
		m_adoRecordSet.GetCollect("barSerchTop",pBar->barSerchTop);
		m_adoRecordSet.GetCollect("barSerchBottom",pBar->barSerchBottom);
		m_adoRecordSet.GetCollect("fovId",pBar->fovId);

		map<long,CFovData*>::iterator it= pTemplateData->m_FOVMap.find(pBar->fovId);
		if( it != pTemplateData->m_FOVMap.end())
		{
			pBar->m_pFovData = (*it).second;
		}

		pTemplateData->m_pBar = pBar;
	}

	m_adoRecordSet.Close();

	return true;
}

// 获取基准点数据
bool CTemplateDataMgr::GetDatumMarkData(CTemplateData* pTemplateData)
{
	CString	sSql;
	sSql.Format("select * from T_DatumMark where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);

	HBITMAP hBitmap=NULL;
	while(!m_adoRecordSet.IsEOF())
	{
		CDatumMark *pDatumMark = new CDatumMark();		
		m_adoRecordSet.GetCollect("datumMarkId",pDatumMark->m_datumMarkId);
		m_adoRecordSet.GetCollect("templateId",pDatumMark->m_templateId);
		m_adoRecordSet.GetCollect("type",pDatumMark->type);
		m_adoRecordSet.GetCollect("markLeft",pDatumMark->left);
		m_adoRecordSet.GetCollect("markRight",pDatumMark->right);
		m_adoRecordSet.GetCollect("marktop",pDatumMark->top);
		m_adoRecordSet.GetCollect("markbottom",pDatumMark->bottom);
		m_adoRecordSet.GetCollect("fovId",pDatumMark->fovId);
		m_adoRecordSet.GetCollect("searchLeft",pDatumMark->searchLeft);
		m_adoRecordSet.GetCollect("searchRight",pDatumMark->searchRight);
		m_adoRecordSet.GetCollect("searchTop",pDatumMark->searchTop);
		m_adoRecordSet.GetCollect("searchBottom",pDatumMark->searchBottom);

		map<long,CFovData*>::iterator it= pTemplateData->m_FOVMap.find(pDatumMark->fovId);
		if( it != pTemplateData->m_FOVMap.end())
		{
			pDatumMark->m_pFovData = (*it).second;
		}

		//m_adoRecordSet.GetChunk("imageData",hBitmap);
		//if(hBitmap != NULL)
		//	pDatumMark->m_ImageObj.Attach(hBitmap);

		GetImage("imageData",&(pDatumMark->m_ImageObj));

		hBitmap=NULL;
		pTemplateData->m_DatumMarkMap.insert(::make_pair(pDatumMark->m_datumMarkId,pDatumMark));
		m_adoRecordSet.MoveNext();
	}	
	m_adoRecordSet.Close();

	return true;
}

//获得元件数据
bool CTemplateDataMgr::GetOrganData(CTemplateData* pTemplateData)
{
	COrganData *pOrgaData=NULL;
	int		iOldOrganId = 0;
	int		iCurrentOrganId = 0;
	int		iState=0;
	CString	sSql;
	sSql.Format("select o.organId,o.*,i.ImageObj from T_Organ o LEFT OUTER JOIN T_OrganImage i ON o.organId = i.organId where o.templateId=%d",
		pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);

	HBITMAP hBitmap =NULL;
	while(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("organId",iCurrentOrganId);

		if(iCurrentOrganId != iOldOrganId)
		{
			pOrgaData = new COrganData();
			pOrgaData->organId = iCurrentOrganId;
			iOldOrganId = iCurrentOrganId;

			m_adoRecordSet.GetCollect("templateId",pOrgaData->templateId);
			m_adoRecordSet.GetCollect("bodyleft",pOrgaData->left);
			m_adoRecordSet.GetCollect("bodyright",pOrgaData->right);
			m_adoRecordSet.GetCollect("bodytop",pOrgaData->top);
			m_adoRecordSet.GetCollect("bodybottom",pOrgaData->bottom);
			m_adoRecordSet.GetCollect("RangeLeft",pOrgaData->RangeLeft);
			m_adoRecordSet.GetCollect("RangeRight",pOrgaData->RangeRight);
			m_adoRecordSet.GetCollect("RrangeTop",pOrgaData->RrangeTop);
			m_adoRecordSet.GetCollect("RangeBottom",pOrgaData->RangeBottom);

			m_adoRecordSet.GetCollect("OrganName",pOrgaData->sName);
			m_adoRecordSet.GetCollect("FootPrint",pOrgaData->sFootPrint);
			m_adoRecordSet.GetCollect("dMidX",pOrgaData->dMidX);
			m_adoRecordSet.GetCollect("dMidY",pOrgaData->dMidY);
			m_adoRecordSet.GetCollect("dPadX",pOrgaData->dPadX);
			m_adoRecordSet.GetCollect("dPadY",pOrgaData->dPadY);
			m_adoRecordSet.GetCollect("dRotation",pOrgaData->dRotation);
			m_adoRecordSet.GetCollect("PState",iState);
			pOrgaData->PState = (PrgState)iState;
			m_adoRecordSet.GetCollect("iLimit",pOrgaData->iLimit);

			m_adoRecordSet.GetCollect("fovId",pOrgaData->fovId);
			map<long,CFovData*>::iterator it= pTemplateData->m_FOVMap.find(pOrgaData->fovId);
			if( it != pTemplateData->m_FOVMap.end())
			{
				pOrgaData->m_pFovData = (*it).second;
			}

			//m_adoRecordSet.GetChunk("ImageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("ImageObj",pImage))
			{
				pOrgaData->m_MainBodyImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage = NULL;
			}
			//}
			//hBitmap = NULL;

			pTemplateData->m_OrganDataMap.insert(::make_pair(pOrgaData->organId,pOrgaData));
		}
		else
		{
			//m_adoRecordSet.GetChunk("ImageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("ImageObj",pImage))
			{
				pOrgaData->m_MainBodyImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage = NULL;
			}
			//}
			//hBitmap=NULL;
		}

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	// 获取焊盘数据
	map<long,COrganData*>::iterator it=pTemplateData->m_OrganDataMap.begin();
	for(;it!=pTemplateData->m_OrganDataMap.end();it++)
	{
		GetPadData( (*it).second );
	}

	return true;
}

//获得元件数据
bool CTemplateDataMgr::GetCoordCalcOrganData(CTemplateData* pTemplateData)
{
	COrganData *pOrgaData=NULL;
	CCoordCalcOrganData *pCoordCalcOrganData=NULL;
	long	lOrganId=0;
	CString	sSql;

	sSql.Format("select * from T_CoordCalcOrgan where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);

	while(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("organId",lOrganId);

		pCoordCalcOrganData = new CCoordCalcOrganData();

		map<long,COrganData*>::iterator it= pTemplateData->m_OrganDataMap.find(lOrganId);
		if( it != pTemplateData->m_OrganDataMap.end())
		{
			pCoordCalcOrganData->pOrganData = (*it).second;
		}

		m_adoRecordSet.GetCollect("templateId",pCoordCalcOrganData->templateId);
		m_adoRecordSet.GetCollect("left",pCoordCalcOrganData->left);
		m_adoRecordSet.GetCollect("right",pCoordCalcOrganData->right);
		m_adoRecordSet.GetCollect("top",pCoordCalcOrganData->top);
		m_adoRecordSet.GetCollect("bottom",pCoordCalcOrganData->bottom);

		pTemplateData->m_CoordCalcOrganDataList.push_back(pCoordCalcOrganData);		

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	return true;
}

// 获取焊盘数据
bool CTemplateDataMgr::GetPadData(COrganData* pOrganData)
{
	CPadData *pPadData=NULL;
	int		iOldPadId = 0;
	int		iCurrentPadId = 0;
	int		iIndex=0;
	CString	sSql;
	sSql.Format("select T_Pad.padId,T_Pad.*,T_PadImage.ImageObj from T_Pad,T_PadImage where T_PadImage.padId=T_Pad.padId and organId=%d",pOrganData->organId);
	m_adoRecordSet.Open(sSql);

	HBITMAP hBitmap= NULL;
	while(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("padId",iCurrentPadId);
		if(iCurrentPadId != iOldPadId)
		{
			pPadData = new CPadData();
			pPadData->padId = iCurrentPadId;
			iOldPadId = iCurrentPadId;

			m_adoRecordSet.GetCollect("organId",pPadData->organId);
			m_adoRecordSet.GetCollect("padleft",pPadData->left);
			m_adoRecordSet.GetCollect("padright",pPadData->right);
			m_adoRecordSet.GetCollect("padtop",pPadData->top);
			m_adoRecordSet.GetCollect("padbottom",pPadData->bottom);

			//m_adoRecordSet.GetChunk("ImageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("ImageObj",pImage))
			{
				pPadData->m_ImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage = NULL;
			}
			//}
			//hBitmap=NULL;

			pOrganData->m_pPadData[iIndex] = pPadData;
			iIndex++;
		}
		else
		{
			//m_adoRecordSet.GetChunk("ImageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("ImageObj",pImage))
			{
				pPadData->m_ImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage = NULL;
			}
			//}
			//hBitmap=NULL;
		}

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	//取焊脚数据
	for(int i=0;i<4;i++)
	{
		if(false == GetFootData(pOrganData->m_pPadData[i]) )
		{
			return false;
		}
	}

	return true;
}

// 获取焊脚数据
bool CTemplateDataMgr::GetFootData(CPadData* pPadData)
{
	if( pPadData == NULL)
	{
		return true;
	}

	CFootData *pFootData=NULL;
	int		iOldFootId = 0;
	int		iCurrentFootId = 0;

	CString	sSql;
	sSql.Format("select T_Foot.footId,T_Foot.*,T_FootImage.imageObj from T_Foot,T_FootImage where T_FootImage.footId=T_Foot.footId and padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);

	HBITMAP hBitmap = NULL;
	while(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("footId",iCurrentFootId);
		if(iCurrentFootId != iOldFootId)
		{
			pFootData = new CFootData();
			pFootData->footId = iCurrentFootId;
			iOldFootId = iCurrentFootId;

			m_adoRecordSet.GetCollect("padId",pFootData->padId);
			m_adoRecordSet.GetCollect("footLeft",pFootData->left);
			m_adoRecordSet.GetCollect("footRight",pFootData->right);
			m_adoRecordSet.GetCollect("footTop",pFootData->top);
			m_adoRecordSet.GetCollect("footBottom",pFootData->bottom);

			//m_adoRecordSet.GetChunk("imageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("imageObj",pImage))
			{
				pFootData->m_ImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage = NULL;
			}
			//}
			//hBitmap=NULL;

			pPadData->m_footList.push_back(pFootData);
		}
		else
		{
			//m_adoRecordSet.GetChunk("imageObj",hBitmap);
			//if(hBitmap != NULL)
			//{
			CImage *pImage = new CImage();
			//pImage->Attach(hBitmap);
			if(GetImage("imageObj",pImage))
			{
				pFootData->m_ImageList.push_back(pImage);
			}
			else
			{
				delete pImage;
				pImage=NULL;
			}
			//}
			//hBitmap=NULL;
		}

		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	return true;
}

bool CTemplateDataMgr::StoreSpellPcb(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	CString sSql;
	sSql.Format("select * from T_SpellPcb where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}

		m_adoRecordSet.MoveNext();
	}

	map<int,CSpellPCB*>::iterator it =pTemplateData->m_SpellPcbMap.begin();
	for(;it!=pTemplateData->m_SpellPcbMap.end();it++)
	{
		if((*it).second->no ==0 || (*it).second->pDatumMark1 == NULL || (*it).second->pDatumMark2 == NULL)
		{
			continue;
		}

		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("templateId",pTemplateData->m_templateId);
		m_adoRecordSet.PutCollect("no",(*it).second->no);
		m_adoRecordSet.PutCollect("DatumMarkId1",(*it).second->pDatumMark1->m_datumMarkId);
		m_adoRecordSet.PutCollect("DatumMarkId2",(*it).second->pDatumMark2->m_datumMarkId);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	return true;
}

bool CTemplateDataMgr::StoreBar(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	CString sSql;
	sSql.Format("select * from T_Bar where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}

		m_adoRecordSet.MoveNext();
	}

	if(pTemplateData->m_pBar != NULL )
	{
		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("templateId",pTemplateData->m_templateId);	

		m_adoRecordSet.PutCollect("bodyLeft",pTemplateData->m_pBar->left);
		m_adoRecordSet.PutCollect("bodyRight",pTemplateData->m_pBar->right);
		m_adoRecordSet.PutCollect("bodyTop",pTemplateData->m_pBar->top);
		m_adoRecordSet.PutCollect("bodyBottom",pTemplateData->m_pBar->bottom);
		m_adoRecordSet.PutCollect("barSerchLeft",pTemplateData->m_pBar->barSerchLeft);
		m_adoRecordSet.PutCollect("barSerchRight",pTemplateData->m_pBar->barSerchRight);
		m_adoRecordSet.PutCollect("barSerchTop",pTemplateData->m_pBar->barSerchTop);
		m_adoRecordSet.PutCollect("barSerchBottom",pTemplateData->m_pBar->barSerchBottom);
		m_adoRecordSet.PutCollect("fovId",pTemplateData->m_pBar->m_pFovData->fovId);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	return true;
}

// 保存基准点
bool CTemplateDataMgr::StoreDatumMark(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	if(pTemplateData->m_bDatumMarkIsChange)
	{
		CString sSql;

		long	lMaxDatumMarkId=0;

		//先获得元件id最大值
		sSql = "select ISNULL( max(datumMarkId),0 ) as MaxId from T_DatumMark";
		m_adoRecordSet.Open(sSql);
		if(!m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("MaxId",lMaxDatumMarkId);
		}
		m_adoRecordSet.Close();

		sSql.Format("select * from T_DatumMark where templateId=%d",pTemplateData->m_templateId);
		m_adoRecordSet.Open(sSql);

		long	lDatumMarkId=0;

		//修改
		while( !m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("datumMarkId",lDatumMarkId);
			map<long,CDatumMark *>::const_iterator it = pTemplateData->m_DatumMarkMap.find(lDatumMarkId);
			if(it != pTemplateData->m_DatumMarkMap.end())	//find it and modify it
			{
				m_adoRecordSet.PutCollect("templateId",(*it).second->m_templateId);
				m_adoRecordSet.PutCollect("type",(*it).second->type);
				m_adoRecordSet.PutCollect("markLeft",(*it).second->left);
				m_adoRecordSet.PutCollect("markRight",(*it).second->right);
				m_adoRecordSet.PutCollect("marktop",(*it).second->top);
				m_adoRecordSet.PutCollect("markbottom",(*it).second->bottom);
				m_adoRecordSet.PutCollect("searchLeft",(*it).second->searchLeft);
				m_adoRecordSet.PutCollect("searchRight",(*it).second->searchRight);
				m_adoRecordSet.PutCollect("searchTop",(*it).second->searchTop);
				m_adoRecordSet.PutCollect("searchBottom",(*it).second->searchBottom);

				(*it).second->fovId = (*it).second->m_pFovData->fovId;
				m_adoRecordSet.PutCollect("fovId",(*it).second->fovId);

				//保存图片
				if( !(*it).second->m_ImageObj.IsNull() )
				{
					//(*it).second->m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
					//m_adoRecordSet.AppendChunk("imageData","c:\\aoi_tmpbitmap.bmp");
					//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

					StoreImage("imageData",&((*it).second->m_ImageObj));
				}
				else
				{
					m_adoRecordSet.PutCollect("imageData",0);
				}

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存修改的数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

			}
			else //no find it then delete it
			{
				m_adoRecordSet.Delete();
				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}
			}

			m_adoRecordSet.MoveNext();
		}//end while

		//新增
		map<long,CDatumMark *>::iterator it = pTemplateData->m_DatumMarkMap.begin();
		for(;it != pTemplateData->m_DatumMarkMap.end();it++)
		{
			(*it).second->m_templateId = pTemplateData->m_templateId;
			if( (*it).first<= 0) //new
			{
				lMaxDatumMarkId++;
				(*it).second->m_datumMarkId = lMaxDatumMarkId;
				m_adoRecordSet.AddNew();

				m_adoRecordSet.PutCollect("datumMarkId",lMaxDatumMarkId);
				m_adoRecordSet.PutCollect("templateId",pTemplateData->m_templateId);
				m_adoRecordSet.PutCollect("type",(*it).second->type);
				m_adoRecordSet.PutCollect("markLeft",(*it).second->left);
				m_adoRecordSet.PutCollect("markRight",(*it).second->right);
				m_adoRecordSet.PutCollect("marktop",(*it).second->top);
				m_adoRecordSet.PutCollect("markbottom",(*it).second->bottom);
				m_adoRecordSet.PutCollect("searchLeft",(*it).second->searchLeft);
				m_adoRecordSet.PutCollect("searchRight",(*it).second->searchRight);
				m_adoRecordSet.PutCollect("searchTop",(*it).second->searchTop);
				m_adoRecordSet.PutCollect("searchBottom",(*it).second->searchBottom);

				(*it).second->fovId = (*it).second->m_pFovData->fovId;
				m_adoRecordSet.PutCollect("fovId",(*it).second->fovId);

				//保存图片
				if( !(*it).second->m_ImageObj.IsNull() )
				{
					//(*it).second->m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
					//m_adoRecordSet.AppendChunk("imageData","c:\\aoi_tmpbitmap.bmp");
					//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

					StoreImage("imageData",&((*it).second->m_ImageObj));
				}
				else
				{
					m_adoRecordSet.PutCollect("imageData",0);
				}

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存新增的FOV数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

			}//end if
		}//end for

		m_adoRecordSet.Close();
	}//end if

	return true;
}

// 保存FOV
bool CTemplateDataMgr::StoreFov(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	if(pTemplateData->m_bFOVIsChange)
	{
		CString sSql;

		long	lMaxFovId=0;

		//先获得元件id最大值
		sSql = "select ISNULL( max(FOVID),0 ) as MaxId from T_FOV";
		m_adoRecordSet.Open(sSql);
		if(!m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("MaxId",lMaxFovId);
		}
		m_adoRecordSet.Close();

		sSql.Format("select * from T_FOV where templateId=%d",pTemplateData->m_templateId);
		m_adoRecordSet.Open(sSql);

		long	lFovId=0;

		//修改
		while( !m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("FOVID",lFovId);
			map<long,CFovData *>::const_iterator it = pTemplateData->m_FOVMap.find(lFovId);
			if(it != pTemplateData->m_FOVMap.end())	//find it and modify it
			{
				m_adoRecordSet.PutCollect("templateId",(*it).second->templateId);
				m_adoRecordSet.PutCollect("FOVLeft",(*it).second->left);
				m_adoRecordSet.PutCollect("FOVRight",(*it).second->right);
				m_adoRecordSet.PutCollect("FOVTop",(*it).second->top);
				m_adoRecordSet.PutCollect("FOVBottom",(*it).second->bottom);
				m_adoRecordSet.PutCollect("CheckOrder",(*it).second->CheckOrder);

				//保存图片
				if( !(*it).second->m_ImageObj.IsNull() )
				{
					//(*it).second->m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
					//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
					//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

					StoreImage("ImageObj",&((*it).second->m_ImageObj));
				}//end if
				else
				{
					//m_adoRecordSet.PutCollect("ImageObj",0);
					sErrorInfo = g_LoadString("IDS_SANDFOV_BEFORE_SAVE","一些FOV的图像为空,请先扫描FOV,然后再保存");
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存修改的FOV数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}
			}
			else	//no find
			{
				m_adoRecordSet.Delete();
				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}
			}

			m_adoRecordSet.MoveNext();

		}//end while


		//新增
		map<long,CFovData *>::iterator it = pTemplateData->m_FOVMap.begin();
		for(;it != pTemplateData->m_FOVMap.end();it++)
		{
			(*it).second->templateId = pTemplateData->m_templateId;
			if( (*it).first<= 0) //new
			{
				m_adoRecordSet.AddNew();

				lMaxFovId++;
				(*it).second->fovId = lMaxFovId;
				m_adoRecordSet.PutCollect("FOVID",(*it).second->fovId);
				m_adoRecordSet.PutCollect("type",(*it).second->type);
				m_adoRecordSet.PutCollect("templateId",pTemplateData->m_templateId);
				m_adoRecordSet.PutCollect("FOVLeft",(*it).second->left);
				m_adoRecordSet.PutCollect("FOVRight",(*it).second->right);
				m_adoRecordSet.PutCollect("FOVTop",(*it).second->top);
				m_adoRecordSet.PutCollect("FOVBottom",(*it).second->bottom);
				m_adoRecordSet.PutCollect("CheckOrder",(*it).second->CheckOrder);

				//保存图片
				if( !(*it).second->m_ImageObj.IsNull() )
				{
					//(*it).second->m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
					//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
					//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
					StoreImage("ImageObj",&((*it).second->m_ImageObj));
				}
				else
				{
					//m_adoRecordSet.PutCollect("ImageObj",0);
					sErrorInfo = g_LoadString("IDS_SANDFOV_BEFORE_SAVE","一些FOV的图像为空,请先扫描FOV,然后再保存");
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存新增的FOV数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

			}//end if
		}//end for		

		m_adoRecordSet.Close();
	}//end if

	return true;
}

// 保存元件
bool CTemplateDataMgr::StoreOrgan(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	if(pTemplateData->m_bOrganDataIsChange)
	{
		CString sSql;
		long	lMaxOrganId=0;

		//先获得元件id最大值
		sSql = "select ISNULL( max(organId),0 ) as MaxId from T_Organ";
		m_adoRecordSet.Open(sSql);
		if(!m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("MaxId",lMaxOrganId);
		}
		m_adoRecordSet.Close();

		sSql.Format("select * from T_Organ where templateId=%d",pTemplateData->m_templateId);
		m_adoRecordSet.Open(sSql);

		long	lOrganId=0;

		//修改
		while( !m_adoRecordSet.IsEOF())
		{
			m_adoRecordSet.GetCollect("organId",lOrganId);
			map<long,COrganData *>::const_iterator it = pTemplateData->m_OrganDataMap.find(lOrganId);
			if(it != pTemplateData->m_OrganDataMap.end())	//find it and modify it
			{
				m_adoRecordSet.PutCollect("templateId",(*it).second->templateId);
				m_adoRecordSet.PutCollect("bodyleft",(*it).second->left);
				m_adoRecordSet.PutCollect("bodyright",(*it).second->right);
				m_adoRecordSet.PutCollect("bodytop",(*it).second->top);
				m_adoRecordSet.PutCollect("bodybottom",(*it).second->bottom);

				m_adoRecordSet.PutCollect("RangeLeft",(*it).second->RangeLeft);
				m_adoRecordSet.PutCollect("RangeRight",(*it).second->RangeRight);
				m_adoRecordSet.PutCollect("RrangeTop",(*it).second->RrangeTop);
				m_adoRecordSet.PutCollect("RangeBottom",(*it).second->RangeBottom);

				m_adoRecordSet.PutCollect("OrganName",(*it).second->sName);
				m_adoRecordSet.PutCollect("FootPrint",(*it).second->sFootPrint);
				m_adoRecordSet.PutCollect("dMidX",(*it).second->dMidX);
				m_adoRecordSet.PutCollect("dMidY",(*it).second->dMidY);
				m_adoRecordSet.PutCollect("dPadX",(*it).second->dPadX);
				m_adoRecordSet.PutCollect("dPadY",(*it).second->dPadY);
				m_adoRecordSet.PutCollect("dRotation",(*it).second->dRotation);
				m_adoRecordSet.PutCollect("PState",(long)((*it).second->PState));
				m_adoRecordSet.PutCollect("iLimit",(*it).second->iLimit);

				if( (*it).second->m_pFovData != NULL)
				{
					(*it).second->fovId = (*it).second->m_pFovData->fovId;
				}
				m_adoRecordSet.PutCollect("fovId",(*it).second->fovId);

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存修改的元件数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}
			}
			else // no find
			{
				m_adoRecordSet.Delete();
				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "删除元件数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}
			}

			m_adoRecordSet.MoveNext();

		}//end while

		//新增
		map<long,COrganData *>::iterator it = pTemplateData->m_OrganDataMap.end();
		it--;
		do
		{
			//for(;it != pTemplateData->m_OrganDataMap.begin();it--)
			//{
			(*it).second->templateId = pTemplateData->m_templateId;
			if( (*it).first<= 0) //new
			{
				m_adoRecordSet.AddNew();

				COrganData *pOrgaData = (*it).second;

				lMaxOrganId++;
				pOrgaData->organId = lMaxOrganId;

				m_adoRecordSet.PutCollect("organId",pOrgaData->organId);
				m_adoRecordSet.PutCollect("templateId",pOrgaData->templateId);
				m_adoRecordSet.PutCollect("bodyleft",pOrgaData->left);
				m_adoRecordSet.PutCollect("bodyright",pOrgaData->right);
				m_adoRecordSet.PutCollect("bodytop",pOrgaData->top);
				m_adoRecordSet.PutCollect("bodybottom",pOrgaData->bottom);

				m_adoRecordSet.PutCollect("RangeLeft",pOrgaData->RangeLeft);
				m_adoRecordSet.PutCollect("RangeRight",pOrgaData->RangeRight);
				m_adoRecordSet.PutCollect("RrangeTop",pOrgaData->RrangeTop);
				m_adoRecordSet.PutCollect("RangeBottom",pOrgaData->RangeBottom);

				m_adoRecordSet.PutCollect("OrganName",pOrgaData->sName);
				m_adoRecordSet.PutCollect("FootPrint",pOrgaData->sFootPrint);
				m_adoRecordSet.PutCollect("dMidX",pOrgaData->dMidX);
				m_adoRecordSet.PutCollect("dMidY",pOrgaData->dMidY);
				m_adoRecordSet.PutCollect("dPadX",pOrgaData->dPadX);
				m_adoRecordSet.PutCollect("dPadY",pOrgaData->dPadY);
				m_adoRecordSet.PutCollect("dRotation",pOrgaData->dRotation);
				m_adoRecordSet.PutCollect("PState",(long)pOrgaData->PState);
				m_adoRecordSet.PutCollect("iLimit",(*it).second->iLimit);

				if( (*it).second->m_pFovData != NULL)
				{
					(*it).second->fovId = (*it).second->m_pFovData->fovId;
				}
				m_adoRecordSet.PutCollect("fovId",(*it).second->fovId);

				if(FALSE == m_adoRecordSet.Update(sErrorInfo))
				{
					sErrorInfo = "保存新增的元件数据失败原因：" + sErrorInfo;
					m_adoConnection.RollbackTrans();
					m_adoRecordSet.Close();
					return false;
				}

				pTemplateData->m_OrganDataMap.insert( pTemplateData->m_OrganDataMap.end(),::make_pair(pOrgaData->organId,pOrgaData));
				if( it == pTemplateData->m_OrganDataMap.begin())
				{
					pTemplateData->m_OrganDataMap.erase(it--);
					break;
				}
				pTemplateData->m_OrganDataMap.erase(it--);

			}//end if
			else
			{
				if( it == pTemplateData->m_OrganDataMap.begin())
				{
					break;
				}
				it--;
			}
		}while(1);//end for

		m_adoRecordSet.Close();

		it = pTemplateData->m_OrganDataMap.begin();
		for(;it != pTemplateData->m_OrganDataMap.end();it++)
		{
			//保存图片
			if( (*it).second->bImagChanged )
			{
				sSql.Format("select * from T_OrganImage where organId=%d",(*it).second->organId);
				m_adoRecordSet.Open(sSql);
				while( !m_adoRecordSet.IsEOF())
				{
					m_adoRecordSet.Delete();

					if(FALSE == m_adoRecordSet.Update(sErrorInfo))
					{
						sErrorInfo = "删除元件图像失败原因：" + sErrorInfo;
						m_adoConnection.RollbackTrans();
						m_adoRecordSet.Close();
						return false;
					}

					m_adoRecordSet.MoveNext();
				}

				m_adoRecordSet.Close();

				m_adoRecordSet.Open(sSql);
				vector<CImage*>::iterator ImagIt=(*it).second->m_MainBodyImageList.begin();
				for(;ImagIt != (*it).second->m_MainBodyImageList.end();ImagIt++)
				{
					if((*ImagIt) == NULL || (*ImagIt)->IsNull())
						continue;

					m_adoRecordSet.AddNew();
					m_adoRecordSet.PutCollect("organId",(*it).second->organId);

					//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
					//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
					//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

					StoreImage("ImageObj",(*ImagIt));

					if(FALSE == m_adoRecordSet.Update(sErrorInfo))
					{
						sErrorInfo = "保存元件图像失败原因：" + sErrorInfo;
						m_adoConnection.RollbackTrans();
						m_adoRecordSet.Close();
						return false;
					}
				}

				m_adoRecordSet.Close();
				(*it).second->bImagChanged = false;

				//删除焊盘
				for(int i=0;i<4;i++)
				{
					if(false == DelPad( (*it).second->m_pDelPadData[i],sErrorInfo ))
					{
						return false;
					}
				}

				//保存焊盘
				for(int i=0;i<4;i++)
				{
					if(false == StorePad( (*it).second,(*it).second->m_pPadData[i],sErrorInfo ))
					{
						return false;
					}
				}
			}
		}

		pTemplateData->m_bOrganDataIsChange = false;

	}//end if

	return true;
}

// 保存参与计算坐标相对值的数据
bool CTemplateDataMgr::StoreCoordCalcOrganData(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	CString sSql;

	sSql.Format("select * from T_CoordCalcOrgan where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);

	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();

	}//end while

	vector<CCoordCalcOrganData*>::iterator it = pTemplateData->m_CoordCalcOrganDataList.begin();

	for(;it != pTemplateData->m_CoordCalcOrganDataList.end();it++)
	{
		m_adoRecordSet.AddNew();

		CCoordCalcOrganData *CoordCalcOrganData = (*it);
		CoordCalcOrganData->templateId = pTemplateData->m_templateId;

		m_adoRecordSet.PutCollect("organId",CoordCalcOrganData->pOrganData->organId);
		m_adoRecordSet.PutCollect("templateId",CoordCalcOrganData->templateId);
		m_adoRecordSet.PutCollect("left",CoordCalcOrganData->left);
		m_adoRecordSet.PutCollect("right",CoordCalcOrganData->right);
		m_adoRecordSet.PutCollect("top",CoordCalcOrganData->top);
		m_adoRecordSet.PutCollect("bottom",CoordCalcOrganData->bottom);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存新增的数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();

	return true;
}

//删除焊盘
bool CTemplateDataMgr::DelPad(CPadData* pPadData,CString& sErrorInfo)
{
	if( pPadData == NULL)
		return true;

	CString sSql;

	sSql.Format("select * from T_Pad where padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);

	if( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存修改的焊盘数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	m_adoRecordSet.Close();

	//删除图片
	sSql.Format("select * from T_PadImage where padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除焊盘图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}	
	m_adoRecordSet.Close();

	//删除引脚
	vector<CFootData*>::iterator footIt=pPadData->m_footList.begin();
	for(;footIt != pPadData->m_footList.end();footIt++)
	{
		(*footIt)->padId = pPadData->padId;
		if(false == DelFoot( (*footIt),sErrorInfo ) )
		{
			return false;
		}
	}
	return true;
}

//保存焊盘
bool CTemplateDataMgr::StorePad(COrganData* pOrganData,CPadData* pPadData,CString& sErrorInfo)
{
	if( pPadData == NULL)
		return true;

	CString sSql;
	long	lMaxPadId=0;

	//先获得焊盘id最大值
	sSql = "select ISNULL( max(padId),0 ) as MaxId from T_Pad";
	m_adoRecordSet.Open(sSql);
	if(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("MaxId",lMaxPadId);
	}
	m_adoRecordSet.Close();

	sSql.Format("select * from T_Pad where padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);	

	//修改
	if( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.PutCollect("organId",pOrganData->organId);
		m_adoRecordSet.PutCollect("padleft",pPadData->left);
		m_adoRecordSet.PutCollect("padright",pPadData->right);
		m_adoRecordSet.PutCollect("padtop",pPadData->top);
		m_adoRecordSet.PutCollect("padbottom",pPadData->bottom);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存修改的焊盘数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}
	else //新增
	{
		m_adoRecordSet.AddNew();

		lMaxPadId++;
		pPadData->padId = lMaxPadId;
		pPadData->organId = pOrganData->organId;

		m_adoRecordSet.PutCollect("padId",pPadData->padId);
		m_adoRecordSet.PutCollect("organId",pPadData->organId);
		m_adoRecordSet.PutCollect("padleft",pPadData->left);
		m_adoRecordSet.PutCollect("padright",pPadData->right);
		m_adoRecordSet.PutCollect("padtop",pPadData->top);
		m_adoRecordSet.PutCollect("padbottom",pPadData->bottom);	

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存新增的焊盘数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}//end for

	m_adoRecordSet.Close();

	//保存图片
	sSql.Format("select * from T_PadImage where padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除焊盘图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	vector<CImage*>::iterator ImagIt=pPadData->m_ImageList.begin();
	for(;ImagIt != pPadData->m_ImageList.end();ImagIt++)
	{
		if((*ImagIt) == NULL || (*ImagIt)->IsNull())
			continue;

		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("padId",pPadData->padId);

		//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
		//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
		StoreImage("ImageObj",(*ImagIt));

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存焊盘图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();

	//保存引脚
	vector<CFootData*>::iterator footIt=pPadData->m_footList.begin();
	for(;footIt != pPadData->m_footList.end();footIt++)
	{
		(*footIt)->padId = pPadData->padId;
		if(false == StoreFoot( (*footIt),sErrorInfo ) )
		{
			return false;
		}
	}

	pOrganData->bPadChanged = false;

	return true;
}

bool CTemplateDataMgr::DelFoot(CFootData* pFootData,CString& sErrorInfo)
{
	if( pFootData == NULL)
		return true;

	CString sSql;
	sSql.Format("select * from T_Foot where footId=%d",pFootData->footId);
	m_adoRecordSet.Open(sSql);

	if( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除引脚数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}	
	m_adoRecordSet.Close();

	//删除图片
	sSql.Format("select * from T_FootImage where footId=%d",pFootData->footId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();
		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除引脚图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}
	m_adoRecordSet.Close();

	return true;
}

bool CTemplateDataMgr::StoreFoot(CFootData* pFootData,CString& sErrorInfo)
{
	if( pFootData == NULL)
		return true;

	CString sSql;
	long	lMaxFootId=0;

	//先获得焊盘id最大值
	sSql = "select ISNULL( max(footId),0 ) as MaxId from T_Foot";
	m_adoRecordSet.Open(sSql);
	if(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.GetCollect("MaxId",lMaxFootId);
	}
	m_adoRecordSet.Close();

	sSql.Format("select * from T_Foot where footId=%d",pFootData->footId);
	m_adoRecordSet.Open(sSql);

	//修改
	if( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.PutCollect("padId",pFootData->padId);
		m_adoRecordSet.PutCollect("footLeft",pFootData->left);
		m_adoRecordSet.PutCollect("footRight",pFootData->right);
		m_adoRecordSet.PutCollect("footTop",pFootData->top);
		m_adoRecordSet.PutCollect("footBottom",pFootData->bottom);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存修改的引脚数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}
	else //新增
	{
		m_adoRecordSet.AddNew();

		lMaxFootId++;
		pFootData->footId = lMaxFootId;

		m_adoRecordSet.PutCollect("footId",pFootData->footId);
		m_adoRecordSet.PutCollect("padId",pFootData->padId);
		m_adoRecordSet.PutCollect("footLeft",pFootData->left);
		m_adoRecordSet.PutCollect("footRight",pFootData->right);
		m_adoRecordSet.PutCollect("footTop",pFootData->top);
		m_adoRecordSet.PutCollect("footBottom",pFootData->bottom);

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存新增的引脚数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}//end for

	m_adoRecordSet.Close();

	//保存图片
	sSql.Format("select * from T_FootImage where footId=%d",pFootData->footId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();
		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "删除引脚图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	vector<CImage*>::iterator ImagIt=pFootData->m_ImageList.begin();
	for(;ImagIt != pFootData->m_ImageList.end();ImagIt++)
	{
		if((*ImagIt) == NULL || (*ImagIt)->IsNull())
			continue;

		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("footId",pFootData->footId);

		//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("imageObj","c:\\aoi_tmpbitmap.bmp");
		//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
		StoreImage("imageObj",(*ImagIt));

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存引脚图片失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();

	return true;
}

//查询出所有的模板（仅模板本身基本数据）
bool CTemplateDataMgr::GetAllTemplate(vector<CTemplateData*>& templateList)
{
	//if(!connectDB())
	if(!m_bIsConnect)
		return false;

	CString	sSql = "select * from T_Template ";

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoRecordSet.Open(sSql);
	m_adoConnection.BeginTrans();

	while( !m_adoRecordSet.IsEOF())
	{
		CTemplateData *pTemplateData= new CTemplateData();

		m_adoRecordSet.GetCollect("templateId",pTemplateData->m_templateId);
		m_adoRecordSet.GetCollect("templateName",pTemplateData->m_templateName);
		m_adoRecordSet.GetCollect("width",pTemplateData->m_width);
		m_adoRecordSet.GetCollect("height",pTemplateData->m_height);
		m_adoRecordSet.GetCollect("editAreaLeft",pTemplateData->m_editAreaLeft);
		m_adoRecordSet.GetCollect("editAreaRight",pTemplateData->m_editAreaRight);
		m_adoRecordSet.GetCollect("editAreaTop",pTemplateData->m_editAreaTop);
		m_adoRecordSet.GetCollect("editAreaBottom",pTemplateData->m_editAreaBottom);

		templateList.push_back(pTemplateData);
		m_adoRecordSet.MoveNext();
	}

	m_adoConnection.CommitTrans();
	m_adoRecordSet.Close();
	return true;
}

// 修改元件
bool CTemplateDataMgr::ModifyOrgan(COrganData* pOrganData,CString& sErrorInfo)
{
	if(!m_bIsConnect)
	{
		sErrorInfo = "No connect to database";
		return false;
	}

	CString	sSql;

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();	
	m_adoConnection.BeginTrans();

	sSql.Format("select * from T_OrganImage where organId=%d",pOrganData->organId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	vector<CImage*>::iterator ImagIt=pOrganData->m_MainBodyImageList.begin();
	for(;ImagIt != pOrganData->m_MainBodyImageList.end();ImagIt++)
	{
		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("organId",pOrganData->organId);

		//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
		//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
		StoreImage("ImageObj",(*ImagIt));

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();

	return true;
}

bool CTemplateDataMgr::ModifyPad(CPadData* pPadData,CString& sErrorInfo)
{
	if(!m_bIsConnect)
	{
		sErrorInfo = "No connect to database";
		return false;
	}

	CString	sSql;

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoConnection.BeginTrans();

	//保存图片
	sSql.Format("select * from T_PadImage where padId=%d",pPadData->padId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	vector<CImage*>::iterator ImagIt=pPadData->m_ImageList.begin();
	for(;ImagIt != pPadData->m_ImageList.end();ImagIt++)
	{
		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("padId",pPadData->padId);

		//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("ImageObj","c:\\aoi_tmpbitmap.bmp");
		//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
		StoreImage("ImageObj",(*ImagIt));

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();
	return true;
}

bool CTemplateDataMgr::ModifyFoot(CFootData* pFootData,CString& sErrorInfo)
{
	if(!m_bIsConnect)
	{
		sErrorInfo = "No connect to database";
		return false;
	}

	CString	sSql;

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();
	m_adoConnection.BeginTrans();

	//保存图片
	sSql.Format("select * from T_FootImage where footId=%d",pFootData->footId);
	m_adoRecordSet.Open(sSql);
	while( !m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();
		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
		m_adoRecordSet.MoveNext();
	}

	vector<CImage*>::iterator ImagIt=pFootData->m_ImageList.begin();
	for(;ImagIt != pFootData->m_ImageList.end();ImagIt++)
	{
		m_adoRecordSet.AddNew();
		m_adoRecordSet.PutCollect("footId",pFootData->footId);

		//(*ImagIt)->Save("c:\\aoi_tmpbitmap.bmp");
		//m_adoRecordSet.AppendChunk("imageObj","c:\\aoi_tmpbitmap.bmp");
		//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");
		StoreImage("imageObj",(*ImagIt));

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			return false;
		}
	}

	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();
	return true;
}

bool CTemplateDataMgr::DelTemplate(CTemplateData* pTemplateData,CString& sErrorInfo)
{
	if( !m_bIsConnect)
	{
		sErrorInfo = "No connect to database";
		return false;
	}

	CString	sSql;

	m_adoRecordSet.SetAdoConnection(&m_adoConnection);
	m_adoRecordSet.SetCursorLocation();

	m_adoConnection.BeginTrans();

	sSql.Format("select * from T_Template where templateId=%d",pTemplateData->m_templateId);
	m_adoRecordSet.Open(sSql);

	if(!m_adoRecordSet.IsEOF())
	{
		m_adoRecordSet.Delete();

		if(FALSE == m_adoRecordSet.Update(sErrorInfo))
		{
			sErrorInfo = "保存模板数据失败原因：" + sErrorInfo;
			m_adoConnection.RollbackTrans();
			m_adoRecordSet.Close();
			m_adoConnection.Close();
			return false;
		}
	}
	m_adoRecordSet.Close();
	m_adoConnection.CommitTrans();
	return true;
}

bool CTemplateDataMgr::StoreImage(const char *fieldName,CImage *pImage)
{
	//pImage->Save("c:\\aoi_tmpbitmap.bmp");
	//m_adoRecordSet.AppendChunk(fieldName,"c:\\aoi_tmpbitmap.bmp");
	//::DeleteFileA("c:\\aoi_tmpbitmap.bmp");

	if(pImage == NULL || pImage->IsNull())
		return true;

	BITMAPINFO bmpInfo;
	memset(&bmpInfo,0,sizeof(bmpInfo));

	bmpInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth			= pImage->GetWidth();
	bmpInfo.bmiHeader.biHeight			= pImage->GetHeight();

	/*
	*	以下设置一般相同，
	*	对于低于8位的位图，还应设置相应的位图调色板
	*/
	bmpInfo.bmiHeader.biPlanes			= 1;
	bmpInfo.bmiHeader.biBitCount		= pImage->GetBPP();
	bmpInfo.bmiHeader.biCompression		= BI_RGB;
	bmpInfo.bmiHeader.biSizeImage		= 0;
	bmpInfo.bmiHeader.biXPelsPerMeter	= 0;
	bmpInfo.bmiHeader.biYPelsPerMeter	= 0;
	bmpInfo.bmiHeader.biClrUsed			= 0;
	bmpInfo.bmiHeader.biClrImportant	= 0;

	int iByteOfPixel = pImage->GetBPP()/8;		//每象素的字节数
	UINT iDataLen = pImage->GetWidth()*pImage->GetHeight()*iByteOfPixel;
	BYTE *pByte = new BYTE[iDataLen + sizeof(bmpInfo)];

	memcpy(pByte,&bmpInfo,sizeof(bmpInfo));

	BYTE  *pBit = (BYTE*)pImage->GetBits();
	int iRowBytes = pImage->GetPitch(); //每一行字节数

	for(int i=0;i<pImage->GetHeight();i++)
	{
		memcpy(&pByte[sizeof(bmpInfo)+pImage->GetWidth()*i*iByteOfPixel],pBit,pImage->GetWidth()*iByteOfPixel);
		pBit += iRowBytes;
	}

	BOOL bResult = m_adoRecordSet.AppendChunk(fieldName,pByte,iDataLen+sizeof(bmpInfo));

	delete []pByte;
	pByte = NULL;

	return bResult;
}

bool CTemplateDataMgr::GetImage(const char *fieldName,CImage *pImage)
{
	/*m_adoRecordSet.GetChunk("imageObj",hBitmap);
	if(hBitmap != NULL)
	{
	CImage *pImage = new CImage();
	pImage->Attach(hBitmap);
	pFootData->m_ImageList.push_back(pImage);
	}
	hBitmap=NULL;*/

	ASSERT(fieldName != NULL);
	ASSERT(pImage != NULL);

	long size = m_adoRecordSet.GetFieldActualSize(fieldName);
	if(size > sizeof(BITMAPINFO))
	{
		BYTE *lpData = new BYTE[size];

		if (m_adoRecordSet.GetChunk(m_adoRecordSet.GetField(fieldName), (LPVOID)lpData))
		{
			BITMAPINFO bmpInfo;
			memset(&bmpInfo,0,sizeof(bmpInfo));

			memcpy(&bmpInfo,lpData,sizeof(bmpInfo));
			BYTE *pSrcBits = lpData+sizeof(bmpInfo);

			if(size < sizeof(BITMAPINFO) + bmpInfo.bmiHeader.biWidth*bmpInfo.bmiHeader.biHeight*(bmpInfo.bmiHeader.biBitCount/8))
			{
				delete []lpData;
				lpData = NULL;
				return false;
			}

			if(!pImage->Create(bmpInfo.bmiHeader.biWidth,bmpInfo.bmiHeader.biHeight,
				bmpInfo.bmiHeader.biBitCount,0))
			{
				::AfxMessageBox("Create image object fail");
				return false;
			}

			BYTE  *pBit = (BYTE*)pImage->GetBits();
			int iRowBytes = pImage->GetPitch(); //每一行字节数(或相临两行首字节偏移)

			for(int i=0;i<pImage->GetHeight();i++)
			{
				memcpy(pBit,pSrcBits,pImage->GetWidth()*bmpInfo.bmiHeader.biBitCount/8);
				pBit += iRowBytes;
				pSrcBits += pImage->GetWidth()*bmpInfo.bmiHeader.biBitCount/8;
			}

		}

		delete []lpData;
		lpData = NULL;
		return true;
	}
	return false;
}
