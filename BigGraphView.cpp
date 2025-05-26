// BigGraphView.cpp : 实现文件
#include "stdafx.h"
#include "AOI.h"
#include "BigGraphView.h"
#include "MainView.h"
#include <math.h>
#include "Camera.h"
#include "PCBState.h"
#include "Motor.h"
#include "WaitDlg.h"
#include "WaitLoadPCB.h"

extern CCamera     g_camera; //全局摄像头对象
extern CPCBState   g_PCBState;//全局板状态
extern CMotor      g_Motor; //全局电机

extern double g_fMotorStep;
extern double g_fPels_MM;

DWORD WINAPI RefreshUI(LPVOID lpParam);

DWORD WINAPI LoadPCBImageThread(LPVOID lpParam);

IMPLEMENT_DYNCREATE(CBigGraphView, CBaseBigGraphView)

CBigGraphView::CBigGraphView()
	:CBaseBigGraphView(CBigGraphView::IDD)
{
	init();
	m_pSpellPrgGuid = NULL;
	m_pBarPrgGuid = NULL;
	m_pMarkPrgGuid = NULL;
	m_pAngleOffsetPrgGuid = NULL;
}

CBigGraphView::~CBigGraphView()
{
	if( !m_TempImage.IsNull())
	{
		m_TempImage.Destroy();
	}
}

void CBigGraphView::init()
{
	m_iCurrentZoom = 0;	//没有放大缩小
	m_lCurrentCenterX = 0;
	m_lCurrentCenterY = 0;

	m_lSrcOriginX = 0;
	m_lSrcOriginY = 0;
	
	m_CurrentWorkStep = NONE;
	m_pCurrentFov = NULL;
	m_pCurrentDatumMark = NULL;

	m_hCurrentCursor = NULL;
	m_preMousePoint = CPoint(0,0);
	m_pProgramGuid = NULL;
	m_bCanDrawRectTracker = false;
	m_pCurrentImage = NULL;

	m_pCurrentShowFov = NULL;
	m_pCoordCalcOrgan1 =  NULL;
	m_pCoordCalcOrgan2 = NULL;
	m_pCoordCalcDlg = NULL;
	m_pOrganProgramDlg = NULL;
	m_pCurrentOrganData = NULL;

	m_CoordCalcOrganRect1 = CRect(0,0,0,0);
	m_CoordCalcOrganRect2 = CRect(0,0,0,0);
	
}

void CBigGraphView::DoDataExchange(CDataExchange* pDX)
{
	CBaseBigGraphView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBigGraphView, CBaseBigGraphView)	
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CBigGraphView::AssertValid() const
{
	CBaseBigGraphView::AssertValid();
}

#ifndef _WIN32_WCE
void CBigGraphView::Dump(CDumpContext& dc) const
{
	CBaseBigGraphView::Dump(dc);
}
#endif
#endif //_DEBUG

void CBigGraphView::SetTemplateData(CTemplateData * pTemplateData)
{
	CBaseBigGraphView::SetTemplateData(pTemplateData);

	if(pTemplateData == NULL)
		return;

	//初始化
	init();

	if(pTemplateData->m_CoordCalcOrganDataList.size()>=2)
	{
		vector<CCoordCalcOrganData*>::iterator it=	pTemplateData->m_CoordCalcOrganDataList.begin();
		m_CoordCalcOrganRect1 = CRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
		m_pCoordCalcOrgan1 = (*it)->pOrganData;

		it++;
		m_CoordCalcOrganRect2 = CRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
		m_pCoordCalcOrgan2 = (*it)->pOrganData;
	}

	m_pCurrentImage = &(pTemplateData->m_ImageObj);//设置当前显示图片为pcb板图片
	m_CurrentImageType = PCBIMAGE;
	ZoomImageToTemp();//将当前图片对象放到临时对象中
}

//切换显示的图片
void CBigGraphView::SetCurrentImage(CImage *pImage,ImageType type)
{
	if(m_CurrentImageType == PCBIMAGE && type == PCBIMAGE)
		return;

	//当前这些编程步骤只能在PCB板图中处理
	if( (m_CurrentWorkStep == EDITAREA || m_CurrentWorkStep == EDITFOVAREA) && type !=PCBIMAGE)
		return;

	int		m_iTempCurrentZoom = m_iCurrentZoom;
	long	m_lTempCurrentCenterX =m_lCurrentCenterX;
	long	m_lTempCurrentCenterY =m_lCurrentCenterY;
	long	m_lTempSrcOriginX =m_lSrcOriginX;
	long	m_lTempSrcOriginY =m_lSrcOriginY;

	if(type == PCBIMAGE)
	{
		//恢复原来的参数
		m_iCurrentZoom = m_iOldPCBCurrentZoom;
		m_lCurrentCenterX = m_lOldPCBCurrentCenterX;
		m_lCurrentCenterY = m_lOldPCBCurrentCenterY;
		m_lSrcOriginX = m_lOldPCBSrcOriginX;
		m_lSrcOriginY = m_lOldPCBSrcOriginY;
	}
	else
	{
		//恢复原来的参数
		m_iCurrentZoom = m_iOldFOVCurrentZoom;
		m_lCurrentCenterX = m_lOldFOVCurrentCenterX;
		m_lCurrentCenterY = m_lOldFOVCurrentCenterY;
		m_lSrcOriginX = m_lOldFOVSrcOriginX;
		m_lSrcOriginY = m_lOldFOVSrcOriginY;
	}

	if(m_CurrentImageType == PCBIMAGE)
	{
		//保存现有参数
		m_iOldPCBCurrentZoom = m_iTempCurrentZoom;
		m_lOldPCBCurrentCenterX = m_lTempCurrentCenterX;
		m_lOldPCBCurrentCenterY = m_lTempCurrentCenterY;
		m_lOldPCBSrcOriginX = m_lTempSrcOriginX;
		m_lOldPCBSrcOriginY = m_lTempSrcOriginY;
	}
	else
	{
		//保存现有参数
		m_iOldFOVCurrentZoom = m_iTempCurrentZoom;
		m_lOldFOVCurrentCenterX = m_lTempCurrentCenterX;
		m_lOldFOVCurrentCenterY = m_lTempCurrentCenterY;
		m_lOldFOVSrcOriginX = m_lTempSrcOriginX;
		m_lOldFOVSrcOriginY = m_lTempSrcOriginY;
	}

	m_pCurrentFov = NULL;
	//m_pCurrentDatumMark = NULL;

	m_hCurrentCursor = NULL;
	m_preMousePoint = CPoint(0,0);
	//m_bCanDrawRectTracker = false;
	if(type ==PCBIMAGE)
	{
		m_pCurrentShowFov = NULL;
	}

	m_pCurrentImage = pImage;
	m_CurrentImageType = type;
	ZoomImageToTemp();//将当前图片对象放到临时对象中

	Invalidate();//触发onpaint
}


// CBigGraphView 消息处理程序
void CBigGraphView::draw(CDC* pInvalidDC)
{
}

//屏幕无效时自动刷新
void CBigGraphView::OnDraw(CDC* pDC)
{
	draw(pDC);
}

void CBigGraphView::OnZoomin()
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

void CBigGraphView::OnZoomout()
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

void CBigGraphView::OnOriginal()
{
	m_iCurrentZoom = 0;

	// 将image对象放大缩小后放到临时CImage对象中
	ZoomImageToTemp();
	
	Invalidate();
}

//创建编辑区域
void CBigGraphView::Editarea(void)
{
	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}
	
	CRect rect(m_pTemplateData->m_editAreaLeft,
		m_pTemplateData->m_editAreaTop,
		m_pTemplateData->m_editAreaRight,
		m_pTemplateData->m_editAreaBottom);
	if( rect.IsRectEmpty() || rect.IsRectNull())
	{
		rect = CRect(5,5,m_pCurrentImage->GetWidth()-5,m_pCurrentImage->GetHeight()-5);
	}
	else
	{
		m_pTemplateData->bEditAreaVisible = false;
	}

	//设置中心点
	CRect clientRc;
	GetClientRect(&clientRc);
	m_lCurrentCenterX = clientRc.Width()/2;
	m_lCurrentCenterY = clientRc.Height()/2;

	ZoomRectOnOrigin(rect);//放大到当前的倍数
	m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
	m_CurrentWorkStep = EDITAREA;

	Invalidate();
}

void CBigGraphView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//按下Control键并且当前可以用鼠标创建橡皮框时
	if( nChar == VK_CONTROL && m_bCanDrawRectTracker) 
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_COPYWAY);
		SetCursor(m_hCurrentCursor);
	}
	else if( nChar == VK_RETURN) //按下回车键
	{
	}

	CBaseBigGraphView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBigGraphView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//结束当前的工作
	//EndCurrentWork();

	CBaseBigGraphView::OnLButtonDblClk(nFlags, point);
}

