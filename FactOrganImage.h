#pragma once


// CFactOrganImage ��ʾԪ��ͼ��ĶԻ���

class CFactOrganImage : public CDialog
{
	DECLARE_DYNAMIC(CFactOrganImage)

public:
	CFactOrganImage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFactOrganImage();

// �Ի�������
	enum { IDD = IDD_FACTIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowImage(const CImage& image);
public:
    virtual BOOL OnInitDialog();
};
