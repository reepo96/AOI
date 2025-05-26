// ObjectImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "ObjectImageDlg.h"
#include "ValidateErr.h"

// CObjectImageDlg 对话框

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


// CObjectImageDlg 消息处理程序

void CObjectImageDlg::OnBnClickedPre()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CObjectImageDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CObjectImageDlg::OnBnClickedClose()
{
	CDialog::OnCancel();
}

BOOL CObjectImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    int iMaxShowWidth = (1024/2) - 8;    //最大显示宽度
    int iMaxShowHeight = iMaxShowWidth;  //最大显示高度

	::g_SetDialogStrings(this);

	// TODO:  在此添加额外的初始化
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
	// 异常: OCX 属性页应返回 FALSE
}

void CObjectImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if( m_pImage != NULL && !m_pImage->IsNull())
	{
		CDC *pDc = m_ImageCtrl.GetWindowDC();
		CRect ctlRect;
		m_ImageCtrl.GetWindowRect(&ctlRect);

		int iShowImageWidth = m_pImage->GetWidth();
        int iShowImageHeight = m_pImage->GetHeight();
        //double dZoomOutScale = 1.00;    //缩小比例

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

	//画跟踪矩型
	if(m_pRectTracker != NULL)
	{
		m_pRectTracker->Draw(&dc);
	}
}

void CObjectImageDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//按下Control键并且当前可以用鼠标创建橡皮框时
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

	//Control键按下并且按下鼠标左键并且当前可以用鼠标创建橡皮框时，在拖动鼠标时，表示在调整橡皮框大小
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_COPYWAY);

		if(m_iNewRectTrackStep == 1)	//现在正在新建橡皮框
		{
			CRect rect(m_NewRectTrackOriginP,point);
			
			if(m_pRectTracker == NULL)
			{
				m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
				m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
			}
			else
			{
				//消去上一个橡皮框图形
				long	lTmp;
				if( m_pRectTracker->m_rect.right < m_pRectTracker->m_rect.left)
				{//橡皮框在做反向运动，所以要将它的RECT调整过来
					lTmp = m_pRectTracker->m_rect.right;
					m_pRectTracker->m_rect.right = m_pRectTracker->m_rect.left;
					m_pRectTracker->m_rect.left = lTmp;
				}
				if( m_pRectTracker->m_rect.bottom < m_pRectTracker->m_rect.top)
				{//橡皮框在做反向运动，所以要将它的RECT调整过来
					lTmp = m_pRectTracker->m_rect.bottom;
					m_pRectTracker->m_rect.bottom = m_pRectTracker->m_rect.top;
					m_pRectTracker->m_rect.top = lTmp;
				}
				m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
				InvalidateRect(m_pRectTracker->m_rect);//使原橡皮框的区间无效，触发onpain重画

				m_pRectTracker->m_rect = rect;
			}
			m_pRectTracker->Draw(&dc);
			
			UpdateWindow();
		}

		return;
	}
	//Control键按下并且按下鼠标左键，在拖动鼠标时，表示在调整橡皮框大小<----end


	bool bLBDown=false;	//鼠标左键是否按下
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
		int nIn; //定义一个鼠标的点击值；
		nIn=m_pRectTracker->HitTest(point); //看看点到了哪了		

		//根据不同的位置设置鼠标光标		
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
	//Control键按下并且当前可以用鼠标创建橡皮框时,则表示开始创建新的橡皮框
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		if( m_pRectTracker != NULL)
		{
			//要将原橡皮框的图形消去（即设置原框的矩形无效）
			m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
			InvalidateRect(m_pRectTracker->m_rect);

			delete m_pRectTracker;
			m_pRectTracker =NULL;
		}

		m_iNewRectTrackStep = 1;	//新建橡皮框的第一步
		m_NewRectTrackOriginP = point;
		return;
	}
	
	if( m_pRectTracker != NULL)
	{
		int nIn; //定义一个鼠标的点击值；
		nIn=m_pRectTracker->HitTest(point); //看看点到了哪了

		if(nIn>=0)  //在四边形区域内，可调整大小
		{
			CClientDC dc(this);
			m_pRectTracker->Draw(&dc);
			m_pRectTracker->Track(this,point,TRUE);
			Invalidate();//引起OnPain函数的发生			
		}		
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CObjectImageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	long	lTmp;

	if(m_iNewRectTrackStep == 1)//之前在拖动橡皮框，现在结束
	{
		m_iNewRectTrackStep = 2;
		CClientDC dc( this);

		CRect rect(m_NewRectTrackOriginP,point);
		if( rect.right < rect.left)
		{//橡皮框在做反向运动，所以要将它的RECT调整过来
			lTmp = rect.right;
			rect.right = rect.left;
			rect.left = lTmp;
		}
		if( rect.bottom < rect.top)
		{//橡皮框在做反向运动，所以要将它的RECT调整过来
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
		sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
		AfxMessageBox(sMsgInfo);
		return;
	}

	CRect originRect;

	//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标
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

	//取得颜色板
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
