// DlgOsd.cpp : 实现文件
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgOsd.h"

// CDlgOsd 对话框

IMPLEMENT_DYNAMIC(CDlgOsd, CDialog)

CDlgOsd::CDlgOsd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOsd::IDD, pParent)	
	, m_name_x(0)
	, m_name_y(0)
	, m_name(_T(""))
	, m_name_enable(FALSE)
	, m_date_y(0)
	, m_date_x(0)
	, m_date_enable(FALSE)
	, m_date_week_enable(FALSE)
{
	//{{AFX_DATA_INIT(CDlgOsd)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

CDlgOsd::~CDlgOsd()
{
}

void CDlgOsd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgOsd)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_chn);	
	DDX_Text(pDX, IDC_EDIT1, m_name_x);
	DDX_Text(pDX, IDC_EDIT6, m_name_y);
	DDX_Text(pDX, IDC_EDIT3, m_name);
	DDX_Check(pDX, IDC_CHECK1, m_name_enable);
	DDX_Text(pDX, IDC_EDIT7, m_date_y);
	DDX_Text(pDX, IDC_EDIT4, m_date_x);
	DDX_Check(pDX, IDC_CHECK2, m_date_enable);
	DDX_Check(pDX, IDC_CHECK3, m_date_week_enable);
	DDX_Control(pDX, IDC_SLD_BRIGHT, m_sld_bright);
	DDX_Control(pDX, IDC_SLD_CONTRAST, m_sld_contrast);
	DDX_Control(pDX, IDC_SLD_SATURATION, m_sld_saturation);
	DDX_Control(pDX, IDC_SLD_HUE, m_sld_hue);
	//}}AFX_MSG_MAP
}


BEGIN_MESSAGE_MAP(CDlgOsd, CDialog)
	//{{AFX_MSG_MAP(CDlgOsd)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgOsd 消息处理程序
extern USER_HANDLE g_server_id ;
extern int g_window_count;
BOOL CDlgOsd::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sld_bright.SetRange(0,255);
	m_sld_contrast.SetRange(0,127);
	m_sld_saturation.SetRange(0,127);
	m_sld_hue.SetRange(0,255);	
	
	m_video = NULL;

	CString str;
	for(int i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d\n",i + 1);
		m_cbo_chn.AddString(str);
	}

	return TRUE;
}

void CDlgOsd::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	refresh();
}

BOOL CDlgOsd::refresh()
{
	int slot = m_cbo_chn.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	

	connect_video();
	
	//色彩
	struct tVideoColor video_color;
	memset(&video_color,0,sizeof(video_color));
	video_color.slot = slot;
	if(HW_NET_SET_GetVideoColor(g_server_id,&video_color) == FALSE)
	{
		TRACE("get video color failed\n");
		return FALSE;
	}
	m_sld_bright.SetPos(video_color.brightness);
	m_sld_contrast.SetPos(video_color.contrast);
	m_sld_saturation.SetPos(video_color.saturation);
	m_sld_hue.SetPos(video_color.hue);

	//osd日期
	struct tOsdDate osd_date;
	memset(&osd_date,0,sizeof(osd_date));
	osd_date.slot = slot;
	if(HW_NET_SET_GetOsdDate(g_server_id,&osd_date) == FALSE)
	{
		TRACE("get osd date failed\n");
		return FALSE;
	}
	m_date_x = osd_date.left;
	m_date_y = osd_date.top;
	m_date_enable = osd_date.isEnable;
	m_date_week_enable = osd_date.isShowWeek;

	//osd通道名称
	struct tOsdChannel osd_name;
	memset(&osd_name,0,sizeof(osd_name));
	osd_name.slot = slot;
	if(HW_NET_SET_GetChannelName(g_server_id,&osd_name) == FALSE)
	{
		TRACE("get osd name failed\n");
		return FALSE;
	}

	m_name_x = osd_name.left;
	m_name_y = osd_name.top;
	m_name_enable = osd_name.isEnable;
	m_name.Format("%s",osd_name.name);

	//更新
	UpdateData(FALSE);

	return TRUE;
}

void CDlgOsd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(GetWindowLong(pScrollBar->m_hWnd,GWL_ID))
	{
	case IDC_SLD_BRIGHT:
	case IDC_SLD_CONTRAST:
	case IDC_SLD_SATURATION:
	case IDC_SLD_HUE:
		if(nSBCode == SB_ENDSCROLL)
		{
			int slot = m_cbo_chn.GetCurSel();
			if(slot < 0 || slot >= g_window_count)
			{
				break;
			}
			int bright = m_sld_bright.GetPos();
			int contrast = m_sld_contrast.GetPos();
			int saturation = m_sld_saturation.GetPos();
			int hue = m_sld_hue.GetPos();

			struct tVideoColor video_color;
			memset(&video_color,0,sizeof(video_color));
			video_color.slot = slot;
			video_color.brightness = bright;
			video_color.contrast = contrast;
			video_color.saturation = saturation;
			video_color.hue = hue;
			if(HW_NET_SET_SetVideoColor(g_server_id,&video_color) == FALSE)
			{
				TRACE("set video color failed\n");
				bright;
			}
		}
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgOsd::close()
{
	close_video();
}

void CDlgOsd::close_video()
{
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
}

BOOL CDlgOsd::connect_video()
{
	int slot = m_cbo_chn.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	

	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}

	m_video = new net_video_test(g_server_id,slot);
	m_video->start_preview(GetDlgItem(IDC_VIDEO)->GetSafeHwnd(),0);
	return TRUE;
}

void CDlgOsd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	close();

	CDialog::OnClose();
}

BOOL CDlgOsd::commit()
{
	UpdateData(TRUE);

	int slot = m_cbo_chn.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	

	//osd date,3511不支持对日期的设置，但是返回成功
	struct tOsdDate osd_date;
	memset(&osd_date,0,sizeof(osd_date));
	osd_date.slot = slot;
	osd_date.isEnable = m_date_enable;
	osd_date.isShowWeek = m_date_week_enable;
	osd_date.left = m_date_x;
	osd_date.top = m_date_y;
	if(HW_NET_SET_SetOsdDate(g_server_id,&osd_date) == FALSE)
	{
		MessageBox("设置osd日期失败!");
		return FALSE;
	}

	//osd name 
	struct tOsdChannel osd_name;
	memset(&osd_name,0,sizeof(osd_name));
	osd_name.slot = slot;
	osd_name.left = m_name_x;
	osd_name.top = m_name_y;
	osd_name.isEnable = m_name_enable;//该设置现在无效
	strncpy(osd_name.name,m_name.GetBuffer(0),32);
	if(HW_NET_SET_SetChannelName(g_server_id,&osd_name) == FALSE)
	{
		MessageBox("设置通道名称失败!");
		return FALSE;			 
	}

	MessageBox("设置成功!");

	return TRUE;
}
