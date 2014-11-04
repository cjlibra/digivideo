#pragma once

#include "play_def.h"
#pragma comment(lib,"play_sdk.lib")
#include "howellnetsdk.h"
#pragma comment(lib,"hwclient.lib")
#include "ts_convert_def.h"
#include "flv_convert_def.h"
#include "net_video_test.h"

#include <vector>
#include <string>
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
// CPlaybackDialog 对话框
extern USER_HANDLE g_server_id ;
extern char g_server_ip[32];

class file_list
{
public:
	file_list(USER_HANDLE handle);
	~file_list();

	bool refresh(tRecFile search);
	bool refresh_smart(tRecFile search,RECT rt);
	int count();
	bool get_file_info(int index,tRecFile& file_info);

private:
	USER_HANDLE m_user_handle;
	std::vector<tRecFile> m_files;
};

class rfid_list
{
public:
	rfid_list(USER_HANDLE handle);
	~rfid_list();

	bool refresh(tRecFile search);
	int count();
	bool get_rfid_info(int index,rfid_data_t* data);

private:
	char* m_data;
	int m_data_len;

	int m_left_len;

	USER_HANDLE m_user_handle;	
	std::vector<rfid_data_t> m_rfid_data;
	static void CALLBACK down_process(long lDownID,char*buf,int len,int videoType,long nUser);
};

class play_test
{
public:
	play_test(USER_HANDLE handle,tRecFile file_info);
	~play_test();

	bool play(HWND hWnd);
	void stop();
	bool pause(bool bpause);
	bool step_back();
	bool step_forward();
	bool get_pos(int& pos);
	bool set_pos(int pos);

	bool get_speed(float& speed);
	bool set_speed(float speed);

	bool get_total_sec(int& total_sec);

	bool get_played_sec(int& played_sec);

	bool is_playing();

	bool get_osd(SYSTEMTIME& osd);

	bool register_draw_fun(draw_callback * fun,long user);

private:
	tRecFile m_file_info;
	USER_HANDLE m_user_handle;
	DOWNLOAD_HANDLE m_download_handle;
	PLAY_HANDLE m_play_handle;
	static void CALLBACK down_process(long lDownID,char*buf,int len,int videoType,long nUser);
	HWND m_hwnd;
	draw_callback* m_draw_fun;
	long m_draw_user;
};

class download_test
{
public:
	download_test(USER_HANDLE handle,tRecFile file_info,const char* save_path);
	~download_test();

	bool start_download();
	void stop_download();
	bool get_download_pos(int& pos);
	bool is_downloading();

private:
	std::string m_save_path;
	HANDLE m_save_handle;
	tRecFile m_file_info;
	USER_HANDLE m_user_handle;
	DOWNLOAD_HANDLE m_download_handle;
	TS_CONVERT_HANDLE m_ts_contert_h;
	FLV_CONVERT_HANDLE m_flv_convert_h;
private:
	static void CALLBACK down_process(long lDownID,char*buf,int len,int videoType,long nUser);
};

class CPlaybackDialog : public CDialog
{
	DECLARE_DYNAMIC(CPlaybackDialog)

public:
	CPlaybackDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlaybackDialog();

// 对话框数据
	//{{AFX_DATA(CPlaybackDialog)
	enum { IDD = IDD_PLAYBACK_DIALOG };
	CComboBox	m_cbo_type;
	CComboBox	m_cbo_slot;
	CListBox	m_ctrl_smart;
	CDateTimeCtrl m_beg_y;
	CDateTimeCtrl m_beg_h;
	CDateTimeCtrl m_end_y;
	CDateTimeCtrl m_end_h;
	CListBox m_ctrl_list;
	CSliderCtrl m_play_pos;
	CProgressCtrl m_download_pos;
	//}}AFX_DATA

	bool is_playing();
	bool is_downloading();

	void stop_play();
	void stop_download();

protected:
	//{{AFX_MSG(CPlaybackDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();	
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStepBack();
	afx_msg void OnBnClickedButtonStepForward();
	afx_msg void OnBnClickedButtonSpeed();
	afx_msg void OnBnClickedButtonSlow();
	afx_msg void OnLbnSelchangeListFile();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();	
	afx_msg void OnBnClickedButtonStartDownload();
	afx_msg void OnButtonStopDwonload();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonSmart();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonShowRect();
	afx_msg void OnButtonShowRect2();
	afx_msg void OnSelchangeListSmartFile();
	//}}AFX_MSG

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaybackDialog)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()

private:
	file_list m_file_list;
	rfid_list m_rfid_list;
	//file_list m_smart_list;
	play_test* m_play_test;
	download_test* m_download_test;
	bool m_scrolling;
	
	bool m_show_smartrt;
	RECT m_smart_rt;

	CPoint m_ori_pt;
	BOOL m_bmouse_down ;

protected:
	static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser);
public:
	afx_msg void OnBnClickedBtnDeleteFile();
};
