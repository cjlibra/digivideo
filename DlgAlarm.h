#if !defined(AFX_DLGALARM_H__05AE76AA_21F8_43C1_BC2F_AB0A51BACDF8__INCLUDED_)
#define AFX_DLGALARM_H__05AE76AA_21F8_43C1_BC2F_AB0A51BACDF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarm.h : header file
//

#include "howellnetsdk.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm dialog

class CDlgAlarm : public CDialog
{
// Construction
public:
	CDlgAlarm(CWnd* pParent = NULL);   // standard constructor
	
	void close();
	BOOL refresh();
	BOOL refresh_date(int day);
	BOOL commit();
// Dialog Data
	//{{AFX_DATA(CDlgAlarm)
	enum { IDD = IDD_DLG_ALARM };
	CComboBox	m_cbo_defend;
	CComboBox	m_cbo_copy_date;
	CComboBox	m_cbo_type;
	CComboBox	m_cbo_date;
	CComboBox	m_cbo_alarm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlarm)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButton2();
	afx_msg void OnButton5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_alarm_in_count;
	int m_alarm_out_count;

	CButton* m_chk_rec[16];//联动录像
	CButton* m_chk_out[16];//联动输出

	CSpinButtonCtrl* m_spin_beg_h[3];//开始时间
	CSpinButtonCtrl* m_spin_beg_m[3];//开始时间
	CSpinButtonCtrl* m_spin_end_h[3];//结束时间
	CSpinButtonCtrl* m_spin_end_m[3];//结束时间

	struct tAlarmInCfg m_alarm_in_cfg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARM_H__05AE76AA_21F8_43C1_BC2F_AB0A51BACDF8__INCLUDED_)
