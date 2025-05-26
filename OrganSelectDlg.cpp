// OrganSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganSelectDlg.h"


// COrganSelectDlg 对话框

IMPLEMENT_DYNAMIC(COrganSelectDlg, CDialog)

COrganSelectDlg::COrganSelectDlg(CTemplateData * pTemplateData,COrganData **pOrgan,CWnd* pParent /*=NULL*/,PrgState *pSelectState,int iSelectStateNum)
	: CDialog(COrganSelectDlg::IDD, pParent)
{
	m_pTemplateData = pTemplateData;
	m_pSelectOrgan = pOrgan;
	m_pSelectState = pSelectState;
	m_iSelectStateNum=iSelectStateNum;
}

COrganSelectDlg::~COrganSelectDlg()
{
}

void COrganSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORGANLIST, m_OrganList);
}


BEGIN_MESSAGE_MAP(COrganSelectDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &COrganSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &COrganSelectDlg::OnBnClickedCancle)
END_MESSAGE_MAP()


// COrganSelectDlg 消息处理程序

BOOL COrganSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_OrganList.m_hWnd,LVS_EX_FULLROWSELECT);

	m_OrganList.InsertColumn(0,_T("name"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(1,_T("Footprint"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(2,_T("Mid X"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(3,_T("Mid Y"),LVCFMT_LEFT,50);

	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 初始化列表控件
void COrganSelectDlg::InitList(void)
{
	m_OrganList.DeleteAllItems();

	EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.begin();
	for(;it!=m_pTemplateData->m_OrganDataMap.end();it++)
	{
		if( m_pSelectState != NULL)
		{
			bool bSelect = false;
			for(int i=0;i<m_iSelectStateNum;i++)
			{
				if(*(m_pSelectState+i) == (*it).second->PState)
				{
					bSelect = true;
					break;
				}
			}
			if(bSelect)
				InsObjToList( (*it).second);
		}
		else
		{
			InsObjToList( (*it).second);
		}
	}
	LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
}

int COrganSelectDlg::InsObjToList(COrganData* pOrganData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_OrganList.GetItemCount();

	int iRow = m_OrganList.InsertItem(&lvItem);
	m_OrganList.SetItemText(iRow,0,pOrganData->sName);
	m_OrganList.SetItemText(iRow,1,pOrganData->sFootPrint);

	sTmp.Format("%f",pOrganData->dMidX);
	m_OrganList.SetItemText(iRow,2,sTmp);

	sTmp.Format("%f",pOrganData->dMidY);
	m_OrganList.SetItemText(iRow,3,sTmp);

	m_OrganList.SetItemData(iRow,pOrganData->organId);

	return 0;
}
void COrganSelectDlg::OnBnClickedOk()
{
	int  nItem = -1;	
	nItem = m_OrganList.GetNextItem(-1, LVNI_SELECTED);
	if(-1 == nItem)
	{
		CString sInfo = g_LoadString("IDS_PLEASESELECTORGAN","请选择元件");
		::AfxMessageBox(sInfo);
		return;
	}

	EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	DWORD dwOrganId = m_OrganList.GetItemData(nItem);
	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
	 if( it == m_pTemplateData->m_OrganDataMap.end())
	 {
		 LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
		 return;
	 }
	 *m_pSelectOrgan = (*it).second;

	 LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

	CDialog::OnOK();
}

void COrganSelectDlg::OnBnClickedCancle()
{
	CDialog::OnCancel();
}
