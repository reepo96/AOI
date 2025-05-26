// DetectCmdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectCmdDlg.h"
#include "DetectMainView.h"
#include "DetectBigGraphView.h"

// CDetectCmdDlg �Ի���

IMPLEMENT_DYNAMIC(CDetectCmdDlg, CDialog)

CDetectCmdDlg::CDetectCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectCmdDlg::IDD, pParent)
{
	m_pDetectBigView = NULL;
	m_pCurrentTTemplateImg = NULL;	//��ǰ���ı�׼ͼ��
	m_pCurrentTImg = NULL;			//��ǰ����ͼ��
	m_CurrentTImgRect = CRect(0,0,0,0);
}

CDetectCmdDlg::~CDetectCmdDlg()
{
}

void CDetectCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEMPLATEIMG, m_TemplateImgCtl);
	DDX_Control(pDX, IDC_CURRENT_IMG, m_CurrentImgCtl);
	DDX_Control(pDX, IDC_MANUAL, m_btnManual);
	DDX_Control(pDX, IDC_AUTO, m_btnAuto);
	DDX_Control(pDX, IDC_DETECT_ORGAN, m_btnDetectOrgan);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
}


BEGIN_MESSAGE_MAP(CDetectCmdDlg, CDialog)	
	ON_BN_CLICKED(IDC_MANUAL, &CDetectCmdDlg::OnBnClickedManual)
	ON_BN_CLICKED(IDC_AUTO, &CDetectCmdDlg::OnBnClickedAuto)
	ON_BN_CLICKED(IDC_STOP, &CDetectCmdDlg::OnBnClickedStop)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DETECT_ORGAN, &CDetectCmdDlg::OnBnClickedDetectOrgan)
END_MESSAGE_MAP()


// CDetectCmdDlg ��Ϣ�������

void CDetectCmdDlg::OnBnClickedManual()
{
	CDetectMainView *pMainView=(CDetectMainView *)GetParent();	
	if( pMainView && pMainView->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pMainView->TManual();
	}

	m_btnManual.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
	m_btnAuto.EnableWindow(TRUE);
	m_btnDetectOrgan.EnableWindow(TRUE);
}

void CDetectCmdDlg::OnBnClickedAuto()
{
	CDetectMainView *pMainView=(CDetectMainView *)GetParent();	
	if( pMainView && pMainView->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pMainView->TAuto();
	}
	m_btnManual.EnableWindow(TRUE);
	m_btnStop.EnableWindow(TRUE);
	m_btnAuto.EnableWindow(FALSE);
	m_btnDetectOrgan.EnableWindow(FALSE);
}

void CDetectCmdDlg::OnBnClickedStop()
{
	CDetectMainView *pMainView=(CDetectMainView *)GetParent();	
	if( pMainView && pMainView->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pMainView->TStop();
	}

	m_btnAuto.EnableWindow(TRUE);
}

void CDetectCmdDlg::NotifyDetectDataChanged(void)
{
	if(m_pDetectBigView == NULL || m_pDetectBigView->m_hWnd == NULL)
		return;

	m_pCurrentTTemplateImg = ((CDetectBigGraphView*)m_pDetectBigView)->m_pCurrentTTemplateImg;
	m_pCurrentTImg = ((CDetectBigGraphView*)m_pDetectBigView)->m_pCurrentTImg;
	m_CurrentTImgRect = ((CDetectBigGraphView*)m_pDetectBigView)->m_CurrentTImgRect;

	this->Invalidate();
}

void CDetectCmdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect myRect;
	GetWindowRect(&myRect);

	if(m_pCurrentTTemplateImg!= NULL && !m_pCurrentTTemplateImg->IsNull())
	{
		CRect ctlRect,showRect;
		m_TemplateImgCtl.GetWindowRect(&ctlRect);

		showRect.left = ctlRect.left - myRect.left;
		showRect.right = ctlRect.right - myRect.left;
		showRect.top = ctlRect.top - myRect.top;
		showRect.bottom = ctlRect.bottom - myRect.top;

		//������С����
		double dWidthMultiple = (double)m_pCurrentTTemplateImg->GetWidth()/(showRect.right-showRect.left);
		double dHeightMultiple = (double)m_pCurrentTTemplateImg->GetHeight()/(showRect.bottom-showRect.top);
		double dMultiple = (dWidthMultiple>dHeightMultiple)?dWidthMultiple:dHeightMultiple;

		//����ԭ��
		int iWidth = (double)m_pCurrentTTemplateImg->GetWidth()/dMultiple;
		int iHeight = (double)m_pCurrentTTemplateImg->GetHeight()/dMultiple;
		int iOriginX = showRect.left + ((showRect.right-showRect.left) - iWidth)/2;
		int iOriginY = showRect.top + ((showRect.bottom-showRect.top) - iHeight)/2;

		SetStretchBltMode(dc.m_hDC,HALFTONE);
		m_pCurrentTTemplateImg->StretchBlt(dc.m_hDC,iOriginX,iOriginY,iWidth,iHeight,SRCCOPY);//�Ŵ���С
		
	}

	if(m_pCurrentTImg!= NULL && !m_pCurrentTImg->IsNull())
	{
		CRect ctlRect,showRect;
		m_CurrentImgCtl.GetWindowRect(&ctlRect);

		showRect.left = ctlRect.left - myRect.left;
		showRect.right = ctlRect.right - myRect.left;
		showRect.top = ctlRect.top - myRect.top;
		showRect.bottom = ctlRect.bottom - myRect.top;

		//������С����
		double dWidthMultiple = (double)m_pCurrentTImg->GetWidth()/(showRect.right-showRect.left);
		double dHeightMultiple = (double)m_pCurrentTImg->GetHeight()/(showRect.bottom-showRect.top);
		double dMultiple = (dWidthMultiple>dHeightMultiple)?dWidthMultiple:dHeightMultiple;

		//����ԭ��
		int iWidth = (double)m_pCurrentTImg->GetWidth()/dMultiple;
		int iHeight = (double)m_pCurrentTImg->GetHeight()/dMultiple;
		int iOriginX = showRect.left + ((showRect.right-showRect.left) - iWidth)/2;
		int iOriginY = showRect.top + ((showRect.bottom-showRect.top) - iHeight)/2;

		SetStretchBltMode(dc.m_hDC,HALFTONE);
		m_pCurrentTImg->StretchBlt(dc.m_hDC,iOriginX,iOriginY,iWidth,iHeight,SRCCOPY);//�Ŵ���С

		if(m_CurrentTImgRect.left + m_CurrentTImgRect.right > 0)
		{
			CRect rect;
			rect.left = iOriginX+ m_CurrentTImgRect.left/dMultiple;
			rect.right = iOriginX+ m_CurrentTImgRect.right/dMultiple;
			rect.top = iOriginY + m_CurrentTImgRect.top/dMultiple;
			rect.bottom = iOriginY + m_CurrentTImgRect.bottom/dMultiple;

			CBrush *pBrush = new CBrush(RGB(255,0,0));
			CRgn rg;
			rg.CreateRectRgnIndirect(&rect);
			dc.FrameRgn(&rg,pBrush,1,1);
			delete pBrush;
			pBrush = NULL;
		}
	}
}

BOOL CDetectCmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	m_btnManual.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDetectCmdDlg::OnBnClickedDetectOrgan()
{
	CDetectMainView *pMainView=(CDetectMainView *)GetParent();	
	if( pMainView && pMainView->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pMainView->TOrgan();
	}
}
