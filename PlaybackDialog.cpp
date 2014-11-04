// PlaybackDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "PlaybackDialog.h"
#include "ts_convert_def.h"

file_list::file_list(USER_HANDLE handle)
:m_user_handle(handle)
{

}

file_list::~file_list()
{

}

bool file_list::refresh_smart(tRecFile search,RECT rt)
{
	tSmartSearchEx smart_search;
	smart_search.recFile = search;
	smart_search.rt_count = 1;
	smart_search.rt[0] = rt;
	smart_search.direction = 0;

	m_files.clear();
	
	long file_handle = HW_NET_SET_FindSmartSearchFileEx(m_user_handle,&smart_search);
	if(file_handle == -1)
	{
		//搜索文件失败
		return false;
	}
	
	while(1)
	{
		long ret = HW_NET_PLAY_FindNextFile(file_handle,&search);
		if(ret == HWNET_ISFINDING)
		{
			Sleep(10);
			continue;
		}
		if(ret == HWNET_NOMOREFILES || ret == HWNET_NOFILE)
		{
			break;
		}
		
		if(ret == HWNET_FILE_SUCCESS)
		{
			TRACE("%d:%d:%d\n",search.beg.wHour,search.beg.wMinute,search.beg.wSecond);
			m_files.push_back(search);
		}
		
		if(ret == HWNET_FILE_EXCEPTION)
		{
			//发生异常
			TRACE("file refresh error\n");
			HW_NET_PLAY_FindFileClose(file_handle);
			return false;
		}
	}
	HW_NET_PLAY_FindFileClose(file_handle);

	return true;
}

bool file_list::refresh(tRecFile search)
{
	m_files.clear();

	long file_handle = HW_NET_PLAY_FindFile(m_user_handle,&search);
	if(file_handle == -1)
	{
		//搜索文件失败
		return false;
	}

	while(1)
	{
		long ret = HW_NET_PLAY_FindNextFile(file_handle,&search);
		if(ret == HWNET_ISFINDING)
		{
			Sleep(10);
			continue;
		}
		if(ret == HWNET_NOMOREFILES || ret == HWNET_NOFILE)
		{
			break;
		}

		if(ret == HWNET_FILE_SUCCESS)
		{
			m_files.push_back(search);
		}

		if(ret == HWNET_FILE_EXCEPTION)
		{
			//发生异常
			TRACE("file refresh error\n");
			HW_NET_PLAY_FindFileClose(file_handle);
			return false;
		}
	}
	HW_NET_PLAY_FindFileClose(file_handle);
	return true;
}

int file_list::count()
{
	return m_files.size();
}

bool file_list::get_file_info(int index,tRecFile& file_info)
{
	if(index >= m_files.size())
	{
		return false;
	}
	file_info = m_files[index];
	return true;
}

rfid_list::rfid_list(USER_HANDLE user_handle)
	:m_user_handle(user_handle)
{
	m_data = new char[1024 * 1024];
}

rfid_list::~rfid_list()
{
	delete[] m_data;
}

bool rfid_list::refresh(tRecFile search)
{
	m_rfid_data.clear();

	//rfid 的搜索类型
	search.type = 7;

	HW_NET_SetTimeout(10000);

	DOWNLOAD_HANDLE download_handle = HW_NET_SET_DownloadFileEx(m_user_handle,&search,down_process,(long) this);
	if(download_handle != INVALID_HANDLE)
	{			
		int size = 0;		
		HW_NET_SET_GetdownloadFileSize(download_handle,&size);
		TRACE("sizel = %d\n",size);

		m_data_len = 0;
		m_left_len = 0;
		HW_NET_SET_StartDownloadCapture(download_handle);

		//等待数据接收完毕
		while(m_data_len < size)
		{
			Sleep(10);
		}		

		HW_NET_SET_StopDownload(download_handle);		

		return true;
	}

	return false;
}

int rfid_list::count()
{
	return m_rfid_data.size();
}

bool rfid_list::get_rfid_info(int index,rfid_data_t* data)
{
	if(index >= m_rfid_data.size())
	{
		return false;
	}

	*data = m_rfid_data[index];
	return true;
}

