// FOVInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "FOVInfoDlg.h"
#include "MainView.h"

// CFOVInfoDlg �Ի���

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


// CFOVInfoDlg ��Ϣ�������

BOOL CFOVInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_SetDialogStrings(this);//��̬���öԻ�������ؼ���ʾ����

	ListView_SetExtendedListViewStyle(m_FovListCtrl.m_hWnd,LVS_EX_FULLROWSELECT);

	CString sColName;
	sColName = ::g_LoadString("IDS_EXCORDER","ִ��˳��");
	m_FovListCtrl.InsertColumn(0,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_MIDX","����X");
	m_FovListCtrl.InsertColumn(1,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_MIDY","����Y");
	m_FovListCtrl.InsertColumn(2,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_WIDTH","���");
	m_FovListCtrl.InsertColumn(3,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_HEIGHT","�߶�");
	m_FovListCtrl.InsertColumn(4,sColName,LVCFMT_LEFT,50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
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

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

			//ɾ�����Ԫ�����
			map<long,COrganData*>::iterator organIt = m_pTemplateData->m_OrganDataMap.begin();
			for(;organIt != m_pTemplateData->m_OrganDataMap.end();organIt++)
			{
				if( (*organIt).second->fovId == m_pSelectFovData->fovId )
					pMainView->DelOrgan( (*organIt).second );
			}

			//ɾ����׼��
			map<long,CDatumMark*>::iterator MarkIt = m_pTemplateData->m_DatumMarkMap.begin();
			while(MarkIt != m_pTemplateData->m_DatumMarkMap.end())
			{
				if( (*MarkIt).second->fovId == m_pSelectFovData->fovId )
				{
					//ɾ��ƴ��
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
