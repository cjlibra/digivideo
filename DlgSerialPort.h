#if !defined(AFX_DLGSERIALPORT_H__BBE2AC35_6D69_463C_BD51_EB3C78CA4298__INCLUDED_)
#define AFX_DLGSERIALPORT_H__BBE2AC35_6D69_463C_BD51_EB3C78CA4298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSerialPort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialPort dialog
#include "howellnetsdk.h"
class CDlgSerialPort : public CDialog
{
// Construction
public:
	CDlgSerialPort(CWnd* pParent = NULL);   // standard constructor

	BOOL refresh();
	BOOL commit();
	void  close();
// Dialog Data
	//{{AFX_DATA(CDlgSerialPort)
	enum { IDD = IDD_DLG_SERIAL };
	CComboBox	m_work_mode;
	CComboBox	m_flow_control;
	CComboBox	m_parity;
	CComboBox	m_stop_bit;
	CComboBox	m_data_bit;
	CComboBox	m_bitrate;
	CComboBox	m_serial_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSerialPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	int m_serial_port_count;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSerialPort)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIALPORT_H__BBE2AC35_6D69_463C_BD51_EB3C78CA4298__INCLUDED_)
