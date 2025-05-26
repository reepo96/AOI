// ReportStatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "ReportStatDlg.h"
#include <math.h>

CString g_sByErrType = "按错误类型";
CString g_sByOrgan = "按元件";

CString g_sHistogram = "柱形";
CString g_sCaky = "饼图";

// CReportStatDlg 对话框
const double PI=3.1415926;

IMPLEMENT_DYNAMIC(CReportStatDlg, CDialog)

CReportStatDlg::CReportStatDlg(vector<CDetectMasterData*>& MasterDataList,CWnd* pParent /*=NULL*/)
	: CDialog(CReportStatDlg::IDD, pParent)
	, m_iOrganTotal(0)
	, m_iErrTotal(0)	
{
	m_MasterDataList = MasterDataList;

	if(g_language == CHINESES)
	{
		g_sByErrType = "按错误类型";
		g_sByOrgan = "按元件";
		g_sHistogram = "柱形";
		g_sCaky = "饼图";
	}
	else
	{
		g_sByErrType = "By Error Type";
		g_sByOrgan = "By Organ";
		g_sHistogram = "Histogram";
		g_sCaky = "Caky Graph";
	}

	m_sStatType=g_sByErrType;
	m_sImageType=g_sHistogram;

	Init();
}

CReportStatDlg::~CReportStatDlg()
{
}

void CReportStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORGANSTATLIST, m_OrganStatList);
	DDX_Control(pDX, IDC_ETYPESTATLIST, m_ETypeStatList);
	DDX_Text(pDX, IDC_ORGANTOTAL, m_iOrganTotal);
	DDX_Text(pDX, IDC_ERRTOTAL, m_iErrTotal);
	DDX_CBString(pDX, IDC_STATTYPE, m_sStatType);
	DDX_CBString(pDX, IDC_IMAGTYPE, m_sImageType);
	DDX_Control(pDX, IDC_STATTYPE, m_StatTypeCtl);
	DDX_Control(pDX, IDC_IMAGTYPE, m_ImageTypeCtl);	
	DDX_Control(pDX, IDC_SHOWEREA, m_ShowErea);
}


BEGIN_MESSAGE_MAP(CReportStatDlg, CDialog)
	ON_BN_CLICKED(IDC_CLOSE, &CReportStatDlg::OnBnClickedClose)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_STATTYPE, &CReportStatDlg::OnCbnSelchangeStattype)
	ON_CBN_SELCHANGE(IDC_IMAGTYPE, &CReportStatDlg::OnCbnSelchangeImagtype)
END_MESSAGE_MAP()


// CReportStatDlg 消息处理程序

