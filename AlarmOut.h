#if !defined(AFX_ALARMOUT_H__C6B98956_A792_4955_9DA3_C19E1805C603__INCLUDED_)
#define AFX_ALARMOUT_H__C6B98956_A792_4955_9DA3_C19E1805C603__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmOut.h : header file
//
#include "howellnetsdk.h"
/////////////////////////////////////////////////////////////////////////////
// AlarmOut dialog

class AlarmOut : public CDialog
{
// Construction
public:
	AlarmOut(CWnd* pParent = NULL);   // standard constructor
	void close();
	BOOL refresh();
	BOOL commit();

// Dialog Data
	//{{AFX_DATA(AlarmOut)
	enum { IDD = IDD_DLG_ALARM_OUT };
	CComboBox	m_cbo_alarmout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AlarmOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AlarmOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAlarmoutStart();
	afx_msg void OnBtnAlarmoutStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMOUT_H__C6B98956_A792_4955_9DA3_C19E1805C603__INCLUDED_)
