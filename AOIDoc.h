// AOIDoc.h : CAOIDoc ��Ľӿ�
//


#pragma once


class CAOIDoc : public CDocument
{
protected: // �������л�����
	CAOIDoc();
	DECLARE_DYNCREATE(CAOIDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CAOIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLogout();
};