//结束当前的工作
void CBigGraphView::EndCurrentWork(bool bDoNext)
{
	CString sMsgInfo ;

	CButton *pOkBtn = NULL;
	CButton *pNextBtn = NULL;
	CButton *pCancleBtn = NULL;
	if(m_pProgramGuid !=NULL && m_pProgramGuid->m_hWnd !=NULL && ::IsWindow(m_pProgramGuid->m_hWnd))
	{
		pOkBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_OK);
		pNextBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_NEXT);
		pCancleBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_CANCLE);
	}

	CWaitDlg *pWaitDlg = NULL;

	switch(m_CurrentWorkStep)
	{
	case LOADPCB:

		//wait
		/*pWaitDlg = new CWaitDlg(this);
		pWaitDlg->Create(IDD_WAIT,this);
		pWaitDlg->ShowWindow(SW_SHOW);*/

		//if(!LoadTemplateImage())
		if(!LoadTemplateImageWork())
			break;

		//wait end
		/*pWaitDlg->DestroyWindow();
		if(pWaitDlg)
		{
			delete pWaitDlg;
			pWaitDlg = NULL;
		}*/

		//执行下一步
		if(bDoNext)
		{
			this->NextStep();
		}
		else
		{
			m_CurrentWorkStep = NONE;
		}
		Invalidate();
		break;
	case EDITAREA:	//编辑区域
		if(m_pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUEREDITAREA","你确定编辑区域已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			CurrentZoomRect.left = m_pRectTracker->m_rect.left +m_lSrcOriginX;
			CurrentZoomRect.right = m_pRectTracker->m_rect.right +m_lSrcOriginX;
			CurrentZoomRect.top = m_pRectTracker->m_rect.top +m_lSrcOriginY;
			CurrentZoomRect.bottom = m_pRectTracker->m_rect.bottom +m_lSrcOriginY;

			GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			m_pTemplateData->m_editAreaBottom = originRect.bottom;
			m_pTemplateData->m_editAreaLeft = originRect.left;
			m_pTemplateData->m_editAreaRight = originRect.right;
			m_pTemplateData->m_editAreaTop = originRect.top;

			m_pTemplateData->bEditAreaVisible = true;
			m_pTemplateData->m_bDataIsChanged = true;
			
			delete m_pRectTracker;
			m_pRectTracker = NULL;
			Invalidate();
		}
		else
		{
			return;
		}

		//执行下一步
		if(bDoNext)
		{
			this->NextStep();
		}
		else
		{
			m_CurrentWorkStep = NONE;
		}
		break;

	case EDITFOVAREA:	//编辑FOV区域
		if(m_pRectTracker == NULL)
		{
			sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
			AfxMessageBox(sMsgInfo);
			return;
		}

		sMsgInfo = g_LoadString("IDS_SUERFOVAREA","你确定此FOV区域已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			CurrentZoomRect.left = m_pRectTracker->m_rect.left +m_lSrcOriginX;
			CurrentZoomRect.right = m_pRectTracker->m_rect.right +m_lSrcOriginX;
			CurrentZoomRect.top = m_pRectTracker->m_rect.top +m_lSrcOriginY;
			CurrentZoomRect.bottom = m_pRectTracker->m_rect.bottom +m_lSrcOriginY;

			GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			m_pCurrentFov->left = originRect.left;
			m_pCurrentFov->top = originRect.top;
			m_pCurrentFov->right = m_pCurrentFov->left + (FOVWIDTH/PCBMULTIPLE);
			m_pCurrentFov->bottom = m_pCurrentFov->top + (FOVHEIGHT/PCBMULTIPLE);

			m_pCurrentFov->bVisible = true;
			m_pTemplateData->m_bFOVIsChange = true;

			if( m_pCurrentFov->fovId ==0)	//此fov是新建的,需要插入map中
			{
				long lId = -1 - m_pTemplateData->m_FOVMap.size();
				while(1)
				{
					map<long,CFovData*>::iterator fovTmpIt = m_pTemplateData->m_FOVMap.find(lId);
					if(fovTmpIt !=  m_pTemplateData->m_FOVMap.end())
					{
						lId--;
					}
					else
					{
						break;
					}
				}

				m_pCurrentFov->CheckOrder = m_pTemplateData->m_FOVMap.size()+1;
				m_pCurrentFov->fovId = lId;
				m_pTemplateData->m_FOVMap.insert(::make_pair(lId,m_pCurrentFov));
			}
			else	//修改
			{
				m_pTemplateData->m_bDataIsChanged = true;
				m_pTemplateData->m_bDatumMarkIsChange = true;
				m_pTemplateData->m_bFOVIsChange = true;
				m_pTemplateData->m_bOrganDataIsChange = true;
				
				//清除此FOV图像数据
				if(!m_pCurrentFov->m_ImageObj.IsNull())
				{
					m_pCurrentFov->m_ImageObj.Destroy();
				}

				//清除此FOV上所有元件编程数据
				map<long,COrganData*>::iterator organIt = m_pTemplateData->m_OrganDataMap.begin();
				for(;organIt!=m_pTemplateData->m_OrganDataMap.end();organIt++)
				{
					if( (*organIt).second->fovId == m_pCurrentFov->fovId)//清除
					{
						m_pTemplateData->m_bOrganDataIsChange = true;
						(*organIt).second->DelPrgData();
					}
				}

				//清除此FOV上所有基准点编程数据
				map<long,CDatumMark *>::iterator markIt = m_pTemplateData->m_DatumMarkMap.begin();
				while(markIt != m_pTemplateData->m_DatumMarkMap.end())
				{
					CDatumMark* pMark = (*markIt).second;

					if(pMark->fovId == m_pCurrentFov->fovId)
					{
						delete pMark;
						pMark=NULL;
						markIt = m_pTemplateData->m_DatumMarkMap.erase(markIt);
					}

					if(markIt == m_pTemplateData->m_DatumMarkMap.end())
					{
						break;
					}
					else
					{
						markIt++;
					}				
				}

				//清除此FOV上条形码编程数据
				if(m_pTemplateData->m_pBar != NULL && m_pTemplateData->m_pBar->fovId == m_pCurrentFov->fovId)
				{
					delete m_pTemplateData->m_pBar;
					m_pTemplateData->m_pBar = NULL;
				}
			}
	
			m_bCanDrawRectTracker = false;	//结束一个FOV时，就不可以用鼠标拖动创建橡皮框
			delete m_pRectTracker;
			m_pRectTracker = NULL;
			Invalidate();
			
		}
		else
		{
			return;
		}

		if(!bDoNext)
		{
			m_CurrentWorkStep = NONE;
		}
		break;

	case LOADFOV:
		sMsgInfo = g_LoadString("IDS_SUERLOADFOV","你确定开始加载FOV图像吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			if(pOkBtn)
				pOkBtn->EnableWindow(FALSE);
			if(pNextBtn)
				pNextBtn->EnableWindow(FALSE);
			if(pCancleBtn)
				pCancleBtn->EnableWindow(FALSE);

			//begin load FOV
			LoadAllFov();

			if(pOkBtn)
				pOkBtn->EnableWindow(TRUE);
			if(pNextBtn)
				pNextBtn->EnableWindow(TRUE);
			if(pCancleBtn)
				pCancleBtn->EnableWindow(TRUE);
		}
		else
		{
			return;
		}

		//执行下一步
		if(bDoNext)
		{
			//this->NextStep();
		}
		else
		{
			m_CurrentWorkStep = NONE;
		}
		break;

	case DATUMMARK:	//编辑基准点		

		if(!bDoNext)
		{
			m_CurrentWorkStep = NONE;
		}
		break;
	case COORDINATEMAP:
	case ORGAN:
	case BAR:
		if(m_pTemplateData->m_pBar !=NULL)
		{
			m_pTemplateData->m_pBar->bSerchVisible = true;
			m_pTemplateData->m_pBar->bVisible = true;
		}
		
		//执行下一步
		if(bDoNext)
		{
			this->NextStep();
		}
		else
		{
			if(pOkBtn)
				pOkBtn->EnableWindow(TRUE);
			if(pNextBtn)
				pNextBtn->EnableWindow(TRUE);
			if(pCancleBtn)
				pCancleBtn->EnableWindow(TRUE);

			m_CurrentWorkStep = NONE;
		}
		break;	
	default:
		break;
	}

	if(bDoNext)
	{
		m_pTemplateData->m_LastWorkStep = m_CurrentWorkStep;//记录向导编程最后一步
	}

	NotifyPrgDataChanged();//通知数据已经改变
}

//取消当前工作
void CBigGraphView::CancelCurrentWork()
{
	switch(m_CurrentWorkStep)
	{	
	case EDITAREA:	//编辑区域
		m_pTemplateData->bEditAreaVisible = true;
		break;
	case EDITFOVAREA:	//编辑FOV区域
		m_pCurrentFov->bVisible = true;
			break;
	case DATUMMARK:	//编辑基准点
		//m_pCurrentDatumMark->bVisible = true;
		break;
	case BAR:
		if(m_pTemplateData->m_pBar)
		{
			m_pTemplateData->m_pBar->bSerchVisible = true;
			m_pTemplateData->m_pBar->bVisible = true;
		}
		break;
	}

	//m_CurrentWorkStep = NONE; //modify at 2008-03-17
	
	if( m_pRectTracker != NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}


	if(m_pProgramGuid && m_pProgramGuid->m_hWnd && ::IsWindow(m_pProgramGuid->m_hWnd))
	{
		CButton * pNewBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_NEW);
		CButton * pOkBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_OK);
		CButton * pNextBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_NEXT);
		CButton * pCancleBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_CANCLE);

		pNewBtn->EnableWindow(TRUE);
		pOkBtn->EnableWindow(TRUE);
		pNextBtn->EnableWindow(TRUE);
		pCancleBtn->EnableWindow(TRUE);
	}

	Invalidate();
}

