#pragma once
#include "afxwin.h"
#include "net_video_test.h"
#include "afxcmn.h"

// CDlgOsd 对话框

class CDlgOsd : public CDialog
{
	DECLARE_DYNAMIC(CDlgOsd)

public:
	CDlgOsd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOsd();

// 对话框数据
	//{{AFX_DATA(CDlgOsd)
	enum { IDD = IDD_DLG_OSD };

	CComboBox m_cbo_chn;
	int m_name_x;
	int m_name_y;
	CString m_name;
	BOOL m_name_enable;
	int m_date_y;
	int m_date_x;
	BOOL m_date_enable;
	BOOL m_date_week_enable;
	CSliderCtrl m_sld_bright;
	CSliderCtrl m_sld_contrast;
	CSliderCtrl m_sld_saturation;
	CSliderCtrl m_sld_hue;
	//}}AFX_DATA

	BOOL refresh();
	void close();
	BOOL commit();
	void close_video();
	BOOL connect_video();
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgOsd)
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	//}}AFX_MSG

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOsd)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()

public:	
	net_video_test* m_video;	
};
