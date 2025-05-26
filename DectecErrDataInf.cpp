// DectecErrDataInf.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "DectecErrDataInf.h"
#include "DetectBigGraphView.h"
#include <algorithm>
using namespace std;

// 参与排序的函数对象（从大到小排序）
bool Lesser(CDetectStatInfo* elem1, CDetectStatInfo* elem2)
{
	return elem1->lErrorCount > elem2->lErrorCount;
}

// CDectecErrDataInf 对话框

IMPLEMENT_DYNAMIC(CDectecErrDataInf, CDialog)

CDectecErrDataInf::CDectecErrDataInf(CWnd* pParent /*=NULL*/)
	: CDialog(CDectecErrDataInf::IDD, pParent)
{

}

CDectecErrDataInf::~CDectecErrDataInf()
{
}

void CDectecErrDataInf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ERRDATALIST, m_ErrDataListCtl);
}


BEGIN_MESSAGE_MAP(CDectecErrDataInf, CDialog)
END_MESSAGE_MAP()


// CDectecErrDataInf 消息处理程序

void CDectecErrDataInf::NotifyDetectDataChanged(void)
{
	GetStatInfListANdSort();
	InitList();
}

BOOL CDectecErrDataInf::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_ErrDataListCtl.m_hWnd,LVS_EX_FULLROWSELECT);

	m_ErrDataListCtl.InsertColumn(0,_T("name"),LVCFMT_LEFT,100);
	m_ErrDataListCtl.InsertColumn(1,_T("error count"),LVCFMT_LEFT,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDectecErrDataInf::InitList(void)
{
	m_ErrDataListCtl.DeleteAllItems();

	vector<CDetectStatInfo*>::iterator it = m_StatInfList.begin();
	for(;it!=m_StatInfList.end();it++)
	{
		InsObjToList( (*it));
	}
}

void CDectecErrDataInf::InsObjToList(CDetectStatInfo* pDetectStatInfo)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_ErrDataListCtl.GetItemCount();

	int iRow = m_ErrDataListCtl.InsertItem(&lvItem);
	m_ErrDataListCtl.SetItemText(iRow,0,pDetectStatInfo->sOrganName);
	
	sTmp.Format("%d",pDetectStatInfo->lErrorCount);
	m_ErrDataListCtl.SetItemText(iRow,1,sTmp);
}

// 获得检测统计数据列表并排序
void CDectecErrDataInf::GetStatInfListANdSort(void)
{
	m_StatInfList.clear();

	//拷贝
	map<CString,CDetectStatInfo*>::iterator mapIt=((CDetectBigGraphView*)m_pDBigView)->m_DStatInfoMap.begin();
	for(;mapIt != ((CDetectBigGraphView*)m_pDBigView)->m_DStatInfoMap.end();mapIt++)
	{
		m_StatInfList.push_back( (*mapIt).second );
	}

	if(m_StatInfList.size()<=1)
		return;

	//排序
	sort(m_StatInfList.begin(),m_StatInfList.end(),Lesser);
}
