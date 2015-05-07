
// net_demoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "net_demoDlg.h"
#include "PlaybackDialog.h"
#include "DlgRemoteSet.h"
#include "DlgSearchDevice.h"

#include "hwsadp.h" 
#pragma comment(lib,"hwsadp.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
USER_HANDLE g_server_id = INVALID_HANDLE;
int g_window_count = 0;
long g_server_version = 0;
char g_server_ip[32];
#define TIMER_HEARTBEAR 2
#define TIMER_STREAMINFO 3

//zoom_video_test zoom_test;
net_video_test* test = NULL;
static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser);
UINT64 g_recved_len = 0;
UINT64 g_total_len = 0;

rfid_data_t g_rfid_data;

// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMyButton 消息处理程序

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CButton::OnLButtonDown(nFlags, point);
	WPARAM wparam = (((DWORD)WM_LBUTTONDOWN<<16) + (DWORD)GetDlgCtrlID());
	::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND,wparam,0);
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CButton::OnLButtonUp(nFlags, point);
	WPARAM wparam = (((DWORD)WM_LBUTTONUP<<16) + (DWORD)GetDlgCtrlID());
	::SendMessage(this->GetParent()->GetSafeHwnd(),WM_COMMAND,wparam,0);
	
}

auto_zoom::auto_zoom()
{
	memset(&m_zoom_rt,0,sizeof(m_zoom_rt));
	m_bmousedown = false;
}

auto_zoom::~auto_zoom()
{
}

void auto_zoom::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(test)
	{
		m_ori = point;
		m_zoom_rt.left = point.x;
		m_zoom_rt.right = point.x;
		m_zoom_rt.top = point.y;
		m_zoom_rt.bottom = point.y;
		m_bmousedown = true;
	}
}

void auto_zoom::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bmousedown)
	{		
		m_zoom_rt.left = point.x > m_ori.x ? m_ori.x:point.x;
		m_zoom_rt.right = m_zoom_rt.left + abs(point.x - m_ori.x);
		m_zoom_rt.top = point.y > m_ori.y ? m_ori.y:point.y;
		m_zoom_rt.bottom = m_zoom_rt.top + abs(point.y - m_ori.y);		
	}
}

RECT auto_zoom::get_zoom_rt()
{
	return m_zoom_rt;
}

void auto_zoom::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bmousedown)
	{
		int video_w,video_h;
		if(test->get_video_size(video_w,video_h))
		{		
			RECT rt;
			GetClientRect(&rt);
			int pic_w = rt.right;
			int pic_h = rt.bottom;
			
			//将窗口坐标转成实际视频的坐标
			RECT real_rt;
			real_rt.left = m_zoom_rt.left * video_w / pic_w;
			real_rt.right = m_zoom_rt.right * video_w / pic_w;
			real_rt.top = m_zoom_rt.top * video_h / pic_h;
			real_rt.bottom = m_zoom_rt.bottom * video_h / pic_h;
			
			//将实际的视频坐标转成 704 * 576比例的
			RECT protocol_rt;
			protocol_rt.left = real_rt.left * 704 / video_w;
			protocol_rt.right = real_rt.right * 704 / video_w;
			protocol_rt.top = real_rt.top * 576 / video_h;
			protocol_rt.bottom = real_rt.bottom * 576 / video_h;

			//if(protocol_rt.right - protocol_rt.left > 10
			//	&& protocol_rt.bottom - protocol_rt.top > 10)
			{
				//发送点击放大协议
				tPtzControl ptzcontrol;
				ptzcontrol.slot = 0;
				ptzcontrol.controlType = 2;
				ptzcontrol.cmd = 1;
				ptzcontrol.rect = protocol_rt;
				TRACE("left %d,top %d,right %d,bottom %d\n",protocol_rt.left,protocol_rt.top,protocol_rt.right,protocol_rt.bottom);
				HW_NET_SET_ControlPTZ(test->user_handle(),&ptzcontrol);
			}			
		}
	}
	m_bmousedown = false;
}

void auto_zoom::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!test)
	{
		return;
	}


	static bool full_size = false;	
	static CWnd* parent = NULL;	
	static WINDOWPLACEMENT old_pos;

	if(!full_size)
	{		
		//全屏	
		full_size = true;		
		GetWindowPlacement(&old_pos);	
		parent = GetParent();		
		SetParent(NULL);		
		int cx=GetSystemMetrics(SM_CXSCREEN);
		int cy=GetSystemMetrics(SM_CYSCREEN);				
		SetForegroundWindow();
		Sleep(10);
		SetWindowPos(&wndTopMost, 0, 0, cx , cy , NULL);		
	}else{
		//原来的位置
		full_size = false;
		SetParent(parent);
		SetWindowPlacement(&old_pos);			
	}
}

BEGIN_MESSAGE_MAP(auto_zoom, CEdit)
ON_WM_SYSCOMMAND()
ON_WM_MOUSEMOVE()	
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

zoom_video_test:: zoom_video_test()
{
	m_btest  = false;
}

zoom_video_test:: ~zoom_video_test()
{
	stop_test();
}

bool zoom_video_test::start_test(PLAY_HANDLE handle,RECT rt)
{
	if(m_btest) return false;
	m_handle = handle;
	m_rt = rt;

	RECT rt_client;
	GetClientRect(&rt_client);

	hwplay_get_video_size(m_handle,&m_real_video_w,&m_real_video_h);

	//m_video_h = (rt_client.right - rt_client.left) * m_real_video_h / (m_real_video_w);
	m_video_h = rt_client.bottom - rt_client.top;
	m_video_w = (rt_client.right - rt_client.left);//只测试720p的时候，如果是d1的话，有可能不成立

	m_back_w = m_video_w;
	m_back_h = 0;
	//m_back_h = (rt_client.bottom - rt_client.top - m_video_h) / 2;

	m_fx = (float)m_real_video_w / (float)m_video_w;
	m_fy = (float)m_real_video_h / (float)m_video_h;

	m_zoom = 2; //默认放大2倍
	m_btest = true;
	return true;
}

