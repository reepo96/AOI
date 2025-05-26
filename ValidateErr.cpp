// ValidateErr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AOI.h"
#include "ValidateErr.h"

CString	g_sInvalidErr = "�ٴ�";
CString g_sLearImag = "ѧϰͼ��";

// CValidateErr �Ի���


IMPLEMENT_DYNAMIC(CValidateErr, CDialog)

CValidateErr::CValidateErr(CDetectMasterData *pDetectMasterData,map<CString,CDetectStatInfo*>* pDStatInfoMap,CWnd* pParent /*=NULL*/)
	: CDialog(CValidateErr::IDD, pParent)
	, m_sOrganName(_T(""))
	, m_iKindredValue(0)
	, m_iLimitValue(0)
	, m_lCenterX(0)
	, m_lCenterY(0)
	, m_sErrorType(_T(""))
	, m_sDoWay(_T(""))
	, m_iSpellNo(0)
{
	m_pDetectMasterData = pDetectMasterData;
	m_pDStatInfoMap = pDStatInfoMap;

	if(g_language == CHINESES)
	{
		g_sInvalidErr = "�ٴ�";
		g_sLearImag = "ѧϰͼ��";
	}
	else
	{
		g_sInvalidErr = "No Fault";
		g_sLearImag = "Learn Image";
	}
	m_pLearnImag = NULL;
}

CValidateErr::~CValidateErr()
{
}

void CValidateErr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ORGANNAME, m_sOrganName);
	DDX_Text(pDX, IDC_KINDRED_VALUE, m_iKindredValue);
	DDX_Text(pDX, IDC_LIMIT_VALUE, m_iLimitValue);
	DDX_Text(pDX, IDC_CENTERX, m_lCenterX);
	DDX_Text(pDX, IDC_CENTERY, m_lCenterY);
	DDX_CBString(pDX, IDC_ERRORTYPE, m_sErrorType);
	DDX_CBString(pDX, IDC_DO_WAY, m_sDoWay);
	DDX_Control(pDX, IDC_DO_WAY, m_DoWayCtrl);
	DDX_Control(pDX, IDC_ERRORTYPE, m_ErrorTypeCtrl);
	DDX_Text(pDX, IDC_SPELL_NO, m_iSpellNo);
}


BEGIN_MESSAGE_MAP(CValidateErr, CDialog)
	ON_CBN_SELCHANGE(IDC_ERRORTYPE, &CValidateErr::OnCbnSelchangeErrortype)
	ON_BN_CLICKED(IDC_OK, &CValidateErr::OnBnClickedOk)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_DO_WAY, &CValidateErr::OnCbnSelchangeDoWay)
END_MESSAGE_MAP()


// CValidateErr ��Ϣ�������

void CValidateErr::OnCbnSelchangeErrortype()
{
	this->UpdateData(TRUE);
	if(m_sErrorType == g_sInvalidErr)
	{
		m_DoWayCtrl.EnableWindow(TRUE);
	}
	else
	{
		m_DoWayCtrl.EnableWindow(FALSE);
	}
}

