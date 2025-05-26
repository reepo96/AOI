// TemplateSelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "TemplateSelDlg.h"
#include "TemplateDataMgr.h"

// CTemplateSelDlg 对话框

IMPLEMENT_DYNAMIC(CTemplateSelDlg, CDialog)

CTemplateSelDlg::CTemplateSelDlg(long* pTemplateID,CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateSelDlg::IDD, pParent)
{
	m_pTemplateID = pTemplateID;
}

CTemplateSelDlg::~CTemplateSelDlg()
{
}

void CTemplateSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEMPLATELIST, m_TemplateListCtrl);
}


BEGIN_MESSAGE_MAP(CTemplateSelDlg, CDialog)
	ON_BN_CLICKED(IDC_OK, &CTemplateSelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCLE, &CTemplateSelDlg::OnBnClickedCancle)
	ON_BN_CLICKED(IDC_DELETE, &CTemplateSelDlg::OnBnClickedDelete)
END_MESSAGE_MAP()


// CTemplateSelDlg 消息处理程序

void CTemplateSelDlg::OnBnClickedOk()
{
	int  nItem = -1;	
	nItem = m_TemplateListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(-1 == nItem)
	{
		//CString sInfo = g_LoadString();
		::AfxMessageBox("Pleas select data");
		return;
	}

	DWORD dwOrganId = m_TemplateListCtrl.GetItemData(nItem);
	*m_pTemplateID = dwOrganId;
	
	CDialog::OnOK();
}

void CTemplateSelDlg::OnBnClickedCancle()
{
	CDialog::OnCancel();
}

BOOL CTemplateSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_TemplateListCtrl.m_hWnd,LVS_EX_FULLROWSELECT);

	m_TemplateListCtrl.InsertColumn(0,_T("PCB Name"),LVCFMT_LEFT,150);
	m_TemplateListCtrl.InsertColumn(1,_T("Width"),LVCFMT_LEFT,50);
	m_TemplateListCtrl.InsertColumn(2,_T("Height"),LVCFMT_LEFT,50);

	CTemplateDataMgr templateDataMgr;
	vector<CTemplateData*> templateList;
	if( false == templateDataMgr.GetAllTemplate(templateList))
	{
		return FALSE;
	}

	m_TemplateListCtrl.DeleteAllItems();
	vector<CTemplateData*>::iterator it = templateList.begin();
	for(;it != templateList.end();it++)
	{
		InsObjToList((*it));

		//清除内存
		delete (*it);
		(*it) = NULL;
	}

	//清空列表
	templateList.clear();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTemplateSelDlg::InsObjToList(CTemplateData* pTemplateData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_TemplateListCtrl.GetItemCount();

	int iRow = m_TemplateListCtrl.InsertItem(&lvItem);
	m_TemplateListCtrl.SetItemText(iRow,0,pTemplateData->m_templateName);

	sTmp.Format("%d",pTemplateData->m_width);
	m_TemplateListCtrl.SetItemText(iRow,1,sTmp);

	sTmp.Format("%d",pTemplateData->m_height);
	m_TemplateListCtrl.SetItemText(iRow,2,sTmp);

	m_TemplateListCtrl.SetItemData(iRow,pTemplateData->m_templateId);
}

void CTemplateSelDlg::OnBnClickedDelete()
{
	int  nItem = -1;	
	nItem = m_TemplateListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if(-1 == nItem)
	{
		//CString sInfo = g_LoadString();
		::AfxMessageBox("Pleas select data");
		return;
	}

	CString sInfo;
	sInfo = g_LoadString("IDS_SUREDELTEMPLATE","你确定要删除当前模板吗？");
	if(IDNO==::AfxMessageBox(sInfo,MB_YESNO) )
	{
		return;
	}

	DWORD dwOrganId = m_TemplateListCtrl.GetItemData(nItem);

	CTemplateData tmplateData;
	tmplateData.m_templateId = dwOrganId;
	CTemplateDataMgr templateDataMgr;

	if(false == templateDataMgr.DelTemplate(&tmplateData,sInfo))
	{
		::AfxMessageBox(sInfo);
		return;
	}

	m_TemplateListCtrl.DeleteItem(nItem);

}
