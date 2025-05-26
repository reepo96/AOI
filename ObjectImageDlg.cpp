// ObjectImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "ObjectImageDlg.h"
#include "ValidateErr.h"

// CObjectImageDlg �Ի���

IMPLEMENT_DYNAMIC(CObjectImageDlg, CDialog)

CObjectImageDlg::CObjectImageDlg(CImage *pImage,CString sTitle,CWnd* pParent /*=NULL*/,CRect *pCurrentImgRect)
	: CDialog(CObjectImageDlg::IDD, pParent)
{
	m_pImage = pImage;
	m_sTitle = sTitle;
	m_bCanDrawRectTracker = false;

	m_CurrentTImgRect = CRect(0,0,0,0);

	m_hCurrentCursor = NULL;
	m_MousePoint = CPoint(0,0);
	m_preMousePoint = CPoint(0,0);
	m_iNewRectTrackStep = 0;
	m_NewRectTrackOriginP = CPoint(0,0);
	m_pLearnImage = NULL;

	m_dZoomOutScale = 1.00;
	m_lDCOriginX = 0;
	m_lDCOriginY = 0;
	m_pRectTracker = NULL;

	if( pCurrentImgRect != NULL)
	{
		m_CurrentTImgRect = *pCurrentImgRect;
	}

	m_pParrentWnd = (CValidateErr*)pParent;
}

CObjectImageDlg::~CObjectImageDlg()
{
	if(!m_pImage->IsNull())
	{
		m_pImage->Destroy();
	}
}

void CObjectImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_ImageCtrl);
}


BEGIN_MESSAGE_MAP(CObjectImageDlg, CDialog)
	ON_BN_CLICKED(IDC_PRE, &CObjectImageDlg::OnBnClickedPre)
	ON_BN_CLICKED(IDC_NEXT, &CObjectImageDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_CLOSE, &CObjectImageDlg::OnBnClickedClose)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_LEARN, &CObjectImageDlg::OnBnClickedLearn)
END_MESSAGE_MAP()


// CObjectImageDlg ��Ϣ�������

void CObjectImageDlg::OnBnClickedPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CObjectImageDlg::OnBnClickedNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CObjectImageDlg::OnBnClickedClose()
{
	CDialog::OnCancel();
}

