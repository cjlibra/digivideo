#if !defined(AFX_DLGREMOTEDO_H__0D33A1F9_60EE_454A_9CEB_2B6A3FB004AE__INCLUDED_)
#define AFX_DLGREMOTEDO_H__0D33A1F9_60EE_454A_9CEB_2B6A3FB004AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRemoteDo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRemoteDo dialog
#include "howellnetsdk.h"
class CDlgRemoteDo : public CDialog
{
// Construction
public:
	CDlgRemoteDo(CWnd* pParent = NULL);   // standard constructor
	
	void close();
	BOOL refresh();
	BOOL commit();
// Dialog Data
	//{{AFX_DATA(CDlgRemoteDo)
	enum { IDD = IDD_DLG_REMOTE_DO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRemoteDo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgRemoteDo)
	afx_msg void OnButton3();
	afx_msg void OnButton2();
	afx_msg void OnButton4();
	afx_msg void OnButton8();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREMOTEDO_H__0D33A1F9_60EE_454A_9CEB_2B6A3FB004AE__INCLUDED_)
