// MyTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "MyTreeView.h"
#include "MyViewSplitter.h"
#include "MainView.h"
#include "DetectMainView.h"

// CMyTreeView

IMPLEMENT_DYNCREATE(CMyTreeView, CFormView)

CMyTreeView::CMyTreeView()
	: CFormView(CMyTreeView::IDD)
{
	m_pTemplateData = NULL;
}

CMyTreeView::~CMyTreeView()
{
	
}

void CMyTreeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyTreeView, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEMENU, &CMyTreeView::OnTvnSelchangedTreemenu)	
END_MESSAGE_MAP()


// CMyTreeView 诊断

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyTreeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyTreeView 消息处理程序

void CMyTreeView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();	
	
	CString		sErrInfo;	
	CRect rect;
	GetClientRect(rect);

	m_pTree = (CTreeCtrl*)GetDlgItem(IDC_TREEMENU);
	m_pTree->MoveWindow(rect);
	m_pTree->DeleteAllItems();
	
	// TODO: Add your specialized code here and/or call the base class
	TV_INSERTSTRUCT TreeCtrlItem;

	//树形控件中加入图标		
	/*m_pImageList->Create(16,16,ILC_COLOR16,3,4);
	HICON hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_COMPUTER));
	m_pImageList->Add(hIcon);
	hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CLOSEFILES));
	m_pImageList->Add(hIcon);
	hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_OPENFILES));
	m_pImageList->Add(hIcon);
	m_pTree->SetImageList(m_pImageList,TVSIL_NORMAL);*/

	//构造树
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;

	CString sNodeName = g_LoadString("IDS_TREEROOTNAME","AOI 系统");
	/*if( g_language == ENGLISH)
	{
		sNodeName.LoadString(IDS_TREEROOTNAME_E);
	}
	else
	{
		sNodeName.LoadString(IDS_TREEROOTNAME_C);
	}*/

	TreeCtrlItem.item.pszText =	sNodeName.GetBuffer(sNodeName.GetLength());	
	TreeCtrlItem.item.lParam = ROOT_VIEW_ID;
	//往树节点插入数据
	/*LPTREENODE pTreeRootNode = new TREENODE();
	pTreeRootNode->iNodeID=ROOT_NODE;
	pTreeRootNode->iViewID=ROOT_VIEW;
	TreeCtrlItem.item.lParam = (LPARAM)pTreeRootNode;*/

    HTREEITEM hTreeItemRoot = m_pTree->InsertItem(&TreeCtrlItem);

	//编程子模快
	if(g_UserInf.enableProgram == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sProgram = g_LoadString("IDS_PROGRAM","编程");
		TreeCtrlItem.item.pszText =	sProgram.GetBuffer(sProgram.GetLength());
		TreeCtrlItem.item.lParam = PROGRAM_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	/*TreeCtrlItem.hParent = hTreeItemRoot;
	CString sProgramModual = g_LoadString("IDS_PROGRAMMODUAL","编程模块");
	TreeCtrlItem.item.pszText =	sProgramModual.GetBuffer(sProgramModual.GetLength());
	TreeCtrlItem.item.lParam = ROOT_VIEW_ID;
	HTREEITEM hTreeItemProgram = m_pTree->InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItemProgram;
	CString sPCBInfo = g_LoadString("IDS_PCBINFO","PCB板信息");
	TreeCtrlItem.item.pszText =	sPCBInfo.GetBuffer(sPCBInfo.GetLength());
	TreeCtrlItem.item.lParam = PCBINFO_VIEW_ID;
	m_pTree->InsertItem(&TreeCtrlItem);	*/

	//检测子模快
	if(g_UserInf.enableDetect == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sDetect = g_LoadString("IDS_PCBDETECT","PCBA检测");
		TreeCtrlItem.item.pszText =	sDetect.GetBuffer(sDetect.GetLength());
		TreeCtrlItem.item.lParam = PCBDETECT_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	//报表模块
	if(g_UserInf.enableReport == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sReport = g_LoadString("IDS_REPORT","报表");
		TreeCtrlItem.item.pszText =	sReport.GetBuffer(sReport.GetLength());
		TreeCtrlItem.item.lParam = REPORT_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	return ;
}
void CMyTreeView::OnTvnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int iViewID = (int)pNMTreeView->itemNew.lParam;
	
	if (AfxGetApp()->GetMainWnd() != NULL)
	{
		MyViewSplitter* pFrm = (MyViewSplitter*)GetParent();

		switch(iViewID)
		{
		case PROGRAM_VIEW_ID:
			if(m_pTemplateData == NULL)	//当前没有模板
			{
				CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
				::AfxMessageBox(sInfo);
			}
			else
			{
				CMainView	*pMainView=(CMainView*)pFrm->getView(PROGRAM_VIEW_ID);
				//pMainView->SetTemplateData(m_pTemplateData);
				pFrm->showView(iViewID);
				pMainView->SetFocus();
			}
			break;
		case PCBDETECT_VIEW_ID:
			if(m_pTemplateData == NULL)	//当前没有模板
			{
				CString sInfo = g_LoadString("IDS_NOTEMPLATE","系统当前不存在模板，请新建模板或者打开模板");
				::AfxMessageBox(sInfo);
			}
			else
			{
				CDetectMainView	*pDetectMainView=(CDetectMainView*)pFrm->getView(PCBDETECT_VIEW_ID);
				pDetectMainView->SetTemplateData(m_pTemplateData);
				pFrm->showView(iViewID);
				pDetectMainView->SetFocus();
			}
			break;

		default:
			pFrm->showView(iViewID);
		}
	}

	*pResult = 0;
}


void CMyTreeView::ReSelectCurrent(void)
{
	/*HTREEITEM hItem =m_pTree->GetSelectedItem();
	if(hItem != NULL)
	{
		m_pTree->SelectItem(hItem);
	}*/
}
