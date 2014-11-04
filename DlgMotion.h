#if !defined(AFX_DLGMOTION_H__6D2EBAA2_46B7_4FFF_A662_D0593DA65037__INCLUDED_)
#define AFX_DLGMOTION_H__6D2EBAA2_46B7_4FFF_A662_D0593DA65037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMotion.h : header file
//

#include "net_video_test.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgMotion dialog

class CDlgMotion;
class net_motion_test: public CEdit
{
public:
	net_motion_test(CWnd* pParent = NULL);
	
public:
	BOOL connect_video(USER_HANDLE server_id,int slot);
	void close_video();

	BOOL show_motion(BOOL bshow);
	
	BOOL is_connected();
	BOOL is_show_motion();

	BOOL is_motion_set(POINT pt);
	BOOL is_motion_set(int x,int y);

	void clear_motion(POINT pt);
	void clear_motion(int x, int y);

	void set_motion(POINT pt);
	void set_motion(int x,int y);

	void clear_all_motion();

protected:
	BOOL getxy(POINT pt,int* x,int* y);
	
	// Generated message map functions
	//{{AFX_MSG(net_motion_test)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_show_motion;
	net_video_test* m_video;
	
	int m_motion_row;
	int m_motion_col;
	int m_int_num_in_one_row;
	tMotionEx m_motion_cfg;

	CBitmap m_mask_bmp;

	POINT m_ori;
	BOOL m_bmouse_down;
	BOOL m_ori_is_set;

	friend CDlgMotion;
private:
	static void CALLBACK motion_draw(PLAY_HANDLE handle,HDC hDc,LONG nUser);
};

class CDlgMotion : public CDialog
{
// Construction
public:
	CDlgMotion(CWnd* pParent = NULL);   // standard constructor

	BOOL refresh();
	void close();
	BOOL commit();
	void close_video();
	BOOL connect_video();


// Dialog Data
	//{{AFX_DATA(CDlgMotion)
	enum { IDD = IDD_DLG_MOTION };
	CComboBox	m_cbo_sensi;
	CComboBox	m_cbo_slot;
	CComboBox	m_cbo_delay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMotion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMotion)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	net_motion_test m_motion_test;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOTION_H__6D2EBAA2_46B7_4FFF_A662_D0593DA65037__INCLUDED_)
