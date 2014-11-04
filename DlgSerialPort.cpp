// DlgSerialPort.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgSerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialPort dialog
CDlgSerialPort::CDlgSerialPort(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSerialPort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSerialPort)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSerialPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSerialPort)
	DDX_Control(pDX, IDC_COMBO8, m_work_mode);
	DDX_Control(pDX, IDC_COMBO7, m_flow_control);
	DDX_Control(pDX, IDC_COMBO6, m_parity);
	DDX_Control(pDX, IDC_COMBO5, m_stop_bit);
	DDX_Control(pDX, IDC_COMBO4, m_data_bit);
	DDX_Control(pDX, IDC_COMBO3, m_bitrate);
	DDX_Control(pDX, IDC_COMBO1, m_serial_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSerialPort, CDialog)
	//{{AFX_MSG_MAP(CDlgSerialPort)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSerialPort message handlers
extern USER_HANDLE g_server_id;
extern long g_server_version;
extern int g_window_count;
BOOL CDlgSerialPort::refresh()
{
	int serial_port = m_serial_port.GetCurSel();
	if(serial_port < 0 || serial_port >= m_serial_port_count)
	{
		return FALSE;
	}

	tRs232Cfg serial_port_info;
	memset(&serial_port_info,0,sizeof(serial_port_info));
	serial_port_info.rs232_no = serial_port;

	if(HW_NET_SET_GetRS232Config(g_server_id,&serial_port_info) == FALSE)
	{
		return FALSE;
	}

	m_bitrate.SetCurSel(serial_port_info.rate);
	m_data_bit.SetCurSel(serial_port_info.data_bit);
	m_stop_bit.SetCurSel(serial_port_info.stop_bit);
	m_parity.SetCurSel(serial_port_info.parity);
	m_flow_control.SetCurSel(serial_port_info.flow_control);
	m_work_mode.SetCurSel(serial_port_info.work_mode);

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgSerialPort::commit()
{
	int serial_prot = m_serial_port.GetCurSel();
	if(serial_prot < 0 || serial_prot >= m_serial_port_count)
	{
		return FALSE;
	}

	UpdateData(TRUE);

	tRs232Cfg serial_port_info;
	memset(&serial_port_info,0,sizeof(serial_port_info));
	serial_port_info.rs232_no = serial_prot;
	serial_port_info.rate = m_bitrate.GetCurSel();
	serial_port_info.data_bit = m_data_bit.GetCurSel();
	serial_port_info.stop_bit = m_stop_bit.GetCurSel();
	serial_port_info.parity = m_parity.GetCurSel();
	serial_port_info.flow_control = m_flow_control.GetCurSel();
	serial_port_info.work_mode = m_work_mode.GetCurSel();

	if(HW_NET_SET_SetRS232Config(g_server_id,&serial_port_info) == FALSE)
	{
		MessageBox("设置串口失败!");
		return FALSE;
	}

	MessageBox("设置成功!");
	return TRUE;
}

void CDlgSerialPort::close()
{

}

BOOL CDlgSerialPort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	tDevConfig dev_config;
	memset(&dev_config,0,sizeof(dev_config));
	HW_NET_SET_GetDevConfig(g_server_id,&dev_config);
	m_serial_port_count = dev_config.rs232_count;	

	int i;
	CString str;
	for(i = 0; i < m_serial_port_count; i++)
	{
		str.Format("串口%d", i + 1);
		m_serial_port.AddString(str);
	}
	m_bitrate.AddString("50");
	m_bitrate.AddString("75");
	m_bitrate.AddString("110");
	m_bitrate.AddString("150");
	m_bitrate.AddString("300");
	m_bitrate.AddString("600");
	m_bitrate.AddString("1200");
	m_bitrate.AddString("2400");
	m_bitrate.AddString("4800");
	m_bitrate.AddString("9600");
	m_bitrate.AddString("19200");
	m_bitrate.AddString("38400");
	m_bitrate.AddString("57600");
	m_bitrate.AddString("76800");
	m_bitrate.AddString("115200");

	for(i = 0; i < 4; i++)
	{
		str.Format("%d", 5 + i);
		m_data_bit.AddString(str);
	}

	m_stop_bit.AddString("1位");
	m_stop_bit.AddString("2位");

	m_parity.AddString("无校验");
	m_parity.AddString("奇校验");
	m_parity.AddString("偶校验");

	m_flow_control.AddString("无");
	m_flow_control.AddString("软流控");
	m_flow_control.AddString("硬流控");

	m_work_mode.AddString("云台");
	m_work_mode.AddString("报警输入");
	m_work_mode.AddString("透明通道");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSerialPort::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
