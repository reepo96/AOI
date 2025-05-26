#pragma once
#include "TemplateData.h"
#include "OrganPrgGuid.h"
#include "afxwin.h"

enum OrganWorkStep{OWS_NONE,OWS_SERCHAREA,OWS_MAINBODY,OWS_PAD,OWS_FOOT};

// COrganProgramDlg 元件编程对话框

class COrganProgramDlg : public CDialog
{
	DECLARE_DYNAMIC(COrganProgramDlg)

protected:
	CWnd *m_pParrentWnd;
	
	OrganWorkStep	m_CurrentWorkStep;//当前工作步骤
	COrganPrgGuid	*m_pProgramGuid;//向导窗口

	bool	m_bIsSingleOrgan;//是否仅处理一个元件
	bool	m_bDoWholeOrgan;//是否处理整个元件（调整拷贝元件的中心位置时需要整体处理）

public:
	COrganProgramDlg(bool bIsSingleOrgan=false,COrganData* pOrganData=NULL,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COrganProgramDlg();

// 对话框数据
	enum { IDD = IDD_ORGANPRG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedProgram();
	afx_msg void OnBnClickedSkip();
	afx_msg void OnBnClickedInvalid();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedCancle();	
protected:
	CString m_sName;
	CString m_sFootPrint;
	double m_dCADX;
	double m_dCADY;
	CString m_sStat;

	CTemplateData * m_pTemplateData;

	map<long,CFovData*>::iterator	m_CurrentFovMapIt;//当前处理的m_FOVMap元素
	map<long,COrganData*>::iterator m_CurrentOrganMapIt;//当前处理的m_OrganDataMap元素
	COrganData						*m_pCurrentOrgan;		//当前处理的元件

	CPadData						*m_pCurrentPad;			//当前处理的焊盘
	int								m_iCurrentPadIndex;		//当前处理焊盘的索引(第几个焊盘)

protected:
	//编辑焊脚
	int		m_iFootNum;				//焊脚数
	long	m_lFootSpaceBetween;	//焊脚间距
	OrganWorkStep	m_WorkStepAfterFoot;	//编辑完焊脚之后的下步动作
	
public:
	CPadData* GetCurrentPad()
	{
		return m_pCurrentPad;
	}

	void SetWorkStepAfterFoot(OrganWorkStep nextStep)
	{
		m_WorkStepAfterFoot = nextStep;
	}
	void SetTemplateData(CTemplateData *pTemplateData)
	{
		m_pTemplateData = pTemplateData;
	}
public:
	// 开始编程
	void BeginPrg(void);
protected:
	// 检查元件是否在FOV内
	bool OrganInFov(const CFovData* pFovData, const COrganData* pOrganData);
	// 根据元件型号查找相同的已经编程好的元件
	COrganData* SearchSameOrgan(const CString sFootPrint);	
	//取得元件在FOV坐标系中的中心点
	CPoint GetOrganInFovCenter(const CFovData* pFovData,const COrganData* pOrganData);
public:
	long m_lCurrentX;
	long m_lCurrentY;
public:
	afx_msg void OnBnClickedOk();
	// 结束当前工作
	void EndCurrentWork(void);

	void CancelCurrentWork();
protected:
	// 取下一个要编程的元件
	COrganData* GetNextOrgan(void);
	// 计算元件的搜索区域
	CRect GetSerchArea(COrganData* pOrganData);	
	// 处理下一步
	void NextStep(void);
	// 计算主体区域
	CRect GetBodyArea(COrganData* pOrganData);
	// 新的工作步骤
	void NewWorkStep(CString sShowInfo, OrganWorkStep step, CRect workRect);
	// 取得焊盘Rect
	CRect GetPadRect(COrganData* pOrganData,CPadData *pPadData);
public:
	// 增加焊脚数
	void AddFootNum(int iNum);
	// 增加焊脚间距
	void AddFootSpaceBetween(long lSpace);
	//处理下一个元件
	bool NextOrganProgram(PrgState state);	
protected:
	// 从源焊盘拷贝编程数据到目的焊盘
	void CopyPad(CPadData * srcPad, CPadData * destPad, int iRelative);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void BeginSinglePrg(void);
public:
	// 橡皮框已经改变
	void NotifyTrakerChanged(void);
protected:
	// 取得整个元件的编程图像
	void GetWholeOrganImage(COrganData* pOrgan);
public:
	void ModiyPrgData(void);
protected:
	CButton m_btnTurn;
public:
	afx_msg void OnBnClickedTurn();
};
