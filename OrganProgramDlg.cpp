// OrganProgramDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "OrganProgramDlg.h"
#include "BigGraphView.h"
#include "FootProgram.h"
#include "NextPadDlg.h"
#include <math.h>

// COrganProgramDlg 对话框

IMPLEMENT_DYNAMIC(COrganProgramDlg, CDialog)

COrganProgramDlg::COrganProgramDlg(bool bIsSingleOrgan/*=false*/,COrganData* pOrganData/*=NULL*/,CWnd* pParent /*=NULL*/)
	: CDialog(COrganProgramDlg::IDD, pParent)
	, m_sName(_T(""))
	, m_sFootPrint(_T(""))
	, m_dCADX(0)
	, m_dCADY(0)
	, m_sStat(_T(""))
	, m_lCurrentX(0)
	, m_lCurrentY(0)
{
	m_pTemplateData = NULL;
	m_pParrentWnd = pParent;

	m_CurrentWorkStep = OWS_NONE;
	m_pProgramGuid = NULL;
	
	m_iFootNum = 0;
	m_lFootSpaceBetween = 0;

	m_bIsSingleOrgan = bIsSingleOrgan;
	m_pCurrentOrgan = pOrganData;//如果仅处理一个元件，则需要传人元件对象

	m_bDoWholeOrgan = false;

	m_iCurrentPadIndex = 1;
		
}

COrganProgramDlg::~COrganProgramDlg()
{
}

void COrganProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ORGANNAME, m_sName);
	DDX_Text(pDX, IDC_FOOTPRINT, m_sFootPrint);
	DDX_Text(pDX, IDC_CADX, m_dCADX);
	DDX_Text(pDX, IDC_CADY, m_dCADY);
	DDX_Text(pDX, IDC_STAT, m_sStat);
	DDX_Text(pDX, IDC_CURRENTX, m_lCurrentX);
	DDX_Text(pDX, IDC_CURRENTY3, m_lCurrentY);
	DDX_Control(pDX, IDC_TURN, m_btnTurn);
}


BEGIN_MESSAGE_MAP(COrganProgramDlg, CDialog)
	ON_BN_CLICKED(IDC_PROGRAM, &COrganProgramDlg::OnBnClickedProgram)
	ON_BN_CLICKED(IDC_SKIP, &COrganProgramDlg::OnBnClickedSkip)
	ON_BN_CLICKED(IDC_INVALID, &COrganProgramDlg::OnBnClickedInvalid)
	ON_BN_CLICKED(IDC_NEXT, &COrganProgramDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_CANCLE, &COrganProgramDlg::OnBnClickedCancle)
	ON_BN_CLICKED(IDC_OK, &COrganProgramDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TURN, &COrganProgramDlg::OnBnClickedTurn)
END_MESSAGE_MAP()


// COrganProgramDlg 消息处理程序

