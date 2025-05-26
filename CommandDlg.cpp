// CommandDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "CommandDlg.h"
#include "MainView.h"
#include "BigGraphView.h"


// CCommandDlg �Ի���

IMPLEMENT_DYNAMIC(CCommandDlg, CDialog)

CCommandDlg::CCommandDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommandDlg::IDD, pParent)
	, m_iCenterX(0)
	, m_iCenterY(0)
	, m_iTrackerWidth(0)
	, m_iTrackerHeight(0)
	, m_sStatus(_T(""))
	, m_sMouseP(_T(""))
	, m_sInfo(_T(""))
{
	m_pBigGraphView = NULL;
}

CCommandDlg::~CCommandDlg()
{
}

void CCommandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CENTERX, m_iCenterX);
	DDX_Text(pDX, IDC_CENTERY, m_iCenterY);
	DDX_Text(pDX, IDC_CENTERW, m_iTrackerWidth);
	DDX_Text(pDX, IDC_CENTERH, m_iTrackerHeight);
	DDX_Text(pDX, IDC_STATUS, m_sStatus);
	DDX_Text(pDX, IDC_MOUSEINFO, m_sMouseP);
	DDX_Text(pDX, IDC_INFO, m_sInfo);
}


BEGIN_MESSAGE_MAP(CCommandDlg, CDialog)		
	ON_BN_CLICKED(IDC_CENTERXDD, &CCommandDlg::OnBnClickedCenterxdd)
	ON_BN_CLICKED(IDC_CENTERXD, &CCommandDlg::OnBnClickedCenterxd)
	ON_BN_CLICKED(IDC_CENTERYAA, &CCommandDlg::OnBnClickedCenteryaa)
	ON_BN_CLICKED(IDC_CENTERYA, &CCommandDlg::OnBnClickedCenterya)
	ON_BN_CLICKED(IDC_CENTERYDD, &CCommandDlg::OnBnClickedCenterydd)
	ON_BN_CLICKED(IDC_CENTERYD, &CCommandDlg::OnBnClickedCenteryd)
	ON_BN_CLICKED(IDC_CENTERWAA, &CCommandDlg::OnBnClickedCenterwaa)
	ON_BN_CLICKED(IDC_CENTERWA, &CCommandDlg::OnBnClickedCenterwa)
	ON_BN_CLICKED(IDC_CENTERWDD, &CCommandDlg::OnBnClickedCenterwdd)
	ON_BN_CLICKED(IDC_CENTERWD, &CCommandDlg::OnBnClickedCenterwd)
	ON_BN_CLICKED(IDC_CENTERHAA, &CCommandDlg::OnBnClickedCenterhaa)
	ON_BN_CLICKED(IDC_CENTERHA, &CCommandDlg::OnBnClickedCenterha)
	ON_BN_CLICKED(IDC_CENTERHDD, &CCommandDlg::OnBnClickedCenterhdd)
	ON_BN_CLICKED(IDC_CENTERHD, &CCommandDlg::OnBnClickedCenterhd)
	ON_EN_CHANGE(IDC_CENTERX, &CCommandDlg::OnEnChangeCenterx)
	ON_EN_CHANGE(IDC_CENTERY, &CCommandDlg::OnEnChangeCentery)
	ON_EN_CHANGE(IDC_CENTERW, &CCommandDlg::OnEnChangeCenterw)
	ON_EN_CHANGE(IDC_CENTERH, &CCommandDlg::OnEnChangeCenterh)
	ON_BN_CLICKED(IDC_CENTERXAA, &CCommandDlg::OnBnClickedCenterxaa)
	ON_BN_CLICKED(IDC_CENTERXA, &CCommandDlg::OnBnClickedCenterxa)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCommandDlg ��Ϣ�������


BOOL CCommandDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE)
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CCommandDlg::NotifyPrgDataChanged(void)
{
}

//����X++
void CCommandDlg::OnBnClickedCenterxaa()
{
	this->CenterXAdd(10);
}

//����X+
void CCommandDlg::OnBnClickedCenterxa()
{
	this->CenterXAdd(1);
}

//����X--
void CCommandDlg::OnBnClickedCenterxdd()
{
	this->CenterXAdd(-10);
}

//����X-
void CCommandDlg::OnBnClickedCenterxd()
{
	this->CenterXAdd(-1);
}

//����Y++
void CCommandDlg::OnBnClickedCenteryaa()
{
	this->CenterYAdd(10);
}

//����Y+
void CCommandDlg::OnBnClickedCenterya()
{
	this->CenterYAdd(1);
}

//����Y--
void CCommandDlg::OnBnClickedCenterydd()
{
	this->CenterYAdd(-10);
}

//����Y-
void CCommandDlg::OnBnClickedCenteryd()
{
	this->CenterYAdd(-1);
}

//���++
void CCommandDlg::OnBnClickedCenterwaa()
{
	this->TrackerWidthAdd(20);
}

//���+
void CCommandDlg::OnBnClickedCenterwa()
{
	this->TrackerWidthAdd(2);
}