//执行下一个步骤
void CBigGraphView::NextStep()
{
	CString sStepInfo;
	if(m_pProgramGuid == NULL || m_pProgramGuid->m_hWnd == NULL || !IsWindow(m_pProgramGuid->m_hWnd))
	{
		m_pProgramGuid = new CProgramGuid(this);
		m_pProgramGuid->Create(IDD_PRGSTEP,this);
	}
		
	CStatic *pInfoCtrl = (CStatic *)m_pProgramGuid->GetDlgItem(IDC_INFO);	
	m_pProgramGuid->ShowWindow(SW_SHOW);
	CButton * pNewBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_NEW);
	CButton * pOkBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_OK);
	CButton * pNextBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_NEXT);
	CButton * pCancleBtn = (CButton *)m_pProgramGuid->GetDlgItem(IDC_CANCLE);

	if(m_pRectTracker != NULL)
	{
		sStepInfo = g_LoadString("IDS_CURENTSTEPSTILLWORK","当前步骤仍处于编程状态，请按确定按钮结束其编程状态，才能进行下一步编程");
		AfxMessageBox(sStepInfo);
		return;
	}

	switch(m_CurrentWorkStep)
	{
	case NONE:
		sStepInfo = g_LoadString("IDS_LOADPCBSTEPINFO","第一步：扫描PCB全板图像\n请按“确定”按钮开始扫描PCB全板图像");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		m_CurrentWorkStep = LOADPCB;
		break;
	case LOADPCB:
		sStepInfo = g_LoadString("IDS_ANGELOFFSETSTEPINFO","第二步：模板角度偏差编程\n请按照模板角度偏差引导窗口进行角度偏差的编程");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);

		PrgAngleoffset();
		break;
	case ANGLE_OFFSET:
		sStepInfo = g_LoadString("IDS_EDITAREASTEPINFO","第三步：调整编辑区域\n请调整主窗口上的编辑区域，然后按\"确定\"按钮");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		Editarea();
		break;
	case EDITAREA:
		sStepInfo = g_LoadString("IDS_FOVSTEPINFO","第四步：创建FOV区域\n请调整FOV区域中心点，然后按“确定”按钮确定");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(TRUE);
		NewFOV();
		break;
	case EDITFOVAREA:
		sStepInfo = g_LoadString("IDS_LOADFOVSTEPINFO","第五步：加载FOV图像\n请按确定键开始加载FOV图像");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		m_CurrentWorkStep = LOADFOV;
		break;
	case LOADFOV:
		if(false==NewDatumMark())
			break;
		sStepInfo = g_LoadString("IDS_MARKSTEPINFO","第六步：制作基准点\n请按照基准点编程引导窗口进行基准点的编程");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(TRUE);
		break;
	case DATUMMARK:
		sStepInfo = g_LoadString("IDS_SPELL_STEPINFO","第七步：拼板编程\n请按照拼板编程引导窗口进行拼板的编程");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);

		PrgSpellPcb();
		break;
	case SPELLPCB:
		if(false==BeginCoordCalc(true))
			break;
		sStepInfo = g_LoadString("IDS_COORDINATEMAPSTEPINFO1","请在“计算坐标”对话框上选择两个对角元件，然后按下一步");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);
		pNextBtn->EnableWindow(FALSE);
		pCancleBtn->EnableWindow(FALSE);
		break;	
	case COORDINATEMAP:
		sStepInfo = g_LoadString("IDS_ORGAN_STEPINFO","第九步：元件编程\n请按照元件编程窗口的引导进行元件的编程");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);

		if( m_pCoordCalcDlg != NULL && m_pCoordCalcDlg->m_hWnd != NULL && IsWindow(m_pCoordCalcDlg->m_hWnd))
		{
			m_pCoordCalcDlg->DestroyWindow();
			m_pCoordCalcDlg = NULL;
		}

		PrgOrgan(false,NULL);

		break;
	case ORGAN:
		sStepInfo = g_LoadString("IDS_BAR_STEPINFO","第十步：条形码编程\n请按照条形码编程引导窗口进行条形码的编程");
		pInfoCtrl->SetWindowTextA(sStepInfo);
		pNewBtn->EnableWindow(FALSE);
		pOkBtn->EnableWindow(FALSE);

		PrgBar(true);
		break;	
	default:
		sStepInfo = g_LoadString("IDS_IS_LASTSTEP","已到最后一步");
		::AfxMessageBox(sStepInfo);
		break;
	}

}

void CBigGraphView::NewCurrentWork()
{
	CString	sInfo;
	switch(m_CurrentWorkStep)
	{	
	case EDITFOVAREA:
		if(m_pRectTracker != NULL)
		{
			sInfo = g_LoadString("IDS_CURENTSTEPSTILLWORK","当前工作仍处于编程状态，请按确定按钮结束其编程状态");
			AfxMessageBox(sInfo);
			return;
		}
		else
		{
			NewFOV();
		}
		break;
	case DATUMMARK:
		if(m_pRectTracker != NULL)
		{
			sInfo = g_LoadString("IDS_CURENTSTEPSTILLWORK","当前工作仍处于编程状态，请按确定按钮结束其编程状态");
			AfxMessageBox(sInfo);
			return;
		}
		else
		{
			NewDatumMark();
		}
		break;

	default:
		break;
	}
}

// 新创建FOV
void CBigGraphView::NewFOV(void)
{
	m_bCanDrawRectTracker = false;
	
	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}

	if(m_CurrentImageType != PCBIMAGE)
	{
		SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	}
	
	m_pCurrentFov = new CFovData();	//新增一个FOV

	int iFovWidth = (1+m_iCurrentZoom*ZOOMSTEP)*(FOVWIDTH/PCBMULTIPLE);
	int iFovHeight = (1+m_iCurrentZoom*ZOOMSTEP)*(FOVHEIGHT/PCBMULTIPLE);
	
	CRect rect;
	rect.left = m_lCurrentCenterX - iFovWidth/2;
	//if(rect.left < 0)
		rect.left = 0;

	rect.top = m_lCurrentCenterY - iFovHeight/2;
	//if(rect.top < 0)
		rect.top = 0;

	rect.right = rect.left + iFovWidth;
	rect.bottom = rect.top + iFovHeight;

	m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

	//设置中心点
	/*CRect rc;
	GetClientRect(rc);
	m_lCurrentCenterX = rect.left + rect.Width()/2;
	m_lCurrentCenterY = rect.top + rect.Height()/2;
	if(m_lCurrentCenterX < rc.Width()/2)
		m_lCurrentCenterX = rc.Width()/2;
	if(m_lCurrentCenterY < rc.Height()/2)
		m_lCurrentCenterY = rc.Height()/2;*/

	m_CurrentWorkStep = EDITFOVAREA;

	this->Invalidate();
}

// 修改FOV
void CBigGraphView::ModifyFov(CFovData *pFov)
{
	m_CurrentWorkStep = EDITFOVAREA;	//当前正在编辑FOV区域
	if( m_pRectTracker != NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}
	if(m_CurrentImageType != PCBIMAGE)
	{
		SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	}

	m_pCurrentFov = pFov;

	CRect FovRc(pFov->left,pFov->top,pFov->right,pFov->bottom);//原FOV的rect(板图片原始大小情况下的rect)
	//pFov->left=pFov->top=pFov->right=pFov->bottom=0;//销毁
	pFov->bVisible = false;//不要显示出来
	if(!pFov->m_ImageObj.IsNull())
	{
		pFov->m_ImageObj.Destroy();
	}

	
	ZoomRectOnOrigin(FovRc);//放大到当前的倍数
	
	//设置中心点和原点
	ComputerCenterAndOrigin(FovRc.left + FovRc.Width()/2, FovRc.top + FovRc.Height()/2);

	CRect trackerRect(FovRc.left-m_lSrcOriginX,FovRc.top-m_lSrcOriginY,FovRc.right-m_lSrcOriginX,FovRc.bottom-m_lSrcOriginY);
	m_pRectTracker = new CRectTracker(&trackerRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

	//刷新屏幕
	//draw();
	Invalidate();
}

// 创建新的基准点
bool CBigGraphView::NewDatumMark(void)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","请先创建FOV区域");
		::AfxMessageBox(sInfo);
		return false;
	}
	
	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}	

	if(m_CurrentImageType != FOVIMAGE)
	{
		CString sInfo =::g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
		::AfxMessageBox(sInfo);
		return false;
	}

	if(m_pTemplateData->m_DatumMarkMap.size()>=2)//最多2个基准点
	{
		m_CurrentWorkStep = NONE;
		CString sMsgInfo = g_LoadString("IDS_OVERDATUMMARKNUM","最多只能编辑2个基准点");
		::AfxMessageBox(sMsgInfo);
		return false;
	}

	m_bCanDrawRectTracker = true;	//新增一个基准点时，可以用鼠标拖动创建橡皮框
	//m_pCurrentDatumMark = new CDatumMark();	
	m_CurrentWorkStep = DATUMMARK;

	if(m_pMarkPrgGuid != NULL && m_pMarkPrgGuid->m_hWnd != NULL && ::IsWindow(m_pMarkPrgGuid->m_hWnd))
	{
		m_pMarkPrgGuid->DestroyWindow();
		m_pMarkPrgGuid = NULL;
	}

	m_pMarkPrgGuid = new CMarkPrgGuid(this);
	m_pMarkPrgGuid->Create(IDD_MARKPRGGUID,this);
	m_pMarkPrgGuid->SetTemplateData(m_pTemplateData);
	m_pMarkPrgGuid->ShowWindow(SW_SHOW);

	return true;
}

