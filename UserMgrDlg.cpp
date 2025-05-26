// UserMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "UserMgrDlg.h"
#include "UserEdit.h"

// CUserMgrDlg 对话框

IMPLEMENT_DYNAMIC(CUserMgrDlg, CDialog)

CUserMgrDlg::CUserMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMgrDlg::IDD, pParent)
{

}

CUserMgrDlg::~CUserMgrDlg()
{
}

void CUserMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERLIST, m_UserListCtl);
}


BEGIN_MESSAGE_MAP(CUserMgrDlg, CDialog)
	ON_BN_CLICKED(IDC_NEW, &CUserMgrDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_MODIFY, &CUserMgrDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CUserMgrDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_CLOSE, &CUserMgrDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CUserMgrDlg 消息处理程序

BOOL CUserMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CUserMgr	userMgr;
	userMgr.GetAllUsers(m_UserList);

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_UserListCtl.m_hWnd,LVS_EX_FULLROWSELECT);

	m_UserListCtl.InsertColumn(0,_T("用户名"),LVCFMT_LEFT,60);
	m_UserListCtl.InsertColumn(1,_T("编程权限"),LVCFMT_LEFT,70);
	m_UserListCtl.InsertColumn(2,_T("检测权限"),LVCFMT_LEFT,70);
	m_UserListCtl.InsertColumn(3,_T("报表权限"),LVCFMT_LEFT,70);
	m_UserListCtl.InsertColumn(4,_T("系统配置"),LVCFMT_LEFT,70);

	InitList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUserMgrDlg::OnBnClickedNew()
{
	CUserEdit userEdit(this);
	userEdit.DoModal();
}

void CUserMgrDlg::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CUserMgrDlg::OnBnClickedDelete()
{
	int  nItem = -1;	
	nItem = m_UserListCtl.GetNextItem(-1, LVNI_SELECTED);
	if(-1 == nItem)
	{
		return;
	}

	CString sUserName = m_UserListCtl.GetItemText(nItem,0);

	CUserInf usrInf;
	usrInf.userName = sUserName;
	CUserMgr	userMgr;

	CString	sErrInfo;
	if(false==userMgr.DelUser(&usrInf,sErrInfo))
	{
		sErrInfo ="删除用户失败\n"+sErrInfo;
		::AfxMessageBox(sErrInfo);
		return;
	}

	m_UserListCtl.DeleteItem(nItem);

	vector<CUserInf*>::iterator it =m_UserList.begin();
	for(;it!=m_UserList.end();it++)
	{
		CUserInf* pUser=(*it);
		if(pUser->userName == sUserName)
		{
			delete pUser;
			pUser = NULL;
			m_UserList.erase(it);
			break;
		}
	}

}

void CUserMgrDlg::OnBnClickedClose()
{
	CDialog::OnCancel();
}

void CUserMgrDlg::NewUser(CUserInf* pUser)
{
	CUserMgr	userMgr;
	CString		sErrInfo;
	if(false==userMgr.AddUser(pUser,sErrInfo))
	{
		sErrInfo ="删除用户失败\n"+sErrInfo;
		::AfxMessageBox(sErrInfo);
		return;
	}

	m_UserList.push_back(pUser);
	InitList();
}

void CUserMgrDlg::InitList(void)
{
	m_UserListCtl.DeleteAllItems();

	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;	

	vector<CUserInf*>::iterator it=	m_UserList.begin();
	for(;it!=m_UserList.end();it++)
	{
		lvItem.iItem = m_UserListCtl.GetItemCount();
		int iRow = m_UserListCtl.InsertItem(&lvItem);
		m_UserListCtl.SetItemText(iRow,0,(*it)->userName);
		
		if((*it)->enableProgram == 1)
		{
			sTmp = "有";
		}
		else
		{
			sTmp = "无";
		}		
		m_UserListCtl.SetItemText(iRow,1,sTmp);

		if((*it)->enableDetect == 1)
		{
			sTmp = "有";
		}
		else
		{
			sTmp = "无";
		}		
		m_UserListCtl.SetItemText(iRow,2,sTmp);

		if((*it)->enableReport == 1)
		{
			sTmp = "有";
		}
		else
		{
			sTmp = "无";
		}		
		m_UserListCtl.SetItemText(iRow,3,sTmp);

		if((*it)->enableSystem == 1)
		{
			sTmp = "有";
		}
		else
		{
			sTmp = "无";
		}		
		m_UserListCtl.SetItemText(iRow,4,sTmp);

	}
}