void CALLBACK rfid_list::down_process(long lDownID,char*buf,int len,int videoType,long nUser)
{
	rfid_list* rf = (rfid_list*)nUser;

	if(len > 0 
		&& len != 40)
	{
		//总共收到的数据
		rf->m_data_len += len;
	}

	if(len > 0
		&& len != 40)
	{
		//m_left_len 是上次未处理完的数据长度
		memcpy(&rf->m_data[rf->m_left_len],buf,len);
		rf->m_left_len += len;
		
		char* p = rf->m_data;
		int left = rf->m_left_len;		
		while(left >= sizeof(stream_head))
		{
			stream_head* sh = (stream_head*)p;
			if(left < sh->len)
			{
				break;
			}

			extra_data_head* eh = (extra_data_head*)(p + sizeof(stream_head));

			TRACE("type=%d\n",eh->type);
			if(eh->type == 2)
			{
				rfid_data_t data;
				memset(&data,0,sizeof(data));

				data.tm = sh->sys_time;
				data.len = sh->len - sizeof(stream_head) - sizeof(extra_data_head);
				TRACE(">>>>>>>>tm =%d,len=%d\n",data.tm,data.len);
				memcpy(data.buf,p + sizeof(stream_head) + sizeof(extra_data_head),data.len);
				for(int i = 0; i < data.len; i++)
				{
					TRACE("%02x ",data.buf[i]);
				}
				TRACE("\n");

				rf->m_rfid_data.push_back(data);
			}			

			left -= sh->len;
			p += sh->len;
		}

		rf->m_left_len = left;
		//如果处理完毕后，还有多余数据(可能未满一帧),需要往前推
		if(p != rf->m_data
			&& rf->m_left_len > 0)
		{
			memcpy(rf->m_data,p,rf->m_left_len);
		}
	}	
}


play_test::play_test(USER_HANDLE handle, tRecFile file_info)
:m_file_info(file_info),m_user_handle(handle),m_play_handle(INVALID_HANDLE)
{

}

bool play_test::play(HWND hWnd)
{
	if(is_playing())
	{
		//以正常倍速播放
		hwplay_pause(m_play_handle,false);
		hwplay_set_speed(m_play_handle,1);

		return true;
	}else{
		m_download_handle = HW_NET_SET_DownloadFileEx(m_user_handle,&m_file_info,down_process,(long) this);
		if(m_download_handle != INVALID_HANDLE)
		{	
			char buf[40];
			int ret_len;
			HW_NET_SET_GetDownloadHead(m_download_handle,buf,40,&ret_len);

			m_play_handle = hwplay_open_stream(buf,40,1024 * 1024,PLAY_FILE);
			if(m_play_handle == INVALID_HANDLE)
			{
				HW_NET_SET_StopDownload(m_download_handle);
				m_download_handle = INVALID_HANDLE;
				return false;
			}

			HW_NET_SET_StartDownloadCapture(m_download_handle);
			if(hwplay_play(m_play_handle,hWnd))
			{
				//AfxMessageBox("play success");
			}
			else
			{
				AfxMessageBox("play failed");
			}
	
			hwplay_auto_adjust_size(m_play_handle,FALSE);
			hwplay_open_sound(m_play_handle);
			m_hwnd = hWnd;
			m_draw_fun = NULL;
			m_draw_user = 0;
		}		
		return m_download_handle != INVALID_HANDLE && m_play_handle != INVALID_HANDLE;
	}
}

void play_test::stop()
{
	if(is_playing())
	{
		hwplay_close_sound(m_play_handle);
		hwplay_stop(m_play_handle);
		m_play_handle = INVALID_HANDLE;
		HW_NET_SET_StopDownload(m_download_handle);	
		m_download_handle =INVALID_HANDLE;
	}
}

void CALLBACK play_test::down_process(long lDownID,char*buf,int len,int videoType,long nUser)
{
	play_test* p = (play_test*)nUser;
	if(videoType != HW_FRAME_VIDEO_HEAD)
	{
		while(p->is_playing() )
		{		
			if(!hwplay_input_data(p->m_play_handle,buf,len))
			{
				Sleep(10);
				continue;	
			}
			break;
		}
	}
}

bool play_test::step_back()
{
	if(!is_playing()) return false;

	return hwplay_step_back(m_play_handle);
}

bool play_test::step_forward()
{
	if(!is_playing()) return false;

	return hwplay_step_froward(m_play_handle);
}

