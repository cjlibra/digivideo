// DlgNet.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNet dialog


CDlgNet::CDlgNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNet)
	m_port = 0;
	m_phy = _T("");
	m_center_port = 0;
	m_snmp_interval = 1;
	//}}AFX_DATA_INIT
}


void CDlgNet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNet)
	DDX_Control(pDX, IDC_IP_SNMP, m_snmp_ip);
	DDX_Control(pDX, IDC_IPADDRESS6, m_centerip);
	DDX_Control(pDX, IDC_IPADDRESS5, m_mulip);
	DDX_Control(pDX, IDC_IPADDRESS4, m_dns);
	DDX_Control(pDX, IDC_IPADDRESS3, m_gateway);
	DDX_Control(pDX, IDC_IPADDRESS2, m_mask);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDX_Text(pDX, IDC_EDIT3, m_phy);
	DDX_Text(pDX, IDC_EDIT4, m_center_port);
	DDX_Text(pDX, IDC_EDT_SNMP_INTERVAL, m_snmp_interval);
	DDV_MinMaxInt(pDX, m_snmp_interval, 1, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNet, CDialog)
	//{{AFX_MSG_MAP(CDlgNet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNet message handlers

extern USER_HANDLE g_server_id;
extern int g_window_count;

void CDlgNet::close()
{

}

BOOL conver_ip(const char* str,int& field0,int& field1,int& field2,int& field3)
{
	int field[3];
	try
	{
		char* p;
		int i = 0;
		while(p = (char*)strchr(str,'.'))
		{
			*p = '\0';
			field[i] = atoi(str);

			i++;
			str = p + 1;

			if(i >= 3)
			{
				break;
			}
		}

		if(i == 3)
		{
			field0 = field[0];
			field1 = field[1];
			field2 = field[2];
			field3 = atoi(str);
			return TRUE;
		}else{
			return FALSE;
		}
		
	}catch(...){
		return FALSE;
	}
}

BOOL CDlgNet::refresh()
{
	tNetworkSetInfo net_info;
	memset(&net_info,0,sizeof(net_info));
	if(HW_NET_SET_GetNetworkSetting(g_server_id,&net_info) == FALSE)
	{
		MessageBox("获取网络状态失败!");
		return FALSE;
	}
	
	int filed0,field1,field2,field3;
	conver_ip(net_info.sDvrIp,filed0,field1,field2,field3);
	m_ip.SetAddress(filed0,field1,field2,field3);

	m_port = net_info.port;

	conver_ip(net_info.sDvrMaskIp,filed0,field1,field2,field3);
	m_mask.SetAddress(filed0,field1,field2,field3);
	
	m_phy.Format("%02x-%02x-%02x-%02x-%02x-%02x",net_info.byMACAddr[0],net_info.byMACAddr[1],net_info.byMACAddr[2],net_info.byMACAddr[3],
		net_info.byMACAddr[4],net_info.byMACAddr[5]);
	//m_phy = "";//当前该值无效

	conver_ip(net_info.gateway,filed0,field1,field2,field3);
	m_gateway.SetAddress(filed0,field1,field2,field3);

	conver_ip(net_info.dns,filed0,field1,field2,field3);
	m_dns.SetAddress(filed0,field1,field2,field3);

	conver_ip(net_info.sMultiCastIP,filed0,field1,field2,field3);
	m_mulip.SetAddress(filed0,field1,field2,field3);

	//center
	tCenter net_center;
	memset(&net_center,0,sizeof(net_center));
	if(HW_NET_SET_GetCenterInfo(g_server_id,&net_center) == FALSE)
	{
		MessageBox("获取中心设置失败!");		
	}else{
		conver_ip(net_center.ip,filed0,field1,field2,field3);
		m_centerip.SetAddress(filed0,field1,field2,field3);
		m_center_port = net_center.port;
	}

// 	NetSnmpCfg net_snmp;
// 	memset(&net_snmp,0,sizeof(net_snmp));
// 	if(HW_NET_SET_GetSnmp(g_server_id,&net_snmp) == FALSE)
// 	{
// 		MessageBox("获取SNMP设置失败!");		
// 	}else{
// 		conver_ip(net_snmp.ip,filed0,field1,field2,field3);
// 		m_snmp_ip.SetAddress(filed0,field1,field2,field3);
// 		m_snmp_interval= net_snmp.cycle;
// 	}

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgNet::commit()
{
	UpdateData(TRUE);

	tNetworkSetInfo net_info;
	memset(&net_info,0,sizeof(net_info));
	
	CString str;
	unsigned char field0,field1,field2,field3;
	m_ip.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_info.sDvrIp,str.GetBuffer(0));

	net_info.port = m_port;

	m_mask.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_info.sDvrMaskIp,str.GetBuffer(0));

	//strcpy(net_info.byMACAddr,m_phy.GetBuffer(0));

	m_gateway.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_info.gateway,str.GetBuffer(0));

	m_dns.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_info.dns,str.GetBuffer(0));

	m_mulip.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_info.sMultiCastIP,str.GetBuffer(0));
	
	if(HW_NET_SET_SetNetworkSetting(g_server_id,&net_info) == FALSE)
	{
		MessageBox("设置网络失败!");
		return FALSE;
	}

	//center
	tCenter net_center;
	memset(&net_center,0,sizeof(net_center));
	m_centerip.GetAddress(field0,field1,field2,field3);
	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
	strcpy(net_center.ip,str.GetBuffer(0));
	net_center.port = m_center_port;
	if(HW_NET_SET_SetCenterInfo(g_server_id,&net_center) == FALSE)
	{
		MessageBox("设置网络中心失败!");
		return FALSE;
	}

// 	NetSnmpCfg net_snmp;
// 	memset(&net_snmp,0,sizeof(net_snmp));
// 	m_snmp_ip.GetAddress(field0,field1,field2,field3);
// 	str.Format("%d.%d.%d.%d",field0,field1,field2,field3);
// 	strcpy(net_snmp.ip,str.GetBuffer(0));
// 	net_snmp.cycle = m_snmp_interval;
// 	if(HW_NET_SET_SetSnmp(g_server_id,&net_snmp) == FALSE)
// 	{
// 		MessageBox("设置SNMP失败!");
// 		return FALSE;
// 	}
	
	MessageBox("设置成功!");
	return TRUE;
}

BOOL CDlgNet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
