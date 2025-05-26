// DetectBassInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectBassInfo.h"
#include "DetectBigGraphView.h"


// CDetectBassInfo 对话框

IMPLEMENT_DYNAMIC(CDetectBassInfo, CDialog)

CDetectBassInfo::CDetectBassInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectBassInfo::IDD, pParent)
	, m_lCurrentId(0)
	, m_sCurrentBarCode(_T(""))
	, m_iPcbNum(0)
	, m_iErrPcbNum(0)
	, m_iOrganNum(0)
	, m_iErrOganNum(0)
	, m_sStarTime(_T(""))
	, m_lStartId(0)
	, m_bAskWhenErr(TRUE)
{

}

CDetectBassInfo::~CDetectBassInfo()
{
}

void CDetectBassInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CURRENT_ID, m_lCurrentId);
	DDX_Text(pDX, IDC_CURRENT_BARCODE, m_sCurrentBarCode);
	DDX_Text(pDX, IDC_DET_PCBNUM, m_iPcbNum);
	DDX_Text(pDX, IDC_ERR_PCBNUM, m_iErrPcbNum);
	DDX_Text(pDX, IDC_DET_ORGANNUM, m_iOrganNum);
	DDX_Text(pDX, IDC_ERR_ORGANNUM, m_iErrOganNum);
	DDX_Text(pDX, IDC_STARTTIME, m_sStarTime);
	DDX_Text(pDX, IDC_STARTID, m_lStartId);
	DDX_Check(pDX, IDC_ASKWHENERR, m_bAskWhenErr);
}


BEGIN_MESSAGE_MAP(CDetectBassInfo, CDialog)
	ON_BN_CLICKED(IDC_ASKWHENERR, &CDetectBassInfo::OnBnClickedAskwhenerr)
END_MESSAGE_MAP()


// CDetectBassInfo 消息处理程序
void CDetectBassInfo::NotifyDetectDataChanged(void)
{
	CDetectBigGraphView::DETECTINFO dtBaseInfo = ((CDetectBigGraphView*)m_pDBigView)->m_DetectBaseInfo;
	m_lCurrentId = dtBaseInfo.lCurrentId;
	m_sCurrentBarCode = dtBaseInfo.sCurrentBarCode;
	m_iPcbNum = dtBaseInfo.iDetPcbNum;
	m_iErrPcbNum = dtBaseInfo.iErrPcbNum;
	m_iOrganNum = dtBaseInfo.iDetOrganNum;
	m_iErrOganNum = dtBaseInfo.iErrOrganNum;
	m_sStarTime = dtBaseInfo.sStartTime;
	m_lStartId = dtBaseInfo.lStartId;
	
	this->UpdateData(FALSE);
}

BOOL CDetectBassInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->UpdateData(FALSE);

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDetectBassInfo::OnBnClickedAskwhenerr()
{
	this->UpdateData(TRUE);
	((CDetectBigGraphView*)m_pDBigView)->m_bAskWhenErr = m_bAskWhenErr;
}