//处理下一个元件
bool COrganProgramDlg::NextOrganProgram(PrgState state)
{
	m_pCurrentOrgan->PState = state; 
	if(m_bIsSingleOrgan)//不处理下一个元件
		return false;

	m_CurrentOrganMapIt++;	//下一个元件
	m_pCurrentOrgan = NULL;
	
	if(NULL ==GetNextOrgan())//取下一个元件
	{
		CString sInfo = ::g_LoadString("IDS_ALLORGAN_PRG_END","所有元件编程完毕");
		::AfxMessageBox(sInfo);

		if(m_bIsSingleOrgan)
			((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);
		else
			((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(true);

		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();

		return false;
	}

	if( m_bDoWholeOrgan == false)
	{
		OnBnClickedProgram();
	}
	return true;
}

void COrganProgramDlg::OnBnClickedOk()
{
	if( m_pCurrentOrgan == NULL)
	{
		::AfxMessageBox("No Organ");
		return;
	}

	if( m_pCurrentOrgan->GetMidP().x==0 || 
		m_pCurrentOrgan->GetMidP().y==0 )
	{
		CString sInfo =::g_LoadString("IDS_INVALID_PRGDATA","无效的编程");
		::AfxMessageBox(sInfo);
		return;
	}

	if(m_bDoWholeOrgan && m_pCurrentOrgan!= NULL)
	{
		GetWholeOrganImage(m_pCurrentOrgan);
	}

	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
	}
	
	m_bDoWholeOrgan = false;
	m_btnTurn.EnableWindow(FALSE);

	((CBigGraphView*)m_pParrentWnd)->NotifyPrgDataChanged();
	((CBigGraphView*)m_pParrentWnd)->Invalidate();

	if(!m_bIsSingleOrgan)
	{
		NextOrganProgram(PS_COMPLETE);
	}
	else
	{
		if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
			this->DestroyWindow();
	}

}

//开始编程
void COrganProgramDlg::OnBnClickedProgram()
{
	if(m_pCurrentOrgan == NULL)
	{
		CString sInfo = g_LoadString("IDS_NOPRGORGAN","当前没有可编程的元件");
		::AfxMessageBox(sInfo);
		return;
	}
	else if(m_pCurrentOrgan->m_pFovData == NULL && m_pCurrentOrgan->dMidX!=0 && m_pCurrentOrgan->dMidY!=0)
	{
		CString sInfo = g_LoadString("IDS_ORGAN_NOIN_FOV","当前元件不在任何FOV中");
		::AfxMessageBox(sInfo);
		return;
	}
	else if(m_pCurrentOrgan->dMidX==0 || m_pCurrentOrgan->dMidY==0)
	{
		CString sInfo = g_LoadString("IDS_ORGARNOCADVAL","元件的CAD坐标值为0，无法自动定位此元件位置，你是否要手动定位此元件？");
		if(IDYES==::AfxMessageBox(sInfo,MB_YESNO) )
		{
			if(((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType != FOVIMAGE)
			{
				sInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
				::AfxMessageBox(sInfo);
				return;
			}

			m_bDoWholeOrgan = false;
			m_btnTurn.EnableWindow(FALSE);
			((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

			CString sStepInfo = g_LoadString("IDS_ORGAN_SERAREA_STEP_M","元件编程第一步：编辑元件搜索区\n请按下CTRL键，用鼠标框选出元件搜索区域，然后按“确定”按钮");
			
			NewWorkStep(sStepInfo,OWS_SERCHAREA,CRect(0,0,0,0));//开始编程搜索区域

			((CBigGraphView*)m_pParrentWnd)->m_bCanDrawRectTracker = true;

			return;
		}
		else
		{
			return;
		}
	}

	m_bDoWholeOrgan = false;
	m_btnTurn.EnableWindow(FALSE);

	CString sStepInfo = g_LoadString("IDS_ORGAN_SERAREA_STEP","元件编程第一步：调整元件搜索区\n请调整调整元件搜索区，然后按“确定”按钮");
	
	CRect rect = GetSerchArea( m_pCurrentOrgan );	

	NewWorkStep(sStepInfo,OWS_SERCHAREA,rect);//开始编程搜索区域
}

void COrganProgramDlg::OnBnClickedSkip()
{
	NextOrganProgram(PS_SKIP);	
}

void COrganProgramDlg::OnBnClickedInvalid()
{
	NextOrganProgram(PS_INVALID);	
}

void COrganProgramDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
}

void COrganProgramDlg::OnBnClickedCancle()
{
	((CBigGraphView*)m_pParrentWnd)->CancelCurrentWork();
	if(this->m_hWnd != NULL && ::IsWindow(this->m_hWnd))
		this->DestroyWindow();
}

void COrganProgramDlg::CancelCurrentWork()
{
	m_CurrentWorkStep = OWS_NONE;
	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}	
}

// 准备编程
void COrganProgramDlg::BeginPrg(void)
{
	if( m_pTemplateData == NULL)
		return;

	m_CurrentFovMapIt = m_pTemplateData->m_FOVMap.begin();
	m_CurrentOrganMapIt = m_pTemplateData->m_OrganDataMap.begin();

	//取下一个元件
	if(NULL ==GetNextOrgan())
	{
		CString sInfo = ::g_LoadString("IDS_ALLORGAN_PRG_END","所有元件编程完毕");
		::AfxMessageBox(sInfo);
	}
}

//取得元件在FOV坐标系中的中心点
CPoint COrganProgramDlg::GetOrganInFovCenter(const CFovData* pFovData,const COrganData* pOrganData)
{
	//pcb坐标系中的位置
	long lOrganInPcbX = pOrganData->dMidX * (m_pTemplateData->m_lCoordScaleX) + m_pTemplateData->m_lOffsetX;
	//pcb坐标系中的位置(CAD坐标原点在坐下角)
	long lOrganInPcbY = m_pTemplateData->m_ImageObj.GetHeight() - 
		(pOrganData->dMidY * (m_pTemplateData->m_lCoordScaleY) + m_pTemplateData->m_lOffsetY);

	//在FOV中的点
	long lOrganInFovX = lOrganInPcbX - pFovData->left;
	long lOrganInFovY = lOrganInPcbY - pFovData->top;

	//放大
	lOrganInFovX = lOrganInFovX * ( double(pFovData->m_ImageObj.GetWidth())/double(pFovData->right - pFovData->left) );
	lOrganInFovY = lOrganInFovY * ( double(pFovData->m_ImageObj.GetHeight())/double(pFovData->bottom - pFovData->top) );

	return CPoint(lOrganInFovX,lOrganInFovY);
}

/* 检查元件是否在FOV内
在返回true,否则返回false
*/
bool COrganProgramDlg::OrganInFov(const CFovData* pFovData, const COrganData* pOrganData)
{
	long	lOrganX;//元件中心X点
	long	lOrganY;//元件中心Y点

	lOrganX = pOrganData->dMidX * m_pTemplateData->m_lCoordScaleX + m_pTemplateData->m_lOffsetX;
	
	//CAD坐标原点是在左下角
	lOrganY = m_pTemplateData->m_ImageObj.GetHeight() - (pOrganData->dMidY * m_pTemplateData->m_lCoordScaleY + m_pTemplateData->m_lOffsetY);

	CRect fovRect(pFovData->left,pFovData->top,pFovData->right,pFovData->bottom);

	return (bool)fovRect.PtInRect( CPoint(lOrganX,lOrganY) );
}

// 根据元件型号查找相同的已经编程好的元件
COrganData* COrganProgramDlg::SearchSameOrgan(const CString sFootPrint)
{
	map<long,COrganData*>::iterator it = m_pTemplateData->m_OrganDataMap.begin();
	for(;it != m_pTemplateData->m_OrganDataMap.end();it++)
	{
		//相同的型号，且此元件已经编程完毕
		if( (*it).second->sFootPrint == sFootPrint && (*it).second->PState == PS_COMPLETE )
		{
			return (*it).second;
		}
	}
	return NULL;
}


// 取下一个要编程的元件
COrganData* COrganProgramDlg::GetNextOrgan(void)
{
	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	double dMultiple = (1+iCurrentZoom*ZOOMSTEP);

	for(;m_CurrentFovMapIt != m_pTemplateData->m_FOVMap.end();m_CurrentFovMapIt++)
	{
		
		for(;m_CurrentOrganMapIt!=m_pTemplateData->m_OrganDataMap.end(); m_CurrentOrganMapIt++)
		{
			//检查元件是否在FOV内
			if( OrganInFov( (*m_CurrentFovMapIt).second,(*m_CurrentOrganMapIt).second ) )
			{
				if( (*m_CurrentFovMapIt).second->m_ImageObj.IsNull())
				{
					CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","请先加载FOV图像");
					::AfxMessageBox(sInfo);					
					return NULL;
				}

				m_bDoWholeOrgan = false;

				//没有编程过的或者之前无效的，才取出
				if( (*m_CurrentOrganMapIt).second->PState == PS_NEW ||
					(*m_CurrentOrganMapIt).second->PState == PS_INVALID )
				{
					m_sName  = (*m_CurrentOrganMapIt).second->sName;
					m_sFootPrint = (*m_CurrentOrganMapIt).second->sFootPrint;
					m_dCADX = (*m_CurrentOrganMapIt).second->dMidX;
					m_dCADY = (*m_CurrentOrganMapIt).second->dMidY;					

					//取得当前元件中心（相对与当前FOV的坐标系）						
					CPoint point = GetOrganInFovCenter((*m_CurrentFovMapIt).second ,(*m_CurrentOrganMapIt).second);	
					m_lCurrentX = point.x;
					m_lCurrentY = point.y;

					this->UpdateData(FALSE);

					//切换到FOV图片
					((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov((*m_CurrentFovMapIt).second);
					((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &((*m_CurrentFovMapIt).second->m_ImageObj),FOVIMAGE);

					m_pCurrentOrgan = (*m_CurrentOrganMapIt).second;
					((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

					//设置此元件所属的FOV
					m_pCurrentOrgan->fovId = (*m_CurrentFovMapIt).second->fovId;
					m_pCurrentOrgan->m_pFovData = (*m_CurrentFovMapIt).second;

					//查找相同型号且已经编程好的元件
					m_btnTurn.EnableWindow(FALSE);
					COrganData* pSameOrgan = SearchSameOrgan((*m_CurrentOrganMapIt).second->sFootPrint);
					if( pSameOrgan != NULL)
					{
						//拷贝编程数据
						(*m_CurrentOrganMapIt).second->CopyPrgData(pSameOrgan,m_lCurrentX,m_lCurrentY);

						m_bDoWholeOrgan = true;
						m_btnTurn.EnableWindow(TRUE);
						m_CurrentWorkStep = OWS_MAINBODY;//如果为OWS_FOOT，则此后修改主体的橡皮框时，会修改引脚的数据

						CString sInfo =::g_LoadString("IDS_FIND_SAME_ORGAN","找到与当前元件相同型号的元件，并拷贝了它的编程数据，如果位置偏移，请调整元件中心位置然后“确定”，结束此元件编程，如果需要重新调整，请按“编程”按钮进入编程流程");
						::AfxMessageBox(sInfo);

						//取元件的主体区域
						CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);
						((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//放大缩小到当前的倍数

						((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);
						long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
						long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

						//橡皮框大小变动
						/*workRect.left *= dMultiple;
						workRect.right *=dMultiple;
						workRect.top *=dMultiple;
						workRect.bottom *=dMultiple;*/
									
						//切换到窗口坐标系
						workRect.left -=lSrcOriginX;
						workRect.right -=lSrcOriginX;
						workRect.top -=lSrcOriginY;
						workRect.bottom -=lSrcOriginY;						
											
						((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
						((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

					}
					else
					{
						//modify at 2008-04-22
						//m_lCurrentX *= dMultiple;
						//m_lCurrentY *= dMultiple;
						((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(m_lCurrentX, m_lCurrentY);
					}
					
					((CBigGraphView*)m_pParrentWnd)->Invalidate();

					return m_pCurrentOrgan;
				}
			}
		}//end for2

		//当前FOV处理完毕，将要出来下一个FOV，元件又要从头找过
		m_CurrentOrganMapIt = m_pTemplateData->m_OrganDataMap.begin();
	}//end for1

	return NULL;
}

// 计算元件的搜索区域
CRect COrganProgramDlg::GetSerchArea(COrganData* pOrganData)
{
	if( (pOrganData->RangeLeft + pOrganData->RangeRight) >0 &&
		(pOrganData->RangeBottom + pOrganData->RrangeTop) >0 )
	{	
		//区域已经存在，复制并销毁
		CRect rect(pOrganData->RangeLeft,pOrganData->RrangeTop,pOrganData->RangeRight,pOrganData->RangeBottom);
		//pOrganData->RangeLeft = pOrganData->RangeRight =0;
		//pOrganData->RrangeTop = pOrganData->RangeBottom = 0;
		
		return rect;
	} 
	else
	{
		//计算焊盘中心与元件中心的距离
		long lWidth= FOVMULTIPLE*fabs(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		if( lWidth == 0)
		{
			lWidth = FOVMULTIPLE*fabs(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;
		}
		if( lWidth == 0)
			lWidth = 150;

		lWidth += 15;
		
		CRect rect;
		rect.left = m_lCurrentX -lWidth;
		rect.right = m_lCurrentX +lWidth;
		rect.top = m_lCurrentY -lWidth;
		rect.bottom = m_lCurrentY +lWidth;

		return rect;
	}	
}

// 结束当前工作
void COrganProgramDlg::EndCurrentWork(void)
{
	CString sMsgInfo ;
	
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	if( ((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType != FOVIMAGE )
	{
		sMsgInfo = g_LoadString("IDS_PLEASE2FOVIMAGE","请切换到FOV模式中");
		AfxMessageBox(sMsgInfo);
		return;
	}

	if(pRectTracker == NULL)
	{
		sMsgInfo = g_LoadString("IDS_CURRENTWORKNOMODIFY","当前的编程工作内容没有修改");
		AfxMessageBox(sMsgInfo);
		return;
	}

	m_pCurrentOrgan->fovId = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov->fovId;
	m_pCurrentOrgan->m_pFovData = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;

	switch(m_CurrentWorkStep)
	{
	case OWS_SERCHAREA:	
		
		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_AREA","你确定此元件搜索区域已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			m_pCurrentOrgan->RangeBottom = originRect.bottom;
			m_pCurrentOrgan->RangeLeft = originRect.left;
			m_pCurrentOrgan->RangeRight = originRect.right;
			m_pCurrentOrgan->RrangeTop = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;

			//元件没有CAD坐标值
			if(m_pCurrentOrgan->dMidX==0 || m_pCurrentOrgan->dMidY==0)
			{
				CFovData *pCurrentShowFov = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;

				m_pCurrentOrgan->fovId = pCurrentShowFov->fovId;
				m_pCurrentOrgan->m_pFovData = pCurrentShowFov;

				m_lCurrentX = originRect.CenterPoint().x;
				m_lCurrentY = originRect.CenterPoint().y;

				long lMidX = originRect.CenterPoint().x/FOVMULTIPLE + pCurrentShowFov->left;
				long lMidY = m_pTemplateData->m_ImageObj.GetHeight() - (originRect.CenterPoint().y/FOVMULTIPLE + pCurrentShowFov->top);

				//计算CAD坐标值
				m_pCurrentOrgan->dMidX = (double)(lMidX - m_pTemplateData->m_lOffsetX)/(double)m_pTemplateData->m_lCoordScaleX;
				m_pCurrentOrgan->dMidY = (double)(lMidY - m_pTemplateData->m_lOffsetY)/(double)m_pTemplateData->m_lCoordScaleY;

				m_pCurrentOrgan->dPadX = m_pCurrentOrgan->dMidX -2.0;
				m_pCurrentOrgan->dPadY= m_pCurrentOrgan->dMidY;
			}
			
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();

		}
		else
		{
			return;
		}
		NextStep();
		break;

	case OWS_MAINBODY:	//
		
		sMsgInfo = g_LoadString("IDS_SUER_ORGAN_BODY","你确定此元件主体已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			//取得元件主体的图像
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(originRect,pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}

			vector<CImage*>::iterator bodyIt=m_pCurrentOrgan->m_MainBodyImageList.begin();
			for(;bodyIt!=m_pCurrentOrgan->m_MainBodyImageList.end();bodyIt++)
			{
				CImage* pOldImage = (*bodyIt);
				pOldImage->Destroy();
				pOldImage = NULL;
			}
			m_pCurrentOrgan->m_MainBodyImageList.clear();

			m_pCurrentOrgan->m_MainBodyImageList.push_back(pImage);

			m_pCurrentOrgan->bottom = originRect.bottom;
			m_pCurrentOrgan->left = originRect.left;
			m_pCurrentOrgan->right = originRect.right;
			m_pCurrentOrgan->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bImagChanged = true;
				
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
			
		}
		else
		{
			return;
		}
		NextStep();
		break;
	case OWS_PAD:
		
		sMsgInfo = g_LoadString("IDS_SURE_PAD","你确定此焊盘已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT
			//取得元件焊盘的图像
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(originRect,pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}

			vector<CImage*>::iterator PadIt=m_pCurrentPad->m_ImageList.begin();
			for(;PadIt!=m_pCurrentPad->m_ImageList.end();PadIt++)
			{
				CImage* pOldImage = (*PadIt);
				pOldImage->Destroy();
				pOldImage = NULL;
			}
			m_pCurrentPad->m_ImageList.clear();

			m_pCurrentPad->m_ImageList.push_back(pImage);

			m_pCurrentPad->bottom = originRect.bottom;
			m_pCurrentPad->left = originRect.left;
			m_pCurrentPad->right = originRect.right;
			m_pCurrentPad->top = originRect.top;

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;			
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bPadChanged = true;
				
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
			
		}
		else
		{
			return;
		}
		NextStep();
		break;
	case OWS_FOOT:
		
		sMsgInfo = g_LoadString("IDS_SURE_FOOT","你确定此引脚已经编程好了吗？");
		if(IDYES==::AfxMessageBox(sMsgInfo,MB_YESNO) )
		{
			CRect CurrentZoomRect;//当前放大倍数下的RECT
			CRect originRect;

			//橡皮框当前的坐标体系是窗口坐标，要将它转换成图像坐标(加上图像与窗口(0,0)点重合的点)
			LONG lTmp = pRectTracker->m_rect.left;
			if(pRectTracker->m_rect.left > pRectTracker->m_rect.right)
			{
				pRectTracker->m_rect.left = pRectTracker->m_rect.right;
				pRectTracker->m_rect.right = lTmp;
			}

			lTmp = pRectTracker->m_rect.top;
			if(pRectTracker->m_rect.top > pRectTracker->m_rect.bottom)
			{
				pRectTracker->m_rect.top = pRectTracker->m_rect.bottom;
				pRectTracker->m_rect.bottom = lTmp;
			}

			CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
			CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
			CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
			CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;

			((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

			//取第一个焊脚
			vector<CFootData*>::iterator it = m_pCurrentPad->m_footList.begin();
			if(it != m_pCurrentPad->m_footList.end() )
			{
				(*it)->bottom = originRect.bottom;
				(*it)->left = originRect.left;
				(*it)->right = originRect.right;
				(*it)->top = originRect.top;

				(*it)->bVisible = true;
			}

			//取焊脚图像
			for(; it!=m_pCurrentPad->m_footList.end(); it++)
			{
				CRect rect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
				CImage* pImage=new CImage();
				((CBigGraphView*)m_pParrentWnd)->GetFactImage(rect,pImage);
				if(pImage->IsNull())
				{
					delete pImage;
					pImage = NULL;
					return;
				}

				vector<CImage*>::iterator FootImagIt=(*it)->m_ImageList.begin();
				for(;FootImagIt!=(*it)->m_ImageList.end();FootImagIt++)
				{
					CImage* pOldImage = (*FootImagIt);
					pOldImage->Destroy();
					pOldImage = NULL;
				}
				(*it)->m_ImageList.clear();

				(*it)->m_ImageList.push_back(pImage);
			}

			m_pTemplateData->m_bDataIsChanged = true;
			m_pTemplateData->m_bOrganDataIsChange = true;
			m_pCurrentOrgan->bDataChanged = true;
			m_pCurrentOrgan->bPadChanged = true;

			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
			pRectTracker = NULL;
			((CBigGraphView*)m_pParrentWnd)->Invalidate();

			if( m_WorkStepAfterFoot == OWS_PAD)
			{
				//处理下一个焊盘及引脚
				int iSelect=0;
				CNextPadDlg nextPad(&iSelect,this);
				if( IDOK == nextPad.DoModal())
				{
					CPadData *pNewPadData=NULL;
					if(m_iCurrentPadIndex == 4)
					{
						NextOrganProgram(PS_COMPLETE);
						m_iCurrentPadIndex = 1;
						return;
					}
					else
					{
						m_iCurrentPadIndex++;
						if( m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] != NULL)
							delete m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
						m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] = new CPadData();
						pNewPadData = m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
					}
	
					CopyPad(m_pCurrentPad, pNewPadData, iSelect);
					m_pCurrentPad = pNewPadData;
					m_CurrentWorkStep = OWS_MAINBODY;//OWS_PAD;
					((CBigGraphView*)m_pParrentWnd)->Invalidate();
					NextStep();
				} //end if( IDOK == nextPad.DoModal())
			}//end if( m_WorkStepAfterFoot == OWS_PAD)
			else
			{
				m_iCurrentPadIndex = 1;
				if(false==NextOrganProgram(PS_COMPLETE))
				{
					/*if(m_bIsSingleOrgan)
						((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);
					else
						((CBigGraphView*)m_pParrentWnd)->EndCurrentWork(false);

					this->DestroyWindow();*/
				}
			}
		}
		else
		{
			return;
		}		
		break;
	
	default:
		break;
	}
	
	((CBigGraphView*)m_pParrentWnd)->NotifyPrgDataChanged();
}

// 处理下一步
void COrganProgramDlg::NextStep(void)
{
	CRect bodyRect;
	CString sStepInfo;
	CRect rect;
	CFootProgram *pFootPrgDlg=NULL;

	switch(m_CurrentWorkStep)
	{
	case OWS_SERCHAREA:		
		sStepInfo = g_LoadString("IDS_ORGAN_BODY_STEP","元件编程第二步：调整元件主体区\n请调整元件主体区，然后按“确定”按钮");
	
		bodyRect = GetBodyArea(m_pCurrentOrgan);
		NewWorkStep(sStepInfo,OWS_MAINBODY,bodyRect);//开始编程主体区域
		break;
	case OWS_MAINBODY:		
		sStepInfo = g_LoadString("IDS_ORGAN_PAD_STEP","元件编程第三步：调整元件焊盘区\n请调整元件焊盘区，然后按“确定”按钮");
	
		if(m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1]==NULL)
		{
			m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1] = new CPadData();
		}
		m_pCurrentPad = m_pCurrentOrgan->m_pPadData[m_iCurrentPadIndex-1];
		//m_iCurrentPadIndex = 1;	//第一焊盘

		rect = GetPadRect( m_pCurrentOrgan,m_pCurrentPad);
		NewWorkStep(sStepInfo,OWS_PAD,rect);//开始编程搜索区域
		break;
	case OWS_PAD:		
		sStepInfo = g_LoadString("IDS_ORGAN_FOOT_STEP","元件编程第四步：调整元件引脚\n请调整元件引脚，然后按“确定”按钮");
	
		//第一引脚做橡皮框
		if( m_pCurrentPad != NULL)
		{
			vector<CFootData*>::iterator footIt = m_pCurrentPad->m_footList.begin();
			if(footIt != m_pCurrentPad->m_footList.end())
			{
				CRect rc((*footIt)->left,(*footIt)->top,(*footIt)->right,(*footIt)->bottom);
				(*footIt)->bVisible = false;

				((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(rc);//放大到当前的倍数	
	
				//设置中心点和原点
				((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(rc.left + rc.Width()/2, rc.top + rc.Height()/2);	
				long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
				long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

				CRect trackerRect(rc.left-lSrcOriginX,rc.top-lSrcOriginY,rc.right-lSrcOriginX,rc.bottom-lSrcOriginY);
				
				((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&trackerRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
				((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
			}
			//刷新屏幕	
			((CBigGraphView*)m_pParrentWnd)->Invalidate();
		}

		if(m_pProgramGuid != NULL && m_pProgramGuid->m_hWnd && IsWindow(m_pProgramGuid->m_hWnd))
		{
			m_pProgramGuid->DestroyWindow();
			delete m_pProgramGuid;
			m_pProgramGuid = NULL;
		}

		pFootPrgDlg = new CFootProgram(this);
		pFootPrgDlg->Create(IDD_FOOTPRG,this);
		pFootPrgDlg ->ShowWindow(SW_SHOW);

		m_CurrentWorkStep = OWS_FOOT;
		break;
	case OWS_FOOT:		
		sStepInfo = g_LoadString("IDS_ORGAN_PAD_STEP","元件编程第三步：调整元件焊盘区\n请调整元件焊盘区，然后按“确定”按钮");
	
		rect = CRect(m_pCurrentPad->left,m_pCurrentPad->top,m_pCurrentPad->right,m_pCurrentPad->bottom);
		NewWorkStep(sStepInfo,OWS_PAD,rect);//开始编程搜索区域
		break;
	}
}

// 计算主体区域
CRect COrganProgramDlg::GetBodyArea(COrganData* pOrganData)
{
	if( (pOrganData->left + pOrganData->right) >0 &&
		(pOrganData->bottom + pOrganData->top) >0 )
	{	
		//区域已经存在，复制并销毁
		CRect rect(pOrganData->left,pOrganData->top,pOrganData->right,pOrganData->bottom);
		pOrganData->left = pOrganData->right =0;
		pOrganData->top = pOrganData->bottom = 0;

		return rect;
	}
	else
	{
		//计算焊盘中心与元件中心的距离
		long lWidth= FOVMULTIPLE*fabs(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		if( lWidth == 0)
		{
			lWidth = FOVMULTIPLE*fabs(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;
		}
		if( lWidth == 0)
			lWidth = 150;

		lWidth -= 10;
		CRect rect;
		rect.left = ((m_lCurrentX -lWidth) > pOrganData->RangeLeft)?(m_lCurrentX -lWidth):(pOrganData->RangeLeft+10);
		rect.right = ((m_lCurrentX +lWidth) < pOrganData->RangeRight)?(m_lCurrentX +lWidth):(pOrganData->RangeRight-10);
		rect.top = ((m_lCurrentY -lWidth) > pOrganData->RrangeTop)?(m_lCurrentY -lWidth):(pOrganData->RrangeTop+10);
		rect.bottom = ((m_lCurrentY +lWidth) < pOrganData->RangeBottom)?(m_lCurrentY +lWidth):(pOrganData->RangeBottom -10);

		return rect;
	}
}

// 新的工作步骤
void COrganProgramDlg::NewWorkStep(CString sShowInfo, OrganWorkStep step, CRect workRect)
{
	if(m_pProgramGuid == NULL || m_pProgramGuid->m_hWnd==NULL || !IsWindow(m_pProgramGuid->m_hWnd))
	{
		m_pProgramGuid = new COrganPrgGuid(this);
		m_pProgramGuid->Create(IDD_ORGANPRGSTEP,this);
	}		
	
	CStatic *pInfoCtrl = (CStatic *)m_pProgramGuid->GetDlgItem(IDC_INFO);
	pInfoCtrl->SetWindowTextA(sShowInfo);
	m_pProgramGuid->ShowWindow(SW_SHOW);

	m_CurrentWorkStep = step;

	if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker !=NULL)
	{
		delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
	}	

	((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//放大到当前的倍数

	//设置中心点和原点
	((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
	long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;
				
	//切换到窗口坐标系
	workRect.left -=lSrcOriginX;
	workRect.right -=lSrcOriginX;
	workRect.top -=lSrcOriginY;
	workRect.bottom -=lSrcOriginY;
		
	((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
	((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;

	//刷新屏幕	
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

// 取得焊盘Rect
CRect COrganProgramDlg::GetPadRect(COrganData* pOrganData,CPadData *pPadData)
{
	//CPadData *pPadData=pOrganData->m_pPadData[0];
	
	if( pPadData!=NULL && (pPadData->left + pPadData->right) >0 &&
		(pPadData->bottom + pPadData->top) >0 )
	{	
		//区域已经存在，复制并销毁
		CRect rect(pPadData->left,pPadData->top,pPadData->right,pPadData->bottom);
		pPadData->left = pPadData->right =0;
		pPadData->top = pPadData->bottom = 0;

		return rect;
	}
	else
	{
		//计算焊盘中心与元件中心的距离
		long lPadMidX= m_lCurrentX + FOVMULTIPLE*(pOrganData->dPadX - pOrganData->dMidX) * m_pTemplateData->m_lCoordScaleX;
		long lPadMidY= m_lCurrentY + FOVMULTIPLE*(pOrganData->dPadY - pOrganData->dMidY) * m_pTemplateData->m_lCoordScaleY;

		long lWidth = labs( labs(lPadMidX - m_lCurrentX) - (pOrganData->right - pOrganData->left)/2 );
		long lHeight = labs( labs(lPadMidY - m_lCurrentY) - (pOrganData->bottom - pOrganData->top)/2 );
		
		CRect rect;
		rect.left = lPadMidX -lWidth;
		rect.right = lPadMidX +lWidth;
		rect.top = lPadMidY -lHeight;
		rect.bottom = lPadMidY +lHeight;

		return rect;
	}
}

// 增加焊脚数
void COrganProgramDlg::AddFootNum(int iNum)
{
	m_pCurrentPad->AddFootNum(iNum);
	
	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	//取第一个引脚做成橡皮框
	vector<CFootData*>::iterator it=m_pCurrentPad->m_footList.begin();
	if(it != m_pCurrentPad->m_footList.end())
	{
		CRect rect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);

		//不要销毁(*it)->left=(*it)->top=(*it)->right=(*it)->bottom=0;
		(*it)->bVisible = false;	//将它置为不可视
		
		((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(rect);//放大到当前倍数
		
		//切换到窗口坐标系
		rect.left -=lSrcOriginX;
		rect.right -=lSrcOriginX;
		rect.top -=lSrcOriginY;
		rect.bottom -=lSrcOriginY;

		if( ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
		{
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker=NULL;
		}

		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&rect,CRectTracker::resizeOutside|CRectTracker::dottedLine);		
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
	
}

/*
*增加焊脚间间隔
*lSpace:间隔
*/
void COrganProgramDlg::AddFootSpaceBetween(long lSpace)
{
	m_pCurrentPad->AddFootSpaceBetween(lSpace);
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

/* 从源焊盘拷贝编程数据到目的焊盘
*srcPad：源
*destPad：目的
*iRelative：目的与源的关系，1：目的在源的对面，2：在左边，3：在右边
**/
void COrganProgramDlg::CopyPad(CPadData * srcPad, CPadData * destPad, int iRelative)
{
	long	lMoveStep=0;
	long lDestPadMidX = 0;
	long lDestPadMidY = 0;
	long lDestFootMidX=0;
	long lDestFootMidY=0;
	
	switch(iRelative)
	{
	case 1://对面
		if( srcPad->GetHeight() >= srcPad->GetWidth() )
		{
			destPad->CopyRect(srcPad);//拷贝源的rect
			lMoveStep = 2*(m_pCurrentOrgan->GetMidP().x - srcPad->GetMidP().x);
			destPad->MoveX(lMoveStep);	//沿X轴平移

			//拷贝焊脚
			vector<CFootData*>::iterator it = srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				CFootData *pFootData = new CFootData();
				pFootData->CopyRect( (*it) );//拷贝源的rect
				pFootData->MoveX(lMoveStep);//沿X轴平移

				destPad->m_footList.push_back(pFootData);
			}			
		}
		else
		{
			destPad->CopyRect(srcPad);//拷贝源的rect
			lMoveStep = 2*(m_pCurrentOrgan->GetMidP().y - srcPad->GetMidP().y);
			destPad->MoveY(lMoveStep);	//沿Y轴平移

			//拷贝焊脚
			vector<CFootData*>::iterator it = srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				CFootData *pFootData = new CFootData();
				pFootData->CopyRect( (*it) );//拷贝源的rect
				pFootData->MoveY(lMoveStep);//沿X轴平移

				destPad->m_footList.push_back(pFootData);
			}
		}
		break;
	case 2:
	case 3:
		if( srcPad->GetHeight() >= srcPad->GetWidth() ) //源焊盘在元件左右两侧
		{
			lDestPadMidX = m_pCurrentOrgan->GetMidP().x;
			
			//拷贝到元件上边
			if( (iRelative == 2 && (srcPad->GetMidP().x < m_pCurrentOrgan->GetMidP().x) ) ||
				(iRelative == 3 && (srcPad->GetMidP().x > m_pCurrentOrgan->GetMidP().x) ) )
			{
				lDestPadMidY = m_pCurrentOrgan->GetMidP().y - m_pCurrentOrgan->GetHeight()/2 -srcPad->GetWidth()/2;
			}
			else //拷贝到元件下边
			{
				lDestPadMidY = m_pCurrentOrgan->GetMidP().y + m_pCurrentOrgan->GetHeight()/2 +srcPad->GetWidth()/2;
			}

			destPad->CopyPrgData(srcPad,lDestPadMidX,lDestPadMidY); //拷贝焊盘
			destPad->Turn();	//旋转

			//拷贝焊脚
			/*lDestFootMidY = lDestPadMidY;

			vector<CFootData*>::iterator it =srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				lDestFootMidX = lDestPadMidX - (srcPad->GetMidP().y - (*it)->GetMidP().y);
				CFootData *pFootData = new CFootData();
				pFootData->CopyPrgData( (*it) ,lDestFootMidX,lDestFootMidY); //拷贝
				pFootData->Turn(); //翻转
				destPad->m_footList.push_back(pFootData);
			}			*/
		}
		else //源焊盘在元件上下两侧
		{
			lDestPadMidY = m_pCurrentOrgan->GetMidP().y;
			
			//拷贝到元件右边
			if( (iRelative == 2 && (srcPad->GetMidP().y < m_pCurrentOrgan->GetMidP().y) ) ||
				(iRelative == 3 && (srcPad->GetMidP().y > m_pCurrentOrgan->GetMidP().y) ) )
			{
				lDestPadMidX = m_pCurrentOrgan->GetMidP().x + m_pCurrentOrgan->GetWidth()/2 +srcPad->GetHeight()/2;
			}
			else //拷贝到元件左边
			{
				lDestPadMidX = m_pCurrentOrgan->GetMidP().x - m_pCurrentOrgan->GetWidth()/2 -srcPad->GetHeight()/2;
			}

			destPad->CopyPrgData(srcPad,lDestPadMidX,lDestPadMidY); //拷贝焊盘
			destPad->Turn();	//旋转

			//拷贝焊脚
			/*lDestFootMidX = lDestPadMidX;

			vector<CFootData*>::iterator it =srcPad->m_footList.begin();
			for(;it != srcPad->m_footList.end();it++)
			{
				lDestFootMidY = lDestPadMidY - (srcPad->GetMidP().x - (*it)->GetMidP().x);
				CFootData *pFootData = new CFootData();
				pFootData->CopyPrgData( (*it) ,lDestFootMidX,lDestFootMidY); //拷贝
				pFootData->Turn(); //翻转
				destPad->m_footList.push_back(pFootData);
			}*/
		}
		break;
	default:
		break;
	}

	//取得元件焊盘的图像
	CImage* pImage=new CImage();
	((CBigGraphView*)m_pParrentWnd)->GetFactImage(CRect(destPad->left,destPad->top,destPad->right,destPad->bottom),pImage);
	if(pImage->IsNull())
	{
		delete pImage;
		pImage = NULL;
		return;
	}
	destPad->m_ImageList.push_back(pImage);

	//第一引脚做橡皮框
	/*long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

	vector<CFootData*>::iterator footIt = destPad->m_footList.begin();
	if(footIt != destPad->m_footList.end())
	{
		CRect rc((*footIt)->left-lSrcOriginX,(*footIt)->top-lSrcOriginY,
			(*footIt)->right-lSrcOriginX,(*footIt)->bottom-lSrcOriginY);
		(*footIt)->bVisible = false;
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&rc,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize = TRACKERSIZE;
	}*/
	//刷新屏幕	
	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}


BOOL COrganProgramDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);
	
	this->SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void COrganProgramDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
}

//单一元件编程
void COrganProgramDlg::BeginSinglePrg(void)
{
	if(m_pCurrentOrgan == NULL)
		return;

	m_sName  = m_pCurrentOrgan->sName;
	m_sFootPrint = m_pCurrentOrgan->sFootPrint;
	m_dCADX = m_pCurrentOrgan->dMidX;
	m_dCADY = m_pCurrentOrgan->dMidY;

	this->UpdateData(FALSE);

	CFovData*	pBelongFovData = NULL;	//元件所属的FOV
	if(m_pCurrentOrgan->m_pFovData != NULL)
	{
		pBelongFovData = m_pCurrentOrgan->m_pFovData;
	}
	else
	{
		map<long,CFovData*>::iterator it = m_pTemplateData->m_FOVMap.begin();
		for(;it != m_pTemplateData->m_FOVMap.end();it++)
		{
			//检查元件是否在FOV内
			if( OrganInFov( (*it).second,m_pCurrentOrgan ) )
			{
				pBelongFovData = (*it).second;
				break;
			}
		}
	}

	if( pBelongFovData == NULL)
		return;
	
	if(pBelongFovData->m_ImageObj.IsNull())
	{
		CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","请先加载FOV图像");
		::AfxMessageBox(sInfo);
		return;
	}

	//设置此元件所属的FOV
	m_pCurrentOrgan->fovId = pBelongFovData->fovId;
	m_pCurrentOrgan->m_pFovData = pBelongFovData;

	//取得当前元件中心（相对与当前FOV的坐标系）						
	CPoint point = GetOrganInFovCenter(pBelongFovData ,m_pCurrentOrgan);	
	m_lCurrentX = point.x;
	m_lCurrentY = point.y;

	this->UpdateData(FALSE);

	//切换到FOV图片
	((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov(pBelongFovData);
	((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &(pBelongFovData->m_ImageObj),FOVIMAGE);			

	((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	double dMultiple = (1+iCurrentZoom*ZOOMSTEP);
	
	//modify at 2008-04-22
	//((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_pCurrentOrgan->GetMidP().x*(1+iCurrentZoom*ZOOMSTEP);
	//((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_pCurrentOrgan->GetMidP().y*(1+iCurrentZoom*ZOOMSTEP);
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_lCurrentX*dMultiple;
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_lCurrentY*dMultiple;
		
	
	if(m_pCurrentOrgan != NULL)
	{
		//查找相同型号且已经编程好的元件		
		COrganData* pSameOrgan = SearchSameOrgan(m_pCurrentOrgan->sFootPrint);
		if( pSameOrgan != NULL)
		{
			//拷贝编程数据
			m_pCurrentOrgan->CopyPrgData(pSameOrgan,m_lCurrentX,m_lCurrentY);

			m_bDoWholeOrgan = true;
			m_btnTurn.EnableWindow(TRUE);
			m_CurrentWorkStep = OWS_MAINBODY;//如果为OWS_FOOT，则此后修改主体的橡皮框时，会修改引脚的数据

			CString sInfo =::g_LoadString("IDS_FIND_SAME_ORGAN","找到与当前元件相同型号的元件，并拷贝了它的编程数据，如果位置偏移，请调整元件中心位置然后“确定”，结束此元件编程，如果需要重新调整，请按“编程”按钮进入编程流程");
			::AfxMessageBox(sInfo);

			//取元件的主体区域
			CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

			((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//放大缩小到当前的倍数

			((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
			long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
			long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

			//橡皮框大小变动
			/*workRect.left *= dMultiple;
			workRect.right *=dMultiple;
			workRect.top *=dMultiple;
			workRect.bottom *=dMultiple;*/

			//切换到窗口坐标系
			workRect.left -=lSrcOriginX;
			workRect.right -=lSrcOriginX;
			workRect.top -=lSrcOriginY;
			workRect.bottom -=lSrcOriginY;						

			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;
		}
		else
		{
			//modify at 2008-04-22
			//m_lCurrentX *= dMultiple;
			//m_lCurrentY *= dMultiple;
			//((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(m_lCurrentX, m_lCurrentY);

			OnBnClickedProgram();
		}
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

// 橡皮框已经改变
void COrganProgramDlg::NotifyTrakerChanged(void)
{
	CRectTracker	*pRectTracker = ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;

	if( pRectTracker == NULL)
		return;

	long			lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
	long			lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;
	
	CRect CurrentZoomRect;//当前放大倍数下的RECT
	CRect originRect;	
	long lMidOffsetX=0;
	long lMidOffsetY=0;
	
	CurrentZoomRect.left = pRectTracker->m_rect.left +lSrcOriginX;
	CurrentZoomRect.right = pRectTracker->m_rect.right +lSrcOriginX;
	CurrentZoomRect.top = pRectTracker->m_rect.top +lSrcOriginY;
	CurrentZoomRect.bottom = pRectTracker->m_rect.bottom +lSrcOriginY;
		
	((CBigGraphView*)m_pParrentWnd)->GetOriginRect(CurrentZoomRect,originRect);//取原始大小的RECT

	if(m_pCurrentOrgan != NULL && m_pParrentWnd != NULL && 
		((CBigGraphView*)m_pParrentWnd)->m_CurrentImageType == FOVIMAGE)
	{
		m_pCurrentOrgan->fovId = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov->fovId;
		m_pCurrentOrgan->m_pFovData = ((CBigGraphView*)m_pParrentWnd)->m_pCurrentShowFov;
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}

	if(m_CurrentWorkStep == OWS_FOOT && pRectTracker != NULL)
	{
		//橡皮框改变，其它焊脚也跟着改变		
		CDataObj dataObj;
		dataObj.left = originRect.left;
		dataObj.right = originRect.right;
		dataObj.top = originRect.top;
		dataObj.bottom = originRect.bottom;
		
		vector<CFootData*>::iterator it =m_pCurrentPad->m_footList.begin();
		
		//取中心点的偏移（橡皮框代表第一焊脚）		
		if(it != m_pCurrentPad->m_footList.end())
		{
			CRect oldRect((*it)->left,(*it)->top,(*it)->right,(*it)->bottom);
			if( oldRect == originRect)
				return;
				
			lMidOffsetX = originRect.CenterPoint().x - (*it)->GetMidP().x;
			lMidOffsetY = originRect.CenterPoint().y - (*it)->GetMidP().y;
		}
		else
		{
			return;
		}
		
		for(;it != m_pCurrentPad->m_footList.end();it++)
		{
			//放大到与当前橡皮框一样大
			(*it)->CopyPrgData(&dataObj,(*it)->GetMidP().x,(*it)->GetMidP().y);	
			
			//平移
			(*it)->MoveX(lMidOffsetX);
			(*it)->MoveY(lMidOffsetY);			
		}
		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
	else if(m_bDoWholeOrgan == true)//元件做整体移动
	{
		CRect oldBodyRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,
			m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

		lMidOffsetX = originRect.CenterPoint().x - oldBodyRect.CenterPoint().x;
		lMidOffsetY = originRect.CenterPoint().y - oldBodyRect.CenterPoint().y;

		//搜索区域
		m_pCurrentOrgan->RangeLeft += lMidOffsetX;
		m_pCurrentOrgan->RrangeTop += lMidOffsetY;
		m_pCurrentOrgan->RangeRight += lMidOffsetX;
		m_pCurrentOrgan->RangeBottom += lMidOffsetY;

		//主体
		m_pCurrentOrgan->left = originRect.left;
		m_pCurrentOrgan->top = originRect.top;
		m_pCurrentOrgan->right = originRect.right;
		m_pCurrentOrgan->bottom = originRect.bottom;

		//焊盘
		for(int i=0;i<4;i++)
		{
			if( m_pCurrentOrgan->m_pPadData[i] != NULL)
			{
				m_pCurrentOrgan->m_pPadData[i]->MoveX(lMidOffsetX);
				m_pCurrentOrgan->m_pPadData[i]->MoveY(lMidOffsetY);

				//引脚
				/*vector<CFootData*>::iterator it =m_pCurrentOrgan->m_pPadData[i]->m_footList.begin();
				for(;it != m_pCurrentOrgan->m_pPadData[i]->m_footList.end();it++)
				{
					(*it)->MoveX(lMidOffsetX);
					(*it)->MoveY(lMidOffsetY);
				}*/
			}//end if
		}//end for

		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
}

// 取得整个元件的编程图像
void COrganProgramDlg::GetWholeOrganImage(COrganData* pOrgan)
{
	if(pOrgan == NULL)
		return;

	m_pTemplateData->m_bDataIsChanged = true;
	m_pTemplateData->m_bOrganDataIsChange = true;
	pOrgan->bDataChanged = true;
	pOrgan->bImagChanged = true;
	pOrgan->bPadChanged = true;
	pOrgan->PState = PS_COMPLETE;

	//取得元件主体的图像
	CImage* pImage=new CImage();
	((CBigGraphView*)m_pParrentWnd)->GetFactImage(pOrgan->getRect(),pImage);
	if(pImage->IsNull())
	{
		delete pImage;
		pImage = NULL;
		return;
	}

	m_pCurrentOrgan->m_MainBodyImageList.push_back(pImage);

	//取得元件焊盘的图像
	for(int i=0;i<4;i++)
	{
		if(pOrgan->m_pPadData[i] != NULL)
		{
			CImage* pImage=new CImage();
			((CBigGraphView*)m_pParrentWnd)->GetFactImage(pOrgan->m_pPadData[i]->getRect(),pImage);
			if(pImage->IsNull())
			{
				delete pImage;
				pImage = NULL;
				return;
			}
			pOrgan->m_pPadData[i]->m_ImageList.push_back(pImage);

			//取焊脚图像
			vector<CFootData*>::iterator it = pOrgan->m_pPadData[i]->m_footList.begin();
			for(; it!=pOrgan->m_pPadData[i]->m_footList.end(); it++)
			{
				CImage* pImage=new CImage();
				((CBigGraphView*)m_pParrentWnd)->GetFactImage((*it)->getRect(),pImage);
				if(pImage->IsNull())
				{
					delete pImage;
					pImage = NULL;
					return;
				}
				(*it)->m_ImageList.push_back(pImage);
			}
		}
	}	
}

void COrganProgramDlg::ModiyPrgData(void)
{
	if(m_pCurrentOrgan == NULL)
		return;

	CFovData*	pBelongFovData = m_pCurrentOrgan->m_pFovData;	//元件所属的FOV
	if( pBelongFovData == NULL)
	{
		return;
	}
		
	if(pBelongFovData->m_ImageObj.IsNull())
	{
		CString sInfo = ::g_LoadString("IDS_MUSTLOADFOVIMAGE","请先加载FOV图像");
		::AfxMessageBox(sInfo);
		return;
	}

	m_sName  = m_pCurrentOrgan->sName;
	m_sFootPrint = m_pCurrentOrgan->sFootPrint;
	m_dCADX = m_pCurrentOrgan->dMidX;
	m_dCADY = m_pCurrentOrgan->dMidY;
	
	//取得当前元件中心（相对与当前FOV的坐标系）						
	CPoint point = GetOrganInFovCenter(pBelongFovData ,m_pCurrentOrgan);	
	m_lCurrentX = point.x;
	m_lCurrentY = point.y;

	this->UpdateData(FALSE);

	//切换到FOV图片
	((CBigGraphView*)m_pParrentWnd)->SetCurrentShowFov(pBelongFovData);
	((CBigGraphView*)m_pParrentWnd)->SetCurrentImage( &(pBelongFovData->m_ImageObj),FOVIMAGE);			

	((CBigGraphView*)m_pParrentWnd)->m_pCurrentOrganData = m_pCurrentOrgan;

	int iCurrentZoom = ((CBigGraphView*)m_pParrentWnd)->m_iCurrentZoom;
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterX = m_pCurrentOrgan->GetMidP().x*(1+iCurrentZoom*ZOOMSTEP);
	((CBigGraphView*)m_pParrentWnd)->m_lCurrentCenterY = m_pCurrentOrgan->GetMidP().y*(1+iCurrentZoom*ZOOMSTEP);			
		
	
	if(m_pCurrentOrgan != NULL)
	{
		OnBnClickedProgram();
	}

	((CBigGraphView*)m_pParrentWnd)->Invalidate();
}

void COrganProgramDlg::OnBnClickedTurn()
{
	if(m_pCurrentOrgan != NULL)
	{
		m_pCurrentOrgan->Turn();
		
		//重新画橡皮框
		if(((CBigGraphView*)m_pParrentWnd)->m_pRectTracker != NULL)
		{
			delete ((CBigGraphView*)m_pParrentWnd)->m_pRectTracker;
			((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = NULL;
		}

		//取元件的主体区域
		CRect workRect(m_pCurrentOrgan->left,m_pCurrentOrgan->top,m_pCurrentOrgan->right,m_pCurrentOrgan->bottom);

		((CBigGraphView*)m_pParrentWnd)->ZoomRectOnOrigin(workRect);//放大到当前的倍数

		((CBigGraphView*)m_pParrentWnd)->ComputerCenterAndOrigin(workRect.left + workRect.Width()/2, workRect.top + workRect.Height()/2);	
		long lSrcOriginX = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginX;
		long lSrcOriginY = ((CBigGraphView*)m_pParrentWnd)->m_lSrcOriginY;

		//切换到窗口坐标系
		workRect.left -=lSrcOriginX;
		workRect.right -=lSrcOriginX;
		workRect.top -=lSrcOriginY;
		workRect.bottom -=lSrcOriginY;						

		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker = new CRectTracker(&workRect,CRectTracker::resizeOutside|CRectTracker::dottedLine);
		((CBigGraphView*)m_pParrentWnd)->m_pRectTracker->m_nHandleSize =  TRACKERSIZE;

		((CBigGraphView*)m_pParrentWnd)->Invalidate();
	}
}
