#if !defined(AFX_DLGWORKSHEET_H__4F46AF9B_6FF9_4480_8357_85498FFCC61C__INCLUDED_)
#define AFX_DLGWORKSHEET_H__4F46AF9B_6FF9_4480_8357_85498FFCC61C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWorkSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkSheet dialog
#include "howellnetsdk.h"
class CDlgWorkSheet : public CDialog
{
// Construction
public:
	CDlgWorkSheet(CWnd* pParent = NULL);   // standard constructor
	
	void close();
	BOOL refresh_date(int day);
	BOOL refresh();
	BOOL commit();
// Dialog Data
	//{{AFX_DATA(CDlgWorkSheet)
	enum { IDD = IDD_DLG_WORKSHEET };
	CComboBox	m_cbo_copy_date;
	CComboBox	m_cbo_date;
	CComboBox	m_cbo_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWorkSheet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWorkSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButton2();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSpinButtonCtrl* m_spin_beg_h[3];//开始时间
	CSpinButtonCtrl* m_spin_beg_m[3];//开始时间
	CSpinButtonCtrl* m_spin_end_h[3];//结束时间
	CSpinButtonCtrl* m_spin_end_m[3];//结束时间
	
	CComboBox* m_cbo_rec_type[3];

	struct tWorkSheet m_work_sheet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWORKSHEET_H__4F46AF9B_6FF9_4480_8357_85498FFCC61C__INCLUDED_)
