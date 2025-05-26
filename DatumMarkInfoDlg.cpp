// DatumMarkInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DatumMarkInfoDlg.h"
#include "MainView.h"
#include "ObjectImageDlg.h"

// CDatumMarkInfoDlg 对话框

IMPLEMENT_DYNAMIC(CDatumMarkInfoDlg, CDialog)

CDatumMarkInfoDlg::CDatumMarkInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDatumMarkInfoDlg::IDD, pParent)
	, m_lLimit(0)
{
	m_pTemplateData = NULL;
	m_pSelectDatumMark = NULL;
}

CDatumMarkInfoDlg::~CDatumMarkInfoDlg()
{
}

void CDatumMarkInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATUMMARKLIST, m_MarkListCtrl);
	DDX_Text(pDX, IDC_LIMIT, m_lLimit);
}


BEGIN_MESSAGE_MAP(CDatumMarkInfoDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DATUMMARKLIST, &CDatumMarkInfoDlg::OnLvnItemchangedDatummarklist)
	ON_BN_CLICKED(IDC_NEW, &CDatumMarkInfoDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_MODIFY, &CDatumMarkInfoDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CDatumMarkInfoDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SHOWIMAGE, &CDatumMarkInfoDlg::OnBnClickedShowimage)
	ON_NOTIFY(NM_RCLICK, IDC_DATUMMARKLIST, &CDatumMarkInfoDlg::OnNMRclickDatummarklist)
	ON_COMMAND(ID_SHOWMARKIMAGE, &CDatumMarkInfoDlg::OnShowmarkimage)
	ON_EN_CHANGE(IDC_LIMIT, &CDatumMarkInfoDlg::OnEnChangeLimit)
END_MESSAGE_MAP()


// CDatumMarkInfoDlg 消息处理程序

BOOL CDatumMarkInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称

	ListView_SetExtendedListViewStyle(m_MarkListCtrl.m_hWnd,LVS_EX_FULLROWSELECT);

	CString sColName;
	sColName = ::g_LoadString("IDS_MIDX","中心X");
	m_MarkListCtrl.InsertColumn(0,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_MIDY","中心Y");
	m_MarkListCtrl.InsertColumn(1,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_WIDTH","宽度");
	m_MarkListCtrl.InsertColumn(2,sColName,LVCFMT_LEFT,50);

	sColName = ::g_LoadString("IDS_HEIGHT","高度");
	m_MarkListCtrl.InsertColumn(3,sColName,LVCFMT_LEFT,50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDatumMarkInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;

	if(pTemplateData == NULL)
		return;

	m_lLimit = m_pTemplateData->m_lDatumLimit;
	this->UpdateData(FALSE);

	InitList();
}

void CDatumMarkInfoDlg::InitList()
{
	m_MarkListCtrl.DeleteAllItems();

	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	map<long,CDatumMark*>::iterator it = m_pTemplateData->m_DatumMarkMap.begin();
	for(;it!=m_pTemplateData->m_DatumMarkMap.end();it++)
	{
		CDatumMark *pDatumMark = (*it).second;
		LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
		InsObjToList( pDatumMark );
		EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
	}
	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
}

int CDatumMarkInfoDlg::InsObjToList(CDatumMark* pDatumMark)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_MarkListCtrl.GetItemCount();

	int iRow = m_MarkListCtrl.InsertItem(&lvItem);

	sTmp.Format("%d",pDatumMark->GetMidP().x);
	m_MarkListCtrl.SetItemText(iRow,0,sTmp);

	sTmp.Format("%d",pDatumMark->GetMidP().y);
	m_MarkListCtrl.SetItemText(iRow,1,sTmp);

	sTmp.Format("%d",pDatumMark->GetWidth());
	m_MarkListCtrl.SetItemText(iRow,2,sTmp);

	sTmp.Format("%d",pDatumMark->GetHeight());
	m_MarkListCtrl.SetItemText(iRow,3,sTmp);

	m_MarkListCtrl.SetItemData(iRow,pDatumMark->m_datumMarkId);

	return 0;
}

void CDatumMarkInfoDlg::OnLvnItemchangedDatummarklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDatumMarkInfoDlg::NotifyPrgDataChanged(void)
{
	InitList();
}

void CDatumMarkInfoDlg::OnBnClickedNew()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->NewDatumMark();
}

void CDatumMarkInfoDlg::OnBnClickedModify()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_MarkListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

		DWORD dwMarkId = m_MarkListCtrl.GetItemData(nItem);
		map<long,CDatumMark*>::iterator it = m_pTemplateData->m_DatumMarkMap.find(dwMarkId);
		if( it != m_pTemplateData->m_DatumMarkMap.end())
		{
			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->ModifyDatumMark((*it).second);
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
	}
}

void CDatumMarkInfoDlg::OnBnClickedDelete()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_MarkListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

		DWORD dwMarkId = m_MarkListCtrl.GetItemData(nItem);
		map<long,CDatumMark*>::iterator it = m_pTemplateData->m_DatumMarkMap.find(dwMarkId);
		if( it != m_pTemplateData->m_DatumMarkMap.end())
		{
			CDatumMark *pDatumMark = (*it).second;
			delete pDatumMark;
			pDatumMark = NULL;

			m_pTemplateData->m_DatumMarkMap.erase(it);
			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bDatumMarkIsChange = true;

			m_MarkListCtrl.DeleteItem(nItem);

			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->InvalidBigGraphView();
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
	}
}

void CDatumMarkInfoDlg::OnBnClickedShowimage()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_MarkListCtrl.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

		DWORD dwMarkId = m_MarkListCtrl.GetItemData(nItem);
		map<long,CDatumMark*>::iterator it = m_pTemplateData->m_DatumMarkMap.find(dwMarkId);
		if( it != m_pTemplateData->m_DatumMarkMap.end())
		{
			CDatumMark *pDatumMark = (*it).second;

			CImage* pTmpMarkTImage = new CImage();
			pTmpMarkTImage->Create(pDatumMark->m_ImageObj.GetWidth(),
				pDatumMark->m_ImageObj.GetHeight(),
				pDatumMark->m_ImageObj.GetBPP());

			pDatumMark->m_ImageObj.BitBlt(pTmpMarkTImage->GetDC(),0,0,SRCCOPY);
			pTmpMarkTImage->ReleaseDC();

			CObjectImageDlg showImageDlg(pTmpMarkTImage,"基准点图像",this);
			showImageDlg.DoModal();
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);
	}
}

void CDatumMarkInfoDlg::OnNMRclickDatummarklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu popMenu;
	if(g_language == ENGLISH)
	{
		popMenu.LoadMenu(IDR_MARK_POP_E);
	}
	else
	{
		popMenu.LoadMenu(IDR_MARK_POP);
	}

	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);

	*pResult = 0;
}

void CDatumMarkInfoDlg::OnShowmarkimage()
{
	OnBnClickedShowimage();
}

void CDatumMarkInfoDlg::OnEnChangeLimit()
{
	this->UpdateData();

	m_pTemplateData->m_lDatumLimit = m_lLimit;
	m_pTemplateData->m_bDataIsChanged = true;
}
