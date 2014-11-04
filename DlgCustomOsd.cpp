// DlgCustomOsd.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgCustomOsd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCustomOsd dialog


CDlgCustomOsd::CDlgCustomOsd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCustomOsd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCustomOsd)
	m_custom_osd_date_show_week = FALSE;
	m_custom_osd_date_enable = FALSE;
	m_custom_osd_date_outline = FALSE;
	m_custom_osd_name_enable = FALSE;
	m_custom_osd_name_outline = FALSE;
	m_custom_osd_date_font = 0;
	m_custom_osd_date_left = 0;
	m_custom_osd_date_top = 0;
	m_custom_osd_name_font = 0;
	m_custom_osd_name_left = 0;
	m_custom_osd_name_row = _T("");
	m_custom_osd_name_top = 0;
	m_custom_osd_name2_enable = FALSE;
	m_custom_osd_name2_outline = FALSE;
	m_custom_osd_name2_font = 0;
	m_custom_osd_name2_left = 0;
	m_custom_osd_name2_top = 0;
	m_custom_osd_name2_row = _T("");
	//}}AFX_DATA_INIT
}


void CDlgCustomOsd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCustomOsd)
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_NAME2_COLOR, m_cbo_custom_osd_name2_color);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_NAME2_ALPHA, m_cbo_custom_osd_name2_alpha);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_STREAM, m_cbo_custom_osd_stream);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_SLOT, m_cbo_custom_osd_slot);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_NAME_COLOR, m_cbo_custom_osd_name_color);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_NAME_ALPHA, m_cbo_custom_osd_name_alpha);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_DATE_MODE, m_cbo_custom_osd_date_mode);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_DATE_COLOR, m_cbo_custom_osd_date_color);
	DDX_Control(pDX, IDC_CBO_CUSTOM_OSD_DATE_ALPHA, m_cbo_custom_osd_date_alpha);
	DDX_Check(pDX, IDC_CHK_CUSTOM_DATE_SHOW_WEEK, m_custom_osd_date_show_week);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_DATE_ENABLE, m_custom_osd_date_enable);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_DATE_OUTLINE, m_custom_osd_date_outline);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_NAME_ENABLE, m_custom_osd_name_enable);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_NAME_OUTLINE, m_custom_osd_name_outline);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_DATE_FONT, m_custom_osd_date_font);
	DDV_MinMaxInt(pDX, m_custom_osd_date_font, 0, 64);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_DATE_LEFT, m_custom_osd_date_left);
	DDV_MinMaxInt(pDX, m_custom_osd_date_left, 0, 703);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_DATE_TOP, m_custom_osd_date_top);
	DDV_MinMaxInt(pDX, m_custom_osd_date_top, 0, 575);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME_FONT, m_custom_osd_name_font);
	DDV_MinMaxInt(pDX, m_custom_osd_name_font, 0, 64);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME_LEFT, m_custom_osd_name_left);
	DDV_MinMaxInt(pDX, m_custom_osd_name_left, 0, 703);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME_ROW, m_custom_osd_name_row);	
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME_TOP, m_custom_osd_name_top);
	DDV_MinMaxInt(pDX, m_custom_osd_name_top, 0, 575);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_NAME2_ENABLE, m_custom_osd_name2_enable);
	DDX_Check(pDX, IDC_CHK_CUSTOM_OSD_NAME2_OUTLINE, m_custom_osd_name2_outline);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME2_FONT, m_custom_osd_name2_font);
	DDV_MinMaxInt(pDX, m_custom_osd_name2_font, 0, 64);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME2_LEFT, m_custom_osd_name2_left);
	DDV_MinMaxInt(pDX, m_custom_osd_name2_left, 0, 703);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME2_TOP, m_custom_osd_name2_top);
	DDV_MinMaxInt(pDX, m_custom_osd_name2_top, 0, 575);
	DDX_Text(pDX, IDC_EDT_CUSTOM_OSD_NAME2_ROW, m_custom_osd_name2_row);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCustomOsd, CDialog)
	//{{AFX_MSG_MAP(CDlgCustomOsd)
	ON_CBN_SELCHANGE(IDC_CBO_CUSTOM_OSD_SLOT, OnSelchangeCboCustomOsdSlot)
	ON_CBN_SELCHANGE(IDC_CBO_CUSTOM_OSD_STREAM, OnSelchangeCboCustomOsdStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCustomOsd message handlers
extern USER_HANDLE g_server_id ;
extern int g_window_count;
BOOL CDlgCustomOsd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_video = NULL;

	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_custom_osd_slot.AddString(str);
	}

	m_cbo_custom_osd_stream.AddString("主码流");
	m_cbo_custom_osd_stream.AddString("子码流");

	m_cbo_custom_osd_date_color.AddString("白");
	m_cbo_custom_osd_date_color.AddString("红");
	m_cbo_custom_osd_date_color.AddString("黑");
	m_cbo_custom_osd_date_color.AddString("蓝");
	m_cbo_custom_osd_date_color.AddString("绿");
	m_cbo_custom_osd_date_color.AddString("黄");
	m_cbo_custom_osd_date_color.AddString("青");	
	m_cbo_custom_osd_name_color.AddString("白");
	m_cbo_custom_osd_name_color.AddString("红");
	m_cbo_custom_osd_name_color.AddString("黑");
	m_cbo_custom_osd_name_color.AddString("蓝");
	m_cbo_custom_osd_name_color.AddString("绿");
	m_cbo_custom_osd_name_color.AddString("黄");
	m_cbo_custom_osd_name_color.AddString("青");
	m_cbo_custom_osd_name2_color.AddString("白");
	m_cbo_custom_osd_name2_color.AddString("红");
	m_cbo_custom_osd_name2_color.AddString("黑");
	m_cbo_custom_osd_name2_color.AddString("蓝");
	m_cbo_custom_osd_name2_color.AddString("绿");
	m_cbo_custom_osd_name2_color.AddString("黄");
	m_cbo_custom_osd_name2_color.AddString("青");

	for(i = 0; i <= 255;i++)
	{
		str.Format("%d\n",i);
		m_cbo_custom_osd_date_alpha.AddString(str);
		m_cbo_custom_osd_name_alpha.AddString(str);
		m_cbo_custom_osd_name2_alpha.AddString(str);
	}

	m_cbo_custom_osd_date_mode.AddString("YYYY-MM-DD HH:MM:SS");
	m_cbo_custom_osd_date_mode.AddString("MM-DD-YYYY HH:MM:SS");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgCustomOsd::refresh()
{
	int slot = m_cbo_custom_osd_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	
	int stream = m_cbo_custom_osd_stream.GetCurSel();
	if(stream < 0 || stream > 1)
	{
		return FALSE;
	}
	
	connect_video();

	//osd date
	net_custom_osd_date_t osd_date;
	memset(&osd_date,0,sizeof(osd_date));
	osd_date.slot = slot;
	osd_date.stream = stream;
	if(HW_NET_SET_GetCustomOsdDate(g_server_id,&osd_date) == FALSE)
	{
		TRACE("GetCustomOsdDate Failed\n");
		return FALSE;
	}
	m_cbo_custom_osd_date_alpha.SetCurSel(osd_date.aplha);
	m_cbo_custom_osd_date_color.SetCurSel(osd_date.color);
	m_cbo_custom_osd_date_mode.SetCurSel(osd_date.mode);
	m_custom_osd_date_font = osd_date.font_size;
	m_custom_osd_date_left = osd_date.left;
	m_custom_osd_date_top = osd_date.top;
	m_custom_osd_date_outline = osd_date.outline;
	m_custom_osd_date_show_week = osd_date.show_week;
	m_custom_osd_date_enable = osd_date.enable;

	//osd name
	net_custom_osd_name_t osd_name;
	osd_name.slot = slot;
	osd_name.stream = stream;
	if(HW_NET_SET_GetCustomOsdName(g_server_id,&osd_name) == FALSE)
	{
		TRACE("GetCustomOsdName Failed\n");
		return FALSE;
	}
	m_cbo_custom_osd_name_alpha.SetCurSel(osd_name.aplha);
	m_cbo_custom_osd_name_color.SetCurSel(osd_name.color);
	m_custom_osd_name_enable = osd_name.enable;
	m_custom_osd_name_font = osd_name.font_size;
	m_custom_osd_name_left = osd_name.left;
	m_custom_osd_name_top = osd_name.top;
	m_custom_osd_name_row = osd_name.name;
	m_custom_osd_name_outline = osd_name.outline;

	//osd custom
	net_custom_osd_name_t osd_custom;
	osd_custom.slot = slot;
	osd_custom.stream = stream;
	if(HW_NET_SET_GetCustomOsdName2(g_server_id,&osd_custom) == FALSE)
	{
		TRACE("GetCustomOsdName2 Failed\n");
		return FALSE;
	}
	m_cbo_custom_osd_name2_alpha.SetCurSel(osd_custom.aplha);
	m_cbo_custom_osd_name2_color.SetCurSel(osd_custom.color);
	m_custom_osd_name2_enable = osd_custom.enable;
	m_custom_osd_name2_font = osd_custom.font_size;
	m_custom_osd_name2_left = osd_custom.left;
	m_custom_osd_name2_top = osd_custom.top;
	m_custom_osd_name2_row = osd_custom.name;
	m_custom_osd_name2_outline = osd_custom.outline;
	
	//更新
	UpdateData(FALSE);
	
	return TRUE;
}