void zoom_video_test::stop_test()
{
	m_btest = false;
	hwplay_zoom_rect(m_handle,FALSE,NULL,NULL);
}

BOOL zoom_video_test::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{		
	if(!m_btest) return FALSE;	

	if(zDelta > 0)
	{
		m_zoom -= 0.2;
		if(m_zoom < 0.2)
		{
			m_zoom = 0.2;
		}
	}else{
		m_zoom += 0.2;
		if(m_zoom > 100)
		{
			m_zoom = 100;
		}
	}

	TRACE("m_zoom = %f\n",m_zoom);

	OnMouseMove(nFlags,m_point);

	return TRUE;
}

void zoom_video_test::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	if(!m_btest && test)
	{
		RECT rt;
		rt.left = rt.top = 0;
		rt.right = rt.bottom = 300;
		start_test(test->handle(),rt);

		OnMouseMove(nFlags, point);
	}

}

void zoom_video_test::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_btest)
	{
		stop_test();
	}
}

void zoom_video_test::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_btest)
	{
		stop_test();
	}

	static bool full_size = false;	
	static CWnd* parent = NULL;	
	static WINDOWPLACEMENT old_pos;
	
	if(!full_size)
	{		
		//全屏	
		full_size = true;		
		GetWindowPlacement(&old_pos);	
		parent = GetParent();		
		SetParent(NULL);		
		int cx=GetSystemMetrics(SM_CXSCREEN);
		int cy=GetSystemMetrics(SM_CYSCREEN);				
		SetForegroundWindow();
		Sleep(10);
		SetWindowPos(&wndTopMost, 0, 0, cx , cy , NULL);		
	}else{
		//原来的位置
		full_size = false;
		SetParent(parent);
		SetWindowPlacement(&old_pos);			
	}
}

void zoom_video_test::OnMouseMove(UINT nFlags, CPoint point)
{		
	if(!m_btest) return ;
	if(point.y <= m_back_h)
	{
		point.y = m_back_h;
	}

	if(point.y >= m_back_h + m_video_h)
	{
		point.y =  m_back_h + m_video_h;
	}	

	m_point = point;

	POINT pt;//实际在视频中的点
	pt.x = m_fx * (float)point.x;
	pt.y = m_fy * (float)(point.y - m_back_h);

	//计算在原始视频中的位置
	int rect_w,rect_h;
	rect_w = (m_rt.right - m_rt.left) * m_fx ;
	rect_h = (m_rt.bottom - m_rt.top) * m_fy ;

	//计算放大后的位置	
	rect_w = (float)rect_w / m_zoom;
	rect_h = (float)rect_h / m_zoom;

	if(rect_w > m_real_video_w) rect_w = m_real_video_w;
	if(rect_h > m_real_video_h) rect_h = m_real_video_h;

	TRACE("rect w = %d,h = %d\n",rect_w,rect_h);

	RECT src;
	src.left = pt.x - rect_w / 2;
	src.top = pt.y - rect_h / 2;
	if(src.left < 0) 
	{
		src.left = 0;
	}
	if(src.top < 0) 
	{
		src .top = 0;
	}

	src.right = src.left + rect_w;
	src.bottom = src.top + rect_h;
	if(src.right > m_real_video_w)
	{
		src.right = m_real_video_w;
		src.left = m_real_video_w - rect_w;
	}
	if(src.bottom > m_real_video_h)
	{
		src.bottom = m_real_video_h;
		src.top = m_real_video_h - rect_h;
	}	

	rect_w = (m_rt.right - m_rt.left);
	rect_h = (m_rt.bottom - m_rt.top);
	RECT dst;
	dst.left = point.x - rect_w / 2;
	dst.top = point.y - rect_h / 2;
	if(dst.left < 0)
	{
		dst.left = 0;
	}
	if(dst.top < 0)
	{
		dst.top = 0;
	}
	dst.right = dst.left + rect_w;
	dst.bottom = dst.top + rect_h;

	if(dst.right > m_video_w)
	{
		dst.right = m_video_w;
		dst.left = m_video_w - rect_w;
	}

	if(dst.bottom > m_video_h + m_back_h* 2)
	{
		dst.bottom = m_video_h + m_back_h * 2;
		dst.top = dst.bottom - rect_h;
	}	

	hwplay_zoom_rect(m_handle,TRUE,&dst,&src);
}

BEGIN_MESSAGE_MAP(zoom_video_test, CEdit)
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEMOVE()	
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//}}AFX_VIRTUAL

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Cnet_demoDlg 对话框




