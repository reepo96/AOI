// OrganImage.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganImage.h"


// COrganImage 对话框

IMPLEMENT_DYNAMIC(COrganImage, CDialog)

COrganImage::COrganImage(COrganData *pOrgan,CWnd* pParent /*=NULL*/)
	: CDialog(COrganImage::IDD, pParent)
	, m_sImageNumInf(_T(""))
{
	m_pOrgan = pOrgan;
	m_pBodyImage = NULL;
	m_iImageIndex = 0;
}

COrganImage::~COrganImage()
{
}

void COrganImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE, m_ImageCtrl);
	DDX_Text(pDX, IDC_IMAGE_NUM, m_sImageNumInf);
}


BEGIN_MESSAGE_MAP(COrganImage, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PRE, &COrganImage::OnBnClickedPre)
	ON_BN_CLICKED(IDC_NEXT, &COrganImage::OnBnClickedNext)
	ON_BN_CLICKED(IDC_CLOSE, &COrganImage::OnBnClickedClose)
	ON_BN_CLICKED(IDC_DELETE, &COrganImage::OnBnClickedDelete)
END_MESSAGE_MAP()


// COrganImage 消息处理程序

BOOL COrganImage::OnInitDialog()
{
	CDialog::OnInitDialog();

    int iMaxShowWidth = (1024/2) - 8;    //最大显示宽度
    int iMaxShowHeight = iMaxShowWidth;  //最大显示高度

	::g_SetDialogStrings(this);

	CRect currentCtlRect,currentDlgRect;
	m_ImageCtrl.GetWindowRect(&currentCtlRect);
	this->GetWindowRect(&currentDlgRect);

	if( m_pOrgan != NULL)
	{
		int iImageWidth = m_pOrgan->RangeRight - m_pOrgan->RangeLeft;
		int iImageHeight = m_pOrgan->RangeBottom - m_pOrgan->RrangeTop;
		
		CWnd *pShowImageNum = GetDlgItem(IDC_IMAGE_NUM);
		CButton *pPreBtn = (CButton*)this->GetDlgItem(IDC_PRE);
		CButton *pNextBtn = (CButton*)this->GetDlgItem(IDC_NEXT);
		CButton *pCloseBtn = (CButton*)this->GetDlgItem(IDC_CLOSE);
		CButton *pDelBtn = (CButton*)this->GetDlgItem(IDC_DELETE);

		CRect showNumRect,PreRect,nextRect,closeRect,delRect;
		pShowImageNum->GetWindowRect(&showNumRect);
		pPreBtn->GetWindowRect(&PreRect);
		pNextBtn->GetWindowRect(&nextRect);
		pCloseBtn->GetWindowRect(&closeRect);
		pDelBtn->GetWindowRect(&delRect);

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
			//iButtonMovY += iImageHeight -currentCtlRect.Height();
		}
        iNewCtlHeigh = (iNewCtlHeigh>iMaxShowHeight)?iMaxShowHeight:iNewCtlHeigh;
        iNewWndHeight = currentDlgRect.Height()+(iNewCtlHeigh -currentCtlRect.Height());
        iButtonMovY += iNewCtlHeigh -currentCtlRect.Height();

		this->MoveWindow(currentDlgRect.left,currentDlgRect.top,iNewWndWidth,iNewWndHeight);
		m_ImageCtrl.MoveWindow(currentCtlRect.left,currentCtlRect.top,iNewCtlWidth,iNewCtlHeigh);

		pShowImageNum->MoveWindow(showNumRect.left+iButtonMovX,showNumRect.top+iButtonMovY,showNumRect.Width(),showNumRect.Height());
		pPreBtn->MoveWindow(PreRect.left+iButtonMovX,PreRect.top+iButtonMovY,PreRect.Width(),PreRect.Height());
		pNextBtn->MoveWindow(nextRect.left+iButtonMovX,nextRect.top+iButtonMovY,nextRect.Width(),nextRect.Height());
		pCloseBtn->MoveWindow(closeRect.left+iButtonMovX,closeRect.top+iButtonMovY,closeRect.Width(),closeRect.Height());
		pDelBtn->MoveWindow(delRect.left+iButtonMovX,delRect.top+iButtonMovY,delRect.Width(),delRect.Height());

		if(m_pOrgan->m_MainBodyImageList.size()>0)
		{
			m_pBodyImage = m_pOrgan->m_MainBodyImageList.at(m_iImageIndex);
			m_sImageNumInf.Format("%d/%d",m_iImageIndex+1,m_pOrgan->m_MainBodyImageList.size());
		}
		else
		{
			m_sImageNumInf = "0/0";
		}

		this->UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COrganImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if( m_pBodyImage != NULL && !m_pBodyImage->IsNull())
	{
		CDC *pDc = m_ImageCtrl.GetWindowDC();
		CRect ctlRect;
		m_ImageCtrl.GetWindowRect(&ctlRect);

		int x=0,y=0;
		int iImageWidth = m_pOrgan->RangeRight - m_pOrgan->RangeLeft;
		int iImageHeight = m_pOrgan->RangeBottom - m_pOrgan->RrangeTop;

        int iShowImageWidth = iImageWidth;
        int iShowImageHeight = iImageHeight;
        double dZoomOutScale = 1.00;    //缩小比例

		x = (ctlRect.Width() - iShowImageWidth)/2;
        if(x < 0)
        {
            x = 0;
            iShowImageWidth = ctlRect.Width();
            dZoomOutScale = (double)iShowImageWidth/(double)iImageWidth;
            iShowImageHeight = iImageHeight*dZoomOutScale;
        }

		y = (ctlRect.Height() -iShowImageHeight)/2;
        if(y < 0)
        {
            y = 0;
            iShowImageHeight = ctlRect.Height();
            dZoomOutScale = (double)iShowImageHeight/(double)iImageHeight;

            iShowImageWidth = iImageWidth*dZoomOutScale;
            x = (ctlRect.Width() - iShowImageWidth)/2;
        }

		m_pBodyImage->StretchBlt(dc.m_hDC,x+( m_pOrgan->left - m_pOrgan->RangeLeft)*dZoomOutScale,
            y+(m_pOrgan->top - m_pOrgan->RrangeTop)*dZoomOutScale,
				m_pBodyImage->GetWidth()*dZoomOutScale,m_pBodyImage->GetHeight()*dZoomOutScale,SRCCOPY);

		for(int i=0;i<4;i++)
		{
			if( m_pOrgan->m_pPadData[i] != NULL)
			{
				CImage *pPadImage = NULL;
				if(m_iImageIndex >= (int)m_pOrgan->m_pPadData[i]->m_ImageList.size())
				{
					pPadImage= m_pOrgan->m_pPadData[i]->m_ImageList.at(m_pOrgan->m_pPadData[i]->m_ImageList.size()-1);
				}
				else
				{
					pPadImage= m_pOrgan->m_pPadData[i]->m_ImageList.at(m_iImageIndex);
				}

				
				if( pPadImage != NULL && !pPadImage->IsNull())
				{
					pPadImage->StretchBlt(dc.m_hDC,x+( m_pOrgan->m_pPadData[i]->left - m_pOrgan->RangeLeft)*dZoomOutScale,
						y+(m_pOrgan->m_pPadData[i]->top - m_pOrgan->RrangeTop)*dZoomOutScale,
						pPadImage->GetWidth()*dZoomOutScale,pPadImage->GetHeight()*dZoomOutScale,SRCCOPY);
				}
			}
		}
	}
}

