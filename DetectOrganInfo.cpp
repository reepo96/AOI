// DetectOrganInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DetectOrganInfo.h"
#include "DetectBigGraphView.h"

// CDetectOrganInfo 对话框

IMPLEMENT_DYNAMIC(CDetectOrganInfo, CDialog)

CDetectOrganInfo::CDetectOrganInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectOrganInfo::IDD, pParent)
	, m_bNeedDetect(FALSE)
{
	m_pCurrentTOrgan = NULL;
	m_pSelectOrganData = NULL;
}

CDetectOrganInfo::~CDetectOrganInfo()
{
}

void CDetectOrganInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORGANLIST, m_OrganListCtl);
	DDX_Control(pDX, IDC_NEED_DETECT, m_btnNeedDetect);
	DDX_Check(pDX, IDC_NEED_DETECT, m_bNeedDetect);
}


BEGIN_MESSAGE_MAP(CDetectOrganInfo, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ORGANLIST, &CDetectOrganInfo::OnNMCustomdrawOrganlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ORGANLIST, &CDetectOrganInfo::OnLvnItemchangedOrganlist)
	ON_BN_CLICKED(IDC_NEED_DETECT, &CDetectOrganInfo::OnBnClickedNeedDetect)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDetectOrganInfo 消息处理程序

void CDetectOrganInfo::SetTemplateData(CTemplateData *pTemplateData)
{
	m_pTemplateData = pTemplateData;

	if(pTemplateData == NULL)
		return;
	
	InitList();
}

void CDetectOrganInfo::NotifyDetectDataChanged(void)
{
	if(m_pDBigView==NULL || m_pDBigView->m_hWnd==NULL || !IsWindow(m_pDBigView->m_hWnd))
		return;

	m_pCurrentTOrgan = ((CDetectBigGraphView*)m_pDBigView)->m_pCurrentTOrgan;
	InitList();
}

BOOL CDetectOrganInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_OrganListCtl.m_hWnd,LVS_EX_FULLROWSELECT);

	m_OrganListCtl.InsertColumn(0,_T("name"),LVCFMT_LEFT,50);
	m_OrganListCtl.InsertColumn(1,_T("Footprint"),LVCFMT_LEFT,50);
	m_OrganListCtl.InsertColumn(2,_T("Mid X"),LVCFMT_LEFT,50);
	m_OrganListCtl.InsertColumn(3,_T("Mid Y"),LVCFMT_LEFT,50);
	m_OrganListCtl.InsertColumn(4,_T("Detect"),LVCFMT_LEFT,50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDetectOrganInfo::InitList(void)
{
	m_OrganListCtl.DeleteAllItems();

	if(m_pTemplateData == NULL)
		return;

	EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.begin();
	for(;it!=m_pTemplateData->m_OrganDataMap.end();it++)
	{
		COrganData *pOrganData = (*it).second;
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
		InsObjToList( pOrganData);
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}
	LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
}

void CDetectOrganInfo::InsObjToList(COrganData* pOrganData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_OrganListCtl.GetItemCount();

	int iRow = m_OrganListCtl.InsertItem(&lvItem);
	m_OrganListCtl.SetItemText(iRow,0,pOrganData->sName);
	m_OrganListCtl.SetItemText(iRow,1,pOrganData->sFootPrint);

	sTmp.Format("%f",pOrganData->dMidX);
	m_OrganListCtl.SetItemText(iRow,2,sTmp);

	sTmp.Format("%f",pOrganData->dMidY);
	m_OrganListCtl.SetItemText(iRow,3,sTmp);

	if(pOrganData->bNeedDetect)
	{
		m_OrganListCtl.SetItemText(iRow,4,"yes");
	}
	else
	{
		m_OrganListCtl.SetItemText(iRow,4,"no");
	}

	m_OrganListCtl.SetItemData(iRow,pOrganData->organId);
}

void CDetectOrganInfo::OnNMCustomdrawOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	m_OrganListCtl.SetExtendedStyle(m_OrganListCtl.GetExtendedStyle()  | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	//
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		
		COLORREF clrNewTextColor, clrNewBkColor;        
		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		DWORD dwOrganId = m_OrganListCtl.GetItemData(nItem);		
		
		//根据进程状态设置背景
		if(m_pCurrentTOrgan!=NULL && m_pCurrentTOrgan->organId==dwOrganId )	//正在检测的元件设置成红色
		{
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(0,150,0);		//Set the bkgrnd color to blue
		}
		else
		{
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(255,255,255);		//Set the bkgrnd color to blue
		}
		
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;		
        
        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;        
	}
}

void CDetectOrganInfo::OnLvnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int  nItem = -1;
	nItem = m_OrganListCtl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		DWORD dwOrganId = m_OrganListCtl.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			m_pSelectOrganData = (*it).second;

			if(m_pSelectOrganData->bNeedDetect)
			{
				m_btnNeedDetect.SetCheck(BST_CHECKED);
			}
			else
			{
				m_btnNeedDetect.SetCheck(BST_UNCHECKED);
			}
			
			if(m_pSelectOrganData->PState == PS_COMPLETE)
			{
				m_btnNeedDetect.EnableWindow(TRUE);
			}
			else
			{
				m_btnNeedDetect.EnableWindow(FALSE);
			}
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}

	*pResult = 0;
}

void CDetectOrganInfo::OnBnClickedNeedDetect()
{
	this->UpdateData(TRUE);
	if(m_pSelectOrganData != NULL)
	{
		m_pSelectOrganData->bNeedDetect = (bool)m_bNeedDetect;
	}

	CString sTmp;
	int  nItem = -1;
	nItem = m_OrganListCtl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		if(m_bNeedDetect)
		{
			m_OrganListCtl.SetItemText(nItem,4,"yes");
		}
		else
		{
			m_OrganListCtl.SetItemText(nItem,4,"no");
		}
	}
}

void CDetectOrganInfo::OnTimer(UINT_PTR nIDEvent)
{
	//InitList();

	CDialog::OnTimer(nIDEvent);
}