BOOL CReportStatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sItemName;
	::g_SetDialogStrings(this);

	m_StatTypeCtl.SetCurSel(0);
	m_ImageTypeCtl.SetCurSel(0);

	m_StatTypeCtl.InsertString(0,g_sByErrType);
	m_StatTypeCtl.InsertString(1,g_sByOrgan);
	m_StatTypeCtl.SetCurSel(0);
	
	m_ImageTypeCtl.InsertString(0,g_sHistogram);
	m_ImageTypeCtl.InsertString(1,g_sCaky);
	m_ImageTypeCtl.SetCurSel(0);

	ListView_SetExtendedListViewStyle(m_OrganStatList.m_hWnd,LVS_EX_FULLROWSELECT);

	sItemName = g_LoadString("IDS_ORGAN","元件");
	m_OrganStatList.InsertColumn(0,sItemName,LVCFMT_LEFT,100);

	sItemName = g_LoadString("IDS_ERROR_NUM","错误数");
	m_OrganStatList.InsertColumn(1,sItemName,LVCFMT_LEFT,70);

	ListView_SetExtendedListViewStyle(m_ETypeStatList.m_hWnd,LVS_EX_FULLROWSELECT);

	sItemName = g_LoadString("IDS_ERRORTYPE","错误类型");
	m_ETypeStatList.InsertColumn(0,sItemName,LVCFMT_LEFT,100);

	sItemName = g_LoadString("IDS_ERROR_NUM","错误数");
	m_ETypeStatList.InsertColumn(1,sItemName,LVCFMT_LEFT,70);

	CString	sTemp;
	LV_ITEM lvItem;
	memset(&lvItem,0,sizeof(LV_ITEM));
	lvItem.mask = LVIF_TEXT | LVIF_STATE;	

	//元件统计
	m_OrganStatList.DeleteAllItems();
	map<CString,int>::iterator OAEIt=m_OrganAndErrCount.begin();
	for(;OAEIt!=m_OrganAndErrCount.end();OAEIt++)
	{
		lvItem.iItem = m_OrganStatList.GetItemCount();
		int iRow = m_OrganStatList.InsertItem(&lvItem);

		m_OrganStatList.SetItemText(iRow,0,(*OAEIt).first);

		sTemp.Format("%d",(*OAEIt).second);
		m_OrganStatList.SetItemText(iRow,1,sTemp);
	}

	//错误类型统计
	m_ETypeStatList.DeleteAllItems();
	map<CString,int>::iterator ETAEIt=m_ErrTypeAndErrCount.begin();
	for(;ETAEIt!=m_ErrTypeAndErrCount.end();ETAEIt++)
	{
		lvItem.iItem = m_ETypeStatList.GetItemCount();
		int iRow = m_ETypeStatList.InsertItem(&lvItem);

		m_ETypeStatList.SetItemText(iRow,0,(*ETAEIt).first);

		sTemp.Format("%d",(*ETAEIt).second);
		m_ETypeStatList.SetItemText(iRow,1,sTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CReportStatDlg::Init(void)
{
	m_iOrganTotal = 0;
	m_iErrTotal = 0;
	
	vector<CDetectMasterData*>::iterator MIt=m_MasterDataList.begin();
	for(;MIt!=m_MasterDataList.end();MIt++)
	{
		vector<CDetectDetailData*>::iterator DIt = (*MIt)->m_Details.begin();
		for(;DIt!=(*MIt)->m_Details.end();DIt++)
		{
			m_iOrganTotal++;
			CDetectDetailData* pDetail = (*DIt);

			if(pDetail->isPass == 0)//error
			{
				m_iErrTotal++;

				//根据元件名称查找
				map<CString,int>::iterator OAEIt=m_OrganAndErrCount.find(pDetail->organName);
				if(OAEIt == m_OrganAndErrCount.end())
				{
					m_OrganAndErrCount.insert(::make_pair(pDetail->organName,1));
				}
				else
				{
					(*OAEIt).second += 1;
				}

				//根据错误类型查找
				map<CString,int>::iterator ETAEIt=m_ErrTypeAndErrCount.find(pDetail->errType);
				if(ETAEIt == m_ErrTypeAndErrCount.end())
				{
					m_ErrTypeAndErrCount.insert(::make_pair(pDetail->errType,1));
				}
				else
				{
					(*ETAEIt).second += 1;
				}

			}
		}
	}
}

void CReportStatDlg::OnBnClickedClose()
{
	CDialog::OnCancel();
}

void CReportStatDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	//int iOldMapMode = dc.SetMapMode(MM_TWIPS);

	const int LEFT = 130+70;
	const int TOP = 25+15;

	CRect drawRect;
	m_ShowErea.GetWindowRect(&drawRect);

	//CRect rc(LEFT,TOP,LEFT+drawRect.Width(),TOP+drawRect.Height());
	//dc.Rectangle(&rc);

	CString	sTemp="";
	CBrush hBrush,*oldBrush;

	if(m_sStatType == g_sByErrType)
	{
		if(m_sImageType == g_sHistogram)
		{
			int iNum = m_ErrTypeAndErrCount.size();
			int iMaxErrCount = GetMaxErrTypeCount();

			dc.MoveTo(LEFT+40,(TOP+drawRect.Height())-40);
			dc.LineTo(LEFT+40,TOP);
			dc.MoveTo(LEFT+40,(TOP+drawRect.Height())-40);
			dc.LineTo(LEFT+drawRect.Width(),(TOP+drawRect.Height())-40);

			int y=(drawRect.Height()-40-10)/iMaxErrCount;//y轴：每个错误占的象素
			int x=(drawRect.Width()-40)/iNum;//y轴：每个立柱占的象素
			int iColWidth = x*0.8;//立柱宽度

			hBrush.CreateSolidBrush(RGB(100,100,250));
			oldBrush=dc.SelectObject(&hBrush);

			int iIndex=0;
			map<CString,int>::iterator it=	m_ErrTypeAndErrCount.begin();
			for(;it!=m_ErrTypeAndErrCount.end();it++)
			{
				CRect rc( (LEFT+40 +iIndex*x),TOP+(drawRect.Height()-(y*(*it).second +40)),
						  ((LEFT+40 +iIndex*x)+iColWidth),((TOP+drawRect.Height())-40));
				dc.Rectangle(&rc);
				dc.TextOutA(rc.left,rc.bottom+10,(*it).first);

				sTemp.Format("%d",(*it).second);
				dc.TextOutA(rc.left,rc.top-10,sTemp);

				iIndex++;
			}

			dc.SelectObject(oldBrush);
			hBrush.DeleteObject();
		}
		else if(m_sImageType == g_sCaky)
		{
			CRect rc(LEFT+100,TOP+70,LEFT+400,TOP+370);

			CRect rc1=rc;
			CRect rc2=rc;
			rc1.InflateRect(-50,-50,-50,-50);
			rc2.InflateRect(30,30,30,30);
			
			double dDegree= ((double)360)/((double)m_iErrTotal);//每个错误数占的度数

			int iIndex=0;
			CPoint point1(rc.right,rc.CenterPoint().y);
			double dCurrentP2Degree = 0.0;	//第二点所在的度数

			map<CString,int>::iterator it=	m_ErrTypeAndErrCount.begin();
			for(;it!=m_ErrTypeAndErrCount.end();it++)
			{
				hBrush.CreateSolidBrush(20000*iIndex);
				oldBrush=dc.SelectObject(&hBrush);

				double dCurrentP1Degree = dCurrentP2Degree;

				dCurrentP2Degree += dDegree * (*it).second;
				double dAngle = (dCurrentP2Degree/180.00)*PI;
				int x =rc.CenterPoint().x + (rc.Width()/2) * cos(dAngle);
				int y =rc.CenterPoint().y - (rc.Width()/2) * sin(dAngle);

				dc.Pie(rc,point1,CPoint(x,y));

				point1 = CPoint(x,y);
				iIndex++;

				dc.SelectObject(oldBrush);
				hBrush.DeleteObject();

				CString sInfo ;					
				sInfo.Format("%s:%.2f",(*it).first,((double)(*it).second/((double)m_iErrTotal))*100);
				sInfo+="%";
				WritPieInfo(rc1,rc2,dCurrentP1Degree+(dCurrentP2Degree-dCurrentP1Degree)/2,sInfo);
			}
			
		}
	}
	else if(m_sStatType == g_sByOrgan)
	{
		if(m_sImageType == g_sHistogram)
		{
			int iNum = m_OrganAndErrCount.size();
			int iMaxErrCount = GetMaxOrganErrCount();

			dc.MoveTo(LEFT+40,(TOP+drawRect.Height())-40);
			dc.LineTo(LEFT+40,TOP);
			dc.MoveTo(LEFT+40,(TOP+drawRect.Height())-40);
			dc.LineTo(LEFT+drawRect.Width(),(TOP+drawRect.Height())-40);

			int y=(drawRect.Height()-40-10)/iMaxErrCount;//y轴：每个错误占的象素
			int x=(drawRect.Width()-40)/iNum;//y轴：每个立柱占的象素
			int iColWidth = x*0.8;//立柱宽度

			hBrush.CreateSolidBrush(RGB(100,100,250));
			oldBrush=dc.SelectObject(&hBrush);

			int iIndex=0;
			map<CString,int>::iterator it=	m_OrganAndErrCount.begin();
			for(;it!=m_OrganAndErrCount.end();it++)
			{
				CRect rc( (LEFT+40 +iIndex*x),TOP+(drawRect.Height()-(y*(*it).second +40)),
						  ((LEFT+40 +iIndex*x)+iColWidth),((TOP+drawRect.Height())-40));
				dc.Rectangle(&rc);
				dc.TextOutA(rc.left,rc.bottom+10,(*it).first);

				sTemp.Format("%d",(*it).second);
				dc.TextOutA(rc.left,rc.top-10,sTemp);

				iIndex++;
			}

			dc.SelectObject(oldBrush);
			hBrush.DeleteObject();
		}
		else if(m_sImageType == g_sCaky)
		{
			CRect rc(LEFT+100,TOP+70,LEFT+400,TOP+370);

			CRect rc1=rc;
			CRect rc2=rc;
			rc1.InflateRect(-50,-50,-50,-50);
			rc2.InflateRect(30,30,30,30);
			
			double dDegree= ((double)360)/((double)m_iErrTotal);//每个错误数占的度数

			int iIndex=0;
			CPoint point1(rc.right,rc.CenterPoint().y);
			double dCurrentP2Degree = 0.0;	//第二点所在的度数

			map<CString,int>::iterator it=	m_OrganAndErrCount.begin();
			for(;it!=m_OrganAndErrCount.end();it++)
			{
				hBrush.CreateSolidBrush(20000*iIndex);
				oldBrush=dc.SelectObject(&hBrush);

				double dCurrentP1Degree = dCurrentP2Degree;

				dCurrentP2Degree += dDegree * (*it).second;
				double dAngle = (dCurrentP2Degree/180.00)*PI;

				int x =rc.CenterPoint().x + (rc.Width()/2) * cos(dAngle);
				int y =rc.CenterPoint().y - (rc.Width()/2) * sin(dAngle);

				dc.Pie(rc,point1,CPoint(x,y));

				point1 = CPoint(x,y);
				iIndex++;

				dc.SelectObject(oldBrush);
				hBrush.DeleteObject();

				CString sInfo ;					
				sInfo.Format("%s:%.2f",(*it).first,((double)(*it).second/((double)m_iErrTotal))*100);
				sInfo+="%";
				WritPieInfo(rc1,rc2,dCurrentP1Degree+(dCurrentP2Degree-dCurrentP1Degree)/2,sInfo);
			}
		}
	}

	/*CRgn rg;
	CRect rc(100,100,100,100);
	rg.SetRectRgn(&rc);
	dc.PaintRgn(&rg);*/

	//dc.SetMapMode(iOldMapMode);
}

// 得到元件最大错误数
int CReportStatDlg::GetMaxOrganErrCount(void)
{
	int iRet=0;
	map<CString,int>::iterator it =	m_OrganAndErrCount.begin();
	for(;it != m_OrganAndErrCount.end();it++)
	{
		if(iRet< (*it).second)
			iRet = (*it).second;
	}
	return iRet;
}

// 得到错误类型的最大错误数
int CReportStatDlg::GetMaxErrTypeCount(void)
{
	int iRet=0;
	map<CString,int>::iterator it =	m_ErrTypeAndErrCount.begin();
	for(;it != m_ErrTypeAndErrCount.end();it++)
	{
		if(iRet< (*it).second)
			iRet = (*it).second;
	}
	return iRet;
}

void CReportStatDlg::OnCbnSelchangeStattype()
{
	this->UpdateData(TRUE);
	this->Invalidate();
}

void CReportStatDlg::OnCbnSelchangeImagtype()
{
	this->UpdateData(TRUE);
	this->Invalidate();
}

// 在饼图上标注信息
void CReportStatDlg::WritPieInfo(CRect rct1, CRect rct2, double dDegree,CString sInfo)
{
	double dAngle = (dDegree/180.00)*PI;

	int x1 =rct1.CenterPoint().x + (rct1.Width()/2) * cos(dAngle);
	int y1 =rct1.CenterPoint().y - (rct1.Width()/2) * sin(dAngle);

	int x2 =rct1.CenterPoint().x + (rct2.Width()/2) * cos(dAngle);
	int y2 =rct1.CenterPoint().y - (rct2.Width()/2) * sin(dAngle);

	CDC *pDc = this->GetWindowDC();
	pDc->MoveTo(x1,y1);
	pDc->LineTo(x2,y2);
	pDc->TextOutA(x2,y2,sInfo);
}