Cnet_demoDlg::Cnet_demoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cnet_demoDlg::IDD, pParent)	
{
	//{{AFX_DATA_INIT(Cnet_demoDlg)
	m_color_adjust_enable = FALSE;
	m_cur_yuv_buf = 0;
	m_audio_preview_enable = FALSE;
	m_enable_alarm = FALSE;
	m_voice_enable = FALSE;
	m_com_addr = 0;
	m_blacklighting_enable = FALSE;
	m_serial_send = FALSE;
	m_sharpen_enable = FALSE;
	m_enable_noise_filter = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cnet_demoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cnet_demoDlg)
	DDX_Control(pDX, IDC_SLD_SHARPEN, m_sharpen);
	DDX_Control(pDX, IDC_CBO_COM_PRESET, m_cbo_com_preset);
	DDX_Control(pDX, IDC_CBO_COM_SPEED, m_cbo_com_speed);
	DDX_Control(pDX, IDC_CBO_BLACKLIGHTING, m_cbo_blacklighting);
	DDX_Control(pDX, IDC_CBO_CONNECTMODE, m_cbo_connect_mode);
	DDX_Control(pDX, IDC_LST_ALARM, m_list_alarm);
	DDX_Control(pDX, IDC_CBO_SCALE, m_cbo_scale);
	DDX_Control(pDX, IDC_CBO_YUV_BUF, m_cbo_yuv_buf);
	DDX_Control(pDX, IDC_SLD_HUE, m_hue);
	DDX_Control(pDX, IDC_SLD_SATURATION, m_saturation);
	DDX_Control(pDX, IDC_SLD_CONTRAST, m_contrast);
	DDX_Control(pDX, IDC_SLD_BRIGHT, m_bright);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);	
	DDX_Control(pDX, IDC_COMBO1, m_slots);
	DDX_Control(pDX, IDC_COMBO2, m_svr_type);
	DDX_Check(pDX, IDC_CHK_COLOR, m_color_adjust_enable);
	DDX_Text(pDX, IDC_EDT_CUR_YUV_BUF, m_cur_yuv_buf);
	DDX_Check(pDX, IDC_CHK_AUDIO_PREVIEW, m_audio_preview_enable);
	DDX_Check(pDX, IDC_CHK_REGALARM, m_enable_alarm);
	DDX_Check(pDX, IDC_VOICE, m_voice_enable);
	DDX_Text(pDX, IDC_EDT_COM_ADDR, m_com_addr);
	DDV_MinMaxInt(pDX, m_com_addr, 0, 255);
	DDX_Check(pDX, IDC_CHK_BLACKLIGHTING, m_blacklighting_enable);
	DDX_Check(pDX, IDC_CHK_SERIAL_SEND, m_serial_send);
	DDX_Check(pDX, IDC_CHK_SHARPEN_ENABLE, m_sharpen_enable);
	DDX_Check(pDX, IDC_CHK_AUDIO_ENHANCE, m_enable_noise_filter);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Cnet_demoDlg, CDialog)
	//{{AFX_MSG_MAP(Cnet_demoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_BTLOGIN, OnBnClickedBtlogin)
	ON_BN_CLICKED(IDC_BTCONNECT, OnBnClickedBtconnect)
	ON_WM_CLOSE()
