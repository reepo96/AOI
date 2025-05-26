#pragma once

typedef struct _DrawContext
{
	COLORREF color;
	double	 dZoom;//放大倍数
	long	lSrcOriginX;//原点
	long	lSrcOriginY;//原点
	CDC		*pDC;
}DRAWCONTEXT;

/**
*数据对象之父类
**/
class CDataObj
{
public:
	CDataObj(void);
public:
	~CDataObj(void);

public:
	//对象区域
	long	left;
	long	right;
	long	top;
	long	bottom;
	
	bool	bVisible;//是否将对象draw出来

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

	//沿X轴平移
	void MoveX(long lMoveStep)
	{
		left += lMoveStep;
		right += lMoveStep;
	}

	//沿Y轴平移
	void MoveY(long lMoveStep)
	{
		top += lMoveStep;
		bottom += lMoveStep;
	}

	//翻转90度
	void Turn();
	
protected:

	//由CLongBinary转换成CImage对象
	void LongBinary2Imag(CLongBinary& LBinary);

	//由CImage 转换成CLongBinary对象
	void Imag2LongBinary(CLongBinary& LBinary);
public:
	void CopyRect(const CDataObj * pSrcDataObj);
};
