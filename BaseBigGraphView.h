#pragma once

#include "GraphView.h"
#include "TemplateData.h"

//#define MOTOR_MM_STEP   8000   //���ÿ���׵Ĳ���

//#define MM_PELS_2048    69.43146     //2048�ֱ���ÿ���׵����ظ���
//#define MM_PELS_1024    34.71573     //1024�ֱ���ÿ���׵����ظ���
//#define MM_PELS_640    21.69733125     //640�ֱ���ÿ���׵����ظ���
//#define MM_PELS_512    17.357865     //512�ֱ���ÿ���׵����ظ���

const double ZOOMSTEP = 0.2;	//�Ŵ���С�ı���

class CBaseBigGraphView : public CGraphView
{
	DECLARE_DYNCREATE(CBaseBigGraphView)

public:
	int		m_iCurrentZoom;		//��ǰ�ķŴ���
	long	m_lCurrentCenterX;	//��ͼ��ǰ����x��(��ָ��������ͼ�����ϵĵ�)
	long	m_lCurrentCenterY;	//��ͼ��ǰ����y��
	long	m_lSrcOriginX;	//CImage���󿽱�����Ļ��Xԭ��
	long	m_lSrcOriginY;	//CImage���󿽱�����Ļ��Yԭ��

	//���ڱ���ͼ���л�ǰ�����ı���
	int		m_iOldPCBCurrentZoom;		//ԭ���ĵ�ǰ�ķŴ���
	long	m_lOldPCBCurrentCenterX;	//ԭ������ͼ��ǰ����x��(��ָ��������ͼ�����ϵĵ�)
	long	m_lOldPCBCurrentCenterY;	//ԭ������ͼ��ǰ����y��
	long	m_lOldPCBSrcOriginX;	//ԭ����CImage���󿽱�����Ļ��Xԭ��
	long	m_lOldPCBSrcOriginY;	//ԭ����CImage���󿽱�����Ļ��Yԭ��

	int		m_iOldFOVCurrentZoom;		//ԭ���ĵ�ǰ�ķŴ���
	long	m_lOldFOVCurrentCenterX;	//ԭ������ͼ��ǰ����x��(��ָ��������ͼ�����ϵĵ�)
	long	m_lOldFOVCurrentCenterY;	//ԭ������ͼ��ǰ����y��
	long	m_lOldFOVSrcOriginX;	//ԭ����CImage���󿽱�����Ļ��Xԭ��
	long	m_lOldFOVSrcOriginY;	//ԭ����CImage���󿽱�����Ļ��Yԭ��

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
	CBaseBigGraphView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	CBaseBigGraphView(UINT nIDTemplate);
	virtual ~CBaseBigGraphView();

protected:	
	CImage		*m_pCurrentImage;	//��ǰ��ʾͼƬ��ԭʼ����
	CImage		m_TempImage;	//��ʱ�ģãɣ�������

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	
	//����ԭʼ�Ļ����ϷŴ���Сrect
	void ZoomRectOnOrigin(CRect& rect);
	
	//���ڵ�ǰrect�����ϷŴ�rect
	void ZoomInRectOnCurrent(CRect& rect);
	
	//���ڵ�ǰrect��������Сrect
	void ZoomOutRectOnCurrent(CRect& rect);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:	
	void GetOriginRect(const CRect currentRect, CRect& OriginRect);
	// ��image����Ŵ���С��ŵ���ʱCImage������
	void ZoomImageToTemp(void);		
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	// ������ͼ����X��
	bool AdjustCenterX(int iAdjustNum);
	bool AdjustCenterY(int iAdjustNum);	
	// ����fov��Χ����fovͼ��
	//bNeedCompensate���Ƿ���Ҫ����
	void LoadFovImage(CRect fovRect, CImage* pDestImage,bool bNeedCompensate=true);

	// ����rect���ԭͼ��rect�����ͼ��
	void GetFactImage(const CRect rect, CImage* pImage);

    // �������õ����ĵ�������������ĵ��ԭ��
	void ComputerCenterAndOrigin(long lNewCenterX, long lNewCenterY);

	int	  InImage(CImage *pImg,int x,int y);

	void  GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam);

	void  GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

	void  SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

	//��ͼ��ˮƽ���񡢴�ֱ�����180����ת(flag�ֱ���1��2��3)
	void Flip(CImage *pImg,int flag);
};