ON_WM_MOUSEWHEEL()
ON_BN_CLICKED(IDC_BUTTON_PLAYBACK, OnBnClickedButtonPlayback)
ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SET2, OnButtonSet2)
	ON_BN_CLICKED(IDC_CHK_COLOR, OnChkColor)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_CBO_YUV_BUF, OnSelchangeCboYuvBuf)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SNAP_BMP, OnBtnSnapBmp)
	ON_BN_CLICKED(IDC_BTN_SNAP_JPG, OnBtnSnapJpg)
	ON_BN_CLICKED(IDC_BTN_START_REC, OnBtnStartRec)
	ON_BN_CLICKED(IDC_BTN_STOP_REC, OnBtnStopRec)
	ON_BN_CLICKED(IDC_CHK_AUDIO_PREVIEW, OnChkAudioPreview)
	ON_BN_CLICKED(IDC_BTCONNECT_SUB, OnBtconnectSub)
	ON_CBN_SELCHANGE(IDC_CBO_SCALE, OnSelchangeCboScale)
	ON_BN_CLICKED(IDC_CHK_REGALARM, OnChkRegalarm)
	ON_BN_CLICKED(IDC_VOICE, OnVoice)	
	ON_BN_CLICKED(IDC_CHK_BLACKLIGHTING, OnChkBlacklighting)
	ON_CBN_SELCHANGE(IDC_CBO_BLACKLIGHTING, OnSelchangeCboBlacklighting)
	ON_BN_CLICKED(IDC_CHK_SERIAL_SEND, OnChkSerialSend)	
	ON_BN_CLICKED(IDC_CBO_COM_PRESET_EXEC, OnCboComPresetExec)
	ON_BN_CLICKED(IDC_CBO_COM_PRESET_SETUP, OnCboComPresetSetup)
	ON_BN_CLICKED(IDC_CHK_SHARPEN_ENABLE, OnChkSharpenEnable)	
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BTN_REMOTE_START_REC, OnBtnRemoteStartRec)
	ON_BN_CLICKED(IDC_BTN_REMOTE_STOP_REC, OnBtnRemoteStopRec)
	ON_BN_CLICKED(IDC_BTCONNECT2, OnBtconnect2)
	ON_BN_CLICKED(IDC_CHK_AUDIO_ENHANCE, OnChkAudioEnhance)
	ON_BN_CLICKED(IDC_BTN_REMOTE_ENCODE_ROTATE, OnBtnRemoteEncodeRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Cnet_demoDlg 消息处理程序
extern void CALLBACK HandleDataCallBack(struct ServerInfo* psinfo,int result);
static CListCtrl* g_list;

void CALLBACK alarm_process(USER_HANDLE lUserID,int type,char *buf,int len,long nUser)
{
	struct tAlarmData alarm_data;
	tAlarmMotionDataEx alarm_data_ex;
	net_rfid_alarm_t rfid_alarm;

	/*if(type == HW_ALARM_RFID )
	{
	memcpy(&rfid_alarm,buf,len);
	}else if(type != HW_ALARM_MOTIONEX)
	{
	memcpy(&alarm_data,buf,len);
	}else{
	memcpy(&alarm_data_ex,buf,len);
	}*/

	SYSTEMTIME time;	
	GetLocalTime(&time);
	CString str;
	str.Format("%02d:%02d:%02d",time.wHour,time.wMinute,time.wSecond);
	
	if(g_list->GetItemCount() >= 100)
	{
		g_list->DeleteAllItems();
	}
	
	//int index = g_list->GetItemCount();
	int index = 0;
	g_list->InsertItem(index,str);
	g_list->SetItemText(index,1,g_server_ip);		

	switch(type)
	{
	case HW_ALARM_DSP:
		{
			memcpy(&alarm_data,buf,len);
			str.Format("Dsp %d 错误 ",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_HARD:	
		{							
			memcpy(&alarm_data,buf,len);
			str.Format("硬盘%d 错误 ",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_MOTION:
		{				
			memcpy(&alarm_data,buf,len);
			str.Format("通道 %d 移动侦测报警",alarm_data.value + 1);			
		}
		break;
	case HW_ALARM_MOTIONEX:
		{		
			memcpy(&alarm_data_ex,buf,len);
			str.Format("通道 %d 移动侦测报警",alarm_data_ex.slot + 1);
		}
		break;
	case HW_ALARM_VIDEO_LOST:
		{				
			memcpy(&alarm_data,buf,len);
			str.Format("通道 %d 视频丢失",alarm_data.value + 1);			
		}
		break;
	case HW_ALARM_STARTREC:
		{	
			memcpy(&alarm_data,buf,len);
			str.Format("通道 %d 启动录像",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_STOPREC:
		{			
			memcpy(&alarm_data,buf,len);
			str.Format("通道 %d 停止录像",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_MASK:
		{	
			memcpy(&alarm_data,buf,len);
			str.Format("通道 %d 遮挡报警",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_IN:
		{			
			memcpy(&alarm_data,buf,len);
			str.Format("报警器 %d 报警",alarm_data.value + 1);
		}
		break;
	case HW_ALARM_HEARTBEAT:
		{			
			memcpy(&alarm_data,buf,len);
			str.Format("心跳信息");
		}
		break;
		
	case HW_ALARM_SLOT_NODEFINE:
		{
			memcpy(&alarm_data,buf,len);
			str.Format("NVR 通道 %d 未定义",alarm_data.value + 1);
		}
		break;

	case HW_ALARM_SLOT_LOST:
		{
			memcpy(&alarm_data,buf,len);
			str.Format("NVR 通道 %d 未连接",alarm_data.value + 1);
		}
		break;

	case HW_ALARM_ANALYZE:
		{
			memcpy(&alarm_data,buf,len);
			if(alarm_data.status == 1)
			{
				//场景变化报警
				str.Format("通道 %d 场景变化",alarm_data.value + 1);
			}
			else if(alarm_data.status == 2)
			{
				//场景遮挡报警
				str.Format("通道 %d 场景遮挡",alarm_data.value + 1);
			}
			else if(alarm_data.status == 4)
			{
				//焦距变化报警
				str.Format("通道 %d 场景遮挡",alarm_data.value + 1);
			}
		}
		break;

	case HW_ALARM_RFID:
	{		
		memcpy(&rfid_alarm,buf,len);
		str.Format("通道 %d rfid报警,data= ",rfid_alarm.slot + 1);
		for(int i = 0; i < rfid_alarm.len; i++)
		{
			CString tmp;
			tmp.Format("%02x ",(unsigned char)rfid_alarm.buf[i]);
			str.Append(tmp);
		}
		break;
	}
		
	default:
		{
			str.Format("未知报警信息,type = %d",type);
			break;
		}
	}
	g_list->SetItemText(index,2,str);
}


BOOL Cnet_demoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ip.SetAddress(192,168,3,2);
	UpdateData(FALSE);
	HW_NET_Init(5198);	
	HW_NET_SetUdpBasePort(8000);
	
	//search device
#if 0
	hwsadp_init();
	hwsadp_settype(0);
	hwsadp_sethandle(HandleDataCallBack);
	hwsadp_start();
#endif

#ifdef AUTO_ZOOM
	auto_zoom_test.SubclassDlgItem(IDC_VIDEO,this);
#else
	zoom_test.SubclassDlgItem(IDC_VIDEO,this);
#endif
	//zoom_test.EnableWindow(FALSE);

	char buf[255];
	for(int i = 0; i < 100; i++)
	{
		sprintf(buf,"%d",i+1);
		m_cbo_yuv_buf.AddString(buf);
	}

	m_cbo_scale.AddString("整个窗口");
	m_cbo_scale.AddString("4:3");
	m_cbo_scale.AddString("16:9");

	m_list_alarm.InsertColumn(0,"时间",LVCFMT_LEFT,70);
	m_list_alarm.InsertColumn(1,"IP地址",LVCFMT_LEFT,125);	
	m_list_alarm.InsertColumn(2,"报警信息",LVCFMT_LEFT,600);
	g_list = &m_list_alarm;
	
	m_list_alarm.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
	HW_NET_SET_SetAlarmCallbackEx( alarm_process,0);

	m_cbo_connect_mode.AddString("TCP");
	m_cbo_connect_mode.AddString("UDP");
	m_cbo_connect_mode.SetCurSel(0);

	m_com_addr = 1;

	for(int i = 0; i <= 64; i++)
	{
		sprintf(buf,"%d", i);
		m_cbo_com_speed.AddString(buf);
	}
	m_cbo_com_speed.SetCurSel(32);

	for(int i = 0; i <= 255; i++)
	{
		sprintf(buf,"%d", i);
		m_cbo_com_preset.AddString(buf);
	}
	m_cbo_com_preset.SetCurSel(0);

	SetTimer(1,1000,NULL);

	for(int i = 0; i <= 100;i++)
	{
		sprintf(buf,"%d",i);
		m_cbo_blacklighting.AddString(buf);
	}
	m_cbo_blacklighting.SetCurSel(2);

	//m_serial_send = TRUE;//默认用透明通道发送

	m_btn_left.SubclassDlgItem(IDC_BTN_COM_LEFT,this);
	m_btn_right.SubclassDlgItem(IDC_BTN_COM_RIGHT,this);
	m_btn_up.SubclassDlgItem(IDC_BTN_COM_UP,this);
	m_btn_down.SubclassDlgItem(IDC_BTN_COM_DOWN,this);
	m_btn_far.SubclassDlgItem(IDC_BTN_COM_FAR,this);
	m_btn_near.SubclassDlgItem(IDC_BTN_COM_NEAR,this);
	m_btn_zoomin.SubclassDlgItem(IDC_BTN_COM_ZOOM_IN,this);
	m_btn_zoomout.SubclassDlgItem(IDC_BTN_COM_ZOOM_OUT,this);

	//hwplay_init_draw(TRUE);

	SetTimer(TIMER_STREAMINFO,1000,NULL);
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cnet_demoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cnet_demoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cnet_demoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cnet_demoDlg::OnBnClickedBtlogin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);

	if(g_server_id != INVALID_HANDLE)
	{
		KillTimer(TIMER_HEARTBEAR);
		HW_NET_Logout(g_server_id);
		g_server_id = INVALID_HANDLE;
	}

	if(g_server_id == INVALID_HANDLE)
	{
		char ip[255];
		BYTE f0,f1,f2,f3;
		m_ip.GetAddress(f0,f1,f2,f3);
		sprintf(ip,"%d.%d.%d.%d",f0,f1,f2,f3);
		sprintf(g_server_ip,"%d.%d.%d.%d",f0,f1,f2,f3);
		g_server_id = HW_NET_Login(ip,5198,"admin","12345");
		if(g_server_id != INVALID_HANDLE)
		{
			m_slots.ResetContent();	
			g_window_count = HW_NET_GetWindowCount(g_server_id);			
			char str[255];
			for(int i = 0; i < g_window_count; i++)
			{
				sprintf(str,"通道%d",i + 1);
				m_slots.AddString(str);
			}
			m_slots.SetCurSel(0);
			
			m_svr_type.ResetContent();	
			
			HW_NET_SET_GetDvrVersion(g_server_id,&g_server_version);

			if(VERSION_IPCAM(g_server_version)){
				m_svr_type.AddString("ip camera");
			}else if(VERSION_NVR(g_server_version)){
				m_svr_type.AddString("nvr");
			}else if(VERSION_HIS(g_server_version)
				|| VERSION_HIS_RAILWAY(g_server_version)){
				m_svr_type.AddString("his");
			}
			m_svr_type.SetCurSel(0);

			HW_NET_SET_RegisterAlarm(g_server_id,m_enable_alarm);

			//SetTimer(TIMER_HEARTBEAR,5000,NULL);

			AfxMessageBox("登录成功!");
		}
		else
		{
			AfxMessageBox("登录失败!");
		}
	}
}


void Cnet_demoDlg::OnBnClickedBtconnect() //连接主码流
{
	// TODO: 在此添加控件通知处理程序代码
	if(test)
	{
		delete test;
		test = NULL;
	}
	
	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,m_slots.GetCurSel());
		test->start_preview(GetDlgItem(IDC_VIDEO)->GetSafeHwnd(),m_cbo_connect_mode.GetCurSel());
		test->register_draw(draw_fun,(long)this);
		
		m_color_adjust_enable = FALSE;
		m_bright.SetPos(0);
		m_contrast.SetPos(0);
		m_saturation.SetPos(0);
		m_hue.SetPos(0);

		int yuv_buf;
		test->get_max_yuv_buf(yuv_buf);
		m_cbo_yuv_buf.SetCurSel(yuv_buf - 1);

		m_cbo_scale.SetCurSel(0);
		
		UpdateData(FALSE);
	}
}

void Cnet_demoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
	if(test)
	{
		delete test;
	}

	if(g_server_id)
	{
		HW_NET_Logout(g_server_id);
	}

	HW_NET_Release();

#if 0
	hwsadp_stop();
	hwsadp_release();
#endif
}

BOOL Cnet_demoDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
#ifndef AUTO_ZOOM
	zoom_test.OnMouseWheel(nFlags,zDelta,pt);
#endif

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void Cnet_demoDlg::OnBnClickedButtonPlayback()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}

// 	long version;
// 	HW_NET_SET_GetDvrVersion(g_server_id,&version);
// 	if(VERSION_IPCAM(version))
// 	{
// 		MessageBox("ip 摄像机没有录像文件!");
// 		return;
// 	}

	if(test)
	{
		delete test;
		test = NULL;
	}

	CPlaybackDialog dlg;
	dlg.DoModal();
}

void Cnet_demoDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if(test)
	{
		delete test;
		test = NULL;
	}

	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}

	CDlgRemoteSet dlg_set(this);
	dlg_set.DoModal();
}

static void CALLBACK draw_fun(PLAY_HANDLE handle,HDC hDc,LONG nUser)
{
	CDC* dc = CDC::FromHandle(hDc);
	dc->SetTextColor(RGB(255,0,0));		
	dc->SetBkMode(TRANSPARENT);	

	Cnet_demoDlg* dlg = (Cnet_demoDlg*)nUser;

	
#if 0
#ifdef AUTO_ZOOM
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("在窗口中画框来启动点击放大",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);

	dc->SelectStockObject(NULL_BRUSH);
	CPen newPen(0,1,RGB(0,0,255));
	dc->SelectObject(&newPen);	
	rt = dlg->auto_zoom_test.get_zoom_rt();
	dc->Rectangle(&rt);
#else
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	dc->DrawText("鼠标左键启动图像缩放,右键关闭缩放，滚轮控制放大倍速,双击窗口/全屏切换!",&rt,DT_SINGLELINE|DT_LEFT|DT_NOCLIP);
#endif
#endif

	//显示数据
	RECT rt;
	CWnd* pWnd = CWnd::FromHandle(test->window_handle());
	pWnd->GetClientRect(&rt);
	if(g_recved_len > 0)
	{
		CString str;
		str.Format("%uKbps / %uKbps",(unsigned long)g_recved_len / 1024 * 8,(unsigned long)g_total_len / 1024 * 8);
		dc->DrawText(str,&rt,DT_SINGLELINE|DT_BOTTOM|DT_NOCLIP);
	}

	if(g_rfid_data.len > 0
		&& (time(NULL) - g_rfid_data.tm) < 3)
	{		
		rt.top = rt.bottom - 20;
		CString str;
		for(int i = 0; i < g_rfid_data.len; i++)
		{
			CString tmp;
			tmp.Format("%02x",g_rfid_data.buf[i]);
			str = str + " " + tmp;
		}
		dc->DrawText(str.GetBuffer(0),&rt,DT_BOTTOM | DT_LEFT);	
	}


	CDC::DeleteTempMap();
}

void Cnet_demoDlg::OnButtonSet2() 
{
	// TODO: Add your control notification handler code here
	CDlgSearchDevice dlg_search;
	dlg_search.DoModal();
}

void Cnet_demoDlg::OnChkColor() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(test)
	{
		test->enable_color_adjust(m_color_adjust_enable ? true : false);
		int bright,contrast,saturation,hue;
		test->get_color(bright,contrast,saturation,hue);
		m_bright.SetPos(bright);
		m_contrast.SetPos(contrast);
		m_saturation.SetPos(saturation);
		m_hue.SetPos(hue);

		CString str;
		str.Format("%d",bright);
		GetDlgItem(IDC_TXT_BRIGHT)->SetWindowText(str);

		str.Format("%d",contrast);
		GetDlgItem(IDC_TXT_CONTRAST)->SetWindowText(str);

		str.Format("%d",saturation);
		GetDlgItem(IDC_TXT_SATURATION)->SetWindowText(str);

		str.Format("%d",hue);
		GetDlgItem(IDC_TXT_HUE)->SetWindowText(str);
	}
}

void Cnet_demoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
	{
	case IDC_SLD_BRIGHT:
	case IDC_SLD_CONTRAST:
	case IDC_SLD_SATURATION:
	case IDC_SLD_HUE:
		{
			if(test)
			{
				int bright = m_bright.GetPos();
				int contrast = m_contrast.GetPos();
				int saturation = m_saturation.GetPos();
				int hue = m_hue.GetPos();
				test->set_color(bright,contrast,saturation,hue);
			}

			CString str;
			str.Format("%d",m_bright.GetPos());
			GetDlgItem(IDC_TXT_BRIGHT)->SetWindowText(str);
			
			str.Format("%d",m_contrast.GetPos());
			GetDlgItem(IDC_TXT_CONTRAST)->SetWindowText(str);
			
			str.Format("%d",m_saturation.GetPos());
			GetDlgItem(IDC_TXT_SATURATION)->SetWindowText(str);
			
			str.Format("%d",m_hue.GetPos());
			GetDlgItem(IDC_TXT_HUE)->SetWindowText(str);
			
			break;
		}
	case IDC_SLD_SHARPEN:
		{
			if(test)
			{				
				test->set_sharpen_value(m_sharpen.GetPos());
			}
			CString str;
			str.Format("%d",m_sharpen.GetPos());
			GetDlgItem(IDC_TXT_SHARPEN)->SetWindowText(str);
			break;
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void Cnet_demoDlg::OnSelchangeCboYuvBuf() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		test->set_max_yuv_buf(m_cbo_yuv_buf.GetCurSel() + 1);
	}
}

void Cnet_demoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		if(test)
		{
			int cur_yuv_buf;		
			test->get_cur_yuv_buf(cur_yuv_buf);
			char s[255];
			sprintf(s,"%d",cur_yuv_buf);
			GetDlgItem(IDC_EDT_CUR_YUV_BUF)->SetWindowText(s);
			//UpdateData(FALSE);
		}
	}

	if(nIDEvent == TIMER_HEARTBEAR)
	{	
		long misec;
		if(!HW_NET_SET_GetNetState(g_server_id,&misec))
		{
			TRACE("Net State ERROR\n");
			//网络状态异常				
			HW_NET_Logout(g_server_id);	
			g_server_id = INVALID_HANDLE;			
		}
		if(g_server_id == INVALID_HANDLE)
		{
			g_server_id = HW_NET_Login(g_server_ip,5198,"admin","12345");
			if(g_server_id != INVALID_HANDLE)
			{
				HW_NET_SET_RegisterAlarm(g_server_id,m_enable_alarm);			
			}		
		}
		
		if(g_server_id != INVALID_HANDLE)
		{
			if(test)
			{
				if(test->is_preview())
				{
					if(!test->state_ok())
					{
						TRACE("Video State ERROR\n");
						test->stop_preview();
					}
				}
				
				if(!test->is_preview())
				{
					test->start_preview(GetDlgItem(IDC_VIDEO)->GetSafeHwnd(),m_cbo_connect_mode.GetCurSel());
					test->register_draw(draw_fun,(long)this);
				}
			}
		}		
	}

	if(nIDEvent == TIMER_STREAMINFO)
	{		
	}

	CDialog::OnTimer(nIDEvent);
}

