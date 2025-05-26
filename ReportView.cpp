// ReportView.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "ReportView.h"
#include "UserMgr.h"
#include "TemplateDataMgr.h"
#include "DetectDataMgr.h"
#include "ReportStatDlg.h"

// CReportView

IMPLEMENT_DYNCREATE(CReportView, CFormView)

CReportView::CReportView()
	: CFormView(CReportView::IDD)
	, m_sPCBName(_T(""))
	, m_sTester(_T(""))
	, m_sRelative(_T(""))
	, m_iErrorCount(0)
	, m_iStartId(0)
	, m_iEndId(0)
	, m_sStartBarNo(_T(""))
	, m_sEndBarNo(_T(""))
{
	m_tStartDate = CTime::GetCurrentTime();
	m_tEndDate = CTime::GetCurrentTime();
	m_tStartTime = CTime::GetCurrentTime();
	m_tEndTime = CTime::GetCurrentTime();
	
}

CReportView::~CReportView()
{
	vector<CDetectMasterData*>::iterator it = m_MasterDataList.begin();
	for(;it!=m_MasterDataList.end();it++)
	{
		CDetectMasterData* pData = (*it);
		delete pData;
		pData = NULL;
	}
	m_MasterDataList.clear();
}

void CReportView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_STAR_TIME, m_tStartTime);
	DDX_DateTimeCtrl(pDX, IDC_STAR_DATE, m_tStartDate);
	DDX_DateTimeCtrl(pDX, IDC_END_DATE, m_tEndDate);
	DDX_DateTimeCtrl(pDX, IDC_END_TIME, m_tEndTime);
	DDX_CBString(pDX, IDC_PCBNAME, m_sPCBName);
	DDX_CBString(pDX, IDC_TESTER, m_sTester);
	DDX_CBString(pDX, IDC_RELATIVE, m_sRelative);
	DDX_Text(pDX, IDC_ERRCOUNT, m_iErrorCount);
	DDX_Text(pDX, IDC_START_ID, m_iStartId);
	DDX_Text(pDX, IDC_END_ID, m_iEndId);
	DDX_Text(pDX, IDC_START_BARNO, m_sStartBarNo);
	DDX_Text(pDX, IDC_ENDBARNO, m_sEndBarNo);
	DDX_Control(pDX, IDC_PCBNAME, m_PcbNameCtl);
	DDX_Control(pDX, IDC_TESTER, m_TesterCtl);	
	DDX_Control(pDX, IDC_DATE_CHECK, m_DateChecker);
	DDX_Control(pDX, IDC_TIME_CHECK, m_TimeChecker);
	DDX_Control(pDX, IDC_STAR_DATE, m_StartDateCtrl);
	DDX_Control(pDX, IDC_END_DATE, m_EndDateCtrl);
	DDX_Control(pDX, IDC_STAR_TIME, m_StartTimeCtrl);
	DDX_Control(pDX, IDC_END_TIME, m_EndTimeCtrl);
	DDV_MinMaxInt(pDX, m_iErrorCount, 0, INT_MAX);
	DDX_Control(pDX, IDC_MASTERLIST, m_MasterListCtl);
	DDX_Control(pDX, IDC_DETAILLIST, m_DetailListCtl);
}

BEGIN_MESSAGE_MAP(CReportView, CFormView)
	ON_BN_CLICKED(IDC_DATE_CHECK, &CReportView::OnBnClickedDateCheck)
	ON_BN_CLICKED(IDC_TIME_CHECK, &CReportView::OnBnClickedTimeCheck)
	ON_BN_CLICKED(IDC_QUERY, &CReportView::OnBnClickedQuery)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MASTERLIST, &CReportView::OnLvnItemchangedMasterlist)
	ON_BN_CLICKED(IDC_STAT, &CReportView::OnBnClickedStat)
END_MESSAGE_MAP()


// CReportView 诊断

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CReportView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CReportView 消息处理程序

