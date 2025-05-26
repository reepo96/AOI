#pragma once

#include "GraphView.h"
#include "TemplateData.h"

//#define MOTOR_MM_STEP   8000   //电机每毫米的步长

//#define MM_PELS_2048    69.43146     //2048分辨率每毫米的像素个数
//#define MM_PELS_1024    34.71573     //1024分辨率每毫米的像素个数
//#define MM_PELS_640    21.69733125     //640分辨率每毫米的像素个数
//#define MM_PELS_512    17.357865     //512分辨率每毫米的像素个数

const double ZOOMSTEP = 0.2;	//放大缩小的比例

class CBaseBigGraphView : public CGraphView
{
	DECLARE_DYNCREATE(CBaseBigGraphView)

public:
	int		m_iCurrentZoom;		//当前的放大倍数
	long	m_lCurrentCenterX;	//视图当前中心x点(是指画布在视图中心上的点)
	long	m_lCurrentCenterY;	//视图当前中心y点
	long	m_lSrcOriginX;	//CImage对象拷贝到屏幕的X原点
	long	m_lSrcOriginY;	//CImage对象拷贝到屏幕的Y原点

	//用于保存图像切换前参数的保存
	int		m_iOldPCBCurrentZoom;		//原来的当前的放大倍数
	long	m_lOldPCBCurrentCenterX;	//原来的视图当前中心x点(是指画布在视图中心上的点)
	long	m_lOldPCBCurrentCenterY;	//原来的视图当前中心y点
	long	m_lOldPCBSrcOriginX;	//原来的CImage对象拷贝到屏幕的X原点
	long	m_lOldPCBSrcOriginY;	//原来的CImage对象拷贝到屏幕的Y原点

	int		m_iOldFOVCurrentZoom;		//原来的当前的放大倍数
	long	m_lOldFOVCurrentCenterX;	//原来的视图当前中心x点(是指画布在视图中心上的点)
	long	m_lOldFOVCurrentCenterY;	//原来的视图当前中心y点
	long	m_lOldFOVSrcOriginX;	//原来的CImage对象拷贝到屏幕的X原点
	long	m_lOldFOVSrcOriginY;	//原来的CImage对象拷贝到屏幕的Y原点

	struct IMAGEPARAMENT {
		int		nWidth;
		int		nHeight;
		int		nBitCount;
		int		nBytesPerLine;
		int		nBytesPerPixel;
		int		nNumColors;
		int		nSize;
	};

protected:
	CBaseBigGraphView();           // 动态创建所使用的受保护的构造函数
	CBaseBigGraphView(UINT nIDTemplate);
	virtual ~CBaseBigGraphView();

protected:	
	CImage		*m_pCurrentImage;	//当前显示图片的原始对象
	CImage		m_TempImage;	//临时的ＣＩｍａｇｅ对象

	CRITICAL_SECTION	m_Critial4CurrentImage;

public:
	enum { IDD = IDD_BIGGRAPHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetTemplateData(CTemplateData * pTemplateData);	
protected:
	void	init();
public:
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnOriginal();
	
protected:
	CPoint ComputerSrcOrigin(CPoint CenterPoint, CPoint SrcWH, CPoint DestWH);
	
	//基于原始的基础上放大缩小rect
	void ZoomRectOnOrigin(CRect& rect);
	
	//基于当前rect基础上放大rect
	void ZoomInRectOnCurrent(CRect& rect);
	
	//基于当前rect基础上缩小rect
	void ZoomOutRectOnCurrent(CRect& rect);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:	
	void GetOriginRect(const CRect currentRect, CRect& OriginRect);
	// 将image对象放大缩小后放到临时CImage对象中
	void ZoomImageToTemp(void);		
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	// 调整视图中心X点
	bool AdjustCenterX(int iAdjustNum);
	bool AdjustCenterY(int iAdjustNum);	
	// 根据fov范围加载fov图像
	//bNeedCompensate：是否需要补偿
	void LoadFovImage(CRect fovRect, CImage* pDestImage,bool bNeedCompensate=true);

	// 根据rect获得原图像rect区间的图像
	void GetFactImage(const CRect rect, CImage* pImage);

    // 根据设置的中心点计算真正的中心点和原点
	void ComputerCenterAndOrigin(long lNewCenterX, long lNewCenterY);

	int	  InImage(CImage *pImg,int x,int y);

	void  GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam);

	void  GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

	void  SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

	//将图像水平镜像、垂直镜像或180度旋转(flag分别是1、2、3)
	void Flip(CImage *pImg,int flag);
};


