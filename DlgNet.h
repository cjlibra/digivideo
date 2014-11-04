#if !defined(AFX_DLGNET_H__E6E35134_FE44_4389_B692_2F80C548601D__INCLUDED_)
#define AFX_DLGNET_H__E6E35134_FE44_4389_B692_2F80C548601D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNet dialog
#include "howellnetsdk.h"
class CDlgNet : public CDialog
{
// Construction
public:
	CDlgNet(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL refresh();
	BOOL commit();
// Dialog Data
	//{{AFX_DATA(CDlgNet)
	enum { IDD = IDD_DLG_NET };
	CIPAddressCtrl	m_snmp_ip;
	CIPAddressCtrl	m_centerip;
	CIPAddressCtrl	m_mulip;
	CIPAddressCtrl	m_dns;
	CIPAddressCtrl	m_gateway;
	CIPAddressCtrl	m_mask;
	CIPAddressCtrl	m_ip;
	int		m_port;
	CString	m_phy;
	int		m_center_port;
	int		m_snmp_interval;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNet)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNET_H__E6E35134_FE44_4389_B692_2F80C548601D__INCLUDED_)
