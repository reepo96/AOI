// FOVInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "FOVInfoDlg.h"
#include "MainView.h"

// CFOVInfoDlg 对话框

IMPLEMENT_DYNAMIC(CFOVInfoDlg, CDialog)

CFOVInfoDlg::CFOVInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFOVInfoDlg::IDD, pParent)
	, m_iSelFovExcOrder(0)
{
	m_pTemplateData = NULL;
	m_pSelectFovData = NULL;
}

CFOVInfoDlg::~CFOVInfoDlg()
{
}

void CFOVInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOVLIST, m_FovListCtrl);
	DDX_Text(pDX, IDC_EXCORDER, m_iSelFovExcOrder);
}


BEGIN_MESSAGE_MAP(CFOVInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW, &CFOVInfoDlg::OnBnClickedNew)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FOVLIST, &CFOVInfoDlg::OnLvnItemchangedFovlist)
	ON_EN_CHANGE(IDC_EXCORDER, &CFOVInfoDlg::OnEnChangeExcorder)
	ON_BN_CLICKED(IDC_MODIFY, &CFOVInfoDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CFOVInfoDlg::OnBnClickedDelete)
	ON_NOTIFY(NM_RCLICK, IDC_FOVLIST, &CFOVInfoDlg::OnNMRclickFovlist)
	ON_BN_CLICKED(IDC_SCANDFOV, &CFOVInfoDlg::OnBnClickedScandfov)
END_MESSAGE_MAP()


// CFOVInfoDlg 消息处理程序

BOOL CFOVInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称

	ListView_SetExtendedListViewStyle(m_FovListCtrl.m_hWnd,LVS_EX_FULLROWSELECT);

	CString sColName;
	sColName = ::g_LoadString("IDS_EXCORDER","执行顺序");
	m_FovListCtrl.InsertColumn(0,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_MIDX","中心X");
	m_FovListCtrl.InsertColumn(1,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_MIDY","中心Y");
	m_FovListCtrl.InsertColumn(2,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_WIDTH","宽度");
	m_FovListCtrl.InsertColumn(3,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_HEIGHT","高度");
	m_FovListCtrl.InsertColumn(4,sColName,LVCFMT_LEFT,50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFOVInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;
	if(pTemplateData == NULL)
		return;

	InitList();
}

void CFOVInfoDlg::InitList()
{
	m_FovListCtrl.DeleteAllItems();

	EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
	for(;it!=m_pTemplateData->m_FOVMap.end();it++)
	{
		CFovData *pFovData = (*it).second;
		LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
		InsObjToList( pFovData);
		EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
}

int CFOVInfoDlg::InsObjToList(CFovData* pFovData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_FovListCtrl.GetItemCount();

	int iRow = m_FovListCtrl.InsertItem(&lvItem);

	sTmp.Format("%d",pFovData->CheckOrder);
	m_FovListCtrl.SetItemText(iRow,0,sTmp);

	sTmp.Format("%d",pFovData->GetMidP().x);
	m_FovListCtrl.SetItemText(iRow,1,sTmp);

	sTmp.Format("%d",pFovData->GetMidP().y);
	m_FovListCtrl.SetItemText(iRow,2,sTmp);

	sTmp.Format("%d",pFovData->GetWidth());
	m_FovListCtrl.SetItemText(iRow,3,sTmp);

	sTmp.Format("%d",pFovData->GetHeight());
	m_FovListCtrl.SetItemText(iRow,4,sTmp);

	m_FovListCtrl.SetItemData(iRow,pFovData->fovId);

	return 0;
}
void CFOVInfoDlg::OnLvnItemchangedFovlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int  nItem = -1;
	nItem = m_FovListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

		DWORD dwFovId = m_FovListCtrl.GetItemData(nItem);
		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.find(dwFovId);
		if( it != m_pTemplateData->m_FOVMap.end())
		{
			m_pSelectFovData = (*it).second;
			m_iSelFovExcOrder = m_pSelectFovData->CheckOrder;
			this->UpdateData(FALSE);

			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->ChangeFovFromInfDlg(m_pSelectFovData);
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
	}

	*pResult = 0;
}

void CFOVInfoDlg::OnEnChangeExcorder()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	this->UpdateData(TRUE);
	if( m_pSelectFovData != NULL)
	{
		m_pSelectFovData->CheckOrder = m_iSelFovExcOrder;
		m_pTemplateData->m_bDataIsChanged = true;
		m_pTemplateData->m_bFOVIsChange = true;
	}

	CString sTmp;
	int  nItem = -1;
	nItem = m_FovListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		sTmp.Format("%d",m_iSelFovExcOrder);
		m_FovListCtrl.SetItemText(nItem,0,sTmp);
	}

	// TODO:  在此添加控件通知处理程序代码
}

void CFOVInfoDlg::NotifyPrgDataChanged(void)
{
	InitList();
}

void CFOVInfoDlg::OnBnClickedNew()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->PrgAddfov();
}

void CFOVInfoDlg::OnBnClickedModify()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_FovListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

		DWORD dwFovId = m_FovListCtrl.GetItemData(nItem);
		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.find(dwFovId);
		if( it != m_pTemplateData->m_FOVMap.end())
		{
			m_pSelectFovData = (*it).second;

			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->ModifyFov(m_pSelectFovData);
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
	}
}

