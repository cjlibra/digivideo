#pragma once
#include "atlcomtime.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"


// CRecordDlg 对话框

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDlg();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_nameren;
	COleDateTime m_datestring;
	CListCtrl m_listrecord;
	afx_msg void OnBnClickedButton1();
	CDateTimeCtrl m_datecrl;
//	afx_msg void OnHdnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	bool  Show_picture(CString imgPath);
	CStatic m_bmppiccontrol;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	
};
