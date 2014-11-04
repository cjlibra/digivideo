// DlgRecordCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgRecordCtrl.h"
#include "howellnetsdk.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordCtrl dialog

extern USER_HANDLE g_server_id ;
extern int g_window_count;
CDlgRecordCtrl::CDlgRecordCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecordCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecordCtrl)
	m_record_ctrl = -1;
	//}}AFX_DATA_INIT
}


void CDlgRecordCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecordCtrl)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	DDX_Radio(pDX, IDC_RADIO1, m_record_ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecordCtrl, CDialog)
	//{{AFX_MSG_MAP(CDlgRecordCtrl)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecordCtrl message handlers

BOOL CDlgRecordCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_slot.AddString(str);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgRecordCtrl::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	net_record_ctrl_t net_record;
	net_record.slot = slot;
	
	if(!HW_NET_SET_GetRecordCtrl(g_server_id,&net_record))
	{
		MessageBox("获取录像控制失败!");
		return FALSE;
	}
	
	m_record_ctrl = net_record.mode;
	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgRecordCtrl::commit()
{
	UpdateData(TRUE);
	
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	net_record_ctrl_t net_record;
	memset(&net_record,0,sizeof(net_record));
	net_record.slot = slot;
	net_record.mode = m_record_ctrl;
	net_record.stream = 0;	
	
	if(!HW_NET_SET_SetRecordCtrl(g_server_id,&net_record))
	{
		MessageBox("设置录像控制失败!");
		return FALSE;
	}
	
	MessageBox("设置录像控制成功");
	
	return TRUE;
}

void CDlgRecordCtrl::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

void CDlgRecordCtrl::close()
{

}