void Cnet_demoDlg::OnBtnSnapBmp() 
{
	// TODO: Add your control notification handler code here
	if(test == NULL)
	{
		return;
	}

	CFileDialog dlg(FALSE, 
		_T("bmp"),
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||"), this);
	if(dlg.DoModal() == IDCANCEL)
	{
		return ;
	}		

	test->save_to_bmp(dlg.GetPathName());
}

void Cnet_demoDlg::OnBtnSnapJpg() 
{
	// TODO: Add your control notification handler code here
	if(test == NULL)
	{
		return;
	}
	
	CFileDialog dlg(FALSE, 
		_T("jpg"),
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文件(*.jpg)|*.jpg|所有文件(*.*)|*.*||"), this);
	if(dlg.DoModal() == IDCANCEL)
	{
		return ;
	}		
	
	test->save_to_jpg(dlg.GetPathName(),90);
}

void Cnet_demoDlg::OnBtnStartRec() 
{
	// TODO: Add your control notification handler code here
	if(test == NULL)
	{
		return;
	}
	
#if SAVE_TS_FILE
	CFileDialog dlg(FALSE, 
		_T("ts"),
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文件(*.ts)|*.ts|所有文件(*.*)|*.*||"), this);
#else
#if SAVE_FLV_FILE
	CFileDialog dlg(FALSE, 
		_T("flv"),
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文件(*.flv)|*.flv|所有文件(*.*)|*.*||"), this);
#else
	CFileDialog dlg(FALSE, 
		_T("mp4"),
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("文件(*.mp4)|*.mp4|所有文件(*.*)|*.*||"), this);
#endif
#endif
	if(dlg.DoModal() == IDCANCEL)
	{
		return ;
	}		
	
	test->save_to_file(dlg.GetPathName());
}

void Cnet_demoDlg::OnBtnStopRec() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		test->stop_save_to_file();
	}
}

void Cnet_demoDlg::OnChkAudioPreview() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(test)
	{
		test->enable_audio_preview(m_audio_preview_enable ? true : false);
	}
}


void Cnet_demoDlg::OnBtconnectSub() //连接子码流
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		delete test;
		test = NULL;
	}
	
	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,m_slots.GetCurSel());
		test->start_preview(GetDlgItem(IDC_VIDEO)->GetSafeHwnd(),m_cbo_connect_mode.GetCurSel(),1);
		test->register_draw(draw_fun,(long)test);
		
		m_color_adjust_enable = FALSE;
		m_bright.SetPos(0);
		m_contrast.SetPos(0);
		m_saturation.SetPos(0);
		m_hue.SetPos(0);
		
		int yuv_buf;
		test->get_max_yuv_buf(yuv_buf);
		m_cbo_yuv_buf.SetCurSel(yuv_buf - 1);

		m_cbo_scale.SetCurSel(0);
		
		UpdateData(FALSE);
	}
}

