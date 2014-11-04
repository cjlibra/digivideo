#pragma once
#include "afxwin.h"


// CDlgQuality 对话框

class CDlgQuality : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuality)

public:
	CDlgQuality(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuality();

// 对话框数据
	//{{AFX_DATA(CDlgQuality)
	enum { IDD = IDD_DLG_QUALITY };
	CComboBox m_cbo_slot;
	CComboBox m_cbo_quality;
	CComboBox m_cbo_max_bps;
	CComboBox m_cbo_encode_type;
	CComboBox m_cbo_frame;
	CComboBox m_cbo_stream_type;
	CComboBox m_cbo_bps_type;
	int		m_max_bps;
	int		m_gop_size;
	//}}AFX_DATA

	BOOL refresh();
	void close();
	BOOL commit();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgQuality)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnSelchangeCombo4();
	//}}AFX_MSG

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQuality)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()

public:	
};
