// AOIDoc.cpp : CAOIDoc 类的实现
//

#include "stdafx.h"
#include "AOI.h"

#include "AOIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAOIDoc

IMPLEMENT_DYNCREATE(CAOIDoc, CDocument)

BEGIN_MESSAGE_MAP(CAOIDoc, CDocument)
	ON_COMMAND(ID_LOGOUT, &CAOIDoc::OnLogout)	
END_MESSAGE_MAP()


// CAOIDoc 构造/析构

CAOIDoc::CAOIDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAOIDoc::~CAOIDoc()
{
}

BOOL CAOIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CAOIDoc 序列化

void CAOIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CAOIDoc 诊断

#ifdef _DEBUG
void CAOIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAOIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAOIDoc 命令

void CAOIDoc::OnLogout()
{
	this->OnFileClose();
	g_UserInf.enableDetect = 0;
	g_UserInf.enableProgram = 0;
	g_UserInf.enableReport = 0;
	g_UserInf.enableSystem = 0;
	g_UserInf.password = "";
	g_UserInf.userName = "";
}

