#include "StdAfx.h"
#include "DataObj.h"

CDataObj::CDataObj(void)
{
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;
	bVisible = true;
}

CDataObj::~CDataObj(void)
{
	if(!m_ImageObj.IsNull())
	{
		m_ImageObj.Destroy();
	}
}

/*
*��Դ���󿽱�������ݵ���ǰ����
*pSrcDataObj:Դ����
*lCurrentCenterX:��ǰ���������X��
*lCurrentCenterY:��ǰ���������Y��
**/
void CDataObj::CopyPrgData(const CDataObj *pSrcDataObj,const long lCurrentCenterX,const long lCurrentCenterY)
{
	long	lWidth = pSrcDataObj->right - pSrcDataObj->left;
	long	lHeight = pSrcDataObj->bottom - pSrcDataObj->top;

	this->left = lCurrentCenterX - lWidth/2;
	this->right = lCurrentCenterX + lWidth/2;
	this->top = lCurrentCenterY - lHeight/2;
	this->bottom = lCurrentCenterY + lHeight/2;
}

//���ص�ǰ�������ĵ�
CPoint CDataObj::GetMidP() const
{
	long x = left + (right - left)/2;
	long y = top + (bottom - top)/2;

	return CPoint(x,y);
}

//��ͼ
void CDataObj::Draw(DRAWCONTEXT DrawContext)
{
	if( !bVisible )
		return;
		
	CBrush *pBrush = new CBrush( DrawContext.color);

	CRect rect(left,top,right,bottom);

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
}

void CDataObj::Turn()
{
	long lWidth = (right-left);
	long lHeight = (bottom - top);
	CPoint MidP = this->GetMidP();

	this->left = MidP.x - lHeight/2;
	this->right = MidP.x + lHeight/2;
	this->top = MidP.y - lWidth/2;
	this->bottom = MidP.y + lWidth/2;		
}

void CDataObj::LongBinary2Imag(CLongBinary &LBinary)
{
	CFile outFile("c:\\aoi_tmpbitmap.bmp",CFile::modeCreate|CFile::modeWrite);
	LPSTR buffer = (LPSTR)GlobalLock(LBinary.m_hData);
	outFile.Write(buffer,LBinary.m_dwDataLength);
	GlobalUnlock(LBinary.m_hData);
	outFile.Close();

	/*HBITMAP hbm = (HBITMAP)::LoadImage(NULL,
		L"c:\\aoi_bitmap.tmp",
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE);

	if(!m_ImageObj.IsNull())
		m_ImageObj.Destroy();

	m_ImageObj.Attach(hbm);*/

	m_ImageObj.Load("c:\\aoi_tmpbitmap.bmp");

	DeleteFile("c:\\aoi_tmpbitmap.bmp");
}

void CDataObj::Imag2LongBinary(CLongBinary& LBinary)
{
	HRESULT hResult;

	hResult = m_ImageObj.Save("c:\\aoi_tmpbitmap.bmp");
	if(FAILED(hResult))
		return;

	CFile fileImage;
	CFileStatus fileStatus;

	if(LBinary.m_hData)
	{
		GlobalFree(LBinary.m_hData);
		LBinary.m_dwDataLength = 0;
	}

	fileImage.Open("c:\\aoi_tmpbitmap.bmp", CFile::modeRead);
	fileImage.GetStatus(fileStatus);//�õ��򿪵�ͼ���ļ���״̬��Ϣ��
	LBinary.m_dwDataLength = fileStatus.m_size;
	HGLOBAL hGlobal= GlobalAlloc(GPTR,fileStatus.m_size);//������ͼ�����ݵĿռ䡣
	LBinary.m_hData = GlobalLock(hGlobal);//���ÿռ丶��m_hData��Ա��
	//�򻺳�����ͼ������
	fileImage.Read(LBinary.m_hData,fileStatus.m_size);//��m_Image��ͼ�����ݡ�	
	GlobalUnlock(hGlobal);

	DeleteFile("c:\\aoi_tmpbitmap.bmp");
}

void CDataObj::CopyRect(const CDataObj * pSrcDataObj)
{
	this->left = pSrcDataObj->left;
	this->right = pSrcDataObj->right;
	this->top = pSrcDataObj->top;
	this->bottom = pSrcDataObj->bottom;
}
