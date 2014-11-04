#if !defined(AFX_DLGANALYZE_H__18B4BF99_F647_4CE9_B102_E5CE3CB194D9__INCLUDED_)
#define AFX_DLGANALYZE_H__18B4BF99_F647_4CE9_B102_E5CE3CB194D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnalyze.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalyze dialog

class CDlgAnalyze : public CDialog
{
// Construction
public:
	CDlgAnalyze(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL refresh();
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(CDlgAnalyze)
	enum { IDD = IDD_DLG_ANALYZE };
	CComboBox	m_cbo_slot;
	BOOL	m_analyze_enable;
	int		m_analyze_focus_changed;
	int		m_analyze_scene_changed;
	int		m_analyze_scene_occluded;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnalyze)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnalyze)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCboAnalyzeSlot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALYZE_H__18B4BF99_F647_4CE9_B102_E5CE3CB194D9__INCLUDED_)
