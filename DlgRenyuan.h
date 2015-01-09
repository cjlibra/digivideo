#pragma once
#include "AMainDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgRenyuan 对话框

class CDlgRenyuan : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRenyuan)

public:
	CDlgRenyuan(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRenyuan();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_RENYUAN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_labelcomment;
	CString m_onlycode;
	afx_msg void OnBnClickedButton3();
	CAMainDlg *myparent;
	CListCtrl m_listlabel;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton5();
	CListCtrl m_listren;
	CString m_renname;
	CString m_rensex;
	CString m_workcode;
	CString m_imagepath;
	CString m_apartment;
	CString m_labelinfo;
	CComboBox m_labelctl;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	bool Show_picture(CString imgPath);
	CStatic m_rentoupic;
	afx_msg void OnBnClickedButton7();
	void RefreshDlg();
	afx_msg void OnBnClickedButton9();
	CString m_passstr;
	int flagtype;
};
