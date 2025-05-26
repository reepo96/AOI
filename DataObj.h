#pragma once

typedef struct _DrawContext
{
	COLORREF color;
	double	 dZoom;//�Ŵ���
	long	lSrcOriginX;//ԭ��
	long	lSrcOriginY;//ԭ��
	CDC		*pDC;
}DRAWCONTEXT;

/**
*���ݶ���֮����
**/
class CDataObj
{
public:
	CDataObj(void);
public:
	~CDataObj(void);

public:
	//��������
	long	left;
	long	right;
	long	top;
	long	bottom;
	
	bool	bVisible;//�Ƿ񽫶���draw����

	CImage	m_ImageObj;

public:
	void CopyPrgData(const CDataObj *pSrcDataObj,const long lCurrentCenterX,const long lCurrentCenterY);

	CPoint GetMidP() const;
	void Draw(DRAWCONTEXT DrawContext);

	long GetWidth()
	{
		return (right-left);
	}

	long GetHeight()
	{
		return (bottom - top);
	}

	CRect getRect()
	{
		return CRect(left,top,right,bottom);
	}

	//��X��ƽ��
	void MoveX(long lMoveStep)
	{
		left += lMoveStep;
		right += lMoveStep;
	}

	//��Y��ƽ��
	void MoveY(long lMoveStep)
	{
		top += lMoveStep;
		bottom += lMoveStep;
	}

	//��ת90��
	void Turn();
	
protected:

	//��CLongBinaryת����CImage����
	void LongBinary2Imag(CLongBinary& LBinary);

	//��CImage ת����CLongBinary����
	void Imag2LongBinary(CLongBinary& LBinary);
public:
	void CopyRect(const CDataObj * pSrcDataObj);
};
