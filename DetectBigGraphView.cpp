// DetectBigGraphView.cpp : 实现文件
#include "stdafx.h"
#include "AOI.h"
#include "DetectBigGraphView.h"
#include "ValidateErr.h"
#include "DetectMainView.h"
#include "OrganSelectDlg.h"
#include "Motor.h"
#include "Camera.h"
#include "PCBState.h"
#include <algorithm>
using namespace std;

#include <time.h>
#include <stdlib.h>
#include <math.h>

extern CCamera     g_camera;	//全局摄像头对象
extern CPCBState   g_PCBState;	//全局板状态
extern CMotor      g_Motor;		//全局电机

#define WM_NOTIFYDATACHANGE WM_USER+100

#define WM_SHOW_IMAGE WM_USER+101

DWORD WINAPI WorkThread(LPVOID lpParam);

DWORD WINAPI ManualWorkThread(LPVOID lpParam);

DWORD WINAPI RefreshView(LPVOID lpParam);

// 参与排序的函数对象(从小到大排序)
bool Lesser(CFovData* elem1, CFovData* elem2)
{
	return elem1->CheckOrder < elem2->CheckOrder;
}

IMPLEMENT_DYNCREATE(CDetectBigGraphView, CBaseBigGraphView)

CDetectBigGraphView::CDetectBigGraphView()
	: CBaseBigGraphView(CDetectBigGraphView::IDD)
{
	m_iCurrentZoom = 0 ;
	m_lCurrentCenterX = 0;
	m_lCurrentCenterY = 0;
	m_lSrcOriginX = 0;
	m_lSrcOriginY = 0;
	m_CurrentRect =CRect(0,0,0,0);
	m_CurrentTImgRect = CRect(0,0,0,0);
	m_pCurrentImage = NULL;

	m_DectType = T_MANUAL;
	m_pCurrentTTemplateImg = NULL;
	m_pCurrentTImg = new CImage();

	m_iCurrentPCBOffsetx = 0;
	m_iCurrentPCBOffsety = 0;
	m_dThiDa = 0.00;
	m_iOrganOffsetx = 0;
	m_iOrganOffsety = 0;

	m_bReady2Organ = false;
	m_pDetectMasterData = NULL;
	m_hManualThread = NULL;
	m_bAskWhenErr = TRUE;
	m_pCurrentDatailData = NULL;

	m_bIsUseFovOrderList = false;
	m_preMousePoint = CPoint(0,0);
}

CDetectBigGraphView::~CDetectBigGraphView()
{
	if( m_pCurrentTImg != NULL)
	{
		if(!m_pCurrentTImg->IsNull())
		{
			m_pCurrentTImg->Destroy();
		}
		delete m_pCurrentTImg;
		m_pCurrentTImg = NULL;
	}

	map<CString,CDetectStatInfo*>::iterator it = m_DStatInfoMap.begin();
	for(;it != m_DStatInfoMap.end();it++)
	{
		CDetectStatInfo *pStatInfo = (*it).second;
		delete pStatInfo;
		pStatInfo = NULL;
	}
	m_DStatInfoMap.clear();
}

void CDetectBigGraphView::DoDataExchange(CDataExchange* pDX)
{
	CBaseBigGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDetectBigGraphView, CBaseBigGraphView)
	ON_MESSAGE(WM_NOTIFYDATACHANGE,NotifyDetectDataChanged)
	ON_MESSAGE(WM_SHOW_IMAGE,ShowImage)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDetectBigGraphView 诊断