// 修改基准点
void CBigGraphView::ModifyDatumMark(CDatumMark * pDatumMark)
{
	m_pCurrentDatumMark = pDatumMark;
	m_CurrentWorkStep = DATUMMARK;	//当前正在编辑基准点

	if( m_pRectTracker != NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}

	//切换到FOV图片
	SetCurrentShowFov(pDatumMark->m_pFovData);
	SetCurrentImage( &(pDatumMark->m_pFovData->m_ImageObj),FOVIMAGE);

	CRect	MarkRect(pDatumMark->left,pDatumMark->top,pDatumMark->right,pDatumMark->bottom);
	//pDatumMark->left=pDatumMark->top=pDatumMark->right=pDatumMark->bottom=0;//销毁
	pDatumMark->bVisible = false;//不要显示出来

	ZoomRectOnOrigin(MarkRect);//放大到当前的倍数
	
	//设置中心点和原点
	ComputerCenterAndOrigin(MarkRect.left + MarkRect.Width()/2, MarkRect.top + MarkRect.Height()/2);
	
	CRect trackerRect(MarkRect.left-m_lSrcOriginX,MarkRect.top-m_lSrcOriginY,
		MarkRect.right-m_lSrcOriginX,MarkRect.bottom-m_lSrcOriginY);
	m_pRectTracker = new CRectTracker(&trackerRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

	//刷新屏幕
	//draw();
	Invalidate();
}

//移动鼠标
void CBigGraphView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_MousePoint = point;

	//Control键按下并且按下鼠标左键并且当前可以用鼠标创建橡皮框时，在拖动鼠标时，表示在调整橡皮框大小
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_COPYWAY);

		if(m_iNewRectTrackStep == 1)	//现在正在新建橡皮框
		{
			CClientDC dc( this);
			
			//考虑拉动到窗口边缘的情况
			CRect clientRc;
			GetClientRect(clientRc);
			if(point.y>= clientRc.bottom-10)//已经拉到窗口底边
			{
				if(AdjustCenterY(10))	//则调整中心Y下移
				{
					m_NewRectTrackOriginP.y -=10;//如果调整中心成功，则橡皮框原点Y要上移
					//Invalidate();
				}
			}
			if(point.y<= 10)//已经拉到窗口上边
			{
				if(AdjustCenterY(-10))	//则调整中心Y上移
				{
					m_NewRectTrackOriginP.y +=10;//如果调整中心成功，则橡皮框原点Y要下移
					//Invalidate();
				}
			}

			if(point.x>= clientRc.right-10)//已经拉到窗口右边
			{
				if(AdjustCenterX(10))	//则调整中心Y右移
				{
					m_NewRectTrackOriginP.x -=10;//如果调整中心成功，则橡皮框原点Y要左移
					//Invalidate();
				}
			}
			if(point.x<= 10)//已经拉到窗口左边
			{
				if(AdjustCenterX(-10))	//则调整中心Y左移
				{
					m_NewRectTrackOriginP.x +=10;//如果调整中心成功，则橡皮框原点Y要右移
					//Invalidate();
				}
			}

			CRect rect(m_NewRectTrackOriginP,point);
			
			if(m_pRectTracker == NULL)
			{
				m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
				m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
			}
			else
			{
				//消去上一个橡皮框图形
				long	lTmp;
				if( m_pRectTracker->m_rect.right < m_pRectTracker->m_rect.left)
				{//橡皮框在做反向运动，所以要将它的RECT调整过来
					lTmp = m_pRectTracker->m_rect.right;
					m_pRectTracker->m_rect.right = m_pRectTracker->m_rect.left;
					m_pRectTracker->m_rect.left = lTmp;
				}
				if( m_pRectTracker->m_rect.bottom < m_pRectTracker->m_rect.top)
				{//橡皮框在做反向运动，所以要将它的RECT调整过来
					lTmp = m_pRectTracker->m_rect.bottom;
					m_pRectTracker->m_rect.bottom = m_pRectTracker->m_rect.top;
					m_pRectTracker->m_rect.top = lTmp;
				}
				m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
				InvalidateRect(m_pRectTracker->m_rect);//使原橡皮框的区间无效，触发onpain重画

				m_pRectTracker->m_rect = rect;
			}
			m_pRectTracker->Draw(&dc);
			
			UpdateWindow();
		}

		return;
	}
	//Control键按下并且按下鼠标左键，在拖动鼠标时，表示在调整橡皮框大小<----end


	bool bLBDown=false;	//鼠标左键是否按下
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		bLBDown = true;
	}
	else
	{
		m_preMousePoint = CPoint(0,0);
	}

	bool	lShiftDown=false;//shift键是否按下
	if( (nFlags & MK_SHIFT) == MK_SHIFT)
	{
		lShiftDown=true;
	}

	if( m_pRectTracker != NULL)
	{
		int nIn; //定义一个鼠标的点击值；
		nIn=m_pRectTracker->HitTest(point); //看看点到了哪了
		
		//如果shift键按下，则移动图像而不是橡皮框，不管是否点在橡皮框中
		if(lShiftDown)
		{
			nIn = CRectTracker::hitNothing;
		}

		//根据不同的位置设置鼠标光标
		
		if(m_CurrentWorkStep == EDITFOVAREA && nIn>=0)//如果在编辑FOV，则FOV框不可调整大小
		{
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRACKMOVE);
		}
		else
		{
			switch(nIn)
			{
			case CRectTracker::hitNothing:
				if(bLBDown)
				{
					//按下鼠标左键时移动图像
					if(m_preMousePoint.x>0)
					{
						//调整中心x
						//m_lCurrentCenterX += m_preMousePoint.x - point.x;
						if( AdjustCenterX(m_preMousePoint.x - point.x) )
						{
							//如果中心调整成功，则橡皮框也要移动
							m_pRectTracker->m_rect.left += point.x - m_preMousePoint.x ;
							m_pRectTracker->m_rect.right += point.x - m_preMousePoint.x ;

							#ifdef _DEBUG
							TRACE("point.x - m_preMousePoint.x=%d\r\n",point.x - m_preMousePoint.x);
							#endif
						}
					}
					if(m_preMousePoint.y>0)
					{
						//m_lCurrentCenterY += m_preMousePoint.y - point.y;
						if( AdjustCenterY(m_preMousePoint.y - point.y) )
						{
							//如果中心调整成功，橡皮框也要移动
							m_pRectTracker->m_rect.top += point.y - m_preMousePoint.y;
							m_pRectTracker->m_rect.bottom += point.y - m_preMousePoint.y;
						}
					}
					//draw();
					//Invalidate();

					m_preMousePoint = point;

					m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDHOLD);
				}
				else
				{
					m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDOPEN);
				}
				break;
				
			case CRectTracker::hitTopLeft:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNWSE);
				break;
			case CRectTracker::hitBottomRight:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNWSE);
				break;
			case CRectTracker::hitTopRight:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNESW);
				break;
			case CRectTracker::hitBottomLeft:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNESW);
			case CRectTracker::hitTop:
			case CRectTracker::hitBottom:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKNS);
				break;
			case CRectTracker::hitRight:
			case CRectTracker::hitLeft:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRCKWE);
				break;
			case CRectTracker::hitMiddle:
				m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_TRACKMOVE);
				break;
			}//end switch
		}//end if(m_CurrentWorkStep == EDITFOVAREA && nIn>=0)
		
	}
	else
	{
		if(bLBDown)
		{
			//按下鼠标左键时移动图像
			if(m_preMousePoint.x>0)
				AdjustCenterX(m_preMousePoint.x - point.x);
			if(m_preMousePoint.y>0)
				AdjustCenterY(m_preMousePoint.y - point.y);
			//draw();
			//Invalidate();

			m_preMousePoint = point;
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDHOLD);
		}
		else
		{
			m_hCurrentCursor = AfxGetApp()->LoadCursor(IDC_HANDOPEN);
		}
	}

	CBaseBigGraphView::OnMouseMove(nFlags, point);
}

BOOL CBigGraphView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( m_hCurrentCursor != NULL)
	{
		SetCursor(m_hCurrentCursor);
		return TRUE;
	}

	return CBaseBigGraphView::OnSetCursor(pWnd, nHitTest, message);
}

void CBigGraphView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//Control键按下并且当前可以用鼠标创建橡皮框时,则表示开始创建新的橡皮框
	if( (nFlags & MK_CONTROL) == MK_CONTROL && m_bCanDrawRectTracker)
	{
		if( m_pRectTracker != NULL)
		{
			//要将原橡皮框的图形消去（即设置原框的矩形无效）
			m_pRectTracker->m_rect.InflateRect(TRACKERSIZE,TRACKERSIZE,TRACKERSIZE,TRACKERSIZE);
			InvalidateRect(m_pRectTracker->m_rect);

			delete m_pRectTracker;
			m_pRectTracker =NULL;
		}

		m_iNewRectTrackStep = 1;	//新建橡皮框的第一步
		m_NewRectTrackOriginP = point;
		return;
	}

	bool	lShiftDown=false;//shift键是否按下
	if( (nFlags & MK_SHIFT) == MK_SHIFT)
	{
		lShiftDown=true;
	}

	if( m_pRectTracker != NULL)
	{
		int nIn; //定义一个鼠标的点击值；
		nIn=m_pRectTracker->HitTest(point); //看看点到了哪了

		//如果shift键是否按下，则不处理橡皮框，不管是否点在橡皮框中
		if(lShiftDown)
		{
			nIn = CRectTracker::hitNothing;
		}

		//如果在编辑FOV，则FOV框不可调整大小
		if(m_CurrentWorkStep == EDITFOVAREA)
		{
			if(nIn==CRectTracker::hitMiddle)  //在四边形区域内，只可移动，不可调整大小
			{
				CClientDC dc(this);
				m_pRectTracker->Draw(&dc);
				m_pRectTracker->Track(this,point,TRUE);
				// Track()是CRectTracker中最富魅力的函数。它时时的改变调用者的m_rect;
				Invalidate();//引起OnPain函数的发生
				//pMainView->ShowBitmap();

				NotifyTrakerChanged();
			}
		}
		else
		{
			if(nIn>=0)  //在四边形区域内，可调整大小
			{
				CClientDC dc(this);
				m_pRectTracker->Draw(&dc);
				m_pRectTracker->Track(this,point,TRUE);
				// Track()是CRectTracker中最富魅力的函数。它时时的改变调用者的m_rect;
				Invalidate();//引起OnPain函数的发生
				//pMainView->ShowBitmap();
				NotifyTrakerChanged();
			}
		}
	}

	CBaseBigGraphView::OnLButtonDown(nFlags, point);
}

