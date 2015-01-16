#pragma once
#include "atlcomtime.h"


// CExcelOutDlg 对话框

class CExcelOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExcelOutDlg)

public:
	CExcelOutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExcelOutDlg();

// 对话框数据
	enum { IDD = IDD_A_EXCELOUT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	COleDateTime m_datetime;
};