void CFOVInfoDlg::OnBnClickedDelete()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_FovListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		DWORD dwFovId = m_FovListCtrl.GetItemData(nItem);
		EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.find(dwFovId);
		if( it != m_pTemplateData->m_FOVMap.end())
		{
			m_pSelectFovData = (*it).second;
			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());

			//删除相关元件编程
			map<long,COrganData*>::iterator organIt = m_pTemplateData->m_OrganDataMap.begin();
			for(;organIt != m_pTemplateData->m_OrganDataMap.end();organIt++)
			{
				if( (*organIt).second->fovId == m_pSelectFovData->fovId )
					pMainView->DelOrgan( (*organIt).second );
			}

			//删除基准点
			map<long,CDatumMark*>::iterator MarkIt = m_pTemplateData->m_DatumMarkMap.begin();
			while(MarkIt != m_pTemplateData->m_DatumMarkMap.end())
			{
				if( (*MarkIt).second->fovId == m_pSelectFovData->fovId )
				{
					//删除拼板
					map<int,CSpellPCB*>::iterator spellIt = m_pTemplateData->m_SpellPcbMap.begin();
					for(;spellIt != m_pTemplateData->m_SpellPcbMap.end();spellIt++)
					{
						CSpellPCB *pSpell = (*spellIt).second;
						if(pSpell->lDatumMarkId1 == (*MarkIt).second->m_datumMarkId ||
							pSpell->lDatumMarkId2 == (*MarkIt).second->m_datumMarkId )
						{
							delete pSpell;
							pSpell = NULL;
							m_pTemplateData->m_SpellPcbMap.erase(spellIt);
							break;
						}
					}

					map<long,CDatumMark*>::iterator tmpMarkIt = MarkIt;
					tmpMarkIt++;
					CDatumMark *pMark = (*MarkIt).second;
					delete pMark;
					pMark = NULL;
					m_pTemplateData->m_DatumMarkMap.erase(MarkIt);
					m_pTemplateData->m_bDatumMarkIsChange = true;
					MarkIt = tmpMarkIt;
					continue;
				}
				MarkIt++;
			}

			delete m_pSelectFovData;
			m_pSelectFovData = NULL;
			m_pTemplateData->m_FOVMap.erase(it);

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bFOVIsChange = true;
			
			m_FovListCtrl.DeleteItem(nItem);
			
			pMainView->InvalidBigGraphView();
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
	}
}

void CFOVInfoDlg::OnNMRclickFovlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu popMenu;

	if(g_language == ENGLISH)
	{
		popMenu.LoadMenu(IDR_FOV_POP_E);
	}
	else
	{
		popMenu.LoadMenu(IDR_FOV_POP);
	}

	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);

	*pResult = 0;
}

void CFOVInfoDlg::OnBnClickedScandfov()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->PrgScanModifyFov();
}