//���--
void CCommandDlg::OnBnClickedCenterwdd()
{
	this->TrackerWidthAdd(-20);
}

//���-
void CCommandDlg::OnBnClickedCenterwd()
{
	this->TrackerWidthAdd(-2);
}

//�߶�++
void CCommandDlg::OnBnClickedCenterhaa()
{
	this->TrackerHeightAdd(20);
}

//�߶�+
void CCommandDlg::OnBnClickedCenterha()
{
	this->TrackerHeightAdd(2);
}

//�߶�--
void CCommandDlg::OnBnClickedCenterhdd()
{
	this->TrackerHeightAdd(-20);
}

//�߶�-
void CCommandDlg::OnBnClickedCenterhd()
{
	this->TrackerHeightAdd(-2);
}

//�޸�����X
void CCommandDlg::OnEnChangeCenterx()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::WaitForSingleObject(m_hEditEvent,500);
	
	if(this->UpdateData(TRUE))
		this->CenterXModify(m_iCenterX);

	::SetEvent(m_hEditEvent);
}

//�޸�����Y
void CCommandDlg::OnEnChangeCentery()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::WaitForSingleObject(m_hEditEvent,500);
	
	if(this->UpdateData(TRUE))
		this->CenterYModify(m_iCenterY);

	::SetEvent(m_hEditEvent);
}

//�޸Ŀ��
void CCommandDlg::OnEnChangeCenterw()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::WaitForSingleObject(m_hEditEvent,500);
	
	if(this->UpdateData(TRUE))
		this->TrackerWidthModify(m_iTrackerWidth);

	::SetEvent(m_hEditEvent);
	
}

//�޸ĸ߶�
void CCommandDlg::OnEnChangeCenterh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::WaitForSingleObject(m_hEditEvent,500);
	
	if(this->UpdateData(TRUE))
		this->TrackerHeightModify(m_iTrackerHeight);

	::SetEvent(m_hEditEvent);

}



BOOL CCommandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);

	m_hEditEvent = ::CreateEvent(NULL,FALSE,TRUE,NULL);
	this->SetTimer(1,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCommandDlg::OnTimer(UINT_PTR nIDEvent)
{
	//��ֹ�ڱ༭ʱ������ͻ
	if(WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEditEvent,50))
	{
		CDialog::OnTimer(nIDEvent);
		return;
	}

	::SetEvent(m_hEditEvent);
	GetPrgData();

	CDialog::OnTimer(nIDEvent);
}

// ��������Ϣ
void CCommandDlg::GetPrgData(void)
{
	if(m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	CPoint MousePoint = ((CBigGraphView*)m_pBigGraphView)->m_MousePoint;
	m_sMouseP.Format("x:%d,y:%d",MousePoint.x,MousePoint.y);
	
	CWnd *pCenterWaa = GetDlgItem(IDC_CENTERWAA);
	CWnd *pCenterWa = GetDlgItem(IDC_CENTERWA);
	CWnd *pCenterWdd = GetDlgItem(IDC_CENTERWDD);
	CWnd *pCenterWd = GetDlgItem(IDC_CENTERWD);
	CWnd *pCenterHaa = GetDlgItem(IDC_CENTERHAA);
	CWnd *pCenterHa = GetDlgItem(IDC_CENTERHA);
	CWnd *pCenterHdd = GetDlgItem(IDC_CENTERHDD);
	CWnd *pCenterHd = GetDlgItem(IDC_CENTERHD);

	CWnd *pCenterW = GetDlgItem(IDC_CENTERW);
	CWnd *pCenterH = GetDlgItem(IDC_CENTERH);

	if(((CBigGraphView*)m_pBigGraphView)->m_CurrentWorkStep == EDITFOVAREA)
	{
		pCenterWaa->EnableWindow(FALSE);
		pCenterWa->EnableWindow(FALSE);
		pCenterWdd->EnableWindow(FALSE);
		pCenterWd->EnableWindow(FALSE);

		pCenterHaa->EnableWindow(FALSE);
		pCenterHa->EnableWindow(FALSE);
		pCenterHdd->EnableWindow(FALSE);
		pCenterHd->EnableWindow(FALSE);

		pCenterW->EnableWindow(FALSE);
		pCenterH->EnableWindow(FALSE);
		
	}
	else
	{
		pCenterWaa->EnableWindow(TRUE);
		pCenterWa->EnableWindow(TRUE);
		pCenterWdd->EnableWindow(TRUE);
		pCenterWd->EnableWindow(TRUE);

		pCenterHaa->EnableWindow(TRUE);
		pCenterHa->EnableWindow(TRUE);
		pCenterHdd->EnableWindow(TRUE);
		pCenterHd->EnableWindow(TRUE);

		pCenterW->EnableWindow(TRUE);
		pCenterH->EnableWindow(TRUE);
	}

	switch(((CBigGraphView*)m_pBigGraphView)->m_CurrentWorkStep)
	{
	case NONE:
		m_sStatus = ::g_LoadString("IDS_WORK_NONE","������...");
		break;
	case LOADPCB:
		m_sStatus = ::g_LoadString("IDS_WORK_LOADPCB","���ڼ���PCB��ͼ��");
		break;
	case EDITAREA:
		m_sStatus = ::g_LoadString("IDS_WORK_EDITAREA","���ڱ�̱༭����");
		break;
	case EDITFOVAREA:
		m_sStatus = ::g_LoadString("IDS_WORK_EDITFOVAREA","���ڱ��FOV����");
		break;
	case LOADFOV:
		m_sStatus = ::g_LoadString("IDS_WORK_LOADFOV","���ڼ���FOVͼ��");
		break;
	case DATUMMARK:
		m_sStatus = ::g_LoadString("IDS_WORK_DATUMMARK","���ڱ�̻�׼��");
		break;
	case COORDINATEMAP:
		m_sStatus = ::g_LoadString("IDS_WORK_COORDINATEMAP","���ڼ����������ֵ");
		break;
	case ORGAN:
		m_sStatus = ::g_LoadString("IDS_WORK_ORGAN","���ڱ��Ԫ��");
		break;
	case BAR:
		m_sStatus = ::g_LoadString("IDS_WORK_BAR","���ڱ��������");
		break;
	case SPELLPCB:
		m_sStatus = ::g_LoadString("IDS_WORK_SPELLPCB","���ڱ��ƴ��");
		break;
	default:
		break;
	}

	this->UpdateData(FALSE);
}

void CCommandDlg::SetBigGraphView(CWnd* pBigView)
{
	m_pBigGraphView = pBigView;
}

// ����X������
void CCommandDlg::CenterXAdd(int iAdd)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		m_iCenterX = pTracker->m_rect.CenterPoint().x;
		m_iCenterX += iAdd;

		this->CenterXModify(m_iCenterX);
		this->UpdateData(FALSE);
	}
}

