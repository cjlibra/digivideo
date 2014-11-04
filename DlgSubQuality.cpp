// DlgSubQuality.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgSubQuality.h"
#include "howellnetsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubQuality dialog


CDlgSubQuality::CDlgSubQuality(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubQuality::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubQuality)
	m_max_bps = 0;
	m_sub_enabled = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgSubQuality::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubQuality)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	DDX_Control(pDX, IDC_COMBO3, m_cbo_quality);
	DDX_Control(pDX, IDC_COMBO4, m_cbo_max_bps);
	DDX_Control(pDX, IDC_COMBO5, m_cbo_encode_type);
	DDX_Control(pDX, IDC_COMBO8, m_cbo_frame);
	DDX_Control(pDX, IDC_COMBO7, m_cbo_bps_type);
	DDX_Text(pDX, IDC_EDT_MAX_BPS, m_max_bps);
	DDX_Check(pDX, IDC_CHECK1, m_sub_enabled);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubQuality, CDialog)
	//{{AFX_MSG_MAP(CDlgSubQuality)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_CLOSE()
		ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
		ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubQuality message handlers
extern USER_HANDLE g_server_id ;
extern int g_window_count;
void CDlgSubQuality::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	close();

	CDialog::OnClose();
}

void CDlgSubQuality::close()
{

}

BOOL CDlgSubQuality::OnInitDialog()
{
	CDialog::OnInitDialog();	

	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_slot.AddString(str);
	}

	m_cbo_quality.AddString("最好");
	m_cbo_quality.AddString("次好");
	m_cbo_quality.AddString("较好");
	m_cbo_quality.AddString("一般");

	m_cbo_max_bps.AddString("不限");
	m_cbo_max_bps.AddString("较小");
	m_cbo_max_bps.AddString("大");
	m_cbo_max_bps.AddString("较大");
	m_cbo_max_bps.AddString("最大");
	m_cbo_max_bps.AddString("自定义");
	
	m_cbo_encode_type.AddString("CIF");
	m_cbo_encode_type.AddString("D1");
	m_cbo_encode_type.AddString("720P");
	m_cbo_encode_type.AddString("1080P");

	m_cbo_bps_type.AddString("定码率");
	m_cbo_bps_type.AddString("变码率");

	m_cbo_frame.AddString("全帧率");
	for(i = 1; i <= 24; i++)
	{
		str.Format("%d",i);
		m_cbo_frame.AddString(str);
	}
	return TRUE;
}

void CDlgSubQuality::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	refresh();
}

BOOL CDlgSubQuality::commit()
{
	UpdateData(TRUE);

	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	struct tSubChannelSet  video_quality;
	memset(&video_quality,0,sizeof(video_quality));
	video_quality.used = m_sub_enabled;
	video_quality.slot = slot;
	video_quality.encodeType = m_cbo_encode_type.GetCurSel();
	if(m_cbo_max_bps.GetCurSel() != m_cbo_max_bps.GetCount() - 1)
	{
		video_quality.maxBps = m_cbo_max_bps.GetCurSel();
	}else{
// 		if(m_max_bps <= 200
// 			|| m_max_bps >= 20000)
// 		{
// 			MessageBox("非法码流");
// 			return FALSE;
// 		}

		video_quality.maxBps = 0x80000000 | m_max_bps;
	}
	
	video_quality.vbr = m_cbo_bps_type.GetCurSel();
	video_quality.framerate = m_cbo_frame.GetCurSel();
	video_quality.qualityLev = m_cbo_quality.GetCurSel();
	if(HW_NET_SET_SetSubChannelSet(g_server_id,&video_quality) == FALSE)
	{
		MessageBox("设置视频图像质量失败!");
		return FALSE;
	}	

	MessageBox("设置成功!");

	return TRUE;
}

BOOL CDlgSubQuality::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	struct tSubChannelSet video_quality;
	memset(&video_quality,0,sizeof(video_quality));
	video_quality.slot = slot;
	if(HW_NET_SET_GetSubChannelSet(g_server_id,&video_quality) == FALSE)
	{
		MessageBox("获取视频图像质量失败!");
		return FALSE;
	}
	
	m_sub_enabled = video_quality.used;

	m_cbo_quality.SetCurSel(video_quality.qualityLev);
	if(video_quality.maxBps & 0x80000000)
	{	
		m_cbo_max_bps.SetCurSel(m_cbo_max_bps.GetCount() - 1);
		m_max_bps = video_quality.maxBps & ~(1 << 31);
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(TRUE);
	}else{
		m_cbo_max_bps.SetCurSel(video_quality.maxBps);
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(FALSE);
	}
	
	m_cbo_encode_type.SetCurSel(video_quality.encodeType);
	//m_cbo_encode_type.EnableWindow(FALSE);
	m_cbo_bps_type.SetCurSel(video_quality.vbr);
	if(video_quality.framerate > 24)
	{
		//remote bug;
		video_quality.framerate = 0;
	}
	m_cbo_frame.SetCurSel(video_quality.framerate);	
	
	UpdateData(FALSE);
	return TRUE;
}

void CDlgSubQuality::OnSelchangeCombo4() 
{
	// TODO: Add your control notification handler code here
	
	if(m_cbo_max_bps.GetCurSel() != m_cbo_max_bps.GetCount() - 1)
	{
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_EDT_MAX_BPS)->EnableWindow(TRUE);
	}
}