void COrganImage::OnBnClickedPre()
{
	m_iImageIndex --;
	m_iImageIndex = (m_iImageIndex<0)?0:m_iImageIndex;

	if(m_pOrgan->m_MainBodyImageList.size()>0)
	{
		m_pBodyImage = m_pOrgan->m_MainBodyImageList.at(m_iImageIndex);
		m_sImageNumInf.Format("%d/%d",m_iImageIndex+1,m_pOrgan->m_MainBodyImageList.size());
	}
	else
	{
		m_sImageNumInf = "0/0";
	}

	this->UpdateData(FALSE);
	this->Invalidate();
}

void COrganImage::OnBnClickedNext()
{
	m_iImageIndex ++;
	m_iImageIndex = (m_iImageIndex < (int)(m_pOrgan->m_MainBodyImageList.size()-1) )?m_iImageIndex:(m_pOrgan->m_MainBodyImageList.size()-1);

	if(m_pOrgan->m_MainBodyImageList.size()>0)
	{
		m_pBodyImage = m_pOrgan->m_MainBodyImageList.at(m_iImageIndex);
		m_sImageNumInf.Format("%d/%d",m_iImageIndex+1,m_pOrgan->m_MainBodyImageList.size());
	}
	else
	{
		m_sImageNumInf = "0/0";
	}
	UpdateData(FALSE);
	this->Invalidate();
}

void COrganImage::OnBnClickedClose()
{
	CDialog::OnCancel();
}

void COrganImage::OnBnClickedDelete()
{
	CImage *pImage = NULL;
	pImage = m_pOrgan->m_MainBodyImageList.at(m_iImageIndex);

	vector<CImage*>::iterator it  = m_pOrgan->m_MainBodyImageList.begin();
	int i = 0;
	for(;i<m_iImageIndex;i++,it++)
	{
		if(it == m_pOrgan->m_MainBodyImageList.end())
			break;
	}

	if(i == m_iImageIndex && it != m_pOrgan->m_MainBodyImageList.end())
	{
		pImage = (*it);
		pImage->Destroy();
		delete pImage;
		pImage = NULL;

		m_pOrgan->m_MainBodyImageList.erase(it);
		m_iImageIndex--;
		m_iImageIndex = (m_iImageIndex<0)?0:m_iImageIndex;

		g_pTemplateData->m_bDataIsChanged = true;
		g_pTemplateData->m_bOrganDataIsChange = true;
		m_pOrgan->bImagChanged = true;
	}

	if(m_pOrgan->m_MainBodyImageList.size()>0)
	{
		m_sImageNumInf.Format("%d/%d",m_iImageIndex+1,m_pOrgan->m_MainBodyImageList.size());
	}
	else
	{
		m_sImageNumInf = "0/0";
	}
	UpdateData(FALSE);
}
