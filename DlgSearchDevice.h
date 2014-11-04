#if !defined(AFX_DLGSEARCHDEVICE_H__96880A5A_7D97_4A9F_96B7_2B8ABE4FAFAB__INCLUDED_)
#define AFX_DLGSEARCHDEVICE_H__96880A5A_7D97_4A9F_96B7_2B8ABE4FAFAB__INCLUDED_

#include "hwsadp.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSearchDevice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchDevice dialog

class CDlgSearchDevice : public CDialog
{
// Construction
public:
	CDlgSearchDevice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSearchDevice)
	enum { IDD = IDD_DLG_SEARCH_DEVICE };
	CListBox	m_lst_device;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSearchDevice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSearchDevice)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSEARCHDEVICE_H__96880A5A_7D97_4A9F_96B7_2B8ABE4FAFAB__INCLUDED_)
