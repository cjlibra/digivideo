#pragma once
#include "explorer1.h"


// CWebWeb �Ի���

class CWebWeb : public CDialogEx
{
	DECLARE_DYNAMIC(CWebWeb)

public:
	CWebWeb(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWebWeb();

// �Ի�������
	enum { IDD = IDD_A_WEBWEB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_webpage;
	CString url;
	virtual BOOL OnInitDialog();
};
