#include "StdAfx.h"
#include "PadData.h"

//������
CFootData::CFootData(void)
{
	footId=0;
	padId=0;	
}

CFootData::~CFootData(void)
{
	//�ͷ�ͼƬ
	vector<CImage*>::iterator it = m_ImageList.begin();
	for(;it != m_ImageList.end();it++)
	{
		CImage *pImage = (*it);
		if(pImage != NULL && !pImage->IsNull())
			pImage->Destroy();
		if(pImage != NULL)
		{
			delete pImage;
			pImage = NULL;
		}
	}

	m_ImageList.clear();
}


//������
CPadData::CPadData(void)
{
	organId=0;
	padId=0;
}

CPadData::~CPadData(void)
{
	//�ͷ�ͼƬ
	vector<CImage*>::iterator it = m_ImageList.begin();
	for(;it != m_ImageList.end();it++)
	{
		CImage *pImage = (*it);
		if(pImage != NULL && !pImage->IsNull())
			pImage->Destroy();
		if(pImage != NULL)
		{
			delete pImage;
			pImage = NULL;
		}
	}
	m_ImageList.clear();

	//�ͷ����Ŷ���
	vector<CFootData*>::iterator FootIt=m_footList.begin();
	for(;FootIt != m_footList.end();FootIt++)
	{
		CFootData *pFootData = (*FootIt);
		if(pFootData != NULL)
		{
			delete pFootData;
			pFootData = NULL;
		}		
	}
	m_footList.clear();
}

/*
*��Դ���󿽱�������ݵ���ǰ����
**/
void CPadData::CopyPrgData(const CPadData *pSrcPadData,const long lCurrentCenterX,const long lCurrentCenterY)
{
	CDataObj::CopyPrgData((CDataObj*)pSrcPadData,lCurrentCenterX,lCurrentCenterY);

	CPoint SrcPadP = pSrcPadData->GetMidP();//Դ���̵����ĵ�

	this->organId = pSrcPadData->organId;
	
	vector<CFootData*>::const_iterator it = pSrcPadData->m_footList.begin();
	for(;it != pSrcPadData->m_footList.end();it++)
	{
		CPoint SrcFootP = (*it)->GetMidP();//Դ���ŵ����ĵ�

		//���㵱ǰ���ŵ����ĵ�
		long lFootX = lCurrentCenterX - (SrcPadP.x - SrcFootP.x);
		long lFootY = lCurrentCenterY - (SrcPadP.y - SrcFootP.y);

		CFootData *pFootData = new CFootData();
		pFootData->CopyPrgData( (CDataObj*)(*it),lFootX,lFootY);

		this->m_footList.push_back(pFootData);
	}
}

void CPadData::MoveX(long lMoveStep)
{
	CDataObj::MoveX(lMoveStep);

	vector<CFootData*>::iterator it =m_footList.begin();
	for(;it != m_footList.end();it++)
	{
		(*it)->MoveX(lMoveStep);		
	}
}

void CPadData::MoveY(long lMoveStep)
{
	CDataObj::MoveY(lMoveStep);

	vector<CFootData*>::iterator it =m_footList.begin();
	for(;it != m_footList.end();it++)
	{
		(*it)->MoveY(lMoveStep);		
	}
}

void CPadData::Turn()
{
	CDataObj::Turn();

	long lCenterX=this->GetMidP().x;
	long lCenterY=this->GetMidP().y;

	//��ת����
	vector<CFootData*>::iterator it =m_footList.begin();
	for(;it != m_footList.end();it++)
	{
		long lFootMidX = GetMidP().x - ( GetMidP().y - (*it)->GetMidP().y);
		long lFootMidY= GetMidP().y - ( GetMidP().x - (*it)->GetMidP().x);
		/*
		//�����ں��̵����,����ת���±�
		if((*it)->GetMidP().x < lCenterX )
		{
			lFootMidX = lCenterX;
			lFootMidY = lCenterY + (lCenterX - (*it)->GetMidP().x);
		}
		else if((*it)->GetMidP().x > lCenterX )
		{//�����ں��̵��ұ�,����ת���ϱ�
			lFootMidX = lCenterX;
			lFootMidY = lCenterY - ((*it)->GetMidP().x -lCenterX) ;			
		}
		else if((*it)->GetMidP().y < lCenterY )
		{//�����ں��̵��ϱ�,����ת�����
			lFootMidX = lCenterX - (lCenterY - (*it)->GetMidP().y);
			lFootMidY = lCenterY;
		}
		else if((*it)->GetMidP().y > lCenterY )
		{//�����ں��̵��±�,����ת���ұ�
			lFootMidX = lCenterX + ((*it)->GetMidP().y -lCenterY );
			lFootMidY = lCenterY;
		}
		else
		{
			lFootMidX = (*it)->GetMidP().x;
			lFootMidY = (*it)->GetMidP().y;
		}*/

		(*it)->MoveX( lFootMidX - (*it)->GetMidP().x);
		(*it)->MoveY( lFootMidY - (*it)->GetMidP().y);
		(*it)->Turn();
	}	
}