void CReportView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	::g_SetDialogStrings(this);

	//pcbName
	m_PcbNameCtl.InsertString(m_PcbNameCtl.GetCount(),"All");
	vector<CTemplateData*> templateList;
	CTemplateDataMgr templateMgr;
	templateMgr.GetAllTemplate(templateList);
	vector<CTemplateData*>::iterator templateIt=templateList.begin();
	for(;templateIt!=templateList.end();templateIt++)
	{
		m_PcbNameCtl.InsertString(m_PcbNameCtl.GetCount(),(*templateIt)->m_templateName);
	}

	//tester
	m_TesterCtl.InsertString(m_TesterCtl.GetCount(),"All");
	vector<CUserInf*> userList;
	CUserMgr	userMgr;
	userMgr.GetAllUsers(userList);
	vector<CUserInf*>::iterator userIt=userList.begin();
	for(;userIt != userList.end();userIt++)
	{
		m_TesterCtl.InsertString(m_TesterCtl.GetCount(),(*userIt)->userName);
	}

	m_DateChecker.SetCheck(BST_CHECKED);
	m_TimeChecker.SetCheck(BST_CHECKED);

	CString sItemName;
	ListView_SetExtendedListViewStyle(m_MasterListCtl.m_hWnd,LVS_EX_FULLROWSELECT);

	sItemName=::g_LoadString("IDS_PCBNAME","板名");
	m_MasterListCtl.InsertColumn(0,sItemName,LVCFMT_LEFT,150);

	sItemName=::g_LoadString("IDS_ID","流水号");
	m_MasterListCtl.InsertColumn(1,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_BARCODENO","条码号");
	m_MasterListCtl.InsertColumn(2,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_DETECTDATE","检测日期");
	m_MasterListCtl.InsertColumn(3,sItemName,LVCFMT_LEFT,150);

	sItemName=::g_LoadString("IDS_DETECTTIME","检测时间");
	m_MasterListCtl.InsertColumn(4,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_DETECTER","检测员");
	m_MasterListCtl.InsertColumn(5,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_ORGAN_NUM","元件数");
	m_MasterListCtl.InsertColumn(6,sItemName,LVCFMT_LEFT,70);

	sItemName=::g_LoadString("IDS_ERROR_NUM","错误数");
	m_MasterListCtl.InsertColumn(7,sItemName,LVCFMT_LEFT,70);

	ListView_SetExtendedListViewStyle(m_DetailListCtl.m_hWnd,LVS_EX_FULLROWSELECT);

	sItemName=::g_LoadString("IDS_ID","流水号");
	m_DetailListCtl.InsertColumn(0,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_ORGAN_NO","元件号");
	m_DetailListCtl.InsertColumn(1,sItemName,LVCFMT_LEFT,100);

	m_DetailListCtl.InsertColumn(2,_T("X"),LVCFMT_LEFT,100);
	m_DetailListCtl.InsertColumn(3,_T("Y"),LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_ISPASS","是否通过");
	m_DetailListCtl.InsertColumn(4,sItemName,LVCFMT_LEFT,100);

	sItemName=::g_LoadString("IDS_ERRORTYPE","错误类型");
	m_DetailListCtl.InsertColumn(5,sItemName,LVCFMT_LEFT,150);

	sItemName=::g_LoadString("IDS_KINDRED_VAL","相似值");
	m_DetailListCtl.InsertColumn(6,sItemName,LVCFMT_LEFT,70);

	sItemName=::g_LoadString("IDS_LIMIT_VAL","阀值");
	m_DetailListCtl.InsertColumn(7,sItemName,LVCFMT_LEFT,70);
}

void CReportView::OnBnClickedDateCheck()
{
	int iCheck=m_DateChecker.GetCheck();
	if(BST_UNCHECKED == iCheck)
	{
		m_StartDateCtrl.EnableWindow(FALSE);
		m_EndDateCtrl.EnableWindow(FALSE);
	}
	else if(BST_CHECKED == iCheck)
	{
		m_StartDateCtrl.EnableWindow(TRUE);
		m_EndDateCtrl.EnableWindow(TRUE);
	}
}

void CReportView::OnBnClickedTimeCheck()
{
	int iCheck=m_TimeChecker.GetCheck();
	if(BST_UNCHECKED == iCheck)
	{
		m_StartTimeCtrl.EnableWindow(FALSE);
		m_EndTimeCtrl.EnableWindow(FALSE);
	}
	else if(BST_CHECKED == iCheck)
	{
		m_StartTimeCtrl.EnableWindow(TRUE);
		m_EndTimeCtrl.EnableWindow(TRUE);
	}
}

void CReportView::OnBnClickedQuery()
{
	this->UpdateData(TRUE);

	CString	sCondition=" where 1=1 ";
	CString sTemp="";

	int iCheck=m_DateChecker.GetCheck();
	if(BST_CHECKED == iCheck)
	{
		sCondition +=" and DetectData>='";
		sTemp = m_tStartDate.Format("%Y-%m-%d");
		sCondition += sTemp;

		sCondition +="' and DetectData<='";		
		sTemp = m_tEndDate.Format("%Y-%m-%d");
		sCondition += sTemp;
		sCondition +="'";
	}

	iCheck=m_TimeChecker.GetCheck();
	if(BST_CHECKED == iCheck)
	{
		sCondition +=" and DetectTime>='";
		sTemp = m_tStartTime.Format("%H:%M");
		sCondition += sTemp;

		sCondition +="' and DetectTime<='";
		sTemp = m_tEndTime.Format("%H:%M");
		sCondition += sTemp;
		sCondition +="'";
	}

	if(m_sPCBName != "" && m_sPCBName !="All")
	{
		sCondition +=" and pcbName ='" +m_sPCBName +"'";
	}

	if(m_sTester != "" && m_sTester !="All")
	{
		sCondition +=" and Detecter ='" +m_sTester +"'";
	}

	if(m_sRelative !="" && m_iErrorCount !=0)
	{
		sTemp.Format("%d",m_iErrorCount);

		sCondition += " and ErrorCount"+m_sRelative+ sTemp;
	}

	if(m_iStartId != 0)
	{
		sTemp.Format("%d",m_iStartId);

		sCondition += " and id>=" +sTemp;
	}

	if(m_iEndId != 0)
	{
		sTemp.Format("%d",m_iEndId);

		sCondition += " and id<=" +sTemp;
	}

	if(m_sStartBarNo != "")
	{
		sCondition += " and barNo>='" +m_sStartBarNo + "'";
	}

	if(m_sEndBarNo != "")
	{
		sCondition += " and barNo<='" +m_sEndBarNo + "'";
	}

	vector<CDetectMasterData*>::iterator it = m_MasterDataList.begin();
	for(;it!=m_MasterDataList.end();it++)
	{
		CDetectMasterData* pData = (*it);
		delete pData;
		pData = NULL;
	}
	m_MasterDataList.clear();

	CDetectDataMgr	DetectDataMgr;
	DetectDataMgr.GetDetectDataByWhere(sCondition,m_MasterDataList);

	InitList();
}

void CReportView::InitList(void)
{
	m_MasterListCtl.DeleteAllItems();
	m_DetailListCtl.DeleteAllItems();

	vector<CDetectMasterData*>::iterator it = m_MasterDataList.begin();
	for(;it!=m_MasterDataList.end();it++)
	{
		InsObjToList((*it));
	}
}

void CReportView::InsObjToList(CDetectMasterData* pDetectMasterData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_MasterListCtl.GetItemCount();

	int iRow = m_MasterListCtl.InsertItem(&lvItem);
	m_MasterListCtl.SetItemText(iRow,0,pDetectMasterData->pcbName);

	sTmp.Format("%d",pDetectMasterData->id);
	m_MasterListCtl.SetItemText(iRow,1,sTmp);

	m_MasterListCtl.SetItemText(iRow,2,pDetectMasterData->barNo);
	m_MasterListCtl.SetItemText(iRow,3,pDetectMasterData->DetectData);
	m_MasterListCtl.SetItemText(iRow,4,pDetectMasterData->DetectTime);
	m_MasterListCtl.SetItemText(iRow,5,pDetectMasterData->Detecter);

	sTmp.Format("%d",pDetectMasterData->DetectCount);
	m_MasterListCtl.SetItemText(iRow,6,sTmp);

	sTmp.Format("%d",pDetectMasterData->ErrorCount);
	m_MasterListCtl.SetItemText(iRow,7,sTmp);
}

void CReportView::OnLvnItemchangedMasterlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int		iItem;
	CString	sTmp;
	
	iItem = m_MasterListCtl.GetNextItem(-1,LVNI_SELECTED);
	if(-1 == iItem)
	{
		*pResult = 0;
		return;
	}

	m_DetailListCtl.DeleteAllItems();
	
	CDetectMasterData *pMasterData = m_MasterDataList.at(iItem);

	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;

	vector<CDetectDetailData*>::iterator it = pMasterData->m_Details.begin();
	for(;it!=pMasterData->m_Details.end();it++)
	{
		lvItem.iItem = m_DetailListCtl.GetItemCount();

		int iRow = m_DetailListCtl.InsertItem(&lvItem);

		sTmp.Format("%d",(*it)->id);
		m_DetailListCtl.SetItemText(iRow,0,sTmp);

		m_DetailListCtl.SetItemText(iRow,1,(*it)->organName);

		sTmp.Format("%d",(*it)->centerX);
		m_DetailListCtl.SetItemText(iRow,2,sTmp);
		sTmp.Format("%d",(*it)->centerX);
		m_DetailListCtl.SetItemText(iRow,3,sTmp);

		if((*it)->isPass == 1)
		{
			sTmp = "pass";
		}
		else
		{
			sTmp = "no pass";
		}
		m_DetailListCtl.SetItemText(iRow,4,sTmp);

		m_DetailListCtl.SetItemText(iRow,5,(*it)->errType);

		sTmp.Format("%d",(*it)->kindred_value);
		m_DetailListCtl.SetItemText(iRow,6,sTmp);
		sTmp.Format("%d",(*it)->limit_value);
		m_DetailListCtl.SetItemText(iRow,7,sTmp);
	}

	*pResult = 0;
}

//统计
void CReportView::OnBnClickedStat()
{
	if(m_MasterDataList.size()==0)
		return;

	CReportStatDlg stdtDlg(m_MasterDataList,this);
	stdtDlg.DoModal();
}
