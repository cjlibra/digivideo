// DlgChannelSet.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgChannelSet.h"
#include "howellnetsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelSet dialog


CDlgChannelSet::CDlgChannelSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChannelSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChannelSet)
	m_user_name = _T("");
	m_user_password = _T("");
	m_ip = _T("");
	m_port = 0;
	m_remote_slot = 0;
	m_uri = _T("");
	//}}AFX_DATA_INIT
}


void CDlgChannelSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChannelSet)
	DDX_Control(pDX, IDC_CBO_REMOTE_TYPE, m_remote_type);
	DDX_Control(pDX, IDC_COMBO1, m_slot);
	DDX_Text(pDX, IDC_EDIT1, m_user_name);
	DDX_Text(pDX, IDC_EDIT2, m_user_password);
	DDX_Text(pDX, IDC_EDIT3, m_ip);
	DDX_Text(pDX, IDC_EDIT5, m_port);
	DDX_Text(pDX, IDC_EDT_REMOTE_SLOT, m_remote_slot);
	DDV_MinMaxInt(pDX, m_remote_slot, 1, 16);
	DDX_Text(pDX, IDC_EDT_RTSP_URI, m_uri);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChannelSet, CDialog)
	//{{AFX_MSG_MAP(CDlgChannelSet)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelSet message handlers
extern USER_HANDLE g_server_id ;
extern int g_window_count;
BOOL CDlgChannelSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_slot.AddString(str);
	}

	m_remote_type.AddString("皓维");
	m_remote_type.AddString("ONVIF");
	m_remote_type.AddString("RTSP");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChannelSet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CDlgChannelSet::close()
{

}

BOOL CDlgChannelSet::refresh()
{
	int slot = m_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	tChannelSetEx channel_set;
	memset(&channel_set,0,sizeof(channel_set));
	channel_set.slot = slot;
	if(!HW_NET_SET_GetChannelSetEx(g_server_id,&channel_set))
	{
		MessageBox("获取通道设置失败!");
		return FALSE;
	}

	m_ip.Format("%s",channel_set.ipaddress);
	m_user_name.Format("%s",channel_set.username);
	m_user_password.Format("%s",channel_set.password);
	m_port = channel_set.nport;
	m_remote_slot=channel_set.nchannelnum + 1;
	
	if(channel_set.ntype == 0)
	{
		//howell		
		m_uri = "";
		m_remote_type.SetCurSel(0);
	}
	else if(channel_set.ntype == 1)
	{
		//ONVIF		
		m_uri = channel_set.profilename;
		m_remote_type.SetCurSel(1);
	}
	else if (channel_set.ntype == 2)
	{
		//RTSP
		m_uri = channel_set.profilename;
		m_remote_type.SetCurSel(2);
	}
	else
	{
		m_uri = "";
		m_remote_type.SetCurSel(-1);
	}

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgChannelSet::commit()
{
	int slot = m_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	UpdateData(TRUE);

	tChannelSetEx channel_set;
	memset(&channel_set,0,sizeof(channel_set));
	channel_set.slot = slot;
	strcpy(channel_set.username,m_user_name.GetBuffer(0));
	strcpy(channel_set.password,m_user_password.GetBuffer(0));
	strcpy(channel_set.ipaddress,m_ip.GetBuffer(0));
	channel_set.nport = m_port;
	channel_set.nchannelnum = m_remote_slot - 1;	
	
	if(m_remote_type.GetCurSel() == 0)
	{
		channel_set.ntype = 0;			
	}
	else if(m_remote_type.GetCurSel() == 1)
	{
		//ONVIF	
		channel_set.ntype = 1;
		channel_set.busingtcp = 1;
		strcpy(channel_set.profilename,m_uri.GetBuffer(0));
	}
	else if(m_remote_type.GetCurSel() == 2)
	{
		channel_set.ntype = 2;
		channel_set.busingtcp = 1;
		strcpy(channel_set.profilename,m_uri.GetBuffer(0));
	}
	
	if(!HW_NET_SET_SetChannelSetEx(g_server_id,&channel_set))
	{
		MessageBox("设置通道设置失败!");
		return FALSE;
	}

	MessageBox("设置成功,重启后生效!");
	return TRUE;
}

void CDlgChannelSet::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
