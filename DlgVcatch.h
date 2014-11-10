#pragma once
#include "afxcmn.h"
#include "atlcomtime.h"
#include "afxdtctl.h"


// CDlgVcatch 对话框

class CDlgVcatch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVcatch)

public:
	CDlgVcatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVcatch();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_VCATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_listvideo;
	CString m_name;
	COleDateTime m_dateole;
	CDateTimeCtrl m_datectrl;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
