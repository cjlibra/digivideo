#if !defined(AFX_DLGSUBQUALITY_H__8FC639FF_F94F_4DC9_9FCD_99158D5FF5A2__INCLUDED_)
#define AFX_DLGSUBQUALITY_H__8FC639FF_F94F_4DC9_9FCD_99158D5FF5A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubQuality.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSubQuality dialog

class CDlgSubQuality : public CDialog
{
// Construction
public:
	CDlgSubQuality(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSubQuality)
	enum { IDD = IDD_DLG_SUB_QUALITY };
	CComboBox m_cbo_slot;
	CComboBox m_cbo_quality;
	CComboBox m_cbo_max_bps;
	CComboBox m_cbo_encode_type;
	CComboBox m_cbo_frame;
	CComboBox m_cbo_bps_type;
	int		m_max_bps;
	BOOL	m_sub_enabled;
	//}}AFX_DATA

	BOOL refresh();
	void close();
	BOOL commit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubQuality)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSubQuality)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnSelchangeCombo4();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBQUALITY_H__8FC639FF_F94F_4DC9_9FCD_99158D5FF5A2__INCLUDED_)
