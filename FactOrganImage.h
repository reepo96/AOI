#pragma once


// CFactOrganImage 显示元件图像的对话框

class CFactOrganImage : public CDialog
{
	DECLARE_DYNAMIC(CFactOrganImage)

public:
	CFactOrganImage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFactOrganImage();

// 对话框数据
	enum { IDD = IDD_FACTIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowImage(const CImage& image);
public:
    virtual BOOL OnInitDialog();
};
