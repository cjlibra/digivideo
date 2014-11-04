#if !defined(AFX_DLGDEVICEINFO_H__0E9051DA_AEAC_4CAE_92DA_B3F974583C0E__INCLUDED_)
#define AFX_DLGDEVICEINFO_H__0E9051DA_AEAC_4CAE_92DA_B3F974583C0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDeviceInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceInfo dialog

class CDlgDeviceInfo : public CDialog
{
// Construction
public:
	void close();
	BOOL refresh();	
	BOOL commit();

	CDlgDeviceInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDeviceInfo)
	enum { IDD = IDD_DLG_DEVICE_INFO };
	CString	m_serial_no;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDeviceInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDeviceInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEVICEINFO_H__0E9051DA_AEAC_4CAE_92DA_B3F974583C0E__INCLUDED_)
