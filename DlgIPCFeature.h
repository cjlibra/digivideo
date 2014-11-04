#if !defined(AFX_DLGIPCFEATURE_H__140C5BFF_7956_4EDD_8ED6_3013353FE118__INCLUDED_)
#define AFX_DLGIPCFEATURE_H__140C5BFF_7956_4EDD_8ED6_3013353FE118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIPCFeature.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCFeature dialog
#include "howellnetsdk.h"
#include "net_video_test.h"
class CDlgIPCFeature : public CDialog
{
// Construction
public:
	CDlgIPCFeature(CWnd* pParent = NULL);   // standard constructor
	
	BOOL refresh();
	void close();
	BOOL commit();

	void refresh_state();
	BOOL connect_video();
	void  close_video();
// Dialog Data
	//{{AFX_DATA(CDlgIPCFeature)
	enum { IDD = IDD_DLG_IPC_FEATURE };
	CComboBox	m_cbo_shutter;
	CComboBox	m_cbo_db;
	CComboBox	m_cbo_channel;
	BOOL	m_ae_enable;
	BOOL	m_auto_focus_rect_enable;
	BOOL	m_enable_flip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIPCFeature)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	

	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIPCFeature)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnClose();
	afx_msg void OnCheck1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBtnAutoFocus();
	afx_msg void OnChkRectEnable();
	afx_msg void OnBtnEnableFlip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
		net_video_test* m_video;
		bool m_bmousedown;
		POINT m_ori_pt;
		RECT m_auto_focus_rt;
		CBitmap m_mask_bmp;

static void __stdcall draw_fun(PLAY_HANDLE handle, HDC hDc, LONG nUser);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIPCFEATURE_H__140C5BFF_7956_4EDD_8ED6_3013353FE118__INCLUDED_)