void Cnet_demoDlg::OnSelchangeCboScale() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		int sel = m_cbo_scale.GetCurSel();
		switch(sel)
		{
		case  0:
			{
				test->set_scale("unknown");
				break;
			}

		case 1:
			{
				test->set_scale("4:3");
				break;
			}

		case 2:
			{
				test->set_scale("16:9");
				break;
			}
		}
	}
}

void Cnet_demoDlg::OnChkRegalarm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(g_server_id != INVALID_HANDLE)
	{
		HW_NET_SET_RegisterAlarm(g_server_id,m_enable_alarm);	
	}
}

void Cnet_demoDlg::OnVoice() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
 
	if(g_server_id != INVALID_HANDLE)
	{
		if(m_voice_enable)
		{
				HW_NET_SET_StartVoiceComEx(g_server_id,m_slots.GetCurSel(),1);
		}else{
				HW_NET_SET_StopVoiceCom(g_server_id);
		}	
	}
}

void Cnet_demoDlg::OnBtnComUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}

	if(m_serial_send)
	{
		//透明通道发送		
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 3;
		data [4] = 0;
		data [5] = m_cbo_com_speed.GetCurSel();
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;		
		rs232send.rs232_no = rs232_no;			
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);
	}
	else
	{
		//网络协议发送		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));		
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 0;//direct
		ptz_control.cmd = 8;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}	
}