bool play_test::get_pos(int& pos)
{
	if(!is_playing()) return false;

	int total_sec,played_sec;
	if(!get_total_sec(total_sec)
		|| ! get_played_sec(played_sec)
		|| total_sec == 0)
	{
		return false;
	}

	pos = played_sec * 100 / total_sec;

	if(pos > 98 )
	{
		int download_pos;
		int frame_count;
		int buf_remain;
		HW_NET_SET_GetdownloadPos(m_download_handle,&download_pos);
		hwplay_get_framenum_in_buf(m_play_handle,&frame_count);
		hwplay_get_stream_buf_remain(m_play_handle,&buf_remain);
		if(download_pos == 100
			&&frame_count == 0
			&& buf_remain == 0)
		{
			pos = 100;
		}
	}
	return true;
}

bool play_test::set_pos(int pos)
{
	int totalsec;
	if(!get_total_sec(totalsec))
	{
		return false;
	}
	
	
	int interval = totalsec * pos / 100;
	CTime tm = CTime(m_file_info.beg) + CTimeSpan(0,0,0,interval);
	
	SYSTEMTIME locate_tm;
	tm.GetAsSystemTime(locate_tm);

	stop();

	tRecFile file_info;
	file_info = m_file_info;
	file_info.beg = locate_tm;

	m_download_handle = HW_NET_SET_DownloadFileEx(m_user_handle,&file_info,down_process,(long) this);
	if(m_download_handle != INVALID_HANDLE)
	{	
		char buf[40];
		int ret_len;
		HW_NET_SET_GetDownloadHead(m_download_handle,buf,40,&ret_len);
		
		m_play_handle = hwplay_open_stream(buf,40,1024 * 1024,PLAY_FILE);
		if(m_play_handle == INVALID_HANDLE)
		{
			HW_NET_SET_StopDownload(m_download_handle);
			m_download_handle = INVALID_HANDLE;
		}
		
		HW_NET_SET_StartDownloadCapture(m_download_handle);
		hwplay_play(m_play_handle,m_hwnd);
		hwplay_auto_adjust_size(m_play_handle,FALSE);
		hwplay_open_sound(m_play_handle);

		if(m_draw_fun != NULL)
		{
			hwplay_register_draw_fun(m_play_handle,m_draw_fun,m_draw_user);
		}
	}

	return m_download_handle != INVALID_HANDLE && m_play_handle != INVALID_HANDLE;
}

bool play_test::get_total_sec(int& totalsec)
{
	CTimeSpan span = CTime(m_file_info.end) - CTime(m_file_info.beg);
	totalsec  = span.GetTotalSeconds();
	return true;
}

bool play_test::get_played_sec(int &playedsec)
{
	if(!is_playing())
	{
		return false;
	}
	
	SYSTEMTIME osd_time;
	if(!get_osd(osd_time))
	{
		return false;
	}	

	CTimeSpan span = CTime(osd_time) - CTime(m_file_info.beg);
	playedsec= span.GetTotalSeconds();
	return true;
}

bool play_test::get_osd(SYSTEMTIME& osd)
{
	if(!is_playing())
	{
		return false;
	}
	
	if(!hwplay_get_osd(m_play_handle,&osd))
	{
		return false;
	}
	return true;
}

bool play_test::register_draw_fun(draw_callback * fun,long user)
{
	if(!is_playing())
	{
		return false;
	}

	hwplay_register_draw_fun(m_play_handle,fun,user);
	m_draw_fun = fun;
	m_draw_user = user;
	return true;
}

bool play_test::pause(bool bpause)
{
	if(!is_playing()) return false;

	return hwplay_pause(m_play_handle,bpause);
}

bool play_test::get_speed(float& speed)
{
	if(!is_playing()) return false;

	return hwplay_get_speed(m_play_handle,&speed);
}

bool play_test::set_speed(float speed)
{
	if(!is_playing()) return false;

	return hwplay_set_speed(m_play_handle,speed);
}

play_test::~play_test()
{
	if(is_playing())
	{
		stop();
	}
}

bool play_test::is_playing()
{
	return m_play_handle != INVALID_HANDLE && m_download_handle != INVALID_HANDLE;
}


download_test::download_test(USER_HANDLE handle, tRecFile file_info,const char* save_path)
:m_user_handle(handle),m_file_info(file_info)
,m_download_handle(INVALID_HANDLE),m_save_path(save_path)
{	
}

download_test::~download_test()
{
	stop_download();
}