void CBigGraphView::OnLButtonUp(UINT nFlags, CPoint point)
{
	long	lTmp;

	if(m_iNewRectTrackStep == 1)//之前在拖动橡皮框，现在结束
	{
		m_iNewRectTrackStep = 2;
		CClientDC dc( this);

		CRect rect(m_NewRectTrackOriginP,point);
		if( rect.right < rect.left)
		{//橡皮框在做反向运动，所以要将它的RECT调整过来
			lTmp = rect.right;
			rect.right = rect.left;
			rect.left = lTmp;
		}
		if( rect.bottom < rect.top)
		{//橡皮框在做反向运动，所以要将它的RECT调整过来
			lTmp = rect.bottom;
			rect.bottom = rect.top;
			rect.top = lTmp;
		}
			
		if(m_pRectTracker != NULL)
		{
			m_pRectTracker->m_rect = rect;
			m_pRectTracker->Draw(&dc);
		}
	}

	Invalidate();

	CBaseBigGraphView::OnLButtonUp(nFlags, point);
}


BOOL CBigGraphView::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;

	return CBaseBigGraphView::OnEraseBkgnd(pDC);
}

void CBigGraphView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	LONG lNewWidth=0;
	LONG lNewHeight=0;

	if(m_pTemplateData == NULL)
		return;

	if(m_pCurrentImage->IsNull())
		return;	

	CRect rc;
	GetClientRect(rc);

	//清除窗口
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(0,0,1023,639);

	if(m_TempImage.IsNull())
	{
		ZoomImageToTemp();
	}

	//重新设置中心点和原点
	ComputerCenterAndOrigin(m_lCurrentCenterX, m_lCurrentCenterY);

	m_TempImage.BitBlt(dc,0,0,m_TempImage.GetWidth(),m_TempImage.GetHeight(),m_lSrcOriginX,m_lSrcOriginY,SRCCOPY);
			
	//画跟踪矩型
	if(m_pRectTracker != NULL)
	{
		m_pRectTracker->Draw(&dc);
	}
	
	if( m_CurrentImageType == PCBIMAGE) //如果当前显示PCB图像，则还要画其他元素
	{
		//画出编辑区域
		if(m_pTemplateData->m_editAreaBottom > 0 && m_pTemplateData->bEditAreaVisible==true)
		{
			CBrush *pBrush = new CBrush(g_EditAreaColor);

			//取得编辑区域坐标（图像的坐标体系）
			CRect rect(m_pTemplateData->m_editAreaLeft,m_pTemplateData->m_editAreaTop,
				m_pTemplateData->m_editAreaRight,m_pTemplateData->m_editAreaBottom);		
			
			//放大缩小
			ZoomRectOnOrigin(rect);

			//将编辑区域坐标转换成窗口的坐标体系
			rect.left -= m_lSrcOriginX;
			rect.right -= m_lSrcOriginX;
			rect.top -= m_lSrcOriginY;
			rect.bottom -= m_lSrcOriginY;

			CRgn rg;
			rg.CreateRectRgnIndirect(&rect);

			//dc.FrameRect(&rect,pBrush);
			dc.FrameRgn(&rg,pBrush,2,2);
			delete pBrush;
			pBrush = NULL;
		}

		//画拼板
		/*EnterCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);
		map<int,CPoint*>::iterator spellIt = m_pTemplateData->m_SpellPcbMap.begin();
		for(;spellIt!=m_pTemplateData->m_SpellPcbMap.end();spellIt++)
		{
			if((*spellIt).second->x == 0 && (*spellIt).second->y == 0)
				continue;

			CRect rect(m_pTemplateData->m_editAreaLeft+(*spellIt).second->x/PCBMULTIPLE,
				m_pTemplateData->m_editAreaTop+(*spellIt).second->y/PCBMULTIPLE,
				m_pTemplateData->m_editAreaRight+(*spellIt).second->x/PCBMULTIPLE,
				m_pTemplateData->m_editAreaBottom+(*spellIt).second->y/PCBMULTIPLE);

			CBrush *pBrush = new CBrush(RGB(0,0,200));

			//放大缩小
			ZoomRectOnOrigin(rect);

			//将编辑区域坐标转换成窗口的坐标体系
			rect.left -= m_lSrcOriginX;
			rect.right -= m_lSrcOriginX;
			rect.top -= m_lSrcOriginY;
			rect.bottom -= m_lSrcOriginY;

			CRgn rg;
			rg.CreateRectRgnIndirect(&rect);

			//dc.FrameRect(&rect,pBrush);
			dc.FrameRgn(&rg,pBrush,2,2);

			CString sNo;
			sNo.Format("%d",(*spellIt).first);
			dc.TextOutA(rect.left+10,rect.top+10,sNo);

			delete pBrush;
			pBrush = NULL;
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4SpellPcbMap);*/

		//画FOV区域
		EnterCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
		for(;it != m_pTemplateData->m_FOVMap.end();it++)
		{
			if((*it).second->bVisible == false)
				continue;

			CBrush *pBrush = new CBrush(g_FovColor);

			//取得FOV区域坐标（图像的坐标体系）
			CRect rect((*it).second->left,(*it).second->top,(*it).second->right,(*it).second->bottom);
			
			//放大缩小
			ZoomRectOnOrigin(rect);

			//将FOV区域坐标转换成窗口的坐标体系
			rect.left -= m_lSrcOriginX;
			rect.right -= m_lSrcOriginX;
			rect.top -= m_lSrcOriginY;
			rect.bottom -= m_lSrcOriginY;

			CRgn rg;
			rg.CreateRectRgnIndirect(&rect);
			//dc.FrameRect(&rect,pBrush);
			dc.FrameRgn(&rg,pBrush,2,2);
			delete pBrush;
			pBrush = NULL;
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4FOVMap);
		
		//画计算坐标系的两个元件
		if( /*m_CurrentWorkStep==COORDINATEMAP &&*/
			!m_CoordCalcOrganRect1.IsRectNull() && !m_CoordCalcOrganRect1.IsRectEmpty())
		{
			CBrush *pBrush = new CBrush(RGB(0,255,0));
			CRect rect = m_CoordCalcOrganRect1;
			ZoomRectOnOrigin(rect);

			rect.left -= m_lSrcOriginX;
			rect.right -= m_lSrcOriginX;
			rect.top -= m_lSrcOriginY;
			rect.bottom -= m_lSrcOriginY;

			dc.FrameRect(&rect,pBrush);

			if( !m_CoordCalcOrganRect2.IsRectNull() && !m_CoordCalcOrganRect2.IsRectEmpty())
			{
				//CBrush *pBrush = new CBrush(RGB(0,255,0));
				CRect rect2 = m_CoordCalcOrganRect2;
				ZoomRectOnOrigin(rect2);

				rect2.left -= m_lSrcOriginX;
				rect2.right -= m_lSrcOriginX;
				rect2.top -= m_lSrcOriginY;
				rect2.bottom -= m_lSrcOriginY;

				dc.FrameRect(&rect2,pBrush);
				
				dc.MoveTo(rect.CenterPoint());
				dc.LineTo(rect2.CenterPoint());
			}

			delete pBrush;
			pBrush = NULL;
		}

		//画条形码(全板模式下)
		if(m_pTemplateData->m_pBar != NULL)
		{
			CBrush *pBrush = new CBrush(g_BarCodeColor);
			CRect searchRt(m_pTemplateData->m_pBar->barSerchLeft,m_pTemplateData->m_pBar->barSerchTop,
				m_pTemplateData->m_pBar->barSerchRight,m_pTemplateData->m_pBar->barSerchBottom);
			CRect bodyRt(m_pTemplateData->m_pBar->left,m_pTemplateData->m_pBar->top,
				m_pTemplateData->m_pBar->right,m_pTemplateData->m_pBar->bottom);
			CRect fovRt(m_pTemplateData->m_pBar->m_pFovData->left,m_pTemplateData->m_pBar->m_pFovData->top,
				m_pTemplateData->m_pBar->m_pFovData->right,m_pTemplateData->m_pBar->m_pFovData->bottom);

			searchRt.left = fovRt.left + searchRt.left/FOVMULTIPLE;
			searchRt.right = fovRt.left + searchRt.right/FOVMULTIPLE;
			searchRt.top = fovRt.top + searchRt.top/FOVMULTIPLE;
			searchRt.bottom = fovRt.top + searchRt.bottom/FOVMULTIPLE;

			bodyRt.left = fovRt.left + bodyRt.left/FOVMULTIPLE;
			bodyRt.right = fovRt.left + bodyRt.right/FOVMULTIPLE;
			bodyRt.top = fovRt.top + bodyRt.top/FOVMULTIPLE;
			bodyRt.bottom = fovRt.top + bodyRt.bottom/FOVMULTIPLE;
			
			ZoomRectOnOrigin(searchRt);
			ZoomRectOnOrigin(bodyRt);

			//将FOV区域坐标转换成窗口的坐标体系
			searchRt.left -= m_lSrcOriginX;
			searchRt.right -= m_lSrcOriginX;
			searchRt.top -= m_lSrcOriginY;
			searchRt.bottom -= m_lSrcOriginY;

			bodyRt.left -= m_lSrcOriginX;
			bodyRt.right -= m_lSrcOriginX;
			bodyRt.top -= m_lSrcOriginY;
			bodyRt.bottom -= m_lSrcOriginY;

			if(m_pTemplateData->m_pBar->bSerchVisible == true)
				dc.FrameRect(&searchRt,pBrush);

			if(m_pTemplateData->m_pBar->bVisible == true)
				dc.FrameRect(&bodyRt,pBrush);

			delete pBrush;
			pBrush = NULL;
		}

	}//end if( m_CurrentImageType == PCBIMAGE)
	else if(m_CurrentImageType ==FOVIMAGE)
	{
		//画基准点
		EnterCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

		map<long,CDatumMark*>::iterator MarkIt = m_pTemplateData->m_DatumMarkMap.begin();
		for(;MarkIt != m_pTemplateData->m_DatumMarkMap.end();MarkIt++)
		{
			if((*MarkIt).second->bVisible == false)
				continue;

			//此基准点不属于当前显示的FOV,则不显示
			if( (*MarkIt).second->fovId != m_pCurrentShowFov->fovId)
			{
				continue;
			}

			CBrush *pBrush = new CBrush(g_MarkColor);

			//取得FOV区域坐标（图像的坐标体系）
			CRect bodyRect((*MarkIt).second->left,(*MarkIt).second->top,(*MarkIt).second->right,(*MarkIt).second->bottom);
			CRect searchRect((*MarkIt).second->searchLeft,(*MarkIt).second->searchTop,(*MarkIt).second->searchRight,(*MarkIt).second->searchBottom);
			
			//放大缩小
			ZoomRectOnOrigin(bodyRect);
			ZoomRectOnOrigin(searchRect);

			//将FOV区域坐标转换成窗口的坐标体系
			bodyRect.left -= m_lSrcOriginX;
			bodyRect.right -= m_lSrcOriginX;
			bodyRect.top -= m_lSrcOriginY;
			bodyRect.bottom -= m_lSrcOriginY;

			searchRect.left -= m_lSrcOriginX;
			searchRect.right -= m_lSrcOriginX;
			searchRect.top -= m_lSrcOriginY;
			searchRect.bottom -= m_lSrcOriginY;

			CRgn rg1,rg2;
			rg1.CreateRectRgnIndirect(&bodyRect);
			rg2.CreateRectRgnIndirect(&searchRect);
			//dc.FrameRect(&rect,pBrush);
			dc.FrameRgn(&rg1,pBrush,2,2);
			dc.FrameRgn(&rg2,pBrush,2,2);
			delete pBrush;
			pBrush = NULL;
		}
		LeaveCriticalSection(&m_pTemplateData->m_Critial4DatumMarkMap);

		if(m_pCurrentOrganData != NULL && m_pCurrentOrganData->fovId == m_pCurrentShowFov->fovId)
		{
			//画元件
			DRAWCONTEXT DrawContext;
			DrawContext.color =g_OrganColor;
			DrawContext.dZoom = m_iCurrentZoom*ZOOMSTEP;
			DrawContext.lSrcOriginX = m_lSrcOriginX;
			DrawContext.lSrcOriginY = m_lSrcOriginY;
			DrawContext.pDC = &dc;

			m_pCurrentOrganData->Draw(DrawContext);
		}

		//画条形码
		if(m_pTemplateData->m_pBar != NULL && m_pTemplateData->m_pBar->fovId == m_pCurrentShowFov->fovId)
		{
			CBrush *pBrush = new CBrush(g_BarCodeColor);
			CRect searchRt(m_pTemplateData->m_pBar->barSerchLeft,m_pTemplateData->m_pBar->barSerchTop,
				m_pTemplateData->m_pBar->barSerchRight,m_pTemplateData->m_pBar->barSerchBottom);
			CRect bodyRt(m_pTemplateData->m_pBar->left,m_pTemplateData->m_pBar->top,
				m_pTemplateData->m_pBar->right,m_pTemplateData->m_pBar->bottom);

			ZoomRectOnOrigin(searchRt);
			ZoomRectOnOrigin(bodyRt);

			//将FOV区域坐标转换成窗口的坐标体系
			searchRt.left -= m_lSrcOriginX;
			searchRt.right -= m_lSrcOriginX;
			searchRt.top -= m_lSrcOriginY;
			searchRt.bottom -= m_lSrcOriginY;

			bodyRt.left -= m_lSrcOriginX;
			bodyRt.right -= m_lSrcOriginX;
			bodyRt.top -= m_lSrcOriginY;
			bodyRt.bottom -= m_lSrcOriginY;

			if(m_pTemplateData->m_pBar->bSerchVisible == true)
				dc.FrameRect(&searchRt,pBrush);

			if(m_pTemplateData->m_pBar->bVisible == true)
				dc.FrameRect(&bodyRt,pBrush);

			delete pBrush;
			pBrush = NULL;
		}
	}

}

