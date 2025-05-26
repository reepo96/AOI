// OrganInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganInfoDlg.h"
#include "OrganImage.h"
#include "MainView.h"
#include "OrganEdit.h"

// COrganInfoDlg 对话框

IMPLEMENT_DYNAMIC(COrganInfoDlg, CDialog)

COrganInfoDlg::COrganInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrganInfoDlg::IDD, pParent)
	, m_iSelectOrganLimit(0)
	, m_sIsPolar(_T(""))
{
	m_pTemplateData=NULL;
	m_pBigGraphView = NULL;
}

COrganInfoDlg::~COrganInfoDlg()
{
}

void COrganInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORGANLIST, m_OrganList);
	DDX_Text(pDX, IDC_LIMIT, m_iSelectOrganLimit);
	DDX_CBString(pDX, IDC_ISPOLAR, m_sIsPolar);
}


BEGIN_MESSAGE_MAP(COrganInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_IMPORT, &COrganInfoDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_CALCRELATIVE, &COrganInfoDlg::OnBnClickedCalcrelative)
	ON_BN_CLICKED(IDC_IMAGE, &COrganInfoDlg::OnBnClickedImage)
	ON_BN_CLICKED(IDC_NEW, &COrganInfoDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_MODIFY, &COrganInfoDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &COrganInfoDlg::OnBnClickedDelete)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &COrganInfoDlg::OnHdnItemchangedOrganlist)
	ON_EN_CHANGE(IDC_LIMIT, &COrganInfoDlg::OnEnChangeLimit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ORGANLIST, &COrganInfoDlg::OnNMCustomdrawOrganlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ORGANLIST, &COrganInfoDlg::OnLvnItemchangedOrganlist)
	ON_NOTIFY(NM_RCLICK, IDC_ORGANLIST, &COrganInfoDlg::OnNMRclickOrganlist)
	ON_BN_CLICKED(IDC_PROGRAM, &COrganInfoDlg::OnBnClickedProgram)
	ON_BN_CLICKED(IDC_DELPRG, &COrganInfoDlg::OnBnClickedDelprg)
	ON_CBN_SELCHANGE(IDC_ISPOLAR, &COrganInfoDlg::OnCbnSelchangeIspolar)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COrganInfoDlg 消息处理程序

