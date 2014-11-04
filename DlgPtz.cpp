// DlgPtz.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgPtz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPtz dialog


CDlgPtz::CDlgPtz(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPtz::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPtz)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPtz::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPtz)
	DDX_Control(pDX, IDC_COMBO13, m_cbo_address);
	DDX_Control(pDX, IDC_COMBO12, m_cbo_protocol);
	DDX_Control(pDX, IDC_COMBO7, m_cbo_flow_control);
	DDX_Control(pDX, IDC_COMBO6, m_cbo_parity);
	DDX_Control(pDX, IDC_COMBO5, m_cbo_stop_bit);
	DDX_Control(pDX, IDC_COMBO4, m_cbo_data_bit);
	DDX_Control(pDX, IDC_COMBO3, m_cbo_bitrate);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_serial_port);
	DDX_Control(pDX, IDC_COMBO11, m_cbo_slot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPtz, CDialog)
	//{{AFX_MSG_MAP(CDlgPtz)
	ON_CBN_SELCHANGE(IDC_COMBO11, OnSelchangeCombo11)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPtz message handlers
extern USER_HANDLE g_server_id;
extern int g_window_count;

BOOL CDlgPtz::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	tPtzRs232Cfg ptz_info;
	memset(&ptz_info,0,sizeof(ptz_info));
	ptz_info.slot = slot;
	if(HW_NET_SET_GetPtzRSConfig(g_server_id,&ptz_info) == FALSE)
	{
		MessageBox("获取云台设置失败!");
		return FALSE;
	}

	m_cbo_serial_port.SetCurSel(ptz_info.rs232_no);
	m_cbo_bitrate.SetCurSel(ptz_info.rate);
	m_cbo_data_bit.SetCurSel(ptz_info.data_bit);
	m_cbo_stop_bit.SetCurSel(ptz_info.stop_bit);
	m_cbo_parity.SetCurSel(ptz_info.parity);
	m_cbo_flow_control.SetCurSel(ptz_info.flow_control);
	m_cbo_protocol.SetCurSel(ptz_info.protocol);
	m_cbo_address.SetCurSel(ptz_info.address);

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgPtz::commit()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	tPtzRs232Cfg ptz_info;
	memset(&ptz_info,0,sizeof(ptz_info));
	ptz_info.slot = slot;
	ptz_info.rs232_no = m_cbo_serial_port.GetCurSel();
	ptz_info.rate = m_cbo_bitrate.GetCurSel();
	ptz_info.data_bit = m_cbo_data_bit.GetCurSel();
	ptz_info.stop_bit = m_cbo_stop_bit.GetCurSel();
	ptz_info.parity = m_cbo_parity.GetCurSel();
	ptz_info.flow_control = m_cbo_flow_control.GetCurSel();
	ptz_info.protocol = m_cbo_protocol.GetCurSel();
	ptz_info.address = m_cbo_address.GetCurSel();
	if(HW_NET_SET_SetPtzRSConfig(g_server_id,&ptz_info) == FALSE)
	{
		MessageBox("设置云台失败!");
		return FALSE;
	}

	MessageBox("设置成功!");

	return TRUE;
}

void CDlgPtz::close()
{

}

BOOL CDlgPtz::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;
	CString str;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d", i + 1);
		m_cbo_slot.AddString(str);
	}

	tDevConfig dev_config;
	memset(&dev_config,0,sizeof(dev_config));
	HW_NET_SET_GetDevConfig(g_server_id,&dev_config);		
	for(i = 0; i < dev_config.rs232_count; i++)
	{
		str.Format("串口%d", i + 1);
		m_cbo_serial_port.AddString(str);
	}
	m_cbo_bitrate.AddString("50");
	m_cbo_bitrate.AddString("75");
	m_cbo_bitrate.AddString("110");
	m_cbo_bitrate.AddString("150");
	m_cbo_bitrate.AddString("300");
	m_cbo_bitrate.AddString("600");
	m_cbo_bitrate.AddString("1200");
	m_cbo_bitrate.AddString("2400");
	m_cbo_bitrate.AddString("4800");
	m_cbo_bitrate.AddString("9600");
	m_cbo_bitrate.AddString("19200");
	m_cbo_bitrate.AddString("38400");
	m_cbo_bitrate.AddString("57600");
	m_cbo_bitrate.AddString("76800");
	m_cbo_bitrate.AddString("115200");
	
	for(i = 0; i < 4; i++)
	{
		str.Format("%d", 5 + i);
		m_cbo_data_bit.AddString(str);
	}
	
	m_cbo_stop_bit.AddString("1位");
	m_cbo_stop_bit.AddString("2位");
	
	m_cbo_parity.AddString("无校验");
	m_cbo_parity.AddString("奇校验");
	m_cbo_parity.AddString("偶校验");
	
	m_cbo_flow_control.AddString("无");
	m_cbo_flow_control.AddString("软流控");
	m_cbo_flow_control.AddString("硬流控");	

	m_cbo_protocol.AddString("PELCO_D");
	m_cbo_protocol.AddString("PELCO_P");
	m_cbo_protocol.AddString("Alec");
	m_cbo_protocol.AddString("YANNAN");

	for(i = 0; i < 255; i++)
	{
		str.Format("%d", i);
		m_cbo_address.AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPtz::OnSelchangeCombo11() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
