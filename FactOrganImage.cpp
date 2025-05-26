// FactOrganImage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "FactOrganImage.h"


// CFactOrganImage �Ի���

IMPLEMENT_DYNAMIC(CFactOrganImage, CDialog)

CFactOrganImage::CFactOrganImage(CWnd* pParent /*=NULL*/)
	: CDialog(CFactOrganImage::IDD, pParent)
{

}

CFactOrganImage::~CFactOrganImage()
{
}

void CFactOrganImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFactOrganImage, CDialog)
END_MESSAGE_MAP()


// CFactOrganImage ��Ϣ�������

void CFactOrganImage::ShowImage(const CImage& image)
{
	CClientDC dc( this);
	image.BitBlt(dc,0,0,image.GetWidth(),image.GetHeight(),0,0,SRCCOPY);
}

BOOL CFactOrganImage::OnInitDialog()
{
    CDialog::OnInitDialog();

    g_SetDialogStrings(this);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}
