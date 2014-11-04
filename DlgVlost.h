#if !defined(AFX_DLGVLOST_H__2D7B94BA_73F6_43AA_9453_A127828957EE__INCLUDED_)
#define AFX_DLGVLOST_H__2D7B94BA_73F6_43AA_9453_A127828957EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVlost.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVlost dialog
#include "howellnetsdk.h"
class CDlgVlost : public CDialog
{
// Construction
public:
	CDlgVlost(CWnd* pParent = NULL);   // standard constructor
	void close();
	BOOL refresh();	
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(CDlgVlost)
	enum { IDD = IDD_DLG_VLOST };
	CComboBox	m_cbo_channel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVlost)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int m_alarm_out_count;
	CButton* m_chk_out[16];//Áª¶¯Êä³ö

	// Generated message map functions
	//{{AFX_MSG(CDlgVlost)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVLOST_H__2D7B94BA_73F6_43AA_9453_A127828957EE__INCLUDED_)
