#include "StdAfx.h"
#include "OrganData.h"

COrganData::COrganData(void)
{
	templateId=0;
	organId=0;

	bDataChanged = false;
	bImagChanged = false;
	bPadChanged = false;

	//范围
	RangeLeft=0;
	RangeRight=0;
	RrangeTop=0;
	RangeBottom=0;
	
	CString		sName="";	
	CString		sFootPrint="";	//封装
	dMidX=0;		//中心X
	dMidY=0;		//中心Y
	dPadX=0;		//第一焊盘X
	dPadY=0;		//第一焊盘Y
	dRotation=0;		//角度	
	iIsPolar=0;
	bNeedDetect = true;

	m_pPadData[0]=NULL;	
	m_pPadData[1]=NULL;	
	m_pPadData[2]=NULL;	
	m_pPadData[3]=NULL;	

	m_pDelPadData[0]=NULL;
	m_pDelPadData[1]=NULL;
	m_pDelPadData[2]=NULL;
	m_pDelPadData[3]=NULL;
	
	PState = PS_NEW;

	iLimit = 70;
	fovId = 0;
	m_pFovData = NULL;
}

COrganData::~COrganData(void)
{
	for(int i=0;i<m_MainBodyImageList.size();i++)
	{
		CImage *pImage = m_MainBodyImageList[i];
		if(pImage != NULL)
		{
			if(!pImage->IsNull())
				pImage->Destroy();
			delete pImage;
			pImage = NULL;
		}
	}
	m_MainBodyImageList.clear();

	for(int i=0;i<4;i++)
	{
		CPadData *pPadData = m_pPadData[i];
		if(pPadData != NULL)
		{
			delete pPadData;
			pPadData = NULL;
		}

		pPadData = m_pDelPadData[i];
		if(pPadData != NULL)
		{
			delete pPadData;
			pPadData = NULL;
		}
	}
}

/*
*从源对象拷贝编程数据到当前对象
**/
void COrganData::CopyPrgData(const COrganData *pSrcOrganData,const long lCurrentCenterX,const long lCurrentCenterY)
{
	CDataObj::CopyPrgData((CDataObj*)pSrcOrganData,lCurrentCenterX,lCurrentCenterY);

	RangeLeft = lCurrentCenterX - (pSrcOrganData->RangeRight - pSrcOrganData->RangeLeft)/2;
	RangeRight= lCurrentCenterX + (pSrcOrganData->RangeRight - pSrcOrganData->RangeLeft)/2;
	RrangeTop = lCurrentCenterY - (pSrcOrganData->RangeBottom - pSrcOrganData->RrangeTop)/2;
	RangeBottom = lCurrentCenterY + (pSrcOrganData->RangeBottom - pSrcOrganData->RrangeTop)/2;

	for(int i=0;i<4;i++)
	{
		CopyPadData(&m_pPadData[i],pSrcOrganData,pSrcOrganData->m_pPadData[i],lCurrentCenterX,lCurrentCenterY);
	}
}

void COrganData::Draw(DRAWCONTEXT DrawContext)
{
	//画范围
	CBrush *pBrush = new CBrush( DrawContext.color);
	CRect rect(RangeLeft,RrangeTop,RangeRight,RangeBottom);
	//放大
	rect.top = (1+DrawContext.dZoom)*rect.top;
	rect.left = (1+DrawContext.dZoom)*rect.left;
	rect.right = (1+DrawContext.dZoom)*rect.right;
	rect.bottom = (1+DrawContext.dZoom)*rect.bottom;
	//转换到窗口坐标系
	rect.left -= DrawContext.lSrcOriginX;
	rect.right -= DrawContext.lSrcOriginX;
	rect.top -= DrawContext.lSrcOriginY;
	rect.bottom -= DrawContext.lSrcOriginY;
	DrawContext.pDC->FrameRect(&rect,pBrush);
	delete pBrush;
	pBrush = NULL;

	//画主体
	CDataObj::Draw(DrawContext);

	//画焊盘
	for(int i=0;i<4;i++)
	{
		if(m_pPadData[i])
			m_pPadData[i]->Draw(DrawContext);
	}		
}
void COrganData::CopyPadData(CPadData** pDestPad, const COrganData* pSrcOrganData, const CPadData* pSrcPad,const long  lCurrentCenterX, const long lCurrentCenterY)
{
	if(pSrcPad == NULL)
		return;

	CPoint SrcOrganP = pSrcOrganData->GetMidP();//源元件的中心点
	CPoint SrcPadP = pSrcPad->GetMidP();//源焊盘的中心点

	long lPadX = lCurrentCenterX - (SrcOrganP.x - SrcPadP.x);
	long lPadY = lCurrentCenterY - (SrcOrganP.y - SrcPadP.y);

	CPadData *pPadData = new CPadData();
	pPadData->CopyPrgData( pSrcPad,lPadX,lPadY);
	*pDestPad = pPadData;	
}