#ifdef _DEBUG
void CDetectBigGraphView::AssertValid() const
{
	CBaseBigGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetectBigGraphView::Dump(CDumpContext& dc) const
{
	CBaseBigGraphView::Dump(dc);
}
#endif
#endif //_DEBUG

// 将FOV map转成list并从小到大排序list
void CDetectBigGraphView::GetFovListANdSort(void)
{
	if(m_bIsUseFovOrderList)
		return;

	m_FOVOrderList.clear();

	//拷贝
	EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	map<long,CFovData*>::iterator mapIt=m_pTemplateData->m_FOVMap.begin();
	for(;mapIt != m_pTemplateData->m_FOVMap.end();mapIt++)
	{
		m_FOVOrderList.push_back( (*mapIt).second );
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);

	if(m_FOVOrderList.size()<=1)
		return;

	//排序
	sort(m_FOVOrderList.begin(),m_FOVOrderList.end(),Lesser);
}

void CDetectBigGraphView::Init(void)
{
	GetFovListANdSort();
}

// CDetectBigGraphView 消息处理程序
void CDetectBigGraphView::SetTemplateData(CTemplateData * pTemplateData)
{
	CBaseBigGraphView::SetTemplateData(pTemplateData);

	m_pCurrentImage = NULL;
	m_pCurrentTTemplateImg = NULL;
	m_pCurrentTImg = new CImage();

	m_pDetectMasterData = NULL;	
	m_pCurrentDatailData = NULL;

	if(pTemplateData == NULL)
		return;

	Init();
}

void CDetectBigGraphView::OnZoomin()
{
	if(m_iCurrentZoom<5)
	{
		m_iCurrentZoom++;

		// 将image对象放大缩小后放到临时CImage对象中
		ZoomImageToTemp();
	}
	else
		return;

	Invalidate();//使整个工作区无效，触发OnPain刷新窗口
}

void CDetectBigGraphView::OnZoomout()
{
	if(m_iCurrentZoom>-4)
	{
		m_iCurrentZoom--;

		// 将image对象放大缩小后放到临时CImage对象中
		ZoomImageToTemp();
	}
	else
		return;

	Invalidate();
}

//测试基准点
bool CDetectBigGraphView::DetectDatumMark(CDatumMark *pBaseDatum[2],CDatumMark *pDatum[2])
{
	CString	sInfo;
	CImage	*pFovImag = NULL;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	long Xi[2] = {0}; //模板基准点的x坐标
	long Yi[2] = {0}; //模板基准点的y坐标

	long Xw[2] = {0}; //当前板基准点的x坐标
	long Yw[2] = {0}; //当前板基准点的y坐标

	if(pBaseDatum[0] == NULL || pBaseDatum[1] == NULL || pDatum[0] == NULL || pDatum[1] == NULL)
	{
		return false;
	}

	for(int i=0;i<2;i++)
	{
		CFovData *pFov = pDatum[i]->m_pFovData;
		//加载fov图像到当前显示图像中
		pFovImag = new CImage(); //modify at 2008-04-01
		LoadFovImage(CRect(pFov->left,pFov->top,pFov->right,pFov->bottom),
					pFovImag);

		::EnterCriticalSection(&m_Critial4CurrentImage);
		if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
		{
			m_pCurrentImage->Destroy();
			delete m_pCurrentImage;
			m_pCurrentImage = NULL;
		}
		m_pCurrentImage = pFovImag;
		::LeaveCriticalSection(&m_Critial4CurrentImage);

		//m_pCurrentImage放大缩小后放到临时CImage对象中
		ZoomImageToTemp();

		this->Invalidate();//显示

		Sleep(100);

		//获得当前基准点图像
		CImage* pMarkCurrentImage = new CImage();
		GetFactImage(CRect(pDatum[i]->searchLeft,pDatum[i]->searchTop,
			pDatum[i]->searchRight,pDatum[i]->searchBottom),pMarkCurrentImage);

		if(pMarkCurrentImage == NULL || pMarkCurrentImage->IsNull() || pDatum[i]->m_ImageObj.IsNull())
			continue;

		m_CurrentRect = CRect(pDatum[i]->searchLeft,pDatum[i]->searchTop,pDatum[i]->searchRight,pDatum[i]->searchBottom);
		m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
		m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;
		this->Invalidate();//显示

		int iKindred_Value=100;//相似值
		iKindred_Value = match( &(pDatum[i]->m_ImageObj),pMarkCurrentImage,&scoredata,80);//比较两个图像

#ifdef _MYUTEST
		if( pBaseDatum[i] ==  pDatum[i])
		{
			scoredata.Bx = pBaseDatum[i]->m_pFovData->left*PCBMULTIPLE + pBaseDatum[i]->left
				- pFov->left*PCBMULTIPLE - pDatum[i]->searchLeft;
			scoredata.By = pBaseDatum[i]->m_pFovData->top*PCBMULTIPLE + pBaseDatum[i]->top
				- pFov->top*PCBMULTIPLE - pDatum[i]->searchTop;
		}
		else
		{
			if(pFov->GetMidP().x == 93 && pFov->GetMidP().y == 69)
			{
				scoredata.Bx = 81;
				scoredata.By = 55;
			}
			else if(pFov->GetMidP().x == 395 && pFov->GetMidP().y == 263)
			{
				scoredata.Bx = 73;
				scoredata.By = 53;
			}
			else if(pFov->GetMidP().x == 574 && pFov->GetMidP().y == 68)
			{
				scoredata.Bx = 65;
				scoredata.By = 44;
			}
			else if(pFov->GetMidP().x == 875 && pFov->GetMidP().y == 260)
			{
				scoredata.Bx = 62;
				scoredata.By = 55;
			}
			else if(pFov->GetMidP().x == 573 && pFov->GetMidP().y == 399)
			{
				scoredata.Bx = 77;
				scoredata.By = 47;
			}
			else if(pFov->GetMidP().x == 872 && pFov->GetMidP().y == 593)
			{
				scoredata.Bx = 84;
				scoredata.By = 52;
			}
		}
#endif

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + pDatum[i]->m_ImageObj.GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + pDatum[i]->m_ImageObj.GetHeight();

		//如果相似值小于阀值
		if(iKindred_Value < m_pTemplateData->m_lDatumLimit)
		{
			CImage* pTmpMarkTImage = new CImage();
			pTmpMarkTImage->Create(pDatum[i]->m_ImageObj.GetWidth(),
				pDatum[i]->m_ImageObj.GetHeight(),
				pDatum[i]->m_ImageObj.GetBPP());

			pDatum[i]->m_ImageObj.BitBlt(pTmpMarkTImage->GetDC(),0,0,SRCCOPY);
			pTmpMarkTImage->ReleaseDC();

			//画框
			CBrush *pBrush = new CBrush(RGB(255,0,0));
			CRgn rg;
			rg.CreateRectRgnIndirect(&m_CurrentTImgRect);
			CDC dc;
			dc.Attach(pMarkCurrentImage->GetDC());
			dc.FrameRgn(&rg,pBrush,2,2);
			dc.Detach();
			pMarkCurrentImage->ReleaseDC();
			delete pBrush;
			pBrush = NULL;

			::PostMessage(this->m_hWnd,WM_SHOW_IMAGE,1,(LPARAM)pTmpMarkTImage);
			::PostMessage(this->m_hWnd,WM_SHOW_IMAGE,2,(LPARAM)pMarkCurrentImage);
			pMarkCurrentImage = NULL;
			
			CString sKindVal =g_LoadString("IDS_KINDRED_VALUE_S","相似值");
			sKindVal.Format("%s %d, ",sKindVal,iKindred_Value);
			CString sMsg = ::g_LoadString("IDS_MARK_NOMATCH","基准点检测不通过，请调整好PCB，然后按“确定”继续检测该板，否则按“取消”，取消检测此板");
			sInfo = sKindVal + sMsg;

			if(IDCANCEL == ::AfxMessageBox(sInfo,MB_OKCANCEL))
			{
				m_bReady2Organ = false;
				return false;
			}
			else
			{
				m_bReady2Organ = false;
				return DetectDatumMark(pBaseDatum,pDatum);
			}
		}

		if(!pMarkCurrentImage->IsNull())
			pMarkCurrentImage->Destroy();
		delete pMarkCurrentImage;
		pMarkCurrentImage = NULL;

		//检测成功，则计算当前板与模版之间的偏差
		int iFactOffsetx = scoredata.Bx;//x坐标实际偏差
		int iFactOffsety = scoredata.By;//y坐标实际偏差

		//得到模板第i个基准点的x,y坐标
		Xi[i] = pBaseDatum[i]->m_pFovData->left*PCBMULTIPLE + pBaseDatum[i]->left;
		Yi[i] = pBaseDatum[i]->m_pFovData->top*PCBMULTIPLE + pBaseDatum[i]->top;

		//得到当前板第i个基准点的x,y坐标
		Xw[i] = pFov->left*PCBMULTIPLE + (pDatum[i]->searchLeft + iFactOffsetx);
		Yw[i] = pFov->top*PCBMULTIPLE + (pDatum[i]->searchTop + iFactOffsety);
	}

	//顺序保持一致
	if( ((Xi[0] < Xi[1]) && (Xw[0] > Xw[1])) ||
		((Xi[0] > Xi[1]) && (Xw[0] < Xw[1]))
		)
	{
		long lTmp = Xw[0];
		Xw[0] = Xw[1];
		Xw[1] = lTmp;

		lTmp = Yw[0];
		Yw[0] = Yw[1];
		Yw[1] = lTmp;
	}

	double thiDa_i = 0.00;//模板角度
	double thiDa_w = 0.00;//当前板角度
	m_dThiDa = 0.00;
	m_dS = 1;

	//计算角度
	//公式：S=(Yw2-Yw1)/[(Xi2-Xi1)*sinQ+(Yi2-Yi1)*cosQ]
	//Q=atan{[(Xi2-Xi1)*(Yw2-Yw1) - (Xw2-Xw1)(Yi2-Yi1)]/[(Xw2-Xw1)*(Xi2-Xi1) + (Yi2-Yi1)*(Yw2-Yw1)]}
	m_dThiDa = atan( double( (Xi[1]-Xi[0])*(Yw[1]-Yw[0]) - (Yi[1]-Yi[0])*(Xw[1]-Xw[0]))/double((Xw[1]- Xw[0])*(Xi[1]-Xi[0]) +(Yi[1]-Yi[0])*(Yw[1]-Yw[0])) );
		m_dS = double(Yw[1]-Yw[0])/double((Xi[1]-Xi[0])*sin(m_dThiDa) +(Yi[1]-Yi[0])*cos(m_dThiDa));

	//计算位移量
	//m_dS = 1;//double(Xw[1]-Xw[0])/double( (Xi[1]-Xi[0])* cos(m_dThiDa) -(Yi[1]-Yi[0])*sin(m_dThiDa) );
	m_iCurrentPCBOffsetx = Xw[0] - m_dS*Xi[0] * cos(m_dThiDa) + m_dS*Yi[0] * sin(m_dThiDa);
	m_iCurrentPCBOffsety = Yw[0] - m_dS*Xi[0] * sin(m_dThiDa) - m_dS*Yi[0] * cos(m_dThiDa);

	return true;
}

// 开始检测PCB
bool CDetectBigGraphView::BeginDetect(void)
{
	CString	sInfo;
	CImage	*pFovImag = NULL;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//找基准点
	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	m_iCurrentPCBOffsetx = 0;
	m_iCurrentPCBOffsety = 0;

	CDatumMark *pDatum[2] = {0};
	int i = 0;

	map<long,CDatumMark *>::iterator markIt=m_pTemplateData->m_DatumMarkMap.begin();
	for(;markIt!=m_pTemplateData->m_DatumMarkMap.end();markIt++)
	{
		if((*markIt).second->type != 0)
			continue;

		pDatum[i] = (*markIt).second;
		i++;
		if(i >= 2)
			break;
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	if(!DetectDatumMark(pDatum,pDatum))
		return false;

	m_bReady2Organ = true;

	//检测的主数据
	//CDetectMasterData	dtMasterData;
	if( m_pDetectMasterData != NULL)
	{
		if(m_DetectBaseInfo.lStartId == 0)
		{
			m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
			m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData +" " +m_pDetectMasterData->DetectTime;
		}

		m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
		m_DetectBaseInfo.iDetPcbNum ++;
		if(m_pDetectMasterData->ErrorCount>0)
		{
			m_DetectBaseInfo.iErrPcbNum++;
		}
		m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
		m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

		delete m_pDetectMasterData;
		m_pDetectMasterData =NULL;
	}

	m_pDetectMasterData = new CDetectMasterData();
	m_pDetectMasterData->pcbName = m_pTemplateData->m_templateName;

	CTime t = CTime::GetCurrentTime();
	m_pDetectMasterData->DetectData = t.Format("%Y-%m-%d");
	m_pDetectMasterData->DetectTime = t.Format("%H:%M");
	m_pDetectMasterData->Detecter = g_sUserName;

	//条形码
	if(m_pTemplateData->m_pBar !=NULL)
	{
		CFovData *pFov = m_pTemplateData->m_pBar->m_pFovData;
		if( pFov != NULL)
		{

			//加载fov图像到当前显示图像中
			pFovImag = new CImage();

			LoadFovImage(CRect(pFov->left,pFov->top,pFov->right,pFov->bottom),pFovImag);
			::EnterCriticalSection(&m_Critial4CurrentImage);

			if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
			{
				m_pCurrentImage->Destroy();
				delete m_pCurrentImage;
				m_pCurrentImage = NULL;
			}
			m_pCurrentImage = pFovImag;

			::LeaveCriticalSection(&m_Critial4CurrentImage);

			//获得条形码的图像
			/*CImage* pBarImage;
			GetFactImage(CRect(m_pTemplateData->m_pBar->barSerchLeft,m_pTemplateData->m_pBar->barSerchTop,
				m_pTemplateData->m_pBar->barSerchRight,m_pTemplateData->m_pBar->barSerchBottom),
				pBarImage);*/
			//TODO
			//根据条形码的图像获得条形码数据
		}
	}

	return true;
}

/* 检查元件是否在FOV内
在返回true,否则返回false
*/
bool CDetectBigGraphView::OrganInFov(const CFovData*  pFovData, const COrganData* pOrganData)
{
	long	lOrganX;//元件中心X点
	long	lOrganY;//元件中心Y点

	lOrganX = pOrganData->dMidX * m_pTemplateData->m_lCoordScaleX + m_pTemplateData->m_lOffsetX;
	
	//CAD坐标原点是在左下角
	lOrganY = m_pTemplateData->m_ImageObj.GetHeight() - (pOrganData->dMidY * m_pTemplateData->m_lCoordScaleY + m_pTemplateData->m_lOffsetY);

	CRect fovRect(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);

	if(true == fovRect.PtInRect( CPoint(lOrganX,lOrganY) ))
	{
		int iImagWidth = pFovData->m_ImageObj.GetWidth();
		int iImagHeight = pFovData->m_ImageObj.GetHeight();

		if( (pOrganData->right < iImagWidth) && ( pOrganData->bottom < iImagHeight) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

/* 检测元件
*pTemplatOrgan：为模板元件
*SerchRect:元件搜索范围
*检查通过返回true
*/
bool CDetectBigGraphView::DetectOrgan(COrganData* pTemplatOrgan,CRect SerchRect,int& kindred_value)
{
	if( pTemplatOrgan == NULL )
		return true;

	bool	bDetectResult=false;

	//获得元件搜索范围内的图像
	//CImage* pOrganSerchtImage = new CImage();
	GetFactImage(SerchRect ,m_pCurrentTImg);
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//先检查元件主体
	CRect organRect(pTemplatOrgan->left,pTemplatOrgan->top,pTemplatOrgan->right,pTemplatOrgan->bottom);

	//GetFactImage(rect,m_pCurrentTImg);//当前检测的图像

	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	int iOrgOffsetx = 0;	//元件实际位置与模板的x坐标偏移
	int iOrgOffsety = 0;	//元件实际位置与模板的y坐标偏移

	int count = 0;
	while(1)
	{
		count++;

		//遍历主体所有标准图片
		vector<CImage*>::const_iterator mainBodyIt = pTemplatOrgan->m_MainBodyImageList.begin();
		for(;mainBodyIt != pTemplatOrgan->m_MainBodyImageList.end();mainBodyIt++)
		{
			while(g_bPauseDetect)
			{
				Sleep(30);
			}

			m_pCurrentTTemplateImg = (*mainBodyIt); //标准图像
			if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
				m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
				continue;

			::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

			kindred_value = match( (*mainBodyIt),m_pCurrentTImg,&scoredata);//比较两个图像

			m_CurrentTImgRect.left = scoredata.Bx;
			m_CurrentTImgRect.top = scoredata.By;
			m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*mainBodyIt)->GetWidth();
			m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*mainBodyIt)->GetHeight();

			//检查元件在搜索区域中实际的偏差
			int iFactOffsetx = scoredata.Bx;//x坐标实际偏差
			int iFactOffsety = scoredata.By;//y坐标实际偏差

			if(kindred_value >= pTemplatOrgan->iLimit)
			{
				//元件检测成功，则检查元件在搜索区域中实际的偏差
				int iTOffsetx = pTemplatOrgan->left - SerchRect.left;	//x坐标模板偏差
				int iTOffsety = pTemplatOrgan->top - SerchRect.top;		//y坐标模板偏差

				iOrgOffsetx = iFactOffsetx - iTOffsetx;	//元件实际位置与模板的x坐标偏移
				iOrgOffsety = iFactOffsety - iTOffsety;	//元件实际位置与模板的y坐标偏移

				bDetectResult = true;
				break;
			}
		}

		if(bDetectResult==false) //上面的检查没有通过
		{
			if(count == 1 && pTemplatOrgan->iIsPolar == 0)
			{
				//如果元件不是极性，翻转图像后再次测试
				Flip(m_pCurrentTImg,3);
				continue;
			}
			else
			{
				m_pCurrentDatailData->m_pErrorOrgan = pTemplatOrgan;
				return false;
			}
		}
		else
		{
			break;
		}
	}

	//检查焊盘和焊脚
	for(int i=0;i<4;i++)
	{
		if( !DetectPadAndFoot(pTemplatOrgan,pTemplatOrgan->m_pPadData[i],iOrgOffsetx,iOrgOffsety,kindred_value) )
		{
			return false;
		}
	}
	return true;
}

/* 检测焊盘和焊脚
*pTemplatOrgan：为模板元件
*pTemplatPad：为模板焊盘
*iOrgOffsetx:元件实际位置与模板的x坐标偏移
*iOrgOffsety:元件实际位置与模板的y坐标偏移
*检查通过返回true
*/
bool CDetectBigGraphView::DetectPadAndFoot(COrganData* pTemplatOrgan, CPadData* pTemplatPad,int iOrgOffsetx,int iOrgOffsety,int& kindred_value)
{
	if( pTemplatPad == NULL)
		return true;

	bool	bDetectResult=false;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//检查焊盘
	CRect rect(pTemplatPad->left+iOrgOffsetx,pTemplatPad->top+iOrgOffsety,pTemplatPad->right+iOrgOffsetx,
		pTemplatPad->bottom+iOrgOffsety);
	rect.InflateRect(12,12);	//再将焊盘区域扩大12个像素

	GetFactImage(rect,m_pCurrentTImg);//当前检测的图像
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//遍历所有标准图片
	vector<CImage*>::const_iterator padIt = pTemplatPad->m_ImageList.begin();
	for(;padIt != pTemplatPad->m_ImageList.end();padIt++)
	{
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		m_pCurrentTTemplateImg = (*padIt); //标准图像
		if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
			m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
			continue;

		::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

		kindred_value = match( (*padIt),m_pCurrentTImg,&scoredata);//比较两个图像

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*padIt)->GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*padIt)->GetHeight();

		//检查焊盘在搜索区域中实际的偏差
		int iFactOffsetx = scoredata.Bx;//x坐标实际偏差
		int iFactOffsety = scoredata.By;//y坐标实际偏差

		if(kindred_value >= pTemplatOrgan->iLimit)
		{
			bDetectResult = true;
			break;
		}

		//取新图像
		/*::EnterCriticalSection(&m_Critial4CurrentImage);

		CImage *pTempImage = m_pCurrentTImg;
		m_pCurrentTImg = new CImage();//创建新的空间接收新的图像
		if(pTempImage != NULL && !pTempImage->IsNull())
		{
			CRect padRect(pTemplatPad->left,pTemplatPad->top,pTemplatPad->right,pTemplatPad->bottom);
			int iImageWidth = padRect.Width();
			int iImageHeight = padRect.Height();

			m_pCurrentTImg->Create(iImageWidth,iImageHeight,pTempImage->GetBPP());
			pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,iImageWidth,iImageHeight,iFactOffsetx,iFactOffsety,SRCCOPY);
			m_pCurrentTImg->ReleaseDC();

			pTempImage->Destroy();
			delete pTempImage;
			pTempImage = NULL;
		}

		::LeaveCriticalSection(&m_Critial4CurrentImage);*/

	}

	//上面的检查没有通过
	if(bDetectResult==false)
	{
		m_pCurrentDatailData->m_pErrorPad = pTemplatPad;
		return false;
	}

	//检查所有焊脚
	vector<CFootData*>::const_iterator footIt = pTemplatPad->m_footList.begin();
	for(;footIt!=pTemplatPad->m_footList.end();footIt++)
	{
		if(false == DetectFoot(pTemplatOrgan,(*footIt),iOrgOffsetx,iOrgOffsety,kindred_value) )
			return false;
	}

	return true;
}

/* 检测焊脚
*pTemplatOrgan：为模板元件
*pTemplateFoot：为模板焊脚
*iOrgOffsetx:元件实际位置与模板的x坐标偏移
*iOrgOffsety:元件实际位置与模板的y坐标偏移
*kindred_value:相似值
*检查通过返回true
*/
bool CDetectBigGraphView::DetectFoot(COrganData* pTemplatOrgan , CFootData* pTemplateFoot,int iOrgOffsetx,int iOrgOffsety,int& kindred_value)
{
	if( pTemplateFoot == NULL)
		return true;

	bool	bDetectResult=false;
	SCOREDATA scoredata;
	memset(&scoredata,0,sizeof(scoredata));

	//检查焊脚
	CRect rect(pTemplateFoot->left+iOrgOffsetx,pTemplateFoot->top+iOrgOffsety,pTemplateFoot->right+iOrgOffsetx,
		pTemplateFoot->bottom+iOrgOffsety);
	rect.InflateRect(6,6);	//再将焊脚区域扩大6个像素

	GetFactImage(rect,m_pCurrentTImg);//当前检测的图像
	if(m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
		return false;

	//遍历所有标准图片
	vector<CImage*>::const_iterator footIt = pTemplateFoot->m_ImageList.begin();
	for(;footIt != pTemplateFoot->m_ImageList.end();footIt++)
	{
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		m_pCurrentTTemplateImg = (*footIt); //标准图像
		if(m_pCurrentTTemplateImg == NULL || m_pCurrentTTemplateImg->IsNull() ||
			m_pCurrentTImg == NULL || m_pCurrentTImg->IsNull())
			continue;

		::PostMessage(this->m_hWnd,WM_NOTIFYDATACHANGE,NULL,NULL);//NotifyDetectDataChanged();

		kindred_value = match( (*footIt),m_pCurrentTImg,&scoredata);//比较两个图像

		m_CurrentTImgRect.left = scoredata.Bx;
		m_CurrentTImgRect.top = scoredata.By;
		m_CurrentTImgRect.right = m_CurrentTImgRect.left + (*footIt)->GetWidth();
		m_CurrentTImgRect.bottom = m_CurrentTImgRect.top + (*footIt)->GetHeight();

		//检查焊盘在搜索区域中实际的偏差
		int iFactOffsetx = scoredata.Bx;//x坐标实际偏差
		int iFactOffsety = scoredata.By;//y坐标实际偏差

		if(kindred_value >= pTemplatOrgan->iLimit)
		{
			bDetectResult = true;
			break;
		}

	}

	if(bDetectResult == false)
	{
		m_pCurrentDatailData->m_pErrorFoot = pTemplateFoot;
	}

	return bDetectResult;
}

//设定为自动模式
void CDetectBigGraphView::TAuto()
{
	m_DectType = T_AUTO;
	m_bStopWork = false;

	DWORD dwThreadId;
	HANDLE hThread=::CreateThread(NULL,0,WorkThread,(LPVOID)this,0,&dwThreadId);//创建线程	
}

//设定为手动模式
void CDetectBigGraphView::TManual()
{
	TStop();
	m_DectType = T_MANUAL;
}

//停止检测
void CDetectBigGraphView::TStop()
{
	m_DectType = T_MANUAL;
	m_bStopWork = true;
}


//x轴回到原点
bool CDetectBigGraphView::TOriginx()
{
    g_Motor.GoHome(1,100,10);
	return true;
}

//y轴回到原点
bool CDetectBigGraphView::TOriginy()
{
    g_Motor.GoHome(2,100,10);
	return true;
}

//元件检测
void CDetectBigGraphView::TOrgan()
{
	if( m_bReady2Organ == false || m_pDetectMasterData == NULL)
	{
		if(false == BeginDetect())
			return;
	}

    COrganData *pOrgan=NULL;
	PrgState selectState[]={PS_COMPLETE};

	if(m_DectType == T_MANUAL)
	{
		DWORD dwThreadId;
		DWORD dwExitCode;
		
		if( m_hManualThread != NULL)
		{
			::GetExitCodeThread(m_hManualThread,&dwExitCode);
			if(STILL_ACTIVE == dwExitCode)
				return;
		}

        COrganSelectDlg selectOrgan(m_pTemplateData,&pOrgan,this,&selectState[0],1);

	    if(IDOK!=selectOrgan.DoModal() || pOrgan == NULL)
	    {
		    return;
	    }

	    CFovData *pCurrentFov = pOrgan->m_pFovData;

	    CRect fovRect(pCurrentFov->left,pCurrentFov->top,pCurrentFov->right,pCurrentFov->bottom);

		fovRect.left = pCurrentFov->left*PCBMULTIPLE*cos(m_dThiDa)*m_dS
		- pCurrentFov->top*PCBMULTIPLE*sin(m_dThiDa)*m_dS +m_iCurrentPCBOffsetx;
		fovRect.top = pCurrentFov->left*PCBMULTIPLE* sin(m_dThiDa)*m_dS
			+ pCurrentFov->top*PCBMULTIPLE* cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

		m_iOrganOffsetx = fovRect.left%PCBMULTIPLE;
		m_iOrganOffsety = fovRect.top%PCBMULTIPLE;
		fovRect.left = fovRect.left/PCBMULTIPLE;
		fovRect.top = fovRect.top/PCBMULTIPLE;

		fovRect.right = fovRect.left + (pCurrentFov->right - pCurrentFov->left);
		fovRect.bottom = fovRect.top + (pCurrentFov->bottom - pCurrentFov->top);

	    CImage	*pFovImag = new CImage();
	    //CImage* pOrganSerchtImage = new CImage();

	    //加载fov图像到当前显示图像中
	    LoadFovImage(fovRect,pFovImag);

        if(!pFovImag->IsNull())
        {
			::EnterCriticalSection(&m_Critial4CurrentImage);

            if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
            {
                m_pCurrentImage->Destroy();
                delete m_pCurrentImage;
                m_pCurrentImage = NULL;
            }
	        m_pCurrentImage = pFovImag;

			::LeaveCriticalSection(&m_Critial4CurrentImage);
        }
        else
        {
            return;
        }

	    //m_pCurrentImage放大缩小后放到临时CImage对象中
	    ZoomImageToTemp();
        m_pCurrentTOrgan = pOrgan;

        m_hManualThread=::CreateThread(NULL,0,ManualWorkThread,(LPVOID)this,0,&dwThreadId);//创建线程

		//this->ManulDetectOrgan();
	}
	else
	{
		m_bIsUseFovOrderList = true;
		this->AutoDetectOrgan();
		m_bIsUseFovOrderList = false;
	}
}

//放下PCB板
bool CDetectBigGraphView::TPcbdown()
{
	m_bReady2Organ = false;

	//保存数据
	CString sErrInfo;

    g_PCBState.Down();

	if(m_DectType == T_MANUAL && m_pDetectMasterData != NULL)
	{

		if(false==m_DetectDataMgr.Store(m_pDetectMasterData,sErrInfo))
		{
			return true;
		}

		if(m_DetectBaseInfo.lStartId == 0)
		{
			m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
			m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData  +" " + m_pDetectMasterData->DetectTime;
		}

		m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
		m_DetectBaseInfo.iDetPcbNum ++;
		if(m_pDetectMasterData->ErrorCount>0)
		{
			m_DetectBaseInfo.iErrPcbNum++;
		}
		m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
		m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

		delete m_pDetectMasterData;
		m_pDetectMasterData = NULL;

		m_bReady2Organ = false;
	}

	return true;

}


//自动检测的工作线程
DWORD WINAPI WorkThread(LPVOID lpParam)
{
	CDetectBigGraphView *pBigView=(CDetectBigGraphView *)lpParam;
	while(1)
	{
		if(pBigView->m_bStopWork)
			break;

        if(!g_PCBState.Up(false))
        {
            Sleep(50);
			continue;
        }

		if(pBigView->m_bStopWork)
			break;

		pBigView->TOrgan();

		if(pBigView->m_bStopWork)
			break;

		pBigView->TPcbdown();

        g_PCBState.Loose();

        g_PCBState.SendOut();
	}

	return 0;
}

//手动检测的工作线程
DWORD WINAPI ManualWorkThread(LPVOID lpParam)
{
	CDetectBigGraphView *pBigView=(CDetectBigGraphView *)lpParam;
	pBigView->ManulDetectOrgan();
	return 0;
}

LRESULT CDetectBigGraphView::NotifyDetectDataChanged(WPARAM wParam, LPARAM lParam)
{
	//CDetectMainView *pParrent=(CDetectMainView*)this->GetParent();

	CDetectMainView *pParrent = (CDetectMainView*)m_pParrent;
	if( pParrent && pParrent->IsKindOf( RUNTIME_CLASS(CDetectMainView) ) )
	{
		pParrent->NotifyDetectDataChanged();
	}
	return 0;
}

LRESULT CDetectBigGraphView::ShowImage(WPARAM wParam, LPARAM lParam)
{
	CImage *pImage = (CImage*)lParam;
	CString sTitle = "";
	switch(wParam)
	{
	case 1:
		sTitle = "基准点标准图像";
		break;
	case 2:
		sTitle = "基准点当前图像";
		break;
	default:
		break;
	}
	CObjectImageDlg *pShowImageDlg = new CObjectImageDlg(pImage,sTitle,this);
	pShowImageDlg->Create(IDD_OBJIMAGE,this);
	pShowImageDlg->ShowWindow(SW_SHOW);

	return 0;
}

// 比较两个图像之间的相似度
//pImag1:模板图象
//pImag2：要检测的图象
//返回相似度
int CDetectBigGraphView::match(CImage *pImag1, CImage *pImag2,struct SCOREDATA* pScoreData,int ar)
{
#ifndef _MYUTEST
	if(pImag2->IsNull() || pImag1->IsNull())
	{
		return 0;
	}

	SCOREDATA scoreData = ::Match(pImag2,pImag1,ar);

	if(pScoreData != NULL)
	{
		memcpy(pScoreData,&scoreData,sizeof(scoreData));
	}

	int iScore = min(scoreData.scoreR,scoreData.scoreG);
	return  min(iScore,scoreData.scoreB);

#else
	memset(pScoreData,0,sizeof(struct SCOREDATA));
	pScoreData->scoreB=pScoreData->scoreG=pScoreData->scoreR = 40;
	Sleep(100);
	return 100;
#endif
}

void CDetectBigGraphView::OnInitialUpdate()
{
	CBaseBigGraphView::OnInitialUpdate();

	m_pParrent = this->GetParent();
}

void CDetectBigGraphView::OnPaint()
{

	CPaintDC dc(this); // device context for painting

	if(m_pTemplateData == NULL)
		return;

	if(m_pCurrentImage == NULL || m_pCurrentImage->IsNull())
	{
		//return CBaseBigGraphView::OnPaint();
		return;
	}

	CRect rc;
	GetClientRect(rc);

	//清除窗口
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(0,0,1023,639);

	ZoomImageToTemp();
    //重新设置中心点和原点
	ComputerCenterAndOrigin(m_lCurrentCenterX, m_lCurrentCenterY);	

	m_TempImage.BitBlt(dc,0,0,m_TempImage.GetWidth(),m_TempImage.GetHeight(),m_lSrcOriginX,m_lSrcOriginY,SRCCOPY);

    if(!m_CurrentRect.IsRectEmpty() && !m_CurrentRect.IsRectNull())
	{
		CBrush *pBrush = new CBrush(RGB(255,0,0));
		CRect rect =m_CurrentRect;
        ZoomRectOnOrigin(rect);

		rect.left -= m_lSrcOriginX;
		rect.right -= m_lSrcOriginX;
		rect.top -= m_lSrcOriginY;
		rect.bottom -= m_lSrcOriginY;

		dc.FrameRect(&rect,pBrush);
		delete pBrush;
		pBrush = NULL;
	}

}

// 自动检测元件
void CDetectBigGraphView::AutoDetectOrgan(void)
{
	m_bHasError  = false;
	bool bCanLoadFovImag = false;

	CDatumMark *pDatum[2] = {0};

	CImage	*pFovImag = NULL;
	//CImage* pOrganSerchtImage = new CImage();

	EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	CDatumMark *pBaseDatum[2] = {0};
	int count = 0;

	map<long,CDatumMark *>::iterator markIt=m_pTemplateData->m_DatumMarkMap.begin();
	for(;markIt!=m_pTemplateData->m_DatumMarkMap.end();markIt++)
	{
		if((*markIt).second->type != 0)
			continue;

		pBaseDatum[count] = (*markIt).second;
		count++;
		if(count >= 2)
			break;
	}

	LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

	EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	if(m_pTemplateData->m_SpellPcbMap.find(0) == m_pTemplateData->m_SpellPcbMap.end())
	{
		CSpellPCB *pSpellPCB = new CSpellPCB();
		m_pTemplateData->m_SpellPcbMap.insert(::make_pair(0,pSpellPCB));
	}
	
	map<int,CSpellPCB*>::iterator spellIt = m_pTemplateData->m_SpellPcbMap.begin();
	for(int i=0;i < m_pTemplateData->m_SpellPcbMap.size();i++)
	{
		CSpellPCB	*pSpellPCB = NULL;

		if(i == 0)//第一次必须检测原板
		{
			pSpellPCB = m_pTemplateData->m_SpellPcbMap[0];
			if( (*spellIt).first == 0)
			{
				spellIt++;
			}
		}
		else
		{
			if( (*spellIt).first == 0)
			{
				spellIt++;
			}
			pSpellPCB = (*spellIt).second;
			spellIt++;
		}

		if(pSpellPCB == NULL)
			continue;

		if(pSpellPCB->pDatumMark1 != NULL && pSpellPCB->pDatumMark2 != NULL)
		{
			pDatum[0] = pSpellPCB->pDatumMark1;
			pDatum[1] = pSpellPCB->pDatumMark2;

			if(!DetectDatumMark(pBaseDatum,pDatum))
				continue;
		}

		LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);
		while(g_bPauseDetect)
		{
			Sleep(30);
		}

		vector<CFovData*>::iterator it = m_FOVOrderList.begin();
		for(;it!=m_FOVOrderList.end();it++) //循环检测所有的FOV
		{
			if(m_bStopWork || g_bStopWork)
			{
				return;
			}

			while(g_bPauseDetect)
			{
				Sleep(30);
			}

			//不是基本板上的FOV
			if( (*it)->type != 0)
			{
				continue;
			}

			CRect tempFovRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);

			CRect fovRect1(0,0,0,0);
			
			//校正原板FOV
			fovRect1.left = (tempFovRect.left*PCBMULTIPLE) * cos(m_dThiDa)*m_dS
				- (tempFovRect.top*PCBMULTIPLE) * sin(m_dThiDa)*m_dS +m_iCurrentPCBOffsetx;
			fovRect1.top = (tempFovRect.left*PCBMULTIPLE) * sin(m_dThiDa)*m_dS
				+ (tempFovRect.top*PCBMULTIPLE) * cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

			m_iOrganOffsetx = fovRect1.left%PCBMULTIPLE;
			m_iOrganOffsety = fovRect1.top%PCBMULTIPLE;
			fovRect1.left = fovRect1.left/PCBMULTIPLE;
			fovRect1.top = fovRect1.top/PCBMULTIPLE;

			fovRect1.right = fovRect1.left + (tempFovRect.right - tempFovRect.left);
			fovRect1.bottom = fovRect1.top + (tempFovRect.bottom - tempFovRect.top);

			bCanLoadFovImag = false;

			EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
			map<long,COrganData*>::iterator organTmpIt=m_pTemplateData->m_OrganDataMap.begin();
			for(;organTmpIt != m_pTemplateData->m_OrganDataMap.end();organTmpIt++)
			{
				if( (*it)->fovId == (*organTmpIt).second->fovId && (*organTmpIt).second->PState == PS_COMPLETE)
				{
					bCanLoadFovImag = true;
					break;
				}
			}
			::LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

			/**加载fov图像到当前显示图像中 --->>>>*/
			if( bCanLoadFovImag )
			{
				pFovImag = new CImage();
				LoadFovImage(fovRect1,pFovImag,true);

				::EnterCriticalSection(&m_Critial4CurrentImage);

				if(m_pCurrentImage != NULL && !m_pCurrentImage->IsNull())
				{
					m_pCurrentImage->Destroy();
					delete m_pCurrentImage;
					m_pCurrentImage = NULL;
				}
				m_pCurrentImage = pFovImag;
				m_CurrentRect = CRect(0,0,0,0);

				::LeaveCriticalSection(&m_Critial4CurrentImage);

				//m_pCurrentImage放大缩小后放到临时CImage对象中
				ZoomImageToTemp();
				this->Invalidate();//显示
			}
			else
			{
				continue;
			}
			/**end 加载fov图像到当前显示图像中<<<<----*/
			
			//遍历所有的元件
			EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
			map<long,COrganData*>::iterator organMapIt=m_pTemplateData->m_OrganDataMap.begin();
			for(;organMapIt != m_pTemplateData->m_OrganDataMap.end();organMapIt++)
			{
				LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
				if(m_bStopWork || g_bStopWork)
					return;

				while(g_bPauseDetect)
				{
					Sleep(30);
				}

				EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
				if((*organMapIt).second->PState == PS_SKIP || (*organMapIt).second->PState == PS_INVALID 
					|| (*organMapIt).second->bNeedDetect == false)
				{
					continue; //忽略此元件
				}
				else if( (*it)->fovId == (*organMapIt).second->fovId && // OrganInFov( (*it),(*organMapIt).second ) &&
					(*organMapIt).second->PState == PS_COMPLETE)
				{//元件在此fov范围内并且已经编程完毕

					m_pDetectMasterData->DetectCount++;//元件数加

					m_pCurrentTOrgan = (*organMapIt).second;

					CDetectDetailData	*pDetectDatailData = new CDetectDetailData();//记录检测数据
					m_pCurrentDatailData = pDetectDatailData;

					m_pDetectMasterData->m_Details.push_back(pDetectDatailData);
					pDetectDatailData->organName = m_pCurrentTOrgan->sName;
					pDetectDatailData->spellNo = pSpellPCB->no;
					pDetectDatailData->centerX = m_pCurrentTOrgan->GetMidP().x;
					pDetectDatailData->centerY = m_pCurrentTOrgan->GetMidP().y;
					pDetectDatailData->isPass = 1;
					pDetectDatailData->limit_value = m_pCurrentTOrgan->iLimit;

					CRect serchRect(m_pCurrentTOrgan->RangeLeft+m_iOrganOffsetx,m_pCurrentTOrgan->RrangeTop+m_iOrganOffsety,
						m_pCurrentTOrgan->RangeRight+m_iOrganOffsetx,m_pCurrentTOrgan->RangeBottom+m_iOrganOffsety);

					//serchRect.left = m_pCurrentTOrgan->RangeLeft * cos(m_dThiDa) - m_pCurrentTOrgan->RrangeTop * sin(m_dThiDa) +m_iCurrentPCBOffsetx;
					//serchRect.top = m_pCurrentTOrgan->RangeLeft * sin(m_dThiDa) + m_pCurrentTOrgan->RrangeTop * cos(m_dThiDa) +m_iCurrentPCBOffsety;
					//serchRect.right = serchRect.left + (m_pCurrentTOrgan->RangeRight - m_pCurrentTOrgan->RangeLeft);
					//serchRect.bottom = serchRect.top + (m_pCurrentTOrgan->RangeBottom - m_pCurrentTOrgan->RrangeTop);

					m_CurrentRect = serchRect;
					m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
					m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;
					this->Invalidate();//显示

					LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);
					bool bResult=DetectOrgan( m_pCurrentTOrgan,serchRect,pDetectDatailData->kindred_value);//检测元件
					EnterCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

					if(bResult == false)
					{
						m_bHasError = true;
						pDetectDatailData->isPass = 0;
						pDetectDatailData->m_pTemplateImg = m_pCurrentTTemplateImg;

						::EnterCriticalSection(&m_Critial4CurrentImage);

						CImage *pTempImage = m_pCurrentTImg;
						pDetectDatailData->m_pCurrentImg = m_pCurrentTImg;//保存当前图像
						pDetectDatailData->m_CurrentTImgRect = m_CurrentTImgRect;
						m_pCurrentTImg = new CImage();//创建新的空间接收新的图像
						if(pTempImage != NULL && !pTempImage->IsNull())
						{
							m_pCurrentTImg->Create(pTempImage->GetWidth(),pTempImage->GetHeight(),pTempImage->GetBPP());
							pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,pTempImage->GetWidth(),pTempImage->GetHeight(),0,0,SRCCOPY);
							m_pCurrentTImg->ReleaseDC();
						}

						::LeaveCriticalSection(&m_Critial4CurrentImage);
					}

					#ifdef _MYUTEST
					::Sleep(30);
					#endif

					//pOrganSerchtImage->Destroy();
				}
			}//end 遍历所有的元件
			LeaveCriticalSection(&m_pTemplateData->m_Critial4OrganDataMap);

		}//end 循环检测所有的FOV
		EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	}//end 循环检测所有的拼板
	LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);

	this->Invalidate();
    //RefreshUITread();

	if(m_bHasError)//有元件检测没有通过
	{
		//验证错误
		if(m_bAskWhenErr)
		{
			CValidateErr validateErr(m_pDetectMasterData,&m_DStatInfoMap,this);
			validateErr.DoModal();
		}
		else
		{
			AutoAdjustErr();
		}
	}

	if(m_pDetectMasterData == NULL)
		return;

	//保存数据
	CString sErrInfo;
	
	if(false==m_DetectDataMgr.Store(m_pDetectMasterData,sErrInfo))
	{
		sErrInfo ="保存失败\n"+sErrInfo;
		::AfxMessageBox(sErrInfo);
		return;
	}

	if(m_DetectBaseInfo.lStartId == 0)
	{
		m_DetectBaseInfo.lStartId = m_pDetectMasterData->id;
		m_DetectBaseInfo.sStartTime = m_pDetectMasterData->DetectData  +" " + m_pDetectMasterData->DetectTime;
	}

	m_DetectBaseInfo.lCurrentId = m_pDetectMasterData->id+1;
	m_DetectBaseInfo.iDetPcbNum ++;
	if(m_pDetectMasterData->ErrorCount>0)
	{
		m_DetectBaseInfo.iErrPcbNum++;
	}
	m_DetectBaseInfo.iDetOrganNum += m_pDetectMasterData->DetectCount;
	m_DetectBaseInfo.iErrOrganNum += m_pDetectMasterData->ErrorCount;

	m_bReady2Organ = false;
	delete m_pDetectMasterData;
	m_pDetectMasterData = NULL;
}