BOOL COrganInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_SetDialogStrings(this);//动态设置对话框各个控件显示名称
	ListView_SetExtendedListViewStyle(m_OrganList.m_hWnd,LVS_EX_FULLROWSELECT);

	m_OrganList.InsertColumn(0,_T("name"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(1,_T("Footprint"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(2,_T("Mid X"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(3,_T("Mid Y"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(4,_T("Limit"),LVCFMT_LEFT,50);
	m_OrganList.InsertColumn(5,_T("Polar"),LVCFMT_LEFT,40);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COrganInfoDlg::SetTemplateData(CTemplateData * pTemplateData)
{
	m_pTemplateData = pTemplateData;
	if(pTemplateData == NULL)
		return;

	InitList();	
}

void COrganInfoDlg::NotifyPrgDataChanged(void)
{
	if(m_pTemplateData == NULL)
		return;

	InitList();	
}

// 初始化列表控件
void COrganInfoDlg::InitList(void)
{
	m_OrganList.DeleteAllItems();

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

int COrganInfoDlg::InsObjToList(COrganData* pOrganData)
{
	CString sTmp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.iItem = m_OrganList.GetItemCount();

	int iRow = m_OrganList.InsertItem(&lvItem);
	m_OrganList.SetItemText(iRow,0,pOrganData->sName);
	m_OrganList.SetItemText(iRow,1,pOrganData->sFootPrint);

	sTmp.Format("%f",pOrganData->dMidX);
	m_OrganList.SetItemText(iRow,2,sTmp);

	sTmp.Format("%f",pOrganData->dMidY);
	m_OrganList.SetItemText(iRow,3,sTmp);

	sTmp.Format("%d",pOrganData->iLimit);
	m_OrganList.SetItemText(iRow,4,sTmp);

	if(pOrganData->iIsPolar == 1)
	{
		m_OrganList.SetItemText(iRow,5,"yes");
	}
	else
	{
		m_OrganList.SetItemText(iRow,5,"no");
	}

	m_OrganList.SetItemData(iRow,pOrganData->organId);

	return 0;
}

//导入CAD文件
void COrganInfoDlg::OnBnClickedImport()
{
	if(m_pTemplateData->m_sTemplateType == "" || m_pTemplateData->m_sTemplateType.IsEmpty())
	{
		::AfxMessageBox("Unknow template type");
		return ;
	}

	CFileDialog fileDlg(TRUE, "txt", "",
					OFN_READONLY|OFN_NOCHANGEDIR, "txt file(*.txt)|*.txt|all file(*.*)|*.*||" );

	if(fileDlg.DoModal() != IDOK)
		return;
	
	CString sFileName = fileDlg.GetPathName();

	FILE *file;
	file = fopen(sFileName,"r");
	if(file == NULL)
	{
		CString	sInfo = g_LoadString("IDS_OPENFILEFAIL","打开文件失败");
		::AfxMessageBox(sInfo);
		return;
	}

	char line[1024] ={0};
	fgets(line,sizeof(line),file) ;	//标题行

	while( !feof(file))
	{
		memset(line,0,sizeof(line));
		fgets(line,sizeof(line),file);
		line[1023]='\0';

		parse(line);
	}
}

// 分析一行数据，得到一个元件
void COrganInfoDlg::parse(char* pLine)
{
	CString	sLine(pLine);
	CString resToken;
	int curPos= 0;
	int iCol=0;		//列号

	sLine.Trim(" ");

	if(sLine=="" || sLine.GetLength()<5)
		return;

	COrganData *pOrganData =new COrganData();
	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.begin();

	resToken= sLine.Tokenize(" ",curPos);
	while(resToken != "")
	{
		switch(iCol)
		{
		case 0:
			for(;it != m_pTemplateData->m_OrganDataMap.end();it++)
			{
				if( (*it).second->sName == resToken)//元件已经存在
				{
					delete pOrganData;
					pOrganData=NULL;
					return;
				}
			}
			pOrganData->sName = resToken;
			break;
		case 1:
			pOrganData->sFootPrint = resToken;
			break;
		case 2:
			resToken.Trim("mM");
			pOrganData->dMidX = ::atof(resToken);
			break;
		case 3:
			resToken.Trim("mM");
			pOrganData->dMidY = ::atof(resToken);
			break;
		case 4:			
			break;
		case 5:			
			break;
		case 6:
			resToken.Trim("mM");
			pOrganData->dPadX = ::atof(resToken);
			break;
		case 7:
			resToken.Trim("mM");
			pOrganData->dPadY = ::atof(resToken);
			break;
		case 8:
			resToken.Trim(" ");
			if(resToken != m_pTemplateData->m_sTemplateType)
			{
				return;
			}
			break;
		case 9:			
			pOrganData->dRotation = ::atof(resToken);
			break;
		default:
			break;
		}

		iCol++;
		resToken= sLine.Tokenize(" ",curPos);
	}

	if(iCol<9) //error data
	{
		delete pOrganData;
		pOrganData = NULL;
	}
	else
	{
		int iOrganId = -1 -m_pTemplateData->m_OrganDataMap.size();
		pOrganData->organId = iOrganId;
		m_pTemplateData->m_bOrganDataIsChange = true;
		m_pTemplateData->m_bDataIsChanged = true;
		m_pTemplateData->m_OrganDataMap.insert(::make_pair(iOrganId,pOrganData));

		InsObjToList(pOrganData);
	}

}

//计算CAD坐标系与本系统坐标系之间的相对值
void COrganInfoDlg::OnBnClickedCalcrelative()
{
	/*UINT uSelectedCount = m_OrganList.GetSelectedCount();
	if( uSelectedCount != 2)
	{
		::AfxMessageBox("请选择两个对角元件，然后才能计算坐标系相对值");
		return;
	}

	COrganData *pOrganData1 =NULL;
	COrganData *pOrganData2 =NULL;
	int  nItem = -1;
	
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	DWORD dwOrganId = m_OrganList.GetItemData(nItem);

	 map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
	 if( it == m_pTemplateData->m_OrganDataMap.end())
		 return;
	 pOrganData1 = (*it).second;

	 nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	 dwOrganId = m_OrganList.GetItemData(nItem);

	 it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
	 if( it == m_pTemplateData->m_OrganDataMap.end())
		 return;
	 pOrganData2 = (*it).second; 

	 if(m_pBigGraphView != NULL)
	 {
		 m_pBigGraphView->CaclRelativValue(pOrganData1,pOrganData2);
	 }*/
        
}

void COrganInfoDlg::SetBigGraphView(CBigGraphView * pBigGraphView)
{
	m_pBigGraphView = pBigGraphView;
}

void COrganInfoDlg::OnBnClickedImage()
{
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
		DWORD dwOrganId = m_OrganList.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			COrganImage imageShowDlg((*it).second,this);
			imageShowDlg.DoModal();
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}

}

void COrganInfoDlg::OnBnClickedNew()
{
	COrganData *pOrganData =NULL;
	COrganEdit organDlg(&pOrganData,this);
	
	if(IDOK==organDlg.DoModal() && pOrganData != NULL)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		int iOrganId = -1 -m_pTemplateData->m_OrganDataMap.size();
		pOrganData->organId = iOrganId;
		m_pTemplateData->m_bOrganDataIsChange = true;
		m_pTemplateData->m_bDataIsChanged = true;
		m_pTemplateData->m_OrganDataMap.insert(::make_pair(iOrganId,pOrganData));

		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		InsObjToList(pOrganData);
	}
}

void COrganInfoDlg::OnBnClickedModify()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		DWORD dwOrganId = m_OrganList.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->ModifyOrgan((*it).second);
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}
}

void COrganInfoDlg::OnBnClickedDelete()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		DWORD dwOrganId = m_OrganList.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			COrganData *pOrganData = (*it).second;
			m_pTemplateData->m_OrganDataMap.erase(it);

			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pTemplateData->m_bDataIsChanged = true;

			delete pOrganData;
			pOrganData = NULL;

			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->SetCurrentOrgan(NULL);

			m_OrganList.DeleteItem(nItem);
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}
}

void COrganInfoDlg::OnHdnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
		
	*pResult = 0;
}

void COrganInfoDlg::OnEnChangeLimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	this->UpdateData(TRUE);
	if( m_pSelectOrganData != NULL)
	{
		m_pSelectOrganData->iLimit = m_iSelectOrganLimit;
		m_pSelectOrganData->bDataChanged = true;
		m_pTemplateData->m_bOrganDataIsChange = true;
		m_pTemplateData->m_bDataIsChanged = true;
	}

	CString sTmp;
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		sTmp.Format("%d",m_iSelectOrganLimit);
		m_OrganList.SetItemText(nItem,4,sTmp);
	}
}

