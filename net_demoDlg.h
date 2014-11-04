
// net_demoDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "net_video_test.h"

#ifndef INVALID_HANDLE
#define  INVALID_HANDLE -1
#endif

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)
		
public:
	CMyButton();
	virtual ~CMyButton();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

class auto_zoom:public CEdit
{
public:
	
	auto_zoom();	
	
	virtual ~auto_zoom();	

	RECT get_zoom_rt();
	
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()

private:
	CPoint m_ori; //坐标位置
	bool m_bmousedown;
	RECT m_zoom_rt;
};

// Cnet_demoDlg 对话框
class zoom_video_test : public CEdit
{
public:

	zoom_video_test();	

	virtual ~zoom_video_test();	

	bool start_test(PLAY_HANDLE handle,RECT rt);	

	void stop_test();
	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	bool m_btest;
	REAL_HANDLE m_handle;

	RECT m_rt; // 屏幕上显示的矩形尺寸

	float m_fx;// 原始视频宽 / 窗口中的视频宽
	float m_fy;// 原始视频高 / 窗口中的视频高

	int m_video_w; //窗口中的视频宽
	int m_video_h; // 窗口中的视频高

	int m_real_video_w; //原始视频宽
	int m_real_video_h;//原始视频高

	int m_back_h; //黑色背景高
	int m_back_w; //黑色背景宽

	float m_zoom;//放大倍数

	POINT m_point; //坐标位置
};

//#define  AUTO_ZOOM

class Cnet_demoDlg : public CDialog
{
// 构造
public:
	Cnet_demoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	//{{AFX_DATA(Cnet_demoDlg)
	enum { IDD = IDD_NET_DEMO_DIALOG };
	CSliderCtrl	m_sharpen;
	CComboBox	m_cbo_com_preset;
	CComboBox	m_cbo_com_speed;	
	CComboBox	m_cbo_blacklighting;
	CComboBox	m_cbo_connect_mode;
	CListCtrl	m_list_alarm;
	CComboBox	m_cbo_scale;
	CComboBox	m_cbo_yuv_buf;
	CSliderCtrl	m_hue;
	CSliderCtrl	m_saturation;
	CSliderCtrl	m_contrast;
	CSliderCtrl	m_bright;
	CIPAddressCtrl m_ip;
	short m_port;
	CString m_log_name;
	CString m_log_psw;
	CComboBox m_slots;
	CComboBox m_svr_type;
	BOOL	m_color_adjust_enable;
	int		m_cur_yuv_buf;
	BOOL	m_audio_preview_enable;
	BOOL	m_enable_alarm;
	BOOL	m_voice_enable;
	int		m_com_addr;
	BOOL	m_blacklighting_enable;
	BOOL	m_serial_send;
	BOOL	m_sharpen_enable;
	BOOL	m_enable_noise_filter;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cnet_demoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	//}}AFX_VIRTUAL
	
	CMyButton m_btn_left;
	CMyButton m_btn_up;
	CMyButton m_btn_down;
	CMyButton m_btn_right;
	CMyButton m_btn_far;
	CMyButton m_btn_near;
	CMyButton m_btn_zoomin;
	CMyButton m_btn_zoomout;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	// Generated message map functions
	//{{AFX_MSG(Cnet_demoDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtlogin();	
	afx_msg void OnBnClickedBtconnect();
	afx_msg void OnClose();	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonPlayback();
	 afx_msg void OnBnClickedButtonSet();	
	afx_msg void OnButtonSet2();
	afx_msg void OnChkColor();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeCboYuvBuf();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSnapBmp();
	afx_msg void OnBtnSnapJpg();
	afx_msg void OnBtnStartRec();
	afx_msg void OnBtnStopRec();
	afx_msg void OnChkAudioPreview();
	afx_msg void OnBtconnectSub();
	afx_msg void OnSelchangeCboScale();
	afx_msg void OnChkRegalarm();
	afx_msg void OnVoice();
	afx_msg void OnBtnComUp();
	afx_msg void OnBtnComLeft();
	afx_msg void OnBtnComRight();
	afx_msg void OnBtnComDown();
	afx_msg void OnBtnComStop();
	afx_msg void OnBtnComZoomOut();
	afx_msg void OnBtnComZoomIn();
	afx_msg void OnChkBlacklighting();
	afx_msg void OnSelchangeCboBlacklighting();
	afx_msg void OnChkSerialSend();
	afx_msg void OnBtnComNear();
	afx_msg void OnBtnComFar();
	afx_msg void OnCboComPresetExec();
	afx_msg void OnCboComPresetSetup();
	afx_msg void OnChkSharpenEnable();
	afx_msg void OnButton5();
	afx_msg void OnButton11();
	afx_msg void OnBtnRemoteStartRec();
	afx_msg void OnBtnRemoteStopRec();
	afx_msg void OnBtconnect2();
	afx_msg void OnChkAudioEnhance();
	afx_msg void OnBtnRemoteEncodeRotate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
#ifdef AUTO_ZOOM
	auto_zoom auto_zoom_test;
#else
	 zoom_video_test zoom_test;	 
#endif 
};
