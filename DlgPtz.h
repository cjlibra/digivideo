#if !defined(AFX_DLGPTZ_H__F0BBFD88_EA2F_4C48_9530_6AC174F5A423__INCLUDED_)
#define AFX_DLGPTZ_H__F0BBFD88_EA2F_4C48_9530_6AC174F5A423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPtz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPtz dialog
#include "howellnetsdk.h"
class CDlgPtz : public CDialog
{
// Construction
public:
	CDlgPtz(CWnd* pParent = NULL);   // standard constructor

	BOOL refresh();
	BOOL commit();
	void close();

// Dialog Data
	//{{AFX_DATA(CDlgPtz)
	enum { IDD = IDD_DLG_PTZ };
	CComboBox	m_cbo_address;
	CComboBox	m_cbo_protocol;
	CComboBox	m_cbo_flow_control;
	CComboBox	m_cbo_parity;
	CComboBox	m_cbo_stop_bit;
	CComboBox	m_cbo_data_bit;
	CComboBox	m_cbo_bitrate;
	CComboBox	m_cbo_serial_port;
	CComboBox	m_cbo_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPtz)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPtz)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo11();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPTZ_H__F0BBFD88_EA2F_4C48_9530_6AC174F5A423__INCLUDED_)