// 手动检测元件
void CDetectBigGraphView::ManulDetectOrgan(void)
{
	m_bHasError  = false;

	m_pDetectMasterData->DetectCount++;//元件数加

	CDetectDetailData	*pDetectDatailData = new CDetectDetailData();//记录检测数据
    /*if(m_pCurrentDatailData != NULL)
    {
        delete m_pCurrentDatailData;
        m_pCurrentDatailData = NULL;
    }*/
	m_pCurrentDatailData = pDetectDatailData;

	CDetectMasterData	TmpMasterData;
	TmpMasterData.m_Details.push_back(pDetectDatailData);
	m_pDetectMasterData->m_Details.push_back(pDetectDatailData);
	pDetectDatailData->organName = m_pCurrentTOrgan->sName;
	pDetectDatailData->centerX = m_pCurrentTOrgan->GetMidP().x;
	pDetectDatailData->centerY = m_pCurrentTOrgan->GetMidP().y;
	pDetectDatailData->isPass = 1;
	pDetectDatailData->limit_value = m_pCurrentTOrgan->iLimit;

	CRect serchRect(m_pCurrentTOrgan->RangeLeft+m_iOrganOffsetx,m_pCurrentTOrgan->RrangeTop+m_iOrganOffsety,
		m_pCurrentTOrgan->RangeRight+m_iOrganOffsetx,m_pCurrentTOrgan->RangeBottom+m_iOrganOffsety);
	//CRect serchRect;

	CFovData *pFov = m_pCurrentTOrgan->m_pFovData;

	/*serchRect.left = (pFov->left*PCBMULTIPLE + m_pCurrentTOrgan->RangeLeft) * cos(m_dThiDa) 
		- (pFov->top*PCBMULTIPLE + m_pCurrentTOrgan->RrangeTop) * sin(m_dThiDa) +m_iCurrentPCBOffsetx;
	serchRect.top = (pFov->left*PCBMULTIPLE + m_pCurrentTOrgan->RangeLeft) * sin(m_dThiDa)*m_dS
		+ (pFov->top*PCBMULTIPLE + m_pCurrentTOrgan->RrangeTop) * cos(m_dThiDa)*m_dS +m_iCurrentPCBOffsety;

	serchRect.left -= pFov->left*PCBMULTIPLE;
	serchRect.top -= pFov->top*PCBMULTIPLE;

	serchRect.right = serchRect.left + (m_pCurrentTOrgan->RangeRight - m_pCurrentTOrgan->RangeLeft);
	serchRect.bottom = serchRect.top + (m_pCurrentTOrgan->RangeBottom - m_pCurrentTOrgan->RrangeTop);*/

	m_CurrentRect = CRect(serchRect.left,serchRect.top,serchRect.right,serchRect.bottom);
	m_lCurrentCenterX = m_CurrentRect.left + m_CurrentRect.Width()/2;
	m_lCurrentCenterY = m_CurrentRect.top + m_CurrentRect.Height()/2;

	m_CurrentRect2= CRect(m_pCurrentTOrgan->RangeLeft,m_pCurrentTOrgan->RrangeTop,m_pCurrentTOrgan->RangeRight,m_pCurrentTOrgan->RangeBottom);

	this->Invalidate();//显示

	bool bResult=DetectOrgan( m_pCurrentTOrgan,serchRect,pDetectDatailData->kindred_value);//检测元件
	if(bResult == false)
	{
		m_bHasError = true;
		pDetectDatailData->isPass = 0;
		pDetectDatailData->m_pTemplateImg = m_pCurrentTTemplateImg;

		::EnterCriticalSection(&m_Critial4CurrentImage);

		CImage *pTempImage = m_pCurrentTImg;
		pDetectDatailData->m_pCurrentImg = m_pCurrentTImg;//保存当前图像
		pDetectDatailData->m_CurrentTImgRect = m_CurrentTImgRect;
		m_pCurrentTImg = new CImage();//创建新的空间接收新的图像
		if(pTempImage != NULL && !pTempImage->IsNull())
		{
			m_pCurrentTImg->Create(pTempImage->GetWidth(),pTempImage->GetHeight(),pTempImage->GetBPP());
			pTempImage->BitBlt(m_pCurrentTImg->GetDC(),0,0,pTempImage->GetWidth(),pTempImage->GetHeight(),0,0,SRCCOPY);
			m_pCurrentTImg->ReleaseDC();
		}

		::LeaveCriticalSection(&m_Critial4CurrentImage);
	}

	this->Invalidate();

	if(m_bHasError)//有元件检测没有通过
	{
		//验证错误
		if(m_bAskWhenErr)
		{
			CValidateErr validateErr(&TmpMasterData,&m_DStatInfoMap,this);
			validateErr.DoModal();
			m_pDetectMasterData->ErrorCount += TmpMasterData.ErrorCount;
		}
		else
		{
			AutoAdjustErr();
		}

	}
	else
	{
		CString sInfo = g_LoadString("IDS_DETECT_COMPLETE","检测完成");
		::AfxMessageBox(sInfo);
	}

	TmpMasterData.m_Details.clear();
}

