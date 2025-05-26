// PCBInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "PCBInfoDlg.h"


// CPCBInfoDlg 对话框

IMPLEMENT_DYNAMIC(CPCBInfoDlg, CDialog)

CPCBInfoDlg::CPCBInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPCBInfoDlg::IDD, pParent)
	, m_sPCBName(_T(""))
	, m_lPCBWidth(0)
	, m_lPCBHeight(0)
	, m_sLastPrgTime(_T(""))
	, m_sLastPrgStep(_T(""))
	, m_iFovNum(0)
	, m_iOrganNum(0)
	, m_iMarkNum(0)
	, m_sTemplateType(_T(""))
	, m_dJiaoDuX(0)
	, m_dJiaoDuY(0)
{
	m_pTemplateData = NULL;
}

CPCBInfoDlg::~CPCBInfoDlg()
{
}

void CPCBInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PCBNAME, m_sPCBName);
	DDX_Text(pDX, IDC_WIDTH, m_lPCBWidth);
	DDV_MinMaxLong(pDX, m_lPCBWidth, 0, 99999999);
	DDX_Text(pDX, IDC_HEIGHT, m_lPCBHeight);
	DDV_MinMaxLong(pDX, m_lPCBHeight, 0, 99999999);
	DDX_Text(pDX, IDC_LASTPRGTIME, m_sLastPrgTime);
	DDX_Text(pDX, IDC_LASTPRGSTEP, m_sLastPrgStep);
	DDX_Text(pDX, IDC_FOVNUM, m_iFovNum);
	DDX_Text(pDX, IDC_ORGANNUM, m_iOrganNum);
	DDX_Text(pDX, IDC_MARKNUM, m_iMarkNum);
	DDX_CBString(pDX, IDC_TEMPTYPE, m_sTemplateType);
	DDX_Text(pDX, IDC_JIAODU_X, m_dJiaoDuX);
	DDX_Text(pDX, IDC_JIAODU_Y, m_dJiaoDuY);
}


BEGIN_MESSAGE_MAP(CPCBInfoDlg, CDialog)

	ON_EN_KILLFOCUS(IDC_PCBNAME, &CPCBInfoDlg::OnEnKillfocusPcbname)
	ON_EN_KILLFOCUS(IDC_WIDTH, &CPCBInfoDlg::OnEnKillfocusWidth)
	ON_EN_KILLFOCUS(IDC_HEIGHT, &CPCBInfoDlg::OnEnKillfocusHeight)
	ON_EN_CHANGE(IDC_HEIGHT, &CPCBInfoDlg::OnEnChangeHeight)
	ON_CBN_SELCHANGE(IDC_TEMPTYPE, &CPCBInfoDlg::OnCbnSelchangeTemptype)
	ON_EN_CHANGE(IDC_WIDTH, &CPCBInfoDlg::OnEnChangeWidth)
END_MESSAGE_MAP()


// CPCBInfoDlg 消息处理程序