void Cnet_demoDlg::OnBtnComLeft() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//透明通道	
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 2;
		data [4] = m_cbo_com_speed.GetCurSel();
		data [5] = 0;
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);
	}
	else
	{
		//网络协议发送		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 0;//direct
		ptz_control.cmd = 4;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnBtnComRight() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//先设置波特率	
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 1;
		data [4] = m_cbo_com_speed.GetCurSel();
		data [5] = 0;
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);
	}
	else
	{
		//网络协议发送		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 0;//direct
		ptz_control.cmd = 6;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
	
}

void Cnet_demoDlg::OnBtnComDown() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 4;
		data [4] = 0;
		data [5] = m_cbo_com_speed.GetCurSel();
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);	
	}
	else
	{		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 0;//direct
		ptz_control.cmd = 2;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnBtnComStop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	if(m_serial_send)
	{		
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 0;
		data [4] = 0;
		data [5] = 0;
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);	
	}
	else
	{		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 0;//direct
		ptz_control.cmd = 5;
		ptz_control.presetno = 0;
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}	
}

void Cnet_demoDlg::OnBtnComZoomOut() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{		
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 6;
		data [4] = 0;
		data [5] = 0;
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);	
	}
	else
	{
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 1;//zoom
		ptz_control.cmd = 4;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnBtnComZoomIn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		int rs232_no = VERSION_NVR(g_server_version) ? m_slots.GetCurSel() : 0;
		long com_id = HW_NET_SET_StartRS232Send(g_server_id,rs232_no);
		if(com_id == INVALID_HANDLE)
		{
			MessageBox("服务器打开串口失败!");
			return;
		}
		
		//向透明通道发送pelcod 协议
		BYTE data[7];
		data[0] = 0xff;
		data[1] = m_com_addr;
		data[2] = 0;
		
		data[3] = 1 << 5;
		data [4] = 0;
		data [5] = 0;
		data[6] = (data[1] + data[2] + data[3] + data[4] + data[5]) % 256;
		
		tRs232Send rs232send;
		rs232send.rs232_no = rs232_no;
		rs232send.protocol = 0;
		rs232send.len = 7;
		memcpy(rs232send.data,data,7);
		
		if(HW_NET_SET_SendRS232Data(com_id,&rs232send) == FALSE)
		{
			MessageBox("发送数据失败!");
		}
		
		HW_NET_SET_StopRS232Send(com_id);	
	}
	else
	{
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 1;//zoom
		ptz_control.cmd = 3;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnChkBlacklighting() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(test)
	{
		test->enable_blacklighting(m_blacklighting_enable ? true : false);
		int value;
		test->get_blacklighting_value(value);
		m_cbo_blacklighting.SetCurSel(value);
	}
}

void Cnet_demoDlg::OnSelchangeCboBlacklighting() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		test->set_blacklighting_value(m_cbo_blacklighting.GetCurSel());
	}
}

