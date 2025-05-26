// PrgWorkSure.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "PrgWorkSure.h"
#include "BigGraphView.h"


// CPrgWorkSure �Ի���

IMPLEMENT_DYNAMIC(CPrgWorkSure, CDialog)

CPrgWorkSure::CPrgWorkSure(CWnd* pParent /*=NULL*/)
	: CDialog(CPrgWorkSure::IDD, pParent)
{
	m_pBgiGraphView = pParent;
}

CPrgWorkSure::~CPrgWorkSure()
{
}

void CPrgWorkSure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrgWorkSure, CDialog)
	ON_BN_CLICKED(IDC_OK, &CPrgWorkSure::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CPrgWorkSure::OnBnClickedCancle)
END_MESSAGE_MAP()


// CPrgWorkSure ��Ϣ�������

void CPrgWorkSure::OnBnClickedOk()
{
	if(m_pBgiGraphView)
	{
		((CBigGraphView*)m_pBgiGraphView)->EndCurrentWork(false);
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
	}
}

void CPrgWorkSure::OnBnClickedCancle()
{
	if(m_pBgiGraphView)
	{
		((CBigGraphView*)m_pBgiGraphView)->CancelCurrentWork();
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
	}
}

BOOL CPrgWorkSure::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
