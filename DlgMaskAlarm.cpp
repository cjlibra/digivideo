// DlgMaskAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgMaskAlarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskAlarm dialog

CDlgMaskAlarm::CDlgMaskAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaskAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaskAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMaskAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaskAlarm)
	DDX_Control(pDX, IDC_CHECK1, m_chk_enable);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaskAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgMaskAlarm)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMaskAlarm message handlers
extern USER_HANDLE g_server_id;
extern int g_window_count;
void CDlgMaskAlarm::close()
{

}

BOOL CDlgMaskAlarm::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	struct tAlarmMask alarm_mask;
	memset(&alarm_mask,0,sizeof(alarm_mask));
	alarm_mask.slot = slot;
	if(HW_NET_SET_GetAlarmMask(g_server_id,&alarm_mask) == FALSE)
	{
		MessageBox("获取遮挡报警失败!");
		return FALSE;
	}
	m_chk_enable.SetCheck(alarm_mask.enabled);

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgMaskAlarm::commit()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	UpdateData(TRUE);

	struct tAlarmMask alarm_mask;
	memset(&alarm_mask,0,sizeof(alarm_mask));
	alarm_mask.slot = slot;
	alarm_mask.enabled = m_chk_enable.GetCheck();
	if(HW_NET_SET_SetAlarmMask(g_server_id,&alarm_mask) == FALSE)
	{
		MessageBox("设置遮挡报警失败!");
		return FALSE;
	}

	MessageBox("设置遮挡报警成功!");
	return TRUE;
}

void CDlgMaskAlarm::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

BOOL CDlgMaskAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;
	CString str;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_slot.AddString(str);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
