// MyViewSplitter.cpp : 实现文件
//

#include "stdafx.h"
#include "AOI.h"
#include "MyViewSplitter.h"


// MyViewSplitter

IMPLEMENT_DYNAMIC(MyViewSplitter, CSplitterWnd)

MyViewSplitter::MyViewSplitter()
{

}

MyViewSplitter::~MyViewSplitter()
{
}


BEGIN_MESSAGE_MAP(MyViewSplitter, CSplitterWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// MyViewSplitter 消息处理程序



int MyViewSplitter::addView(int iViewID, int iRow, int iCol, CRuntimeClass * pViewClass, CCreateContext* pContext)
{
	int iPreviousID = hideCurrentView(iRow, iCol);

	if (CreateView(iRow, iCol, pViewClass, CSize(0,0), pContext) == 0)
	{
		if (iPreviousID != -1)
			setCurrentView(iRow, iCol, iPreviousID);
		return -1;
	}

	CWnd* pNewWnd = GetPane(iRow, iCol);
	CPoint pane(iRow, iCol);
	long paneID = MAKELONG(pane.x,pane.y);
	
	m_mapViewPane.insert(map<int, long>::value_type(iViewID,paneID));

	m_mapIDViews.insert(map<int, CWnd*>::value_type(iViewID, pNewWnd));

	// set the new view current
	setCurrentView(iRow, iCol, iViewID);

	RedrawWindow();
	return iViewID;
}

void MyViewSplitter::showView(int iViewID)
{
	if (getView(iViewID) == NULL)
		return;

	// find the pane containing the view 
	CPoint pane;
	getPaneFromViewID(iViewID, pane);

	// switch views
	hideCurrentView(pane.x, pane.y);
	setCurrentView(pane.x, pane.y, iViewID);

	RecalcLayout();	
}

CWnd* MyViewSplitter::getView(int iViewID)
{
	map<int, CWnd*>::iterator itView;

	itView = m_mapIDViews.find(iViewID);
	if(itView==m_mapIDViews.end())
		return NULL;
	else
		return (*itView).second;
}

CWnd* MyViewSplitter::getCurrentView(int iRow, int iCol, int& iCurID)
{
	long paneID = MAKELONG(iRow,iCol);
	
	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur == m_mapCurrentViews.end())
		return NULL;
	else
	{
		int iPreviousID = (*itCur).second;
		iCurID = iPreviousID;
		return getView(iPreviousID);
	}
}

void MyViewSplitter::setCurrentView(int iRow, int iCol, int iViewID)
{
	long paneID = MAKELONG(iRow,iCol);

	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur != m_mapCurrentViews.end())
		(*itCur).second = iViewID;
	else
		m_mapCurrentViews.insert(map<long,int>::value_type(paneID,iViewID));

	CWnd * pView = getView(iViewID);
	pView->SetDlgCtrlID(IdFromRowCol(iRow, iCol));
	pView->ShowWindow(SW_SHOW);
}

int MyViewSplitter::hideCurrentView(int iRow, int iCol)
{
	int iPrevID;
	CWnd * pCurView = getCurrentView(iRow, iCol, iPrevID);
	if (pCurView == NULL)
		return -1;
	else
	{
		pCurView->SetDlgCtrlID(0);
		pCurView->ShowWindow(SW_HIDE);	
		return iPrevID;
	}
}

void MyViewSplitter::getPaneFromViewID(int iViewID, CPoint& pane)
{
	map<int, long>::iterator itPane;
	itPane = m_mapViewPane.find(iViewID);
	if (itPane==m_mapViewPane.end())
	{
		pane = NULL;
		return;
	}
	long paneID = (*itPane).second;
	CPoint p(paneID);
	pane.x = p.x;
	pane.y = p.y;
}

void MyViewSplitter::OnLButtonDown(UINT nFlags, CPoint point)
{
	return;	//禁止拖动

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}

void MyViewSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
	return;	//禁止改变鼠标光标

	//CSplitterWnd::OnMouseMove(nFlags, point);
}
