#if !defined(AFX_DLGLOG_H__9E3036C4_46FC_4148_BE9E_5EDCFB86CA9D__INCLUDED_)
#define AFX_DLGLOG_H__9E3036C4_46FC_4148_BE9E_5EDCFB86CA9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLog dialog
#include "howellnetsdk.h"
class CDlgLog : public CDialog
{
// Construction
public:
	CDlgLog(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL commit();
	BOOL refresh();

// Dialog Data
	//{{AFX_DATA(CDlgLog)
	enum { IDD = IDD_DLG_LOG };
	CComboBox	m_cbo_type;
	CDateTimeCtrl	m_date;
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOG_H__9E3036C4_46FC_4148_BE9E_5EDCFB86CA9D__INCLUDED_)
