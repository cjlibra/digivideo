#if !defined(AFX_DLGCUSTOMOSD_H__F3E22165_6466_4998_8E6E_B0AF01ED475C__INCLUDED_)
#define AFX_DLGCUSTOMOSD_H__F3E22165_6466_4998_8E6E_B0AF01ED475C__INCLUDED_

#include "net_video_test.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCustomOsd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCustomOsd dialog

class CDlgCustomOsd : public CDialog
{
// Construction
public:
	CDlgCustomOsd(CWnd* pParent = NULL);   // standard constructor

	BOOL refresh();
	void close();
	BOOL commit();
	void close_video();
	BOOL connect_video();

// Dialog Data
	//{{AFX_DATA(CDlgCustomOsd)
	enum { IDD = IDD_DLG_CUSTOM_OSD };
	CComboBox	m_cbo_custom_osd_name2_color;
	CComboBox	m_cbo_custom_osd_name2_alpha;
	CComboBox	m_cbo_custom_osd_stream;
	CComboBox	m_cbo_custom_osd_slot;
	CComboBox	m_cbo_custom_osd_name_color;
	CComboBox	m_cbo_custom_osd_name_alpha;
	CComboBox	m_cbo_custom_osd_date_mode;
	CComboBox	m_cbo_custom_osd_date_color;
	CComboBox	m_cbo_custom_osd_date_alpha;
	BOOL	m_custom_osd_date_show_week;
	BOOL	m_custom_osd_date_enable;
	BOOL	m_custom_osd_date_outline;
	BOOL	m_custom_osd_name_enable;
	BOOL	m_custom_osd_name_outline;
	int		m_custom_osd_date_font;
	int		m_custom_osd_date_left;
	int		m_custom_osd_date_top;
	int		m_custom_osd_name_font;
	int		m_custom_osd_name_left;
	CString	m_custom_osd_name_row;
	int		m_custom_osd_name_top;
	BOOL	m_custom_osd_name2_enable;
	BOOL	m_custom_osd_name2_outline;
	int		m_custom_osd_name2_font;
	int		m_custom_osd_name2_left;
	int		m_custom_osd_name2_top;
	CString	m_custom_osd_name2_row;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCustomOsd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCustomOsd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCboCustomOsdSlot();
	afx_msg void OnSelchangeCboCustomOsdStream();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	net_video_test* m_video;	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCUSTOMOSD_H__F3E22165_6466_4998_8E6E_B0AF01ED475C__INCLUDED_)
