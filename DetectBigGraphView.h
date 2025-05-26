#pragma once
#include "BaseBigGraphView.h"
#include "DetectMasterData.h"
#include "DetectDataMgr.h"
#include "DetectCmdDlg.h"
#include "DetectOrganInfo.h"
#include "DetectStatInfo.h"
#include "DectecErrDataInf.h"
#include "DetectBassInfo.h"
#include "./ImageCmp/MatchLib.h"

enum DetectType{T_AUTO,T_MANUAL};
// CDetectBigGraphView ������ͼ

class CDetectBigGraphView : public CBaseBigGraphView
{
	DECLARE_DYNCREATE(CDetectBigGraphView)

	friend class CDetectCmdDlg;
	friend class CDetectOrganInfo;
	friend class CDectecErrDataInf;
	friend class CDetectBassInfo;

public:
	bool	m_bStopWork;

protected:
	//long m_lCurrentCenterX;
	//long m_lCurrentCenterY;
	CRect m_CurrentRect;	//��ǰ���Ľ����
	CRect m_CurrentRect2;
	CRect m_CurrentTImgRect;//��ǰ���ͼ��Ŀ�

	CWnd	*m_pParrent;
	vector<CFovData*>	m_FOVOrderList;//�����FOV�б�
	bool				m_bIsUseFovOrderList;
	CImage*				m_pTemplateFovImg;//

	HANDLE				m_hManualThread;//�ֶ������߳̾��
	DetectType			m_DectType;	//��ⷽʽ
	COrganData			*m_pCurrentTOrgan;	//��ǰ����Ԫ��

	bool				m_bReady2Organ;//�Ƿ�׼���ü��Ԫ��

	CDetectDetailData	*m_pCurrentDatailData;
	CImage*		m_pCurrentTTemplateImg;	//��ǰ���ı�׼ͼ��
	CImage*		m_pCurrentTImg;			//��ǰ����ͼ��
	bool		m_bHasError;			//���������Ƿ���Ԫ��û��ͨ��

	int		m_iCurrentPCBOffsetx ;	//��ǰPCB����ģ���x����ƫ��
	int		m_iCurrentPCBOffsety ;	//��ǰPCB����ģ���y����ƫ��
	double	m_dThiDa;				//ģ��������׼���뵱ǰ��������׼��Ƕȵ�ƫ��
	double	m_dS;
	int		m_iOrganOffsetx ;	//Ԫ����FOV�е�x����ƫ��
	int		m_iOrganOffsety ;	//Ԫ����FOV�е�y����ƫ��

	CDetectMasterData				*m_pDetectMasterData;
	map<CString,CDetectStatInfo*>	m_DStatInfoMap;//���ͳ����Ϣ


	typedef struct tagDetectInfo
	{
		long	lCurrentId;
		CString	sCurrentBarCode;
		int		iDetPcbNum;
		int		iErrPcbNum;
		int		iDetOrganNum;
		int		iErrOrganNum;
		CString	sStartTime;
		long	lStartId;

		tagDetectInfo()
		{
			lCurrentId= 0;
			sCurrentBarCode = "";
			iDetPcbNum = 0;
			iErrPcbNum = 0;
			iDetOrganNum = 0;
			iErrOrganNum = 0;
			sStartTime = "";
			lStartId = 0;
		}
	}DETECTINFO;

	DETECTINFO		m_DetectBaseInfo;
	BOOL			m_bAskWhenErr;

	CDetectDataMgr	m_DetectDataMgr;

	CPoint	m_preMousePoint;	//���ǰһ��λ��

protected:
	CDetectBigGraphView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDetectBigGraphView();

public:
	enum { IDD = IDD_DETECTBIGGRAPHVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetTemplateData(CTemplateData * pTemplateData);

	afx_msg void OnZoomin();
	afx_msg void OnZoomout();	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	DetectType GetDetectType()
	{
		return m_DectType;
	}

	// ��ʼ���
	bool BeginDetect(void);
protected:
	//���Ի�׼��
	//pBaseDatum������׼��
	//pDatum��ǰҪ���Ļ�׼��
	bool DetectDatumMark(CDatumMark *pBaseDatum[2],CDatumMark *pDatum[2]);

	void Init(void);
	// ��FOV mapת��list����С��������list
	void GetFovListANdSort(void);	
	bool OrganInFov(const CFovData*  pFovData, const COrganData* pOrganData);
	// ���Ԫ��
	bool DetectOrgan(COrganData* pTemplatOrgan,CRect SerchRect,int& kindred_value);
	bool DetectPadAndFoot(COrganData* pTemplatOrgan, CPadData* pTemplatPad,int iOrgOffsetx,int iOrgOffsety,int& kindred_value);
	bool DetectFoot(COrganData* pTemplatOrgan , CFootData* pTemplateFoot,int iOrgOffsetx,int iOrgOffsety,int& kindred_value);

    //ˢ��UI�߳�
    void RefreshUITread();

public:
	 void TAuto();

	 void TManual();

	 void TStop();

	 bool TOriginx();

	 bool TOriginy();

	 void TOrgan();

	 bool TPcbdown();

	 LRESULT NotifyDetectDataChanged(WPARAM wParam, LPARAM lParam);

	 LRESULT ShowImage(WPARAM wParam, LPARAM lParam);

protected:	
	// �Ƚ�����ͼ��֮������ƶ�
	int match(CImage *pImag1, CImage *pImag2,struct SCOREDATA* pScoreData,int ar=20);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnPaint();

	// �Զ����Ԫ��
	void AutoDetectOrgan(void);
	// �ֶ����Ԫ��
	void ManulDetectOrgan(void);
protected:
	// �Զ��жϴ���
	void AutoAdjustErr(void);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	void PcbStateChange(void);
};


