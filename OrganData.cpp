#include "StdAfx.h"
#include "OrganData.h"

COrganData::COrganData(void)
{
	templateId=0;
	organId=0;

	bDataChanged = false;
	bImagChanged = false;
	bPadChanged = false;

	//��Χ
	RangeLeft=0;
	RangeRight=0;
	RrangeTop=0;
	RangeBottom=0;
	
	CString		sName="";	
	CString		sFootPrint="";	//��װ
	dMidX=0;		//����X
	dMidY=0;		//����Y
	dPadX=0;		//��һ����X
	dPadY=0;		//��һ����Y
	dRotation=0;		//�Ƕ�	
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
*��Դ���󿽱�������ݵ���ǰ����
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
	//����Χ
	CBrush *pBrush = new CBrush( DrawContext.color);
	CRect rect(RangeLeft,RrangeTop,RangeRight,RangeBottom);
	//�Ŵ�
	rect.top = (1+DrawContext.dZoom)*rect.top;
	rect.left = (1+DrawContext.dZoom)*rect.left;
	rect.right = (1+DrawContext.dZoom)*rect.right;
	rect.bottom = (1+DrawContext.dZoom)*rect.bottom;
	//ת������������ϵ
	rect.left -= DrawContext.lSrcOriginX;
	rect.right -= DrawContext.lSrcOriginX;
	rect.top -= DrawContext.lSrcOriginY;
	rect.bottom -= DrawContext.lSrcOriginY;
	DrawContext.pDC->FrameRect(&rect,pBrush);
	delete pBrush;
	pBrush = NULL;

	//������
	CDataObj::Draw(DrawContext);

	//������
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

	CPoint SrcOrganP = pSrcOrganData->GetMidP();//ԴԪ�������ĵ�
	CPoint SrcPadP = pSrcPad->GetMidP();//Դ���̵����ĵ�

	long lPadX = lCurrentCenterX - (SrcOrganP.x - SrcPadP.x);
	long lPadY = lCurrentCenterY - (SrcOrganP.y - SrcPadP.y);

	CPadData *pPadData = new CPadData();
	pPadData->CopyPrgData( pSrcPad,lPadX,lPadY);
	*pDestPad = pPadData;	
}

// ɾ���������
void COrganData::DelPrgData(void)
{
	bDataChanged = true;
	bImagChanged = true;
	bPadChanged = true;

	this->left = 0;
	this->right = 0;
	this->top = 0;
	this->bottom = 0;

	//��Χ
	RangeLeft=0;
	RangeRight=0;
	RrangeTop=0;
	RangeBottom=0;

	//ɾ��ͼ��
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
			//���ݿ��д��ڣ������ŵ�ɾ��������
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

// ��ʱ����ת90��
void COrganData::Turn(void)
{
	long lCenterX=this->GetMidP().x;
	long lCenterY=this->GetMidP().y;
	long lOldHeight = this->GetHeight();
	long lOldWidth = this->GetWidth();

	//����ת����
	for(int i=0;i<4;i++)
	{
		if(m_pPadData[i] == NULL)
			continue;

		long lNewPadMidX = 0;
		long lNewPadMidY = 0;

		//������ԭ��������,����ת���±�
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
		{//������ԭ������ұ�,����ת���ϱ�
			lNewPadMidX = lCenterX;
			lNewPadMidY = lCenterY + (m_pPadData[i]->GetMidP().x -lCenterX) ;
			//lNewPadMidY = lCenterY - (m_pPadData[i]->GetMidP().x -lCenterX) ;//lOldWidth/2 -m_pPadData[i]->GetWidth()/2;
		}
		else if(m_pPadData[i]->GetMidP().y < this->top &&
			m_pPadData[i]->GetMidP().x > this->left &&
			m_pPadData[i]->GetMidP().x < this->right)
		{//������ԭ������ϱ�,����ת�����
			lNewPadMidY = lCenterY;
			lNewPadMidX = lCenterX -(lCenterY-m_pPadData[i]->GetMidP().y);//lOldHeight/2 -m_pPadData[i]->GetHeight()/2;
		}
		else if(m_pPadData[i]->GetMidP().y > this->bottom &&
			m_pPadData[i]->GetMidP().x > this->left &&
			m_pPadData[i]->GetMidP().x < this->right)
		{//������ԭ������±�,����ת���ұ�
			lNewPadMidY = lCenterY;
			lNewPadMidX = lCenterX +(m_pPadData[i]->GetMidP().y-lCenterY);//lOldHeight/2 +m_pPadData[i]->GetHeight()/2;
		}

		//�����Ƶ��µ����ĵ�
		m_pPadData[i]->MoveX(lNewPadMidX - m_pPadData[i]->GetMidP().x);
		m_pPadData[i]->MoveY(lNewPadMidY - m_pPadData[i]->GetMidP().y);

		//��ת
		m_pPadData[i]->Turn();
	}

	//��ת����
	this->left = lCenterX - lOldHeight/2;
	this->right = lCenterX + lOldHeight/2;
	this->top = lCenterY - lOldWidth/2;
	this->bottom = lCenterY + lOldWidth/2;

	//��ת������Χ
	long lSerchCenterX=RangeLeft + (RangeRight-RangeLeft)/2;
	long lSerchCenterY=RrangeTop + (RangeBottom-RrangeTop)/2;
	long lOldSerchHeight = RangeBottom-RrangeTop;
	long lOldSerchWidth = RangeRight-RangeLeft;

	RangeLeft = lSerchCenterX - lOldSerchHeight/2;
	RangeRight = lSerchCenterX + lOldSerchHeight/2;
	RrangeTop = lSerchCenterY - lOldSerchWidth/2;
	RangeBottom = lSerchCenterY + lOldSerchWidth/2;
	
}
