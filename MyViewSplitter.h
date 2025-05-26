#pragma once

#include <map>

using namespace std;

// MyViewSplitter

class MyViewSplitter : public CSplitterWnd
{
	DECLARE_DYNAMIC(MyViewSplitter)

protected:
	map<int, long> m_mapViewPane;						// the container to record the viewID and its position relationship
	map<long, int> m_mapCurrentViews;					// record the current view of the specified postion(panel)
	map<int, CWnd*> m_mapIDViews;						// record the relationship between viewID and the window

public:
	MyViewSplitter();
	virtual ~MyViewSplitter();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int addView(int iViewID, int iRow, int iCol, CRuntimeClass * pViewClass, CCreateContext* pContext);
public:
	void showView(int iViewID);
	CWnd* getView(int iViewID);
	CWnd* getCurrentView(int iRow, int iCol, int& iCurID);
public:
	void setCurrentView(int iRow, int iCol, int iViewID);
public:
	int hideCurrentView(int iRow, int iCol);
public:
	void getPaneFromViewID(int iViewID, CPoint& pane);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