void Cnet_demoDlg::OnChkSerialSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void Cnet_demoDlg::OnBtnComNear() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//请用户自己根据PELCOD协议发送数据
	}
	else
	{
		//网络协议发送		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 1;//zoom
		ptz_control.cmd = 6;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnBtnComFar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//请用户自己根据PELCOD协议发送数据
	}
	else
	{
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 1;//zoom
		ptz_control.cmd = 5;
		ptz_control.presetno = m_cbo_com_speed.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnCboComPresetExec() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//请用户自己根据PELCOD协议发送数据
	}
	else
	{		
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 3;//preset
		ptz_control.cmd = 3;
		ptz_control.presetno = m_cbo_com_preset.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

void Cnet_demoDlg::OnCboComPresetSetup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(g_server_id == INVALID_HANDLE)
	{
		MessageBox("请先登录!");
		return;
	}
	
	if(m_serial_send)
	{
		//请用户自己根据PELCOD协议发送数据
	}
	else
	{				
		struct tPtzControl ptz_control;
		memset(&ptz_control,0,sizeof(ptz_control));
		ptz_control.slot = m_slots.GetCurSel();
		ptz_control.controlType = 3;//preset
		ptz_control.cmd = 1;
		ptz_control.presetno = m_cbo_com_preset.GetCurSel();
		if(!HW_NET_SET_ControlPTZ(g_server_id,&ptz_control))
		{
			MessageBox("发送云台控制命令失败");
			return;
		}
	}
}

BOOL Cnet_demoDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//if(LOWORD(WPARAM))
	switch(LOWORD(wParam))
	{
	case IDC_BTN_COM_LEFT:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnleft down\n");
				OnBtnComLeft();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnleft up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_RIGHT:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnright down\n");
				OnBtnComRight();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnright up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_UP:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnup down\n");
				OnBtnComUp();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnup up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_DOWN:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btndown down\n");
				OnBtnComDown();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btndown up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_FAR:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnfar down\n");
				OnBtnComFar();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnfar up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_NEAR:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnnear down\n");
				OnBtnComNear();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnnear up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_ZOOM_IN:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnzoomin down\n");
				OnBtnComZoomIn();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnzoomin up\n");
				OnBtnComStop();
			}
			break;
		}

	case IDC_BTN_COM_ZOOM_OUT:
		{
			if(HIWORD(wParam) == WM_LBUTTONDOWN)
			{
				TRACE("on btnzoomout down\n");
				OnBtnComZoomOut();
			}
			else if(HIWORD(wParam) == WM_LBUTTONUP)
			{
				TRACE("on btnzoomout up\n");
				OnBtnComStop();
			}
			break;
		}
	}

	return CDialog::OnCommand(wParam,lParam);
}

void Cnet_demoDlg::OnChkSharpenEnable() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(test)
	{
		test->enable_sharpen(m_sharpen_enable ? true : false);	

		int value;
		test->get_sharpen_value(value);
		m_sharpen.SetPos(value);		

		CString str;
		str.Format("%d",value);
		GetDlgItem(IDC_TXT_SHARPEN)->SetWindowText(str);	
	}
}

void Cnet_demoDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		test->set_volume(0xffff);
	}
}

void Cnet_demoDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		test->set_volume(0xffff0000);
	}
}

void Cnet_demoDlg::OnBtnRemoteStartRec() 
{
	// TODO: Add your control notification handler code here
	if(g_server_id != INVALID_HANDLE)
	{
		HW_NET_SET_StartRecord(g_server_id,m_slots.GetCurSel());
	}
}

void Cnet_demoDlg::OnBtnRemoteStopRec() 
{
	// TODO: Add your control notification handler code here
	if(g_server_id != INVALID_HANDLE)
	{
		HW_NET_SET_StopRecord(g_server_id,m_slots.GetCurSel());
	}
}

void Cnet_demoDlg::OnBtconnect2() 
{
	// TODO: Add your control notification handler code here
	if(test)
	{
		delete test;
		test = NULL;
	}
	
	if(g_server_id != INVALID_HANDLE)
	{
		test = new net_video_test(g_server_id,m_slots.GetCurSel());
		test->start_nvr_preview(GetDlgItem(IDC_VIDEO)->GetSafeHwnd());
		test->register_draw(draw_fun,(long)this);
		
		m_color_adjust_enable = FALSE;
		m_bright.SetPos(0);
		m_contrast.SetPos(0);
		m_saturation.SetPos(0);
		m_hue.SetPos(0);
		
		int yuv_buf;
		test->get_max_yuv_buf(yuv_buf);
		m_cbo_yuv_buf.SetCurSel(yuv_buf - 1);
		
		m_cbo_scale.SetCurSel(0);
		
		UpdateData(FALSE);
	}
}

void Cnet_demoDlg::OnChkAudioEnhance() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(g_server_id != -1)
	{
		net_audio_enhance_t audio_enhance;
		memset(&audio_enhance,0,sizeof(audio_enhance));
		audio_enhance.slot = m_slots.GetCurSel();
		if(HW_NET_SET_GetAudioEnhance(g_server_id,&audio_enhance) == FALSE)
		{
			MessageBox("获取音频加强信息失败");
			return;
		}

		audio_enhance.enable_noise_filter = m_enable_noise_filter;

		if(HW_NET_SET_SetAudioEnhance(g_server_id,&audio_enhance) == FALSE)
		{
			MessageBox("设置音频加强信息失败");
			return;
		}
		
	}
}

void Cnet_demoDlg::OnBtnRemoteEncodeRotate() 
{
	// TODO: Add your control notification handler code here
	
	if(g_server_id != -1)
	{
		net_encode_rotate_t rotate;
		memset(&rotate,0,sizeof(rotate));
		rotate.slot = m_slots.GetCurSel();

		if(HW_NET_SET_GetEncodeRotate(g_server_id,&rotate) == FALSE)
		{
			MessageBox("获取旋转信息失败");
			return;
		}

		if(rotate.clock_wise)
		{
			rotate.clock_wise = 0;
		}
		else
		{
			rotate.clock_wise = 1;
		}

		if(HW_NET_SET_SetEncodeRotate(g_server_id,&rotate) == FALSE)
		{
			MessageBox("设置旋转信息失败");
			return;
		}
	}
}
