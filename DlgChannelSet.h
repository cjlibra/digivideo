#if !defined(AFX_DLGCHANNELSET_H__A48DA4A4_5781_4FED_BE5E_2D1204800C7D__INCLUDED_)
#define AFX_DLGCHANNELSET_H__A48DA4A4_5781_4FED_BE5E_2D1204800C7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChannelSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelSet dialog

class CDlgChannelSet : public CDialog
{
// Construction
public:
	CDlgChannelSet(CWnd* pParent = NULL);   // standard constructor
	
	BOOL refresh();
	void close();
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(CDlgChannelSet)
	enum { IDD = IDD_DLG_CHANNEL_SET };
	CComboBox	m_remote_type;
	CComboBox	m_slot;
	CString	m_user_name;
	CString	m_user_password;
	CString	m_ip;
	int		m_port;
	int		m_remote_slot;
	CString	m_uri;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChannelSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChannelSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANNELSET_H__A48DA4A4_5781_4FED_BE5E_2D1204800C7D__INCLUDED_)