BOOL CObjectImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    int iMaxShowWidth = (1024/2) - 8;    //�����ʾ���
    int iMaxShowHeight = iMaxShowWidth;  //�����ʾ�߶�

	::g_SetDialogStrings(this);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetWindowTextA(m_sTitle);

	CRect currentCtlRect,currentDlgRect;
	m_ImageCtrl.GetWindowRect(&currentCtlRect);
	this->GetWindowRect(&currentDlgRect);

	if( m_pImage != NULL && !m_pImage->IsNull())
	{
		int iImageWidth = m_pImage->GetWidth();
		int iImageHeight = m_pImage->GetHeight();

		CButton *pLearnBtn = (CButton*)this->GetDlgItem(IDC_LEARN);		
		CButton *pCloseBtn = (CButton*)this->GetDlgItem(IDC_CLOSE);

		CRect LearnRect,closeRect;
		pLearnBtn->GetWindowRect(&LearnRect);		
		pCloseBtn->GetWindowRect(&closeRect);

		int iNewWndWidth = currentDlgRect.Width();
		int iNewWndHeight = currentDlgRect.Height();
		int iNewCtlWidth = currentCtlRect.Width();
		int iNewCtlHeigh = currentCtlRect.Height();

		int iButtonMovX=0,iButtonMovY=-75;

		if(iImageWidth > currentCtlRect.Width() )
		{
			iNewCtlWidth = iImageWidth;
		}
        iNewCtlWidth = (iNewCtlWidth>iMaxShowWidth)?iMaxShowWidth:iNewCtlWidth;
        iNewWndWidth = iNewCtlWidth +4;
        iButtonMovX = (iNewCtlWidth-currentCtlRect.Width())/2;

		if(iImageHeight > currentCtlRect.Height())
		{
			iNewCtlHeigh = iImageHeight;
		}
        iNewCtlHeigh = (iNewCtlHeigh>iMaxShowHeight)?iMaxShowHeight:iNewCtlHeigh;
        iNewWndHeight = currentDlgRect.Height()+(iNewCtlHeigh -currentCtlRect.Height());
        iButtonMovY += iNewCtlHeigh -currentCtlRect.Height();

		this->MoveWindow(currentDlgRect.left,currentDlgRect.top,iNewWndWidth,iNewWndHeight);
		m_ImageCtrl.MoveWindow(currentCtlRect.left-currentDlgRect.left,
			currentCtlRect.top-currentDlgRect.top,iNewCtlWidth,iNewCtlHeigh);

		pLearnBtn->MoveWindow(LearnRect.left-currentDlgRect.left+iButtonMovX,
			LearnRect.top-currentDlgRect.top+20+iButtonMovY,LearnRect.Width(),LearnRect.Height());
		pCloseBtn->MoveWindow(closeRect.left-currentDlgRect.left+iButtonMovX,
			closeRect.top-currentDlgRect.top+20+iButtonMovY,closeRect.Width(),closeRect.Height());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CObjectImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if( m_pImage != NULL && !m_pImage->IsNull())
	{
		CDC *pDc = m_ImageCtrl.GetWindowDC();
		CRect ctlRect;
		m_ImageCtrl.GetWindowRect(&ctlRect);

		int iShowImageWidth = m_pImage->GetWidth();
        int iShowImageHeight = m_pImage->GetHeight();
        //double dZoomOutScale = 1.00;    //��С����

		m_lDCOriginX = (ctlRect.Width() - iShowImageWidth)/2;
        if(m_lDCOriginX < 0)
        {
            m_lDCOriginX = 0;
            iShowImageWidth = ctlRect.Width();
            m_dZoomOutScale = (double)iShowImageWidth/(double)m_pImage->GetWidth();
            iShowImageHeight = m_pImage->GetHeight()*m_dZoomOutScale;			
        }

		m_lDCOriginY = (ctlRect.Height() -iShowImageHeight)/2;
        if(m_lDCOriginY < 0)
        {
            m_lDCOriginY = 0;
            iShowImageHeight = ctlRect.Height();
            m_dZoomOutScale = (double)iShowImageHeight/(double)m_pImage->GetHeight();

            iShowImageWidth = m_pImage->GetWidth()*m_dZoomOutScale;
            m_lDCOriginX = (ctlRect.Width() - iShowImageWidth)/2;			
        }

		m_pImage->StretchBlt(dc.m_hDC,m_lDCOriginX,m_lDCOriginY,iShowImageWidth,iShowImageHeight,SRCCOPY);

		if( !m_CurrentTImgRect.IsRectEmpty())
		{
			CRect rect;
			rect.left = m_CurrentTImgRect.left*m_dZoomOutScale +m_lDCOriginX;
			rect.right = m_CurrentTImgRect.right*m_dZoomOutScale +m_lDCOriginX;
			rect.top = m_CurrentTImgRect.top*m_dZoomOutScale +m_lDCOriginY;
			rect.bottom = m_CurrentTImgRect.bottom*m_dZoomOutScale +m_lDCOriginY;

			CBrush *pBrush = new CBrush(RGB(255,0,0));
			CRgn rg;
			rg.CreateRectRgnIndirect(&rect);
			dc.FrameRgn(&rg,pBrush,1,1);
			delete pBrush;
			pBrush = NULL;
		}
	}

	//�����پ���
	if(m_pRectTracker != NULL)
	{
		m_pRectTracker->Draw(&dc);
	}
}

void CObjectImageDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//����Control�����ҵ�ǰ��������괴����Ƥ��ʱ
	if( nChar == VK_CONTROL && m_bCanDrawRectTracker) 
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_COPYWAY);
		SetCursor(m_hCurrentCursor);
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CObjectImageDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	m_MousePoint = point;
	CClientDC dc(this);

	//Control�����²��Ұ������������ҵ�ǰ��������괴����Ƥ��ʱ�����϶����ʱ����ʾ�ڵ�����Ƥ���С
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_COPYWAY);

		if(m_iNewRectTrackStep == 1)	//���������½���Ƥ��
		{
			CRect rect(m_NewRectTrackOriginP,point);
			
			if(m_pRectTracker == NULL)
			{
				m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
				m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
			}
			else
			{
				//��ȥ��һ����Ƥ��ͼ��
				long	lTmp;
				if( m_pRectTracker->m_rect.right < m_pRectTracker->m_rect.left)
				{//��Ƥ�����������˶�������Ҫ������RECT��������
					lTmp = m_pRectTracker->m_rect.right;
					m_pRectTracker->m_rect.right = m_pRectTracker->m_rect.left;
					m_pRectTracker->m_rect.left = lTmp;
				}
				if( m_pRectTracker->m_rect.bottom < m_pRectTracker->m_rect.top)
				{//��Ƥ�����������˶�������Ҫ������RECT��������
					lTmp = m_pRectTracker->m_rect.bottom;
					m_pRectTracker->m_rect.bottom = m_pRectTracker->m_rect.top;
					m_pRectTracker->m_rect.top = lTmp;
				}
				m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
				InvalidateRect(m_pRectTracker->m_rect);//ʹԭ��Ƥ���������Ч������onpain�ػ�

				m_pRectTracker->m_rect = rect;
			}
			m_pRectTracker->Draw(&dc);
			
			UpdateWindow();
		}

		return;
	}
	//Control�����²��Ұ��������������϶����ʱ����ʾ�ڵ�����Ƥ���С<----end


	bool bLBDown=false;	//�������Ƿ���
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		bLBDown = true;
	}
	else
	{
		m_preMousePoint = CPoint(0,0);
	}

	if( m_pRectTracker != NULL)
	{
		int nIn; //����һ�����ĵ��ֵ��
		nIn=m_pRectTracker->HitTest(point); //�����㵽������		

		//���ݲ�ͬ��λ�����������		
		switch(nIn)
		{
		case CRectTracker::hitNothing:
			if(bLBDown)
			{
				m_preMousePoint = point;
			}

			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDOPEN);
			break;

		case CRectTracker::hitTopLeft:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNWSE);
			break;
		case CRectTracker::hitBottomRight:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNWSE);
			break;
		case CRectTracker::hitTopRight:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNESW);
			break;
		case CRectTracker::hitBottomLeft:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNESW);
		case CRectTracker::hitTop:
		case CRectTracker::hitBottom:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNS);
			break;
		case CRectTracker::hitRight:
		case CRectTracker::hitLeft:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKWE);
			break;
		case CRectTracker::hitMiddle:
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRACKMOVE);
			break;
		}//end switch		
	}
	else
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDOPEN);		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CObjectImageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Control�����²��ҵ�ǰ��������괴����Ƥ��ʱ,���ʾ��ʼ�����µ���Ƥ��
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		if( m_pRectTracker != NULL)
		{
			//Ҫ��ԭ��Ƥ���ͼ����ȥ��������ԭ��ľ�����Ч��
			m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
			InvalidateRect(m_pRectTracker->m_rect);

			delete m_pRectTracker;
			m_pRectTracker =NULL;
		}

		m_iNewRectTrackStep = 1;	//�½���Ƥ��ĵ�һ��
		m_NewRectTrackOriginP = point;
		return;
	}
	
	if( m_pRectTracker != NULL)
	{
		int nIn; //����һ�����ĵ��ֵ��
		nIn=m_pRectTracker->HitTest(point); //�����㵽������

		if(nIn>=0)  //���ı��������ڣ��ɵ�����С
		{
			CClientDC dc(this);
			m_pRectTracker->Draw(&dc);
			m_pRectTracker->Track(this,point,TRUE);
			Invalidate();//����OnPain�����ķ���			
		}		
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CObjectImageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	long	lTmp;

	if(m_iNewRectTrackStep == 1)//֮ǰ���϶���Ƥ�����ڽ���
	{
		m_iNewRectTrackStep = 2;
		CClientDC dc( this);

		CRect rect(m_NewRectTrackOriginP,point);
		if( rect.right < rect.left)
		{//��Ƥ�����������˶�������Ҫ������RECT��������
			lTmp = rect.right;
			rect.right = rect.left;
			rect.left = lTmp;
		}
		if( rect.bottom < rect.top)
		{//��Ƥ�����������˶�������Ҫ������RECT��������
			lTmp = rect.bottom;
			rect.bottom = rect.top;
			rect.top = lTmp;
		}
			
		if(m_pRectTracker != NULL)
		{
			m_pRectTracker->m_rect = rect;
			m_pRectTracker->Draw(&dc);
		}
	}

	Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

void CObjectImageDlg::Learn(void)
{
	m_bCanDrawRectTracker = true;
	CWnd *pButton = GetDlgItem(IDC_LEARN);
	if(pButton != NULL)
	{
		pButton->EnableWindow(TRUE);
	}
}

void CObjectImageDlg::OnBnClickedLearn()
{
	CString sMsgInfo;

	if(m_pRectTracker == NULL)
	{
		sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","��ǰ�ı�̹�������û���޸�");
		AfxMessageBox(sMsgInfo);
		return;
	}

	CRect originRect;

	//��Ƥ��ǰ��������ϵ�Ǵ������꣬Ҫ����ת����ͼ������
	originRect.left = (m_pRectTracker->m_rect.left - m_lDCOriginX)/m_dZoomOutScale;
	originRect.right = (m_pRectTracker->m_rect.right -m_lDCOriginX)/m_dZoomOutScale;
	originRect.top = (m_pRectTracker->m_rect.top -m_lDCOriginY)/m_dZoomOutScale;
	originRect.bottom = (m_pRectTracker->m_rect.bottom -m_lDCOriginY)/m_dZoomOutScale;

	if(m_pLearnImage == NULL)
	{
		m_pLearnImage = new CImage();
	}

	if( !m_pLearnImage->IsNull())
	{
		m_pLearnImage->Destroy();
	}

	if(!m_pLearnImage->Create(originRect.Width(),originRect.Height(),m_pImage->GetBPP()))
	{
		::AfxMessageBox("Create image object fail");		
		return;
	}

	//ȡ����ɫ��
	if(m_pImage->IsDIBSection())
	{
		int MaxColors = m_pImage->GetMaxColorTableEntries();
		if(MaxColors>0)
		{
			RGBQUAD* ColorTable;
			ColorTable = new RGBQUAD[MaxColors];
			m_pImage->GetColorTable(1,MaxColors,ColorTable);
			m_pLearnImage->SetColorTable(0,MaxColors,ColorTable);
			delete []ColorTable;
		}
	}

	if(m_pLearnImage->IsNull())
	{
		return;
	}

	m_pImage->BitBlt(m_pLearnImage->GetDC(),0,0,originRect.Width(),originRect.Height(),
		originRect.left,originRect.top,SRCCOPY);
	m_pLearnImage->ReleaseDC();

	m_pParrentWnd->SetLearnImage(m_pLearnImage);

	delete m_pRectTracker;
	m_pRectTracker = NULL;
	Invalidate();
}

void CObjectImageDlg::UnLearn(void)
{
	m_bCanDrawRectTracker = false;
	CWnd *pButton = GetDlgItem(IDC_LEARN);
	if(pButton != NULL)
	{
		pButton->EnableWindow(FALSE);
	}
}