bool download_test::start_download()
{
	if(is_downloading()) return false;	
	
	m_download_handle = HW_NET_SET_DownloadFileEx(m_user_handle,&m_file_info,down_process,(long) this);
	if(m_download_handle != INVALID_HANDLE)
	{
#if SAVE_TS_FILE
		m_ts_contert_h = ts_convert_start(m_save_path.c_str(),25);
#else
#if SAVE_FLV_FILE
		m_flv_convert_h = flv_convert_start(m_save_path.c_str(),25);
#else
		m_save_handle = CreateFile(m_save_path.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		
		//先保存头
		char buf[40];
		int ret_len;
		DWORD write_num;

		HW_NET_SET_GetDownloadHead(m_download_handle,buf,40,&ret_len);
		WriteFile(m_save_handle,buf,40,&write_num,NULL);
#endif
#endif

		HW_NET_SET_StartDownloadCapture(m_download_handle);
	}
	return m_download_handle != INVALID_HANDLE;
}

void download_test::stop_download()
{
	if(is_downloading())
	{
		HW_NET_SET_StopDownload(m_download_handle);
		m_download_handle = INVALID_HANDLE;
#if SAVE_TS_FILE
		ts_convert_stop(m_ts_contert_h);
#else
#if SAVE_FLV_FILE
		flv_convert_stop(m_flv_convert_h);
#else
		CloseHandle(m_save_handle);
#endif
#endif
	}	
}

bool download_test::is_downloading()
{
	return m_download_handle != INVALID_HANDLE;
}

bool download_test::get_download_pos(int &pos)
{
	if(is_downloading())
	{
		return HW_NET_SET_GetdownloadPos(m_download_handle,&pos);
	}
	return false;
}

void CALLBACK download_test::down_process(long lDownID,char*buf,int len,int videoType,long nUser)
{
	download_test* test = (download_test*)nUser;
	DWORD write_num;
	if(len != 40)
	{
#if SAVE_TS_FILE
		ts_convert_input(test->m_ts_contert_h,buf,len);
#else
#if SAVE_FLV_FILE
		flv_convert_input(test->m_flv_convert_h,buf,len);
#else
		WriteFile(test->m_save_handle,buf,len,&write_num,NULL);
#endif
#endif
	}
}

// CPlaybackDialog 对话框

IMPLEMENT_DYNAMIC(CPlaybackDialog, CDialog)

CPlaybackDialog::CPlaybackDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaybackDialog::IDD, pParent)
	,m_file_list(g_server_id),m_play_test(NULL),m_download_test(NULL)	
	,m_rfid_list(g_server_id)
{
	//{{AFX_DATA_INIT(CPlaybackDialog)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

CPlaybackDialog::~CPlaybackDialog()
{	
}

bool CPlaybackDialog::is_playing()
{
	return m_play_test != NULL;
}

bool CPlaybackDialog::is_downloading()
{
	return m_download_test != NULL;
}

void CPlaybackDialog::stop_play()
{
	if(m_play_test)
	{
		m_play_test->stop();
		delete m_play_test;
		m_play_test = NULL;
	}
}

void CPlaybackDialog::stop_download()
{
	if(m_download_test)
	{
		delete m_download_test;
		m_download_test = NULL;
	}
}

BOOL CPlaybackDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//添加通道数
	int slot_count = HW_NET_GetWindowCount(g_server_id);
	for(int i = 0; i < slot_count; i++)
	{
		CString str;
		str.Format("通道%d",i+1);
		m_cbo_slot.AddString(str.GetBuffer(0));
	}

	if(slot_count > 0)
	{
		m_cbo_slot.SetCurSel(0);
	}

	m_cbo_type.AddString("全部");
	m_cbo_type.AddString("实时");
	m_cbo_type.AddString("移动侦测");
	m_cbo_type.AddString("报警");	

	m_cbo_type.SetCurSel(0);
	
	SYSTEMTIME beg,end;
	GetLocalTime(&beg);
	GetLocalTime(&end);

	beg.wHour = 8;
	beg.wMinute = 0;
	beg.wSecond = 0;
	m_beg_h.SetTime(&beg);
	end.wHour = 23;
	end.wMinute = 59;
	end.wSecond = 59;
	m_end_h.SetTime(&end);

	m_scrolling = false;
	m_show_smartrt =true;
	m_bmouse_down = false;
	memset(&m_smart_rt,0,sizeof(m_smart_rt));

	SetTimer(1,1000,NULL);
	UpdateData(TRUE);

	return TRUE;
}

void CPlaybackDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaybackDialog)
	DDX_Control(pDX, IDC_CBO_FILE_TYPE, m_cbo_type);
	DDX_Control(pDX, IDC_CBO_SLOT, m_cbo_slot);
	DDX_Control(pDX, IDC_LIST_SMART_FILE, m_ctrl_smart);
	DDX_Control(pDX, IDC_DATE_BEG_Y, m_beg_y);
	DDX_Control(pDX, IDC_DATE_BEG_H, m_beg_h);
	DDX_Control(pDX, IDC_DATE_END_Y, m_end_y);
	DDX_Control(pDX, IDC_DATE_END_H, m_end_h);
	DDX_Control(pDX, IDC_LIST_FILE, m_ctrl_list);
	DDX_Control(pDX, IDC_PLAY_POS, m_play_pos);
	DDX_Control(pDX, IDC_DOWNLOAD_POS, m_download_pos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaybackDialog, CDialog)
	//{{AFX_MSG_MAP(CPlaybackDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STEP_BACK, OnBnClickedButtonStepBack)
	ON_BN_CLICKED(IDC_BUTTON_STEP_FORWARD, OnBnClickedButtonStepForward)
	ON_BN_CLICKED(IDC_BUTTON_SPEED, OnBnClickedButtonSpeed)
	ON_BN_CLICKED(IDC_BUTTON_SLOW, OnBnClickedButtonSlow)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, OnLbnSelchangeListFile)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_START_DOWNLOAD, OnBnClickedButtonStartDownload)
	ON_BN_CLICKED(IDC_BUTTON_STOP_DWONLOAD, OnButtonStopDwonload)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_SMART, OnButtonSmart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_RECT, OnButtonShowRect)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_RECT2, OnButtonShowRect2)
	ON_LBN_SELCHANGE(IDC_LIST_SMART_FILE, OnSelchangeListSmartFile)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_DELETE_FILE, &CPlaybackDialog::OnBnClickedBtnDeleteFile)
END_MESSAGE_MAP()


// CPlaybackDialog 消息处理程序

void CPlaybackDialog::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME beg,end,beg_temp,end_temp;
	m_beg_y.GetTime(&beg);
	m_beg_h.GetTime(&beg_temp);
	beg.wHour = beg_temp.wHour;
	beg.wMinute = beg_temp.wMinute;
	beg.wSecond = beg_temp.wSecond;

	m_end_y.GetTime(&end);
	m_end_h.GetTime(&end_temp);
	end.wHour = end_temp.wHour;
	end.wMinute = end_temp.wMinute;
	end.wSecond = end_temp.wSecond;

	tRecFile search;
	search.beg = beg;
	search.end = end;
	search.type = m_cbo_type.GetCurSel();	
	search.slot = m_cbo_slot.GetCurSel();
	if(!m_file_list.refresh(search))
	{
		MessageBox("搜索文集列表失败!");
		return;
	}

	//清空控件
	m_ctrl_list.ResetContent();

	tRecFile file_info;
	for(int i = 0; i < m_file_list.count(); i++)
	{
		m_file_list.get_file_info(i,file_info);
		CString type;

		if(file_info.type == 5)
		{
			type = "(迁移)";
		}

		CString str;
		str.Format("%02d:%02d:%02d ---- %02d:%02d:%02d %s",
			file_info.beg.wHour,file_info.beg.wMinute,file_info.beg.wSecond,
			file_info.end.wHour,file_info.end.wMinute,file_info.end.wSecond,
			type.GetBuffer(0));
		
		m_ctrl_list.AddString(str.GetBuffer(0));
	}
}

void CPlaybackDialog::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		m_play_test->play(GetDlgItem(IDC_VIDEO)->GetSafeHwnd());
	}
}

void CPlaybackDialog::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		m_play_test->pause(true);
	}
}

void CPlaybackDialog::OnBnClickedButtonStepBack()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		m_play_test->step_back();
	}
}

void CPlaybackDialog::OnBnClickedButtonStepForward()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		m_play_test->step_forward();
	}
}

void CPlaybackDialog::OnBnClickedButtonSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		float speed;
		m_play_test->get_speed(speed);
		if(speed < 4)
		{
			m_play_test->set_speed(speed * 2);
		}
	}
}

void CPlaybackDialog::OnBnClickedButtonSlow()
{
	// TODO: 在此添加控件通知处理程序代码
	if(is_playing())
	{
		float speed;
		m_play_test->get_speed(speed);
		if(speed > 0.25)
		{
			m_play_test->set_speed(speed / 2);
		}
	}
}

