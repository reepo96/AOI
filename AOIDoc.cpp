// AOIDoc.cpp : CAOIDoc ���ʵ��
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


// CAOIDoc ����/����

CAOIDoc::CAOIDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CAOIDoc::~CAOIDoc()
{
}

BOOL CAOIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CAOIDoc ���л�

void CAOIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CAOIDoc ���

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


// CAOIDoc ����

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