// 删除编程数据
void COrganData::DelPrgData(void)
{
	bDataChanged = true;
	bImagChanged = true;
	bPadChanged = true;

	this->left = 0;
	this->right = 0;
	this->top = 0;
	this->bottom = 0;

	//范围
	RangeLeft=0;
	RangeRight=0;
	RrangeTop=0;
	RangeBottom=0;

	//删除图像
	vector<CImage*>::iterator it = m_MainBodyImageList.begin();
	while(it!=m_MainBodyImageList.end())
	{
		CImage* pImage = (*it);		
		delete pImage;
		pImage=NULL;
		
		it = m_MainBodyImageList.erase(it);
	}

	for(int i=0;i<4;i++)
	{
		if( m_pPadData[i] == NULL)
			continue;

		if( m_pPadData[i]->padId >0)
		{
			//数据库中存在，将它放到删除数组中
			if( m_pDelPadData[i] != NULL)
			{
				delete m_pDelPadData[i];
				m_pDelPadData[i] = NULL;
			}
			m_pDelPadData[i] = m_pPadData[i];
		}
		else
		{
			delete m_pPadData[i];
		}

		m_pPadData[i] = NULL;
	}
	
	PState = PS_NEW;
	
	fovId = 0;
	m_pFovData = NULL;
}

// 逆时针旋转90度
void COrganData::Turn(void)
{
	long lCenterX=this->GetMidP().x;
	long lCenterY=this->GetMidP().y;
	long lOldHeight = this->GetHeight();
	long lOldWidth = this->GetWidth();

	//先旋转焊盘
	for(int i=0;i<4;i++)
	{
		if(m_pPadData[i] == NULL)
			continue;

		long lNewPadMidX = 0;
		long lNewPadMidY = 0;

		//焊盘在原主体的左边,现在转到下边
		if(m_pPadData[i]->GetMidP().x < this->left &&
			m_pPadData[i]->GetMidP().y > this->top &&
			m_pPadData[i]->GetMidP().y < this->bottom)
		{
			lNewPadMidX = lCenterX;
			lNewPadMidY = lCenterY - (lCenterX - m_pPadData[i]->GetMidP().x);
			//lNewPadMidY = lCenterY + (lCenterX - m_pPadData[i]->GetMidP().x); //lOldWidth/2 +m_pPadData[i]->GetWidth()/2;
		}
		else if(m_pPadData[i]->GetMidP().x > this->right &&
			m_pPadData[i]->GetMidP().y > this->top &&
			m_pPadData[i]->GetMidP().y < this->bottom)
		{//焊盘在原主体的右边,现在转到上边
			lNewPadMidX = lCenterX;
			lNewPadMidY = lCenterY + (m_pPadData[i]->GetMidP().x -lCenterX) ;
			//lNewPadMidY = lCenterY - (m_pPadData[i]->GetMidP().x -lCenterX) ;//lOldWidth/2 -m_pPadData[i]->GetWidth()/2;
		}
		else if(m_pPadData[i]->GetMidP().y < this->top &&
			m_pPadData[i]->GetMidP().x > this->left &&
			m_pPadData[i]->GetMidP().x < this->right)
		{//焊盘在原主体的上边,现在转到左边
			lNewPadMidY = lCenterY;
			lNewPadMidX = lCenterX -(lCenterY-m_pPadData[i]->GetMidP().y);//lOldHeight/2 -m_pPadData[i]->GetHeight()/2;
		}
		else if(m_pPadData[i]->GetMidP().y > this->bottom &&
			m_pPadData[i]->GetMidP().x > this->left &&
			m_pPadData[i]->GetMidP().x < this->right)
		{//焊盘在原主体的下边,现在转到右边
			lNewPadMidY = lCenterY;
			lNewPadMidX = lCenterX +(m_pPadData[i]->GetMidP().y-lCenterY);//lOldHeight/2 +m_pPadData[i]->GetHeight()/2;
		}

		//焊盘移到新的中心点
		m_pPadData[i]->MoveX(lNewPadMidX - m_pPadData[i]->GetMidP().x);
		m_pPadData[i]->MoveY(lNewPadMidY - m_pPadData[i]->GetMidP().y);

		//旋转
		m_pPadData[i]->Turn();
	}

	//旋转主体
	this->left = lCenterX - lOldHeight/2;
	this->right = lCenterX + lOldHeight/2;
	this->top = lCenterY - lOldWidth/2;
	this->bottom = lCenterY + lOldWidth/2;

	//旋转搜索范围
	long lSerchCenterX=RangeLeft + (RangeRight-RangeLeft)/2;
	long lSerchCenterY=RrangeTop + (RangeBottom-RrangeTop)/2;
	long lOldSerchHeight = RangeBottom-RrangeTop;
	long lOldSerchWidth = RangeRight-RangeLeft;

	RangeLeft = lSerchCenterX - lOldSerchHeight/2;
	RangeRight = lSerchCenterX + lOldSerchHeight/2;
	RrangeTop = lSerchCenterY - lOldSerchWidth/2;
	RangeBottom = lSerchCenterY + lOldSerchWidth/2;
	
}
