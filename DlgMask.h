#if !defined(AFX_DLGMASK_H__1DC421E2_61E0_4F34_AF18_48FDB09D47E0__INCLUDED_)
#define AFX_DLGMASK_H__1DC421E2_61E0_4F34_AF18_48FDB09D47E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMask.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMask dialog
#include "howellnetsdk.h"
#include "net_video_test.h"
class CDlgMask : public CDialog
{
// Construction
public:
	CDlgMask(CWnd* pParent = NULL);   // standard constructor

	void close();
	BOOL refresh();
	BOOL commit();
	
	BOOL connect_video();
	void close_video();

// Dialog Data
	//{{AFX_DATA(CDlgMask)
	enum { IDD = IDD_DLG_MASK };
	CComboBox	m_cbo_channel;
	BOOL	m_mask_enable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMask)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMask)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCboSlot();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	net_video_test* m_video;
	bool m_bmousedown;
	POINT m_ori_pt;
	RECT m_mask_rt;

static void __stdcall draw_fun(PLAY_HANDLE handle, HDC hDc, LONG nUser);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMASK_H__1DC421E2_61E0_4F34_AF18_48FDB09D47E0__INCLUDED_)