//准备计算坐标相对值
bool CBigGraphView::BeginCoordCalc(bool bDoNext)
{
	SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);

	if(m_pTemplateData->m_OrganDataMap.size()<=0)
	{
		CString sInfo = g_LoadString("IDS_NOORGAN","当前模板中没有元件，请在元件面板中导入或添加元件信息");
		::AfxMessageBox(sInfo);
		return false;
	}

	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}

	if(m_pCoordCalcDlg == NULL || m_pCoordCalcDlg->m_hWnd == NULL || !IsWindow(m_pCoordCalcDlg->m_hWnd))
	{
		m_pCoordCalcDlg = new CCoordCalc(bDoNext,this);
		m_pCoordCalcDlg->Create(IDD_COORDCALC,this);
	}
	
	m_pCoordCalcDlg->SetTemplateData(m_pTemplateData);
	m_pCoordCalcDlg->ShowWindow(SW_SHOW);
	m_CurrentWorkStep = COORDINATEMAP;

	return true;
}

// 计算坐标相对值步骤1
/*bool CBigGraphView::CoordCalcStep1(void)
{
	if(m_pCoordCalcOrgan1 == NULL || m_pCoordCalcOrgan2 == NULL)
	{
		CString sInfo = g_LoadString("IDS_MUSTSELECT_ORGAN","必须选择两个元件，才能计算坐标相对值");
		::AfxMessageBox(sInfo);
		return false;
	}

	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}
	
	m_bCanDrawRectTracker = true;	//新增一个基准点时，可以用鼠标拖动创建橡皮框
	m_CurrentWorkStep = COORDINATEMAPSTEP1;
	return true;
}

// 计算坐标相对值步骤2
bool CBigGraphView::CoordCalcStep2(void)
{
	if( m_pRectTracker !=NULL)
	{
		delete m_pRectTracker;
		m_pRectTracker = NULL;
	}	

	if(m_pProgramGuid == NULL || m_pProgramGuid->m_hWnd == NULL)
	{
		m_pProgramGuid = new CProgramGuid(this);
		m_pProgramGuid->Create(IDD_PRGSTEP,this);
	}
	
	m_bCanDrawRectTracker = true;	//新增一个基准点时，可以用鼠标拖动创建橡皮框
	m_CurrentWorkStep = COORDINATEMAPSTEP2;
	return true;
}*/

// 编程向导
void CBigGraphView::PrgGuide(void)
{
	//modify at 2008-03-18
	//if(m_pCurrentImage == NULL || m_pCurrentImage->IsNull())
	//	return;

	m_CurrentWorkStep = m_pTemplateData->m_LastWorkStep;

	//从上次结束的步骤开始
	if(m_CurrentWorkStep != NONE )
	{
		m_CurrentWorkStep = (WorkStep)((int)m_CurrentWorkStep - 1);
	}
	this->NextStep();	
}

// 加载模板图像
bool CBigGraphView::LoadTemplateImage(void)
{
	//wait
	CWaitLoadPCB *pWaitDlg = new CWaitLoadPCB(this);
	pWaitDlg->Create(IDD_WAIT_LOADPCB,this);
	pWaitDlg->ShowWindow(SW_SHOW);
	//pWaitDlg->CenterWindow(this);
	
	g_bStopWork = false;
	CreateThread(NULL,0,LoadPCBImageThread,(LPVOID)this,0,NULL);//创建线程
	
	//wait end
	//pWaitDlg->DestroyWindow();
	//if(pWaitDlg)
	//{
	//	delete pWaitDlg;
	//	pWaitDlg = NULL;
	//}

	return true;
}

DWORD WINAPI LoadPCBImageThread(LPVOID lpParam)
{
	CBigGraphView *pView =(CBigGraphView*)lpParam;
	if(pView->LoadTemplateImageWork())
	{
		g_bIsWait = false;
		return 0;
	}
	else
	{
		g_bIsWait = false;
		return 1;
	}
}