void CPlaybackDialog::OnLbnSelchangeListFile()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ctrl_list.GetCurSel();
	if(index < 0) return ;

	tRecFile file_info;
	m_file_list.get_file_info(index,file_info);

	//先停止播放
	TRACE("stop play 1\n");
	stop_play();
	TRACE("stop play 2\n");

	m_play_test = new play_test(g_server_id,file_info);
	if(m_play_test->play(GetDlgItem(IDC_VIDEO)->GetSafeHwnd()))
	{	
		m_play_test->register_draw_fun(draw_fun,(long)this);
	}else{
		delete m_play_test;
		m_play_test = NULL;
	}
}

void CPlaybackDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(is_playing())
	{
		if(!m_scrolling)
		{
			int pos;
			if(m_play_test->get_pos(pos))
			{
				TRACE("pos %d\n",pos);
				m_play_pos.SetPos(pos);
			}		
		}		
	}else{
		m_play_pos.SetPos(0);
	}

	if(is_downloading())
	{
		int pos;
		m_download_test->get_download_pos(pos);
		m_download_pos.SetPos(pos);
		TRACE("download pos=%d\n",pos);

		if(pos == 100)
		{
			stop_download();

			MessageBox("下载完毕!");
		}
	}else{
		m_download_pos.SetPos(0);
	}

	CDialog::OnTimer(nIDEvent);
}

void CPlaybackDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID)) {
	case IDC_PLAY_POS :
		if(nSBCode==SB_ENDSCROLL)
		{	
			m_scrolling = FALSE;
			if(is_playing())
			{
				m_play_test->set_pos(m_play_pos.GetPos());				
			}
		}else if(nSBCode==SB_THUMBTRACK ){
			m_scrolling = TRUE;
		}		
		break;

	default:
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlaybackDialog::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	stop_play();
	
}

void CPlaybackDialog::OnBnClickedButtonStartDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ctrl_list.GetCurSel();
	if(index < 0) return ;

	tRecFile file_info;
	m_file_list.get_file_info(index,file_info);

	stop_download();

 	//file_info.beg.wHour = file_info.beg.wHour -1;
 	//file_info.type = 0;
	m_download_test = new download_test(g_server_id,file_info,"c:\\test.mp4");
	if(m_download_test->start_download())
	{

	}else{
		delete m_download_test;
		m_download_test = NULL;
		MessageBox("下载失败!");
	}
}

void CPlaybackDialog::OnButtonStopDwonload() 
{
	// TODO: Add your control notification handler code here
	
	if(is_downloading())
	{
		m_download_test->stop_download();
		delete m_download_test;
		m_download_test = NULL;
	}
}

void CPlaybackDialog::draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser)
{
	CDC* dc = CDC::FromHandle(hDc);
	CPlaybackDialog* test= (CPlaybackDialog*)nUser;
	if(test->m_show_smartrt)
	{
		CPen newPen(0,1,RGB(0,0,255));
		CPen *oldPen = dc->SelectObject(&newPen);

		dc->MoveTo(test->m_smart_rt.left,test->m_smart_rt.top);
		dc->LineTo(test->m_smart_rt.right,test->m_smart_rt.top);
		dc->LineTo(test->m_smart_rt.right,test->m_smart_rt.bottom);
		dc->LineTo(test->m_smart_rt.left,test->m_smart_rt.bottom);
		dc->LineTo(test->m_smart_rt.left,test->m_smart_rt.top);
	}
}

void CPlaybackDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(is_playing())
	{
		RECT PlayRect;
		GetDlgItem(IDC_VIDEO)->GetWindowRect(&PlayRect);
		ScreenToClient((LPPOINT)&PlayRect);
		ScreenToClient((LPPOINT)&PlayRect+1);	
		if(PtInRect(&PlayRect,point))
		{
			m_ori_pt = point;
			m_bmouse_down = true;
		}	
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CPlaybackDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bmouse_down)
	{
		RECT PlayRect;
		GetDlgItem(IDC_VIDEO)->GetWindowRect(&PlayRect);
		ScreenToClient((LPPOINT)&PlayRect);
		ScreenToClient((LPPOINT)&PlayRect+1);	

		if(PtInRect(&PlayRect,point))
		{
			m_smart_rt.left = (m_ori_pt.x < point.x ? m_ori_pt.x : point.x) - PlayRect.left;
			m_smart_rt.top = (m_ori_pt.y  < point.y ? m_ori_pt.y : point.y) - PlayRect.top;
			m_smart_rt.right = m_smart_rt.left + abs(point.x - m_ori_pt.x) ;
			m_smart_rt.bottom = m_smart_rt.top + abs(point.y - m_ori_pt.y) ;
		}		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CPlaybackDialog::OnButtonSmart() 
{
	// TODO: Add your control notification handler code here
	int index = m_ctrl_list.GetCurSel();
	if(index < 0) return ;
	
	tRecFile search;
	m_file_list.get_file_info(index,search);	
		
	//清空控件
	m_ctrl_smart.ResetContent();

	m_rfid_list.refresh(search);

	rfid_data_t data;
	for(int i = 0; i < m_rfid_list.count(); i++)
	{
		m_rfid_list.get_rfid_info(i,&data);		

		time_t temp = data.tm;
		struct tm* t = gmtime(&temp);

		SYSTEMTIME sys_tm;
		
		sys_tm.wYear = t->tm_year + 1900;
		sys_tm.wMonth = t->tm_mon + 1;
		sys_tm.wDay = t->tm_mday;
		sys_tm.wHour = t->tm_hour;
		sys_tm.wMinute = t->tm_min;
		sys_tm.wSecond = t->tm_sec;

		CString str;
		str.Format("%02d:%02d:%02d data=",sys_tm.wHour,sys_tm.wMinute,sys_tm.wSecond);
		for(int j = 0; j < data.len; j++)
		{
			CString tmp;
			tmp.Format("%02x ",data.buf[j]);
			str.Append(tmp);
		}

		m_ctrl_smart.AddString(str);
	}

	MessageBox("搜索完成");
}

void CPlaybackDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	m_bmouse_down = false;

	CDialog::OnLButtonUp(nFlags, point);
}

void CPlaybackDialog::OnButtonShowRect() 
{
	// TODO: Add your control notification handler code here
	m_show_smartrt = true;
}

void CPlaybackDialog::OnButtonShowRect2() 
{
	// TODO: Add your control notification handler code here
	m_show_smartrt = false;
}

void CPlaybackDialog::OnSelchangeListSmartFile() 
{
	// TODO: Add your control notification handler code here
	int index = m_ctrl_smart.GetCurSel();
	if(index < 0) return ;	
	
	//先停止播放
	stop_play();

	rfid_data_t data;
	if(!m_rfid_list.get_rfid_info(index,&data))
	{
		return;
	}

	TRACE("data.tm=%d\n",data.tm);
	time_t temp_beg = data.tm;
	time_t temp_end = temp_beg + 10;

	struct tm t_beg;
	struct tm t_end;
	gmtime_s(&t_beg,&temp_beg);	
	gmtime_s(&t_end,&temp_end);

	tRecFile file_info;
	memset(&file_info,0,sizeof(file_info));

	file_info.beg.wYear = t_beg.tm_year + 1900;
	file_info.beg.wMonth = t_beg.tm_mon + 1;
	file_info.beg.wDay = t_beg.tm_mday;
	file_info.beg.wHour = t_beg.tm_hour;
	file_info.beg.wMinute = t_beg.tm_min;
	file_info.beg.wSecond = t_beg.tm_sec;
	file_info.end.wYear = t_end.tm_year + 1900;
	file_info.end.wMonth = t_end.tm_mon + 1;
	file_info.end.wDay = t_end.tm_mday;
	file_info.end.wHour = t_end.tm_hour;
	file_info.end.wMinute = t_end.tm_min;
	file_info.end.wSecond = t_end.tm_sec;	
	file_info.type = 0;
	file_info.slot = m_cbo_slot.GetCurSel();
	
	m_play_test = new play_test(g_server_id,file_info);
	if(m_play_test->play(GetDlgItem(IDC_VIDEO)->GetSafeHwnd()))
	{	
		//m_play_test->register_draw_fun(draw_fun,(long)this);
	}else{
		delete m_play_test;
		m_play_test = NULL;
	}
}


void CPlaybackDialog::OnBnClickedBtnDeleteFile()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ctrl_list.GetCurSel();
	if(index < 0) return ;

	tRecFile file_info;
	m_file_list.get_file_info(index,file_info);

	if(!HW_NET_SET_DeleteFile(g_server_id,&file_info))
	{
		MessageBox("删除失败");
	}
}
