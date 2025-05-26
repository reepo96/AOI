// MyTreeView.cpp : ʵ���ļ�
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


// CMyTreeView ���

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


// CMyTreeView ��Ϣ�������

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

	//���οؼ��м���ͼ��		
	/*m_pImageList->Create(16,16,ILC_COLOR16,3,4);
	HICON hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_COMPUTER));
	m_pImageList->Add(hIcon);
	hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CLOSEFILES));
	m_pImageList->Add(hIcon);
	hIcon=::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_OPENFILES));
	m_pImageList->Add(hIcon);
	m_pTree->SetImageList(m_pImageList,TVSIL_NORMAL);*/

	//������
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;

	CString sNodeName = g_LoadString("IDS_TREEROOTNAME","AOI ϵͳ");
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
	//�����ڵ��������
	/*LPTREENODE pTreeRootNode = new TREENODE();
	pTreeRootNode->iNodeID=ROOT_NODE;
	pTreeRootNode->iViewID=ROOT_VIEW;
	TreeCtrlItem.item.lParam = (LPARAM)pTreeRootNode;*/

    HTREEITEM hTreeItemRoot = m_pTree->InsertItem(&TreeCtrlItem);

	//�����ģ��
	if(g_UserInf.enableProgram == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sProgram = g_LoadString("IDS_PROGRAM","���");
		TreeCtrlItem.item.pszText =	sProgram.GetBuffer(sProgram.GetLength());
		TreeCtrlItem.item.lParam = PROGRAM_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	/*TreeCtrlItem.hParent = hTreeItemRoot;
	CString sProgramModual = g_LoadString("IDS_PROGRAMMODUAL","���ģ��");
	TreeCtrlItem.item.pszText =	sProgramModual.GetBuffer(sProgramModual.GetLength());
	TreeCtrlItem.item.lParam = ROOT_VIEW_ID;
	HTREEITEM hTreeItemProgram = m_pTree->InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent = hTreeItemProgram;
	CString sPCBInfo = g_LoadString("IDS_PCBINFO","PCB����Ϣ");
	TreeCtrlItem.item.pszText =	sPCBInfo.GetBuffer(sPCBInfo.GetLength());
	TreeCtrlItem.item.lParam = PCBINFO_VIEW_ID;
	m_pTree->InsertItem(&TreeCtrlItem);	*/

	//�����ģ��
	if(g_UserInf.enableDetect == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sDetect = g_LoadString("IDS_PCBDETECT","PCBA���");
		TreeCtrlItem.item.pszText =	sDetect.GetBuffer(sDetect.GetLength());
		TreeCtrlItem.item.lParam = PCBDETECT_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	//����ģ��
	if(g_UserInf.enableReport == 1)
	{
		TreeCtrlItem.hParent = hTreeItemRoot;
		CString sReport = g_LoadString("IDS_REPORT","����");
		TreeCtrlItem.item.pszText =	sReport.GetBuffer(sReport.GetLength());
		TreeCtrlItem.item.lParam = REPORT_VIEW_ID;
		m_pTree->InsertItem(&TreeCtrlItem);
	}

	return ;
}
void CMyTreeView::OnTvnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iViewID = (int)pNMTreeView->itemNew.lParam;
	
	if (AfxGetApp()->GetMainWnd() != NULL)
	{
		MyViewSplitter* pFrm = (MyViewSplitter*)GetParent();

		switch(iViewID)
		{
		case PROGRAM_VIEW_ID:
			if(m_pTemplateData == NULL)	//��ǰû��ģ��
			{
				CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
			if(m_pTemplateData == NULL)	//��ǰû��ģ��
			{
				CString sInfo = g_LoadString("IDS_NOTEMPLATE","ϵͳ��ǰ������ģ�壬���½�ģ����ߴ�ģ��");
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