bool CBigGraphView::LoadTemplateImageWork(void)
{
#ifndef _MYUTEST
	if(!m_pTemplateData)
		return false;
    if(g_PCBState.GetState() != CPCBState::S_UP)
    {
        if(!g_PCBState.Up())
        {
            return false;
        }
    }

	if( m_pTemplateData->m_ImageObj.IsNull()==false)
	{
		CString sInfo=::g_LoadString("IDS_TEMPLATEIMAGE_EXIST","模板图像已经存在了，你确认要加载新的模板图像吗？");
		if(IDNO==::AfxMessageBox(sInfo,MB_YESNO) )
		{
			return false;
		}

		::EnterCriticalSection(&m_Critial4CurrentImage);
		//m_pTemplateData->m_ImageObj.ReleaseDC();
		m_pTemplateData->m_ImageObj.Destroy();
		::LeaveCriticalSection(&m_Critial4CurrentImage);
	}

	//g_Motor.AxisRunS(1,-10000000,100,1,0.001);
	//g_Motor.AxisRunS(2,-10000000,100,1,0.001);

	//电机回原点
    g_Motor.GoHome(1,100,10);
    g_Motor.GoHome(2,100,10);

    //g_Motor.GoHome(3,100,1);	

    int iWinWidth = 0;      //输出窗口宽度
    int iWinHeight = 0;     //输出窗口高度

	double	dLenOfPels = 0.00;	//每个象素的长度（单位毫米）

	int iMultiple = PCBMULTIPLE;	//pcb板缩略图缩小倍数

    //计算板的宽、高（像素）
    int iWidth = 0;
    int iHeight = 0;
	int iFovWidth = 0;
	int iFovHeight = 0;

	iWidth = m_pTemplateData->m_width * g_fPels_MM;
	iHeight = m_pTemplateData->m_height * g_fPels_MM;
	dLenOfPels = 1.00/g_fPels_MM;

    HV_RESOLUTION mode = g_camera.GetResolutionMode();
    switch(mode)
    {
    case RES_MODE0:
        //iWidth = m_pTemplateData->m_width * MM_PELS_2048;
        //iHeight = m_pTemplateData->m_height * MM_PELS_2048;
		//dLenOfPels = 1.00/(double)MM_PELS_2048;
        iWinWidth = 2048;
        iWinHeight = 1536;
        break;
    case RES_MODE1:
        iWinWidth = 1024;
        iWinHeight = 768;
        break;
    case RES_MODE2:
        iWinWidth = 640;
        iWinHeight = 480;
        break;
    case RES_MODE3:
        iWinWidth = 512;
        iWinHeight = 384;
        break;
    default:
        ::AfxMessageBox("Unknow the camera mode");
        return false;
    }

    if(!g_camera.SetOutputWindow(0, 0,iWinWidth,iWinHeight))
    {
        ::AfxMessageBox("Set camera outputwindows error");
        return false;
    }

	iWidth = iWidth/iMultiple;
	iHeight = iHeight/iMultiple;

	iFovWidth = iWinWidth/iMultiple;
	iFovHeight = iWinHeight/iMultiple;

	if(!m_pTemplateData->m_ImageObj.Create(iWidth,iHeight,24,0))
	{
		::AfxMessageBox("Create Image object error");
        return false;
	}

	int xDest = 0;
	int yDest = iHeight - iFovHeight;

	//亮灯
	GT_ExOpt(OUT_EVT_BLUE_LAMP&OUT_EVT_GREEN_LAMP&OUT_EVT_RED_LAMP);

	::Sleep(500);

	long iPosX = 0;
	long iPosY = 0;
    int iCountY = 1;    //Y轴计数
    int iCountX = 0;    //X轴计数
    int iCountStepX = 1;    //X轴计数步长
	
	while(1)
	{
		if(g_bStopWork)
		{
			break;
		}

		if(g_bPauseWork)
		{
			Sleep(50);
			continue;
		}

		TRACE("Begin get image\r\n");

		CImage tempImage[1];
		if(!g_camera.GetImage(tempImage,1,iWinWidth,iWinHeight))
		{
			::AfxMessageBox("Get image from camera error");
			m_pTemplateData->m_ImageObj.ReleaseDC();
			break;
		}

		::EnterCriticalSection(&m_Critial4CurrentImage);
		
		HDC hdc = m_pTemplateData->m_ImageObj.GetDC();
		SetStretchBltMode(hdc,HALFTONE);
		tempImage[0].StretchBlt(hdc,xDest,yDest,iFovWidth,iFovHeight,SRCCOPY);
		m_pTemplateData->m_ImageObj.ReleaseDC();
		
		::LeaveCriticalSection(&m_Critial4CurrentImage);

		ZoomImageToTemp();
		Invalidate();
        RefreshUITread();

        if(xDest <= 0 && iCountStepX < 0) //x轴超出左边边界
        {
            iCountStepX = 1;

            iCountY++;
            yDest = iHeight - iCountY*iFovHeight;	//计算下次拍照的Y原点
			if(yDest >=  0 ) //没有超出下边边界
			{
				//计算y轴移动距离
				iPosY += (dLenOfPels * iFovHeight)* g_fMotorStep*iMultiple;
				g_Motor.AxisRunS(2,iPosY,g_iMotorSpeed,1.00,0.001);
				Sleep(100);
				g_Motor.WaitStop(2);

				//直到运动到目标位置
				for(int i=0;i<10;i++)
				{
					long lMotorNowPos = 0 ;
					g_Motor.GetPos(2,&lMotorNowPos);
					lMotorNowPos *= 4;
					long lPosOffset = iPosY - lMotorNowPos;

					if(lPosOffset > 200 || lPosOffset < -200)
					{
						g_Motor.AxisRunS(2,iPosY,g_iMotorSpeed,1.00,0.001);
						::Sleep(100);
						g_Motor.WaitStop(2);
					}
					else
					{
						break;
					}
				}

                continue;
			}
			else	//超出下边边界
			{
				break;
			}//end if(yDest >=0 ) //没有超出下边边界
        }//end if(xDest <= 0 && iCountStepX < 0)

        iCountX += iCountStepX;
        xDest += iCountStepX*iFovWidth; //计算下次拍照的X原点

        //计算x轴移动距离
		iPosX = iCountX*(dLenOfPels * iFovWidth)* g_fMotorStep*iMultiple;

		if(xDest < iWidth) //x轴没有超出右边界
		{
			g_Motor.AxisRunS(1,iPosX,g_iMotorSpeed,1.00,0.001);
			Sleep(100);
			g_Motor.WaitStop(1);

			for(int i=0;i<10;i++)
			{
				long lMotorNowPos = 0 ;
				g_Motor.GetPos(1,&lMotorNowPos);
				lMotorNowPos *= 4;
				long lPosOffset = iPosX - lMotorNowPos;

				if(lPosOffset > 200 || lPosOffset < -200)
				{
					g_Motor.AxisRunS(1,iPosX,g_iMotorSpeed,1.00,0.001);
					::Sleep(100);
					g_Motor.WaitStop(1);
				}
				else
				{
					break;
				}
			}
		}
		else //x轴超出右边边界
        {
            iCountStepX = -1;
            iCountX += iCountStepX;
            xDest += iCountStepX*iFovWidth; //计算下次拍照的X原点

			iPosX = iCountX*(dLenOfPels * iFovWidth)* g_fMotorStep*iMultiple;

			/*g_Motor.GoHome(1,100,10);
			g_Motor.WaitStop(1);
			iCountX = 0;
			xDest = 0;*/

            iCountY++;
            yDest = iHeight - iCountY*iFovHeight;       //计算下次拍照的Y原点
			if(yDest >=  0 ) //没有超出下边边界
			{
				//计算y轴移动距离
				iPosY += (dLenOfPels * iFovHeight)* g_fMotorStep*iMultiple;
				g_Motor.AxisRunS(2,iPosY,g_iMotorSpeed,1.00,0.001);
				Sleep(100);

				g_Motor.WaitStop(2);

				//直到运动到目标位置
				for(int i=0;i<10;i++)
				{
					long lMotorNowPos = 0 ;
					g_Motor.GetPos(2,&lMotorNowPos);
					lMotorNowPos *= 4;
					long lPosOffset = iPosY - lMotorNowPos;

					if(lPosOffset > 200 || lPosOffset < -200)
					{
						g_Motor.AxisRunS(2,iPosY,g_iMotorSpeed,1.00,0.001);
						::Sleep(100);
						g_Motor.WaitStop(2);
					}
					else
					{
						break;
					}
				}

			}
			else//超出下边边界
			{
				break;
			}//end if(yDest >=0 )
		}//end if(xDest < iWidth )
	}//end while(1)

	GT_ExOpt(OUT_ALL_STOP);//灭灯
#else
	if( m_pTemplateData->m_ImageObj.IsNull()==false)
	{
		::EnterCriticalSection(&m_Critial4CurrentImage);
		m_pTemplateData->m_ImageObj.Destroy();
		::LeaveCriticalSection(&m_Critial4CurrentImage);
	}
	//m_pTemplateData->m_ImageObj.Load("../pcb.bmp");

	int iMultiple = PCBMULTIPLE;	//pcb板缩略图缩小倍数
	int iWidth = 1020;
	int iHeight = 768;
	int iFovWidth = 170;
	int iFovHeight = 128;
	if(!m_pTemplateData->m_ImageObj.Create(iWidth,iHeight,24,0))
	{
		::AfxMessageBox("Create Image object error");
        return false;
	}

	int xDest = 0;
	int yDest = iHeight - iFovHeight;

	long iPosX = xDest;
	long iPosY = yDest;
    int iCountY = 1;    //Y轴计数
    int iCountX = 0;    //X轴计数
    int iCountStepX = 1;    //X轴计数步长

	while(1)
	{
		CImage tempImage;
		//long lNewPosX = iPosX * ::cos(0.007) - (iPosY - iHeight+iFovHeight)*sin(0.007);
		//long lNewPosY = iPosY + iPosX * ::sin(0.007);
		CRect fovRect(iPosX,iPosY,iPosX+iFovWidth,iPosY+iFovHeight);
		LoadFovImage(fovRect,&tempImage,false);

		::Sleep(200);

		::EnterCriticalSection(&m_Critial4CurrentImage);
		
		HDC hdc = m_pTemplateData->m_ImageObj.GetDC();
		SetStretchBltMode(hdc,HALFTONE);
		tempImage.StretchBlt(hdc,xDest,yDest,iFovWidth,iFovHeight,SRCCOPY);
		m_pTemplateData->m_ImageObj.ReleaseDC();
		
		::LeaveCriticalSection(&m_Critial4CurrentImage);

		ZoomImageToTemp();
		Invalidate();
        RefreshUITread();

        if(xDest <= 0 && iCountStepX < 0) //x轴超出左边边界
        {
            iCountStepX = 1;

            iCountY++;
            yDest = iHeight - iCountY*iFovHeight;	//计算下次拍照的Y原点
			if(yDest >=  0 ) //没有超出下边边界
			{
				//计算y轴移动距离
				iPosY -= iFovHeight;
                continue;
			}
			else	//超出下边边界
			{
				break;
			}//end if(yDest >=0 ) //没有超出下边边界
        }//end if(xDest <= 0 && iCountStepX < 0)

        iCountX += iCountStepX;
        xDest += iCountStepX*iFovWidth; //计算下次拍照的X原点

        //计算x轴移动距离
		iPosX = iCountX*iFovWidth;

		if(xDest < iWidth) //x轴没有超出右边界
		{
		}
		else //x轴超出右边边界
        {
            iCountStepX = -1;
            iCountX += iCountStepX;
            xDest += iCountStepX*iFovWidth; //计算下次拍照的X原点

			iPosX = iCountX* iFovWidth;

            iCountY++;
            yDest = iHeight - iCountY*iFovHeight;       //计算下次拍照的Y原点
			if(yDest >=  0 ) //没有超出下边边界
			{
				//计算y轴移动距离
				iPosY -= iFovHeight;
			}
			else//超出下边边界
			{
				break;
			}//end if(yDest >=0 )
		}//end if(xDest < iWidth )
	}//end while(1)

#endif

	m_pTemplateData->m_bDataIsChanged = true;

	Invalidate();

	//ZoomImageToTemp();
	return true;
}

