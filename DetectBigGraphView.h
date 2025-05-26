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
// CDetectBigGraphView 窗体视图

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
	CRect m_CurrentRect;	//当前检测的焦点框
	CRect m_CurrentRect2;
	CRect m_CurrentTImgRect;//当前检测图像的框

	CWnd	*m_pParrent;
	vector<CFovData*>	m_FOVOrderList;//排序的FOV列表
	bool				m_bIsUseFovOrderList;
	CImage*				m_pTemplateFovImg;//

	HANDLE				m_hManualThread;//手动检测的线程句柄
	DetectType			m_DectType;	//检测方式
	COrganData			*m_pCurrentTOrgan;	//当前检测的元件

	bool				m_bReady2Organ;//是否准备好检测元件

	CDetectDetailData	*m_pCurrentDatailData;
	CImage*		m_pCurrentTTemplateImg;	//当前检测的标准图像
	CImage*		m_pCurrentTImg;			//当前检测的图像
	bool		m_bHasError;			//检测过程中是否有元件没有通过

	int		m_iCurrentPCBOffsetx ;	//当前PCB板与模板的x坐标偏移
	int		m_iCurrentPCBOffsety ;	//当前PCB板与模板的y坐标偏移
	double	m_dThiDa;				//模板两个基准点与当前板两个基准点角度的偏差
	double	m_dS;
	int		m_iOrganOffsetx ;	//元件在FOV中的x坐标偏移
	int		m_iOrganOffsety ;	//元件在FOV中的y坐标偏移

	CDetectMasterData				*m_pDetectMasterData;
	map<CString,CDetectStatInfo*>	m_DStatInfoMap;//检测统计信息


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

	CPoint	m_preMousePoint;	//鼠标前一个位置

protected:
	CDetectBigGraphView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DetectType GetDetectType()
	{
		return m_DectType;
	}

	// 开始检测
	bool BeginDetect(void);
protected:
	//测试基准点
	//pBaseDatum基本基准点
	//pDatum当前要检测的基准点
	bool DetectDatumMark(CDatumMark *pBaseDatum[2],CDatumMark *pDatum[2]);

	void Init(void);
	// 将FOV map转成list并从小到大排序list
	void GetFovListANdSort(void);	
	bool OrganInFov(const CFovData*  pFovData, const COrganData* pOrganData);
	// 检测元件
	bool DetectOrgan(COrganData* pTemplatOrgan,CRect SerchRect,int& kindred_value);
	bool DetectPadAndFoot(COrganData* pTemplatOrgan, CPadData* pTemplatPad,int iOrgOffsetx,int iOrgOffsety,int& kindred_value);
	bool DetectFoot(COrganData* pTemplatOrgan , CFootData* pTemplateFoot,int iOrgOffsetx,int iOrgOffsety,int& kindred_value);

    //刷新UI线程
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
	// 比较两个图像之间的相似度
	int match(CImage *pImag1, CImage *pImag2,struct SCOREDATA* pScoreData,int ar=20);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnPaint();

	// 自动检测元件
	void AutoDetectOrgan(void);
	// 手动检测元件
	void ManulDetectOrgan(void);
protected:
	// 自动判断错误
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