void CPadData::Draw(DRAWCONTEXT DrawContext)
{
	//������
	CDataObj::Draw(DrawContext);

	//������
	vector<CFootData*>::iterator it =m_footList.begin();
	for(;it!= m_footList.end();it++)
	{
		(*it)->Draw(DrawContext);
	}
}
/* ���Ӻ�����
iNum��1����1
*/
void CPadData::AddFootNum(int iNum)
{
	if(iNum>0)
	{
		CFootData *pFootData = new CFootData();
		m_footList.push_back(pFootData);
	}
	else if(m_footList.size() > 0 )
	{
		vector<CFootData*>::iterator FootIt=m_footList.end();
		FootIt--;		
		delete (*FootIt);
		(*FootIt) = NULL;
		m_footList.erase(FootIt);
	}

	if( m_footList.size() <=0)
		return;

	//���µ�������size
	if( this->GetHeight() >= this->GetWidth())
	{
		long lFootMidX = this->GetMidP().x;	//���ĵ���x���ϲ���
		//����ÿһ��foot�ĸ߶ȣ���������֮��Ĭ�ϵļ��5)
		long lHeight = ( GetHeight() - (m_footList.size()+1)*5 )/m_footList.size();

		long lFootMidY = this->top +5 +lHeight/2;
		vector<CFootData*>::iterator it=m_footList.begin();
		do
		{
			(*it)->left = lFootMidX - this->GetWidth()/2 +5;
			(*it)->right = lFootMidX + this->GetWidth()/2 -5;
			(*it)->top = lFootMidY - lHeight/2;
			(*it)->bottom = lFootMidY + lHeight/2;

			lFootMidY = lFootMidY + lHeight+5;
			it++;
		}while(it!=m_footList.end());
	}
	else //���̿�ȴ��ڸ߶�
	{
		long lFootMidY = this->GetMidP().y;	//���ĵ���y���ϲ���
		//����ÿһ��foot�ĸ߶ȣ���������֮��Ĭ�ϵļ��5)
		long lWidth = ( GetWidth() - (m_footList.size()+1)*5 )/m_footList.size();

		long lFootMidX = this->left +5 +lWidth/2;
		vector<CFootData*>::iterator it=m_footList.begin();
		do
		{
			(*it)->left = lFootMidX - lWidth/2;
			(*it)->right = lFootMidX + lWidth/2;
			(*it)->top = lFootMidY - this->GetHeight()/2 +5;
			(*it)->bottom = lFootMidY + this->GetHeight()/2 -5;

			lFootMidX = lFootMidX + lWidth+5;
			it++;
		}while(it!=m_footList.end());
	}
}

/*
*���Ӻ��ż���
*lSpace:���������
*/
void CPadData::AddFootSpaceBetween(long lSpace)
{
	int iFootNum = m_footList.size();
	double dEachSpace =0.0;//�䶯��ÿ����֮��ļ��
	bool bMoveX=true;//�Ƿ���x���ƶ�
	long lFirstFootPos = 0;//��һ������λ��

	vector<CFootData*>::iterator firstIt=m_footList.begin();
	vector<CFootData*>::reverse_iterator lastIt=m_footList.rbegin();
	
	if( this->GetHeight() >= this->GetWidth())
	{
		bMoveX = false;

		if(firstIt != m_footList.end() &&
			lastIt != m_footList.rend() && iFootNum > 1)
		{
			dEachSpace = (double)((*lastIt)->GetMidP().y - (*firstIt)->GetMidP().y + lSpace)/(double)(iFootNum-1);
			lFirstFootPos = (*firstIt)->GetMidP().y;
		}
	}
	else
	{
		bMoveX=true;

		if(firstIt != m_footList.end() &&
			lastIt != m_footList.rend() && iFootNum > 1)
		{
			dEachSpace = (double)((*lastIt)->GetMidP().x - (*firstIt)->GetMidP().x + lSpace)/(double)(iFootNum-1);
			lFirstFootPos = (*firstIt)->GetMidP().x;
		}
	}

	int i=0;
	vector<CFootData*>::iterator it=m_footList.begin();
	for(;it != m_footList.end();it++)
	{
		if( (*it)->left + (*it)->right >0 )
		{
			if( bMoveX)
				(*it)->MoveX( (long)(i*dEachSpace) - ((*it)->GetMidP().x -lFirstFootPos) );
			else
				(*it)->MoveY( (long)(i*dEachSpace) - ((*it)->GetMidP().y -lFirstFootPos) );
			
			i++;
		}
	}
}