void CValidateErr::OnBnClickedOk()
{
	this->UpdateData(TRUE);
	CDetectDetailData* pDetailData = (*m_DetailIt);
	
	if(m_sErrorType == g_sInvalidErr)
	{
		CString sErrorInfo;
		if(m_sDoWay==g_sLearImag && m_pLearnImag != NULL && !m_pLearnImag->IsNull())
		{
			if(pDetailData->m_pErrorOrgan != NULL)
			{
				pDetailData->m_pErrorOrgan->m_MainBodyImageList.push_back(m_pLearnImag);
				if(false==m_templateDataMgr.ModifyOrgan(pDetailData->m_pErrorOrgan,sErrorInfo))
				{
					sErrorInfo="����ͼ��ʧ��\n"+sErrorInfo;
					::AfxMessageBox(sErrorInfo);
				}
			}
			else if(pDetailData->m_pErrorPad != NULL)
			{
				pDetailData->m_pErrorPad->m_ImageList.push_back(m_pLearnImag);
				if(false==m_templateDataMgr.ModifyPad(pDetailData->m_pErrorPad,sErrorInfo))
				{
					sErrorInfo="����ͼ��ʧ��\n"+sErrorInfo;
					::AfxMessageBox(sErrorInfo);
				}
			}
			else if(pDetailData->m_pErrorFoot != NULL)
			{
				pDetailData->m_pErrorFoot->m_ImageList.push_back(m_pLearnImag);
				if(false==m_templateDataMgr.ModifyFoot(pDetailData->m_pErrorFoot,sErrorInfo))
				{
					sErrorInfo="����ͼ��ʧ��\n"+sErrorInfo;
					::AfxMessageBox(sErrorInfo);
				}
			}
		}
	}
	else
	{
		if(m_pLearnImag != NULL && !m_pLearnImag->IsNull())
		{
			m_pLearnImag->Destroy();
			delete m_pLearnImag;
			m_pLearnImag = NULL;
		}

		pDetailData->errType = m_sErrorType;
		m_pDetectMasterData->ErrorCount++;

		//ͳ����Ϣ
		map<CString,CDetectStatInfo*>::iterator it = m_pDStatInfoMap->find(pDetailData->organName);
		if(it != m_pDStatInfoMap->end())
		{
			(*it).second->lErrorCount +=1;
		}
		else
		{
			CDetectStatInfo* pStatInfo = new CDetectStatInfo();
			pStatInfo->sOrganName = pDetailData->organName;
			pStatInfo->lErrorCount = 1;
			m_pDStatInfoMap->insert(::make_pair(pStatInfo->sOrganName,pStatInfo));
		}		
	}

	CRect wndRect;
	this->GetWindowRect(&wndRect);

	m_DetailIt++;
	for(;m_DetailIt != m_pDetectMasterData->m_Details.end();m_DetailIt++)
	{
		CDetectDetailData* pDetailData = (*m_DetailIt);
		if(pDetailData->isPass == 0)
		{
			m_sOrganName = pDetailData->organName;
			m_iSpellNo = pDetailData->spellNo;
			m_iKindredValue = pDetailData->kindred_value;
			m_iLimitValue = pDetailData->limit_value;
			m_lCenterX = pDetailData->centerX;
			m_lCenterY = pDetailData->centerY;

			//��ʾͼ��
			if( m_pTemplateImgDlg != NULL && m_pTemplateImgDlg->m_hWnd != NULL && IsWindow(m_pTemplateImgDlg->m_hWnd))
			{
				m_pTemplateImgDlg->DestroyWindow();
				m_pTemplateImgDlg = NULL;
			}
			if( m_pCurrentImgDlg != NULL && m_pCurrentImgDlg->m_hWnd != NULL && IsWindow(m_pCurrentImgDlg->m_hWnd))
			{
				m_pCurrentImgDlg->DestroyWindow();
				m_pCurrentImgDlg = NULL;
			}
			m_pTemplateImgDlg = new CObjectImageDlg(pDetailData->m_pTemplateImg,"ģ��ͼ��",this);
			m_pCurrentImgDlg = new CObjectImageDlg(pDetailData->m_pCurrentImg,
				"��ǰͼ��",this,&pDetailData->m_CurrentTImgRect);

			//���ԭ����ͼ��
			if(m_pTempImg != NULL && !m_pTempImg->IsNull())
			{
				m_pTempImg->Destroy();
				m_pTempImg = NULL;
			}
			m_pTempImg = pDetailData->m_pCurrentImg;

			m_pTemplateImgDlg->Create(IDD_OBJIMAGE,this);
			CRect wndRect1;
			m_pTemplateImgDlg->GetWindowRect(wndRect1);
			m_pTemplateImgDlg->MoveWindow(wndRect.left-20,wndRect.bottom+5,wndRect1.Width(),wndRect1.Height());
			m_pTemplateImgDlg->ShowWindow(SW_SHOW);

			m_pCurrentImgDlg->Create(IDD_OBJIMAGE,this);
			CRect wndRect2;
			m_pCurrentImgDlg->GetWindowRect(wndRect2);
			m_pCurrentImgDlg->MoveWindow(wndRect.left-20+wndRect1.Width(),wndRect.bottom+5,wndRect2.Width(),wndRect2.Height());
			m_pCurrentImgDlg->ShowWindow(SW_SHOW);

			this->UpdateData(FALSE);
			break;
		}
	}

	m_DoWayCtrl.SetCurSel(0);

	//����
	if(m_DetailIt == m_pDetectMasterData->m_Details.end())
	{
		//���ԭ����ͼ��
		if(m_pTempImg != NULL && !m_pTempImg->IsNull())
		{
			m_pTempImg->Destroy();
			m_pTempImg = NULL;
		}

		CDialog::OnOK();
	}
}

