// MotorCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "MotorCtrl.h"


// CMotorCtrl �Ի���

IMPLEMENT_DYNAMIC(CMotorCtrl, CDialog)

CMotorCtrl::CMotorCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CMotorCtrl::IDD, pParent)
    , m_iAxis(0)
	, m_lPos(0)
{

}

CMotorCtrl::~CMotorCtrl()
{
}

void CMotorCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_AXIS, m_iAxis);
	DDX_Text(pDX, IDC_POS, m_lPos);
	DDX_Control(pDX, IDC_AXIS, m_axisCtrl);
}


BEGIN_MESSAGE_MAP(CMotorCtrl, CDialog)
	ON_BN_CLICKED(IDC_GOHOME, &CMotorCtrl::OnBnClickedGohome)
	ON_BN_CLICKED(IDC_MOVE, &CMotorCtrl::OnBnClickedMove)
	ON_BN_CLICKED(IDC_CLOSE, &CMotorCtrl::OnBnClickedClose)
    ON_CBN_SELCHANGE(IDC_AXIS, &CMotorCtrl::OnCbnSelchangeAxis)
END_MESSAGE_MAP()


// CMotorCtrl ��Ϣ�������

void CMotorCtrl::OnBnClickedGohome()
{
	int iSpeed = g_iMotorSpeed;

    if(m_iAxis == 2)
    {
	    CString sMsgInfo = g_LoadString("IDS_TAKEOUTPCB_ADJUST_TRACKER","�ڵ���Z��֮ǰ������ȷ���Ѿ�����ȡ��");
        if(IDCANCEL == ::AfxMessageBox(sMsgInfo,MB_OKCANCEL))
        {
            return;
        }

		iSpeed = 50;
    }

    g_Motor.GoHome(m_iAxis+1,iSpeed,10);
}

void CMotorCtrl::OnBnClickedMove()
{
	this->UpdateData();

	int iSpeed = g_iMotorSpeed;

    if(m_iAxis == 2)
    {
	    CString sMsgInfo = g_LoadString("IDS_TAKEOUTPCB_ADJUST_TRACKER","�ڵ���Z��֮ǰ������ȷ���Ѿ�����ȡ��");
        if(IDCANCEL == ::AfxMessageBox(sMsgInfo,MB_OKCANCEL))
        {
            return;
        }

		iSpeed = 50;
    }
	
    g_Motor.AxisRunS(m_iAxis+1,m_lPos,iSpeed,1.00,0.001);
}

void CMotorCtrl::OnBnClickedClose()
{
	this->OnOK();
}

void CMotorCtrl::OnCbnSelchangeAxis()
{
    this->UpdateData();
    g_Motor.GetPos(m_iAxis+1,&m_lPos);

	if(m_iAxis != 2)
	{
		m_lPos = m_lPos*4;
	}

    this->UpdateData(FALSE);
}

BOOL CMotorCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);

    CString sAxis = g_LoadString("IDS_X_AXIS","X-��");
    m_axisCtrl.InsertString(-1,sAxis);

    sAxis = g_LoadString("IDS_Y_AXIS","Y-��");
    m_axisCtrl.InsertString(-1,sAxis);

    sAxis = g_LoadString("IDS_Z_AXIS","Z-��");
    m_axisCtrl.InsertString(-1,sAxis);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
