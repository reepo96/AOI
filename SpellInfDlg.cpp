// SpellInfDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "SpellInfDlg.h"
#include "MainView.h"

// CSpellInfDlg 对话框

IMPLEMENT_DYNAMIC(CSpellInfDlg, CDialog)

CSpellInfDlg::CSpellInfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpellInfDlg::IDD, pParent)
{
	m_pTemplateData = NULL;
}

CSpellInfDlg::~CSpellInfDlg()
{
}

void CSpellInfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPELLLIST, m_SpellListCtrl);
}


BEGIN_MESSAGE_MAP(CSpellInfDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW, &CSpellInfDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_DELETE, &CSpellInfDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CSpellInfDlg 消息处理程序

BOOL CSpellInfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称

	ListView_SetExtendedListViewStyle(m_SpellListCtrl.m_hWnd,LVS_EX_FULLROWSELECT);

	m_SpellListCtrl.InsertColumn(0,"No",LVCFMT_LEFT,50);
	m_SpellListCtrl.InsertColumn(1,"Mark ID1",LVCFMT_LEFT,100);
	m_SpellListCtrl.InsertColumn(2,"Mark ID2",LVCFMT_LEFT,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSpellInfDlg::SetTemplateData(CTemplateData* pTemplateData)
{
	m_pTemplateData = pTemplateData;
	if(pTemplateData == NULL)
		return;
	InitList();
}

void CSpellInfDlg::InitList(void)
{
	m_SpellListCtrl.DeleteAllItems();

	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;

	EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	map<int,CSpellPCB*>::iterator it = m_pTemplateData->m_SpellPcbMap.begin();
	for(;it!=m_pTemplateData->m_SpellPcbMap.end();it++)
	{
		if((*it).second->no ==0)
			continue;

		lvItem.iItem = m_SpellListCtrl.GetItemCount();
		int iRow = m_SpellListCtrl.InsertItem(&lvItem);

		sTmp.Format("%d",(*it).second->no);
		m_SpellListCtrl.SetItemText(iRow,0,sTmp);
		
		sTmp.Format("%d",(*it).second->lDatumMarkId1);
		m_SpellListCtrl.SetItemText(iRow,1,sTmp);

		sTmp.Format("%d",(*it).second->lDatumMarkId2);
		m_SpellListCtrl.SetItemText(iRow,2,sTmp);
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);
}

void CSpellInfDlg::NotifyPrgDataChanged(void)
{
	InitList();
}

void CSpellInfDlg::OnBnClickedNew()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->PrgSpellpcb();
}

void CSpellInfDlg::OnBnClickedDelete()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_SpellListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		CString sNo = m_SpellListCtrl.GetItemText(nItem,1);

		CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
		pMainView->DelSpellPrg(::atoi(sNo));

		m_SpellListCtrl.DeleteItem(nItem);
	}
}