BOOL CValidateErr::OnInitDialog()
{
	CDialog::OnInitDialog();

	::g_SetDialogStrings(this);
	this->SetTimer(1,20,NULL);

	m_ErrorTypeCtrl.InsertString(0,g_sInvalidErr);
	m_DoWayCtrl.InsertString(0,g_sLearImag);
	if(g_language == CHINESES)
	{
		m_ErrorTypeCtrl.InsertString(1,"��λ");
		m_ErrorTypeCtrl.InsertString(2,"�Ž�");
		m_ErrorTypeCtrl.InsertString(3,"����ȱ©");
		m_ErrorTypeCtrl.InsertString(4,"��������");
		m_ErrorTypeCtrl.InsertString(5,"������");
		m_ErrorTypeCtrl.InsertString(6,"ȱ��");
		m_ErrorTypeCtrl.InsertString(7,"��������");

		m_DoWayCtrl.InsertString(0,"�ݲ�����");
	}
	else
	{
		m_ErrorTypeCtrl.InsertString(1,"Error Location");
		m_ErrorTypeCtrl.InsertString(2,"Bridge Meet");
		m_ErrorTypeCtrl.InsertString(3,"Missing Organ");
		m_ErrorTypeCtrl.InsertString(4,"Over Tin");
		m_ErrorTypeCtrl.InsertString(5,"Error Polarity");
		m_ErrorTypeCtrl.InsertString(6,"Missing Tin");
		m_ErrorTypeCtrl.InsertString(7,"Error Organ");

		m_DoWayCtrl.InsertString(0,"No Deal With");
	}

	m_ErrorTypeCtrl.SetCurSel(0);
	m_DoWayCtrl.SetCurSel(0);
		
	CRect wndRect;
	this->GetWindowRect(&wndRect);

	m_DetailIt = m_pDetectMasterData->m_Details.begin();
	for(;m_DetailIt != m_pDetectMasterData->m_Details.end();m_DetailIt++)
	{
		CDetectDetailData* pDetailData = (*m_DetailIt);
		if(pDetailData->isPass == 0)
		{
			m_sOrganName = pDetailData->organName;
			m_iSpellNo = pDetailData->spellNo;
			m_iKindredValue = pDetailData->kindred_value;
			m_iLimitValue = pDetailData->limit_value;
			m_lCenterX = pDetailData->centerX;
			m_lCenterY = pDetailData->centerY;

			//��ʾͼ��
			m_pTemplateImgDlg = new CObjectImageDlg(pDetailData->m_pTemplateImg,"ģ��ͼ��",this);
			m_pCurrentImgDlg = new CObjectImageDlg(pDetailData->m_pCurrentImg,"��ǰͼ��",this,&pDetailData->m_CurrentTImgRect);

			m_pTempImg = pDetailData->m_pCurrentImg;

			m_pTemplateImgDlg->Create(IDD_OBJIMAGE,this);
			CRect wndRect1;
			m_pTemplateImgDlg->GetWindowRect(wndRect1);
			m_pTemplateImgDlg->MoveWindow(wndRect.left-20,wndRect.bottom+5,wndRect1.Width(),wndRect1.Height());
			m_pTemplateImgDlg->ShowWindow(SW_SHOW);
			

			m_pCurrentImgDlg->Create(IDD_OBJIMAGE,this);
			CRect wndRect2;
			m_pCurrentImgDlg->GetWindowRect(wndRect2);
			m_pCurrentImgDlg->MoveWindow(wndRect.left-20+wndRect1.Width(),wndRect.bottom+5,wndRect2.Width(),wndRect2.Height());
			
			m_pCurrentImgDlg->ShowWindow(SW_SHOW);

			this->UpdateData(FALSE);
			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CValidateErr::OnTimer(UINT_PTR nIDEvent)
{
#ifndef _MYUTEST
	GT_ExInpt(&g_uDevInEvent);
	if((~g_uDevInEvent)&IN_EVT_GREEN_BUTTON)
	{
		m_ErrorTypeCtrl.SetCurSel(0);
		OnBnClickedOk();
	}
#endif

	CDialog::OnTimer(nIDEvent);
}

void CValidateErr::OnCbnSelchangeDoWay()
{
	this->UpdateData(TRUE);
	if(m_sDoWay==g_sLearImag)
	{
		if(m_pCurrentImgDlg != NULL && m_pCurrentImgDlg->m_hWnd >0 &&
			::IsWindow(m_pCurrentImgDlg->m_hWnd))
		{
			CString sMsgInfo = g_LoadString("IDS_LEARNIMAGE","�밴��CTRL�����������\"��ǰͼ��\"�����Ͽ�ѡ��Ҫѧϰ��ͼ��Ȼ�󰴡�ѧϰ����ť");
			::AfxMessageBox(sMsgInfo);
			m_pCurrentImgDlg->Learn();
		}
	}
	else
	{
		if(m_pCurrentImgDlg != NULL && m_pCurrentImgDlg->m_hWnd >0 &&
			::IsWindow(m_pCurrentImgDlg->m_hWnd))
		{
			m_pCurrentImgDlg->UnLearn();
		}
	}
}

void CValidateErr::SetLearnImage(CImage* pImage)
{
	m_pLearnImag = pImage;
}
