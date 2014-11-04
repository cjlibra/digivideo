// DlgDeviceInfo.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgDeviceInfo.h"
#include "howellnetsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceInfo dialog

extern USER_HANDLE g_server_id ;
CDlgDeviceInfo::CDlgDeviceInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDeviceInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDeviceInfo)
	m_serial_no = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDeviceInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDeviceInfo)
	DDX_Text(pDX, IDC_EDT_SERIAL_NO, m_serial_no);
	DDV_MaxChars(pDX, m_serial_no, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDeviceInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgDeviceInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDeviceInfo message handlers

BOOL CDlgDeviceInfo::refresh()
{
	tDevConfig dev_config;
	memset(&dev_config,0,sizeof(dev_config));
	if(HW_NET_SET_GetDevConfig(g_server_id,&dev_config) == FALSE)
	{
		MessageBox("获取设备信息失败");
		return FALSE;
	}

	m_serial_no.Format("%s",dev_config.devSerialID);
	UpdateData(FALSE);

	return TRUE;
}

BOOL CDlgDeviceInfo::commit()
{
	UpdateData(TRUE);

	if(HW_NET_SetSerialID(g_server_id,m_serial_no.GetBuffer(0)))
	{	
		MessageBox("保持ID成功，重启后生效!");
		return TRUE;
	}	
		MessageBox("保持ID失败!");
	return FALSE;
}

void CDlgDeviceInfo::close()
{

}