BOOL CPCBInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPCBInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	if(pTemplateData == NULL)
		return;

	m_sPCBName = pTemplateData->m_templateName;
	m_lPCBWidth = pTemplateData->m_width;
	m_lPCBHeight = pTemplateData->m_height;
	m_dJiaoDuX = pTemplateData->m_dJiaoDuX;
	m_dJiaoDuY = pTemplateData->m_dJiaoDuY;

	m_sLastPrgTime = pTemplateData->m_sLastModifyTime;

	m_sTemplateType = pTemplateData->m_sTemplateType;
	
	switch(pTemplateData->m_LastWorkStep)
	{
	case NONE:
		m_sLastPrgStep = ::g_LoadString("IDS_NONE","无");
		break;
	case LOADPCB:
		m_sLastPrgStep = ::g_LoadString("IDS_LOADPCB","加载PCB");
		break;
	case EDITAREA:
		m_sLastPrgStep = ::g_LoadString("IDS_EDITAREA","编辑区域");
		break;
	case EDITFOVAREA:
		m_sLastPrgStep = ::g_LoadString("IDS_EDITFOVAREA","编辑FOV区域");
		break;
	case LOADFOV:
		m_sLastPrgStep = ::g_LoadString("IDS_LOADFOV","加载FOV");
		break;
	case DATUMMARK:
		m_sLastPrgStep = ::g_LoadString("IDS_PRGDATUMMARK","编辑基准点");
		break;
	case COORDINATEMAP:
		m_sLastPrgStep = ::g_LoadString("IDS_PRGCOORDINATEMAP","坐标映射");
		break;
	case ORGAN:
		m_sLastPrgStep = ::g_LoadString("IDS_PRGORGAN","元件编程");
		break;
	case BAR:
		m_sLastPrgStep = ::g_LoadString("IDS_PRGBAR","条形码编程");
		break;
	case SPELLPCB:
		m_sLastPrgStep = ::g_LoadString("IDS_PRGSPELLPCB","拼板编程");
		break;
	default:
		m_sLastPrgStep = ::g_LoadString("IDS_NONE","无");
		break;
	}

	m_iFovNum = pTemplateData->m_FOVMap.size();
	m_iOrganNum = pTemplateData->m_OrganDataMap.size();
	m_iMarkNum = pTemplateData->m_DatumMarkMap.size();

	this->UpdateData(FALSE);
}


void CPCBInfoDlg::OnEnKillfocusPcbname()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sOldName = m_sPCBName;
	UpdateData(TRUE);
	if(sOldName != m_sPCBName)
	{
		m_pTemplateData->m_templateName = m_sPCBName;
		m_pTemplateData->m_bDataIsChanged = true;
	}
}

void CPCBInfoDlg::OnEnKillfocusWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	long lOldPCBWidth = m_lPCBWidth;
	UpdateData(TRUE);
	if(lOldPCBWidth != m_lPCBWidth)
	{
		m_pTemplateData->m_width =m_lPCBWidth;
		m_pTemplateData->m_bDataIsChanged = true;
	}
}

void CPCBInfoDlg::OnEnKillfocusHeight()
{
	// TODO: 在此添加控件通知处理程序代码
	//long lOldPCBHeight = m_lPCBHeight;
	UpdateData(TRUE);
	if(m_lOldPCBHeight != m_lPCBHeight && m_lOldPCBHeight !=0)
	{
		CString sMsgInfo = g_LoadString("IDS_ARE_ADJUST_TRACKER","板跟轨道宽可能不一致，如要调整，请先取出板再按“确定”按钮，如不调整按“取消”按钮");
		if(IDOK == ::AfxMessageBox(sMsgInfo,MB_OKCANCEL))
		{
			g_Motor.AdjustTrackerHeight(m_lPCBHeight);
		}

		m_pTemplateData->m_height = m_lPCBHeight;
		m_pTemplateData->m_bDataIsChanged = true;
	}
	m_lOldPCBHeight = m_lPCBHeight;
}

void CPCBInfoDlg::OnEnChangeHeight()
{
	m_lOldPCBHeight = m_lPCBHeight;

	UpdateData(TRUE);
	m_pTemplateData->m_height = m_lPCBHeight;
	m_pTemplateData->m_bDataIsChanged = true;
}

void CPCBInfoDlg::OnCbnSelchangeTemptype()
{
	UpdateData(TRUE);
	if(m_sTemplateType != m_pTemplateData->m_sTemplateType)
	{
		m_pTemplateData->m_sTemplateType = m_sTemplateType;
		m_pTemplateData->m_bDataIsChanged = true;
	}

}

void CPCBInfoDlg::OnEnChangeWidth()
{
	UpdateData(TRUE);
	m_pTemplateData->m_width =m_lPCBWidth;
	m_pTemplateData->m_bDataIsChanged = true;
}