// ����Y������
void CCommandDlg::CenterYAdd(int iAdd)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		m_iCenterY = pTracker->m_rect.CenterPoint().y;
		m_iCenterY += iAdd;

		this->CenterYModify(m_iCenterY);
		this->UpdateData(FALSE);
	}
}

void CCommandDlg::CenterXModify(int iNewValue)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	if(iNewValue<=0)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iWidth=pTracker->m_rect.Width();
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.left = iNewValue - iWidth/2;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.right = iNewValue + iWidth/2;

		((CBigGraphView*)m_pBigGraphView)->Invalidate();
	}
}

void CCommandDlg::CenterYModify(int iNewValue)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	if(iNewValue<=0)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iHeight=pTracker->m_rect.Height();
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.top = iNewValue - iHeight/2;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.bottom = iNewValue + iHeight/2;

		((CBigGraphView*)m_pBigGraphView)->Invalidate();
	}
}

void CCommandDlg::TrackerWidthAdd(int iAdd)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iWidth=pTracker->m_rect.Width();
		iWidth += iAdd;
		if( iWidth<=0)
			return;

		m_iTrackerWidth =iWidth;

		this->TrackerWidthModify(m_iTrackerWidth);
		this->UpdateData(FALSE);
	}
}

void CCommandDlg::TrackerHeightAdd(int iAdd)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iHeight=pTracker->m_rect.Height();
		iHeight += iAdd;
		if( iHeight<=0)
			return;

		m_iTrackerHeight =iHeight;

		this->TrackerHeightModify(m_iTrackerHeight);
		this->UpdateData(FALSE);
	}
}

void CCommandDlg::TrackerWidthModify(int iNewValue)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	if(iNewValue<=0)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iCenterX=pTracker->m_rect.CenterPoint().x;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.left = iCenterX - iNewValue/2;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.right = iCenterX + iNewValue/2;

		((CBigGraphView*)m_pBigGraphView)->Invalidate();
	}
}

void CCommandDlg::TrackerHeightModify(int iNewValue)
{
	if( m_pBigGraphView == NULL || m_pBigGraphView->m_hWnd == NULL)
		return;

	if(iNewValue<=0)
		return;

	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		int iCenterY=pTracker->m_rect.CenterPoint().y;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.top = iCenterY - iNewValue/2;
		((CBigGraphView*)m_pBigGraphView)->m_pRectTracker->m_rect.bottom = iCenterY + iNewValue/2;

		((CBigGraphView*)m_pBigGraphView)->Invalidate();
	}
}

// ��Ƥ���Ѿ��޸�
void CCommandDlg::NotifyTrakerChanged(void)
{
	CRectTracker *pTracker = ((CBigGraphView*)m_pBigGraphView)->m_pRectTracker;
	if( pTracker != NULL)
	{
		m_iCenterX = pTracker->m_rect.CenterPoint().x;
		m_iCenterY = pTracker->m_rect.CenterPoint().y;
		m_iTrackerWidth = pTracker->m_rect.Width();
		m_iTrackerHeight = pTracker->m_rect.Height();		
	}

	this->UpdateData(FALSE);
}