void COrganInfoDlg::OnNMCustomdrawOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	m_OrganList.SetExtendedStyle(m_OrganList.GetExtendedStyle()  | LVS_EX_FULLROWSELECT);
	
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
		DWORD dwOrganId = m_OrganList.GetItemData(nItem);

		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		 if( it == m_pTemplateData->m_OrganDataMap.end())
		 {
			 LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
			 return;
		 }

		 COrganData *pOrganData = (*it).second;
		 LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
		
		//根据进程状态设置背景
		 if(pOrganData->PState == PS_COMPLETE )	//编程完毕的元件，底色设成绿色
		{
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(0,180,0);		
		}
	    else if(pOrganData->PState == PS_SKIP )	//忽略的元件，底色设成灰色
		{
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(200,200,200);		//Set the bkgrnd color to blue
		}
	    else if(pOrganData->PState == PS_INVALID )	//无效的元件，底色设成红色
		{
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(180,0,0);		//Set the bkgrnd color to blue
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

void COrganInfoDlg::OnLvnItemchangedOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		DWORD dwOrganId = m_OrganList.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			m_pSelectOrganData = (*it).second;
			m_iSelectOrganLimit = m_pSelectOrganData->iLimit;

			if(m_pSelectOrganData->iIsPolar == 1)
			{
				m_sIsPolar = "yes";
			}
			else
			{
				m_sIsPolar = "no";
			}

			this->UpdateData(FALSE);

			if(m_pSelectOrganData->PState == PS_COMPLETE)
			{
				CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
				pMainView->SetCurrentOrgan(m_pSelectOrganData);
				if(m_pSelectOrganData->m_pFovData != NULL)
				{
					pMainView->SetCurrentShowFov(m_pSelectOrganData->m_pFovData);
					pMainView->SetCurrentImage(&m_pSelectOrganData->m_pFovData->m_ImageObj,FOVIMAGE);
				}
				
			}//end if(m_pSelectOrganData->PState == PS_COMPLETE)
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}

	*pResult = 0;
}