// 自动判断错误
void CDetectBigGraphView::AutoAdjustErr(void)
{
	bool	bIsError = true;//元件是否是错误的
	CString	sErrType ;	//错误类型

	vector<CDetectDetailData*>::iterator DetailIt = m_pDetectMasterData->m_Details.begin();
	for(;DetailIt != m_pDetectMasterData->m_Details.end();DetailIt++)
	{
		CDetectDetailData* pDetailData = (*DetailIt);
		if(pDetailData->isPass == 0)
		{
			//TODO:根据pDetailData->kindred_value,pDetailData->m_pTemplateImg,pDetailData->m_pCurrentImg
			//判断元件是否真错及错误类型

			//just for test:设真错，错误类型为"错位"，实际中要将此代码删除
			bIsError = true;
			sErrType = "错位";

			if(bIsError)
			{
				pDetailData->errType = sErrType;
				m_pDetectMasterData->ErrorCount++;

				//统计信息
				map<CString,CDetectStatInfo*>::iterator it = m_DStatInfoMap.find(pDetailData->organName);
				if(it != m_DStatInfoMap.end())
				{
					(*it).second->lErrorCount +=1;
				}
				else
				{
					CDetectStatInfo* pStatInfo = new CDetectStatInfo();
					pStatInfo->sOrganName = pDetailData->organName;
					pStatInfo->lErrorCount = 1;
					m_DStatInfoMap.insert(::make_pair(pStatInfo->sOrganName,pStatInfo));
				}
			}//end if(bIsError)
		}//end if(pDetailData->isPass == 0)
	}//end for
}

