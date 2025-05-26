#pragma once
#include "DetectCmdDlg.h"
#include "DetectSmallView.h"
#include "DetectBigGraphView.h"
#include "DetectInfoDlg.h"
#include "TemplateData.h"


// CDetectMainView ������ͼ

class CDetectMainView : public CFormView
{
	DECLARE_DYNCREATE(CDetectMainView)

protected:
	CDetectMainView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDetectMainView();

protected:
	CDetectBigGraphView		*m_pDBigGraphView;		//��ʾ��ͼ��ĶԻ���
	CDetectSmallView		*m_pDSmallgView;	//��ʾ����ͼ��ĶԻ���
	CDetectCmdDlg			 m_DCommandDlg;		//�������ť��״̬�ĶԻ���
	CDetectInfoDlg			 m_DetectInfoDlg;	//��ʾ�����Ϣ�ĶԻ���	

	CTemplateData	*m_pTemplateData;	//ģ������

	CRect	m_DBigGraphRs;
	CRect	m_DSmallGraphRs;

public:
	enum { IDD = IDD_DETECTMAINVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();
	
	void SetTemplateData(CTemplateData * pTemplateData);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	 void TAuto();

	 void TManual();

	 void TStop();

	 void TAcceptpcb();

	 void TClarmp();

	 void TOriginx();

	 void TOriginy();

	 void TPcbup();

	 void TOrgan();

	 void TPcbdown();

	 void TLoosenpcb();

	 void TSendpcb();
public:
	void NotifyDetectDataChanged(void);
};