bool COrganInfoDlg::OrganIsInFov(const CFovData* pFovData, const COrganData* pOrganData)
{
	long	lOrganX;//元件中心X点
	long	lOrganY;//元件中心Y点

	lOrganX = pOrganData->dMidX * m_pTemplateData->m_lCoordScaleX + m_pTemplateData->m_lOffsetX;
	
	//CAD坐标原点是在左下角
	lOrganY = m_pTemplateData->m_ImageObj.GetHeight() - (pOrganData->dMidY * m_pTemplateData->m_lCoordScaleY + m_pTemplateData->m_lOffsetY);

	CRect fovRect(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);

	return fovRect.PtInRect( CPoint(lOrganX,lOrganY) );
}

void COrganInfoDlg::OnNMRclickOrganlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu popMenu;

	if(g_language == ENGLISH)
	{
		popMenu.LoadMenu(IDR_ORGAN_POP_E);
	}
	else
	{
		popMenu.LoadMenu(IDR_ORGAN_POP);
	}
	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);

	*pResult = 0;
}

void COrganInfoDlg::OnBnClickedProgram()
{
	CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
	pMainView->PrgOrgan();
}

void COrganInfoDlg::OnBnClickedDelprg()
{
	CString sTmp;
	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		DWORD dwOrganId = m_OrganList.GetItemData(nItem);
		map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.find(dwOrganId);
		if( it != m_pTemplateData->m_OrganDataMap.end())
		{
			CMainView *pMainView = (CMainView*)( ((this->GetParent())->GetParent())->GetParent());
			pMainView->DelOrgan((*it).second);
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
	}
}

void COrganInfoDlg::OnCbnSelchangeIspolar()
{
	this->UpdateData(TRUE);
	if( m_pSelectOrganData != NULL)
	{
		if(m_sIsPolar == "yes")
		{
			m_pSelectOrganData->iIsPolar = 1;
		}
		else
		{
			m_pSelectOrganData->iIsPolar = 0;
		}
		
		m_pSelectOrganData->bDataChanged = true;
		m_pTemplateData->m_bOrganDataIsChange = true;
		m_pTemplateData->m_bDataIsChanged = true;
	}

	int  nItem = -1;
	nItem = m_OrganList.GetNextItem(nItem, LVNI_SELECTED);
	if(nItem != -1)
	{
		m_OrganList.SetItemText(nItem,5,m_sIsPolar);
	}
}

void COrganInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	//InitList();

	CDialog::OnTimer(nIDEvent);
}
