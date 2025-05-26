#pragma once
#include "afxwin.h"

class CValidateErr;

// CObjectImageDlg 显示对象的图像

class CObjectImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CObjectImageDlg)

public:
	CObjectImageDlg(CImage *pImage,CString  sTitle,CWnd* pParent = NULL,CRect *pCurrentImgRect = NULL);   // 标准构造函数
	virtual ~CObjectImageDlg();

// 对话框数据
	enum { IDD = IDD_OBJIMAGE };

public:
	bool	m_bCanDrawRectTracker;	//能否用鼠标拖动创建橡皮框
	
protected:
	CImage	*m_pImage;
	CString	m_sTitle;
	CValidateErr *m_pParrentWnd;

	CStatic m_ImageCtrl;

	HCURSOR	m_hCurrentCursor;
	CPoint	m_MousePoint;
	CPoint	m_preMousePoint;	//鼠标前一个位置
	int		m_iNewRectTrackStep;//新建橡皮框的步骤（分两步）
	CPoint	m_NewRectTrackOriginP;//新橡皮框的原点
	CRectTracker *m_pRectTracker;	//选择矩形框
	CImage	*m_pLearnImage;	//学习的图像

	long m_lDCOriginX;	//图像显示在dc上的原点x
	long m_lDCOriginY; //图像显示在dc上的原点y
	double m_dZoomOutScale;//缩小比例

	CRect m_CurrentTImgRect;//当前检测图像的框
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedPre();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedClose();
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	void Learn(void);
public:
	afx_msg void OnBnClickedLearn();
public:
	void UnLearn(void);
};
