#pragma once
#include "afxwin.h"

class CValidateErr;

// CObjectImageDlg ��ʾ�����ͼ��

class CObjectImageDlg : public CDialog
{
	DECLARE_DYNAMIC(CObjectImageDlg)

public:
	CObjectImageDlg(CImage *pImage,CString  sTitle,CWnd* pParent = NULL,CRect *pCurrentImgRect = NULL);   // ��׼���캯��
	virtual ~CObjectImageDlg();

// �Ի�������
	enum { IDD = IDD_OBJIMAGE };

public:
	bool	m_bCanDrawRectTracker;	//�ܷ�������϶�������Ƥ��
	
protected:
	CImage	*m_pImage;
	CString	m_sTitle;
	CValidateErr *m_pParrentWnd;

	CStatic m_ImageCtrl;

	HCURSOR	m_hCurrentCursor;
	CPoint	m_MousePoint;
	CPoint	m_preMousePoint;	//���ǰһ��λ��
	int		m_iNewRectTrackStep;//�½���Ƥ��Ĳ��裨��������
	CPoint	m_NewRectTrackOriginP;//����Ƥ���ԭ��
	CRectTracker *m_pRectTracker;	//ѡ����ο�
	CImage	*m_pLearnImage;	//ѧϰ��ͼ��

	long m_lDCOriginX;	//ͼ����ʾ��dc�ϵ�ԭ��x
	long m_lDCOriginY; //ͼ����ʾ��dc�ϵ�ԭ��y
	double m_dZoomOutScale;//��С����

	CRect m_CurrentTImgRect;//��ǰ���ͼ��Ŀ�
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
