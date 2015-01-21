#pragma once
#include "explorer1.h"


// CWebWeb 对话框

class CWebWeb : public CDialogEx
{
	DECLARE_DYNAMIC(CWebWeb)

public:
	CWebWeb(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebWeb();

// 对话框数据
	enum { IDD = IDD_A_WEBWEB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_webpage;
	CString url;
	virtual BOOL OnInitDialog();
};
