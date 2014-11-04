#if !defined(AFX_DLGMASKALARM_H__D647B67A_371A_48E2_B65E_72B91C27D660__INCLUDED_)
#define AFX_DLGMASKALARM_H__D647B67A_371A_48E2_B65E_72B91C27D660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMaskAlarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskAlarm dialog
#include "howellnetsdk.h"
class CDlgMaskAlarm : public CDialog
{
// Construction
public:
	CDlgMaskAlarm(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL refresh();
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(CDlgMaskAlarm)
	enum { IDD = IDD_DLG_MASK_ALARM };
	CButton	m_chk_enable;
	CComboBox	m_cbo_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMaskAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMaskAlarm)
	afx_msg void OnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMASKALARM_H__D647B67A_371A_48E2_B65E_72B91C27D660__INCLUDED_)
