// DlgSearchDevice.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgSearchDevice.h"
#include "protocol_type.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchDevice dialog

static CListBox* g_lst_box;
void CALLBACK HandleDataCallBack(struct ServerInfo* psinfo,int result)
{
	CString str;
	if(VERSION_NVR(psinfo->dev_type))
	{
		str.Format("NVR　ip %s,    mask %s,    port %d,    mac %s",		
			psinfo->IP,		
			psinfo->subnetmask,
			psinfo->port,
		psinfo->MAC);
	}else if (VERSION_IPCAM(psinfo->dev_type)){
		str.Format("IP CAMERA　ip %s,    mask %s,    port %d,    mac %s",		
			psinfo->IP,		
			psinfo->subnetmask,
			psinfo->port,
		psinfo->MAC);
	}else if(VERSION_HIS(psinfo->dev_type)){
		str.Format("dvs　ip %s,    mask %s,    port %d,    mac %s",		
			psinfo->IP,		
			psinfo->subnetmask,
			psinfo->port,
		psinfo->MAC);
	}
	else{
		str.Format("unknown　ip %s,    mask %s,    port %d,    mac %s",		
			psinfo->IP,		
			psinfo->subnetmask,
			psinfo->port,
		psinfo->MAC);
	}
	
	
	g_lst_box->AddString(str);	
}

CDlgSearchDevice::CDlgSearchDevice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSearchDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSearchDevice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSearchDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSearchDevice)
	DDX_Control(pDX, IDC_LST_DEVICE, m_lst_device);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSearchDevice, CDialog)
	//{{AFX_MSG_MAP(CDlgSearchDevice)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSearchDevice message handlers

BOOL CDlgSearchDevice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_lst_box = &m_lst_device;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSearchDevice::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	while(m_lst_device.GetCount())
	{
		m_lst_device.DeleteString(0);
	}
	
	hwsadp_inquiry();
}
