#pragma once


// CMyRectTracker

class CMyRectTracker : public CRectTracker
{	
public:
	CMyRectTracker();
	CMyRectTracker(LPCRECT lpSrcRect,UINT nStyle );
	virtual ~CMyRectTracker();

	void OnChangedRect(const CRect& rectOld );

protected:	
};