void CDetectBigGraphView::RefreshUITread()
{
    CreateThread(NULL,0,RefreshView,(LPVOID)this,0,NULL);//创建线程
}

DWORD WINAPI RefreshView(LPVOID lpParam)
{
    CDetectBigGraphView *pView = (CDetectBigGraphView*)lpParam;
    pView->Invalidate();
    return 0;
}
BOOL CDetectBigGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta<0)
	{
		this->OnZoomout();
	}
	else
	{
		this->OnZoomin();
	}

	return CBaseBigGraphView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDetectBigGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	bool bLBDown=false;	//鼠标左键是否按下
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		bLBDown = true;
	}
	else
	{
		m_preMousePoint = CPoint(0,0);
		return;
	}

	if(m_TempImage.IsNull())
		return;

	if(m_preMousePoint.x>0)
		AdjustCenterX(m_preMousePoint.x - point.x);
	if(m_preMousePoint.y>0)
		AdjustCenterY(m_preMousePoint.y - point.y);

	m_preMousePoint = point;

	CBaseBigGraphView::OnMouseMove(nFlags, point);
}

void CDetectBigGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	Invalidate();

	CBaseBigGraphView::OnLButtonUp(nFlags, point);
}

void CDetectBigGraphView::PcbStateChange(void)
{
	m_bReady2Organ = false;
}