void CDlgCustomOsd::close()
{
	close_video();
}

void CDlgCustomOsd::close_video()
{
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
}

BOOL CDlgCustomOsd::connect_video()
{
	int slot = m_cbo_custom_osd_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	

	int stream = m_cbo_custom_osd_stream.GetCurSel();
	if(stream < 0)
	{
		return FALSE;
	}
	
	if(m_video)
	{
		delete m_video;
		m_video = NULL;
	}
	
	m_video = new net_video_test(g_server_id,slot);
	m_video->start_preview(GetDlgItem(IDC_CUSTOM_OSD_VIDEO)->GetSafeHwnd(),0,stream);
	return TRUE;
}

BOOL CDlgCustomOsd::commit()
{
	UpdateData(TRUE);
	
	int slot = m_cbo_custom_osd_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}	
	
	int stream = m_cbo_custom_osd_stream.GetCurSel();
	if(stream < 0)
	{
		return FALSE;
	}

	//设置日期
	net_custom_osd_date_t osd_date;
	memset(&osd_date,0,sizeof(osd_date));
	osd_date.slot = slot;
	osd_date.stream = stream;
	osd_date.enable = m_custom_osd_date_enable;
	osd_date.color = m_cbo_custom_osd_date_color.GetCurSel();
	osd_date.font_size = m_custom_osd_date_font;
	osd_date.outline = m_custom_osd_date_outline;
	osd_date.aplha = m_cbo_custom_osd_date_alpha.GetCurSel();
	osd_date.show_week = m_custom_osd_date_show_week;
	osd_date.mode = m_cbo_custom_osd_date_mode.GetCurSel();
	osd_date.left = m_custom_osd_date_left;
	osd_date.top = m_custom_osd_date_top;
	if(HW_NET_SET_SetCustomOsdDate(g_server_id,&osd_date) == FALSE)
	{
		MessageBox("设置OSD日期失败");
		return FALSE;
	}

	//设置通道
	net_custom_osd_name_t osd_name;
	memset(&osd_name,0,sizeof(osd_name));
	osd_name.slot = slot;
	osd_name.stream = stream;
	osd_name.enable = m_custom_osd_name_enable;
	osd_name.color = m_cbo_custom_osd_name_color.GetCurSel();
	osd_name.font_size = m_custom_osd_name_font;
	osd_name.outline = m_custom_osd_name_outline;
	osd_name.aplha = m_cbo_custom_osd_name_alpha.GetCurSel();
	osd_name.left = m_custom_osd_name_left;
	osd_name.top = m_custom_osd_name_top;
	strcpy(osd_name.name,m_custom_osd_name_row.GetBuffer(0));
	if(HW_NET_SET_SetCustomOsdName(g_server_id,&osd_name) == FALSE)
	{
		MessageBox("设置OSD名称失败");
		return FALSE;
	}

	//设置osd custom
	net_custom_osd_name_t osd_custom;
	memset(&osd_custom,0,sizeof(osd_custom));
	osd_custom.slot = slot;
	osd_custom.stream = stream;
	osd_custom.enable = m_custom_osd_name2_enable;
	osd_custom.color = m_cbo_custom_osd_name2_color.GetCurSel();
	osd_custom.font_size = m_custom_osd_name2_font;
	osd_custom.outline = m_custom_osd_name2_outline;
	osd_custom.aplha = m_cbo_custom_osd_name2_alpha.GetCurSel();
	osd_custom.left = m_custom_osd_name2_left;
	osd_custom.top = m_custom_osd_name2_top;
	strcpy(osd_custom.name,m_custom_osd_name2_row.GetBuffer(0));
	if(HW_NET_SET_SetCustomOsdName2(g_server_id,&osd_custom) == FALSE)
	{
		MessageBox("设置OSD名称失败");
		return FALSE;
	}

	MessageBox("设置成功!");
	
	return TRUE;
}

void CDlgCustomOsd::OnSelchangeCboCustomOsdSlot() 
{
	// TODO: Add your control notification handler code here

	m_cbo_custom_osd_stream.SetCurSel(0);

	refresh();
}

void CDlgCustomOsd::OnSelchangeCboCustomOsdStream() 
{
	// TODO: Add your control notification handler code here
	
	refresh();
}