// 加载所有的FOV图像
void CBigGraphView::LoadAllFov(void)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","请先创建FOV区域");
		::AfxMessageBox(sInfo);
		return;
	}

	if(g_PCBState.GetState() != CPCBState::S_UP)
    {
        if(!g_PCBState.Up())
        {
            AfxMessageBox("No find PCB board");
            return;
        }
    }

	if(m_CurrentImageType != PCBIMAGE)
	{
		SetCurrentImage( &(m_pTemplateData->m_ImageObj),PCBIMAGE);
	}
	
	m_pTemplateData->m_bFOVIsChange = true;
	map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
	for(;it!=m_pTemplateData->m_FOVMap.end();it++)
	{
		//加载fov图像
		LoadFovImage(CRect((*it).second->left,(*it).second->top,(*it).second->right,(*it).second->bottom),
			&( (*it).second->m_ImageObj ));
	}

	m_pTemplateData->m_bDataIsChanged = true;
	m_pTemplateData->m_bFOVIsChange = true;	
}

void CBigGraphView::LoadModifyFov(void)
{
	map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
	for(;it!=m_pTemplateData->m_FOVMap.end();it++)
	{
		if( (*it).second->m_ImageObj.IsNull() )
		{
			//加载fov图像
			LoadFovImage(CRect((*it).second->left,(*it).second->top,(*it).second->right,(*it).second->bottom),
				&( (*it).second->m_ImageObj ));

			m_pTemplateData->m_bFOVIsChange = true;
			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bFOVIsChange = true;
		}
	}
}

// 元件编程
void CBigGraphView::PrgOrgan(bool bIsSingleOrgan,COrganData* pOrganData)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","请先创建FOV区域");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pTemplateData->m_lCoordScaleX<=0.00 || m_pTemplateData->m_lCoordScaleY<=0.00)
	{
		CString sInfo = ::g_LoadString("IDS_MUSTCOORDSCALE","元件编程前，请先计算坐标相对值");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pOrganProgramDlg != NULL && m_pOrganProgramDlg->m_hWnd != NULL && IsWindow(m_pOrganProgramDlg->m_hWnd))
	{
		m_pOrganProgramDlg->DestroyWindow();
		m_pOrganProgramDlg = NULL;
	}
	m_pOrganProgramDlg  = new COrganProgramDlg(bIsSingleOrgan,pOrganData,this);
	m_pOrganProgramDlg->SetTemplateData(m_pTemplateData);
	m_pOrganProgramDlg->Create(IDD_ORGANPRG,this);
	m_pOrganProgramDlg->ShowWindow(SW_SHOW);	
	if(bIsSingleOrgan)
	{
		m_pOrganProgramDlg->BeginSinglePrg();
	}
	else
	{
		m_pOrganProgramDlg->BeginPrg();
	}

	m_CurrentWorkStep = ORGAN;
}

// 通知各窗口编程数据已经改变
void CBigGraphView::NotifyPrgDataChanged(void)
{
	CMainView *pParrent=(CMainView*)this->GetParent();

	pParrent->NotifyPrgDataChanged();
}

// 通知其它窗口橡皮框已经改变
void CBigGraphView::NotifyTrakerChanged(void)
{
	CMainView *pParrent=(CMainView*)this->GetParent();
	pParrent->NotifyTrakerChanged();

	if(m_pOrganProgramDlg != NULL && m_pOrganProgramDlg->m_hWnd != NULL && IsWindow(m_pOrganProgramDlg->m_hWnd))
	{
		m_pOrganProgramDlg->NotifyTrakerChanged();
	}
}

// 条形码编程
void CBigGraphView::PrgBar(bool bDoNextStep)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","请先创建FOV区域");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_CurrentImageType != FOVIMAGE)
	{
		CString sInfo =::g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pBarPrgGuid != NULL && m_pBarPrgGuid->m_hWnd != NULL && IsWindow(m_pBarPrgGuid->m_hWnd))
	{
		m_pBarPrgGuid->DestroyWindow();
		m_pBarPrgGuid = NULL;
	}
	m_pBarPrgGuid  = new CBarPrgGuid(bDoNextStep,this);	
	m_pBarPrgGuid->Create(IDD_BARPRGGUID,this);
	m_pBarPrgGuid->SetTemplateData(m_pTemplateData);
	m_pBarPrgGuid->ShowWindow(SW_SHOW);	

	m_CurrentWorkStep = BAR;
}

// 拼板
void CBigGraphView::PrgSpellPcb(void)
{
	if(m_pTemplateData->m_editAreaBottom<=0 || m_pTemplateData->m_editAreaRight<=0)
	{
		CString sInfo = g_LoadString("IDS_PLEASECREATEEDITAREA","请先创建编辑区域");
		::AfxMessageBox(sInfo);
		return ;
	}

	if(m_pSpellPrgGuid != NULL && m_pSpellPrgGuid->m_hWnd != NULL && IsWindow(m_pSpellPrgGuid->m_hWnd))
	{
		m_pSpellPrgGuid->DestroyWindow();
		m_pSpellPrgGuid = NULL;
	}
	m_pSpellPrgGuid  = new CSpellPrgGuid(this);
	m_pSpellPrgGuid->SetTemplateData(m_pTemplateData);
	m_pSpellPrgGuid->Create(IDD_SPELLPRGGUID,this);
	m_pSpellPrgGuid->ShowWindow(SW_SHOW);

	m_CurrentWorkStep = SPELLPCB;
}



void CBigGraphView::SetCurrentOrgan(COrganData* pCurrentOrganData)
{
	m_pCurrentOrganData = pCurrentOrganData;

	if(pCurrentOrganData==NULL)
		return;

	m_lCurrentCenterX = m_pCurrentOrganData->left + m_pCurrentOrganData->GetWidth()/2;
	m_lCurrentCenterY = m_pCurrentOrganData->top + m_pCurrentOrganData->GetHeight()/2;
}

BOOL CBigGraphView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

// 修改元件编程数据
void CBigGraphView::ModifyOrganPrg(COrganData* pOrgan)
{
	if(m_pTemplateData->m_FOVMap.size()<=0)
	{
		CString sInfo = ::g_LoadString("IDS_PLEASECREATEFOV","请先创建FOV区域");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pTemplateData->m_lCoordScaleX<=0.00 || m_pTemplateData->m_lCoordScaleY<=0.00)
	{
		CString sInfo = ::g_LoadString("IDS_MUSTCOORDSCALE","元件编程前，请先计算坐");//标相对值
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_pOrganProgramDlg != NULL && m_pOrganProgramDlg->m_hWnd != NULL && IsWindow(m_pOrganProgramDlg->m_hWnd))
	{
		m_pOrganProgramDlg->DestroyWindow();
		m_pOrganProgramDlg = NULL;
	}
	m_pOrganProgramDlg  = new COrganProgramDlg(true,pOrgan,this);
	m_pOrganProgramDlg->SetTemplateData(m_pTemplateData);
	m_pOrganProgramDlg->Create(IDD_ORGANPRG,this);
	m_pOrganProgramDlg->ShowWindow(SW_SHOW);	
	
	m_pOrganProgramDlg->ModiyPrgData();
	
	m_CurrentWorkStep = ORGAN;
}

void CBigGraphView::RefreshUITread()
{
    CreateThread(NULL,0,RefreshUI,(LPVOID)this,0,NULL);//创建线程
}

DWORD WINAPI RefreshUI(LPVOID lpParam)
{
    CBigGraphView *pView = (CBigGraphView*)lpParam;
    pView->Invalidate();
    return 0;
}
void CBigGraphView::PrgAngleoffset(void)
{
	/*if(m_CurrentImageType != FOVIMAGE)
	{
		CString sInfo =::g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
		::AfxMessageBox(sInfo);
		return;
	}*/

	if(m_pAngleOffsetPrgGuid != NULL && m_pAngleOffsetPrgGuid->m_hWnd != NULL
		&& IsWindow(m_pAngleOffsetPrgGuid->m_hWnd))
	{
		m_pAngleOffsetPrgGuid->DestroyWindow();
		m_pAngleOffsetPrgGuid = NULL;
	}
	m_pAngleOffsetPrgGuid  = new CAngleOffsetPrgGuid(this);
	m_pAngleOffsetPrgGuid->SetTemplateData(m_pTemplateData);
	m_pAngleOffsetPrgGuid->Create(IDD_ANGLEOFFSETPRGGUID,this);
	m_pAngleOffsetPrgGuid->ShowWindow(SW_SHOW);

	m_CurrentWorkStep = ANGLE_OFFSET;
}
