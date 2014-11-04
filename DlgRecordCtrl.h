#if !defined(AFX_DLGRECORDCTRL_H__48CC14DC_B954_4C69_9951_DAD8C63700B6__INCLUDED_)
#define AFX_DLGRECORDCTRL_H__48CC14DC_B954_4C69_9951_DAD8C63700B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRecordCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordCtrl dialog

class CDlgRecordCtrl : public CDialog
{
// Construction
public:
	CDlgRecordCtrl(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL refresh();
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(CDlgRecordCtrl)
	enum { IDD = IDD_DLG_RECORD_CTRL };
	CComboBox	m_cbo_slot;
	int		m_record_ctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRecordCtrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRecordCtrl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRECORDCTRL_H__48CC14DC_B954_4C69_9951_DAD8C63700B6__INCLUDED_)
