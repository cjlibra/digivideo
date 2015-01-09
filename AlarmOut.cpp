// AlarmOut.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "AlarmOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AlarmOut dialog


AlarmOut::AlarmOut(CWnd* pParent /*=NULL*/)
	: CDialog(AlarmOut::IDD, pParent)
{
	//{{AFX_DATA_INIT(AlarmOut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void AlarmOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AlarmOut)
	DDX_Control(pDX, IDC_CBO_ALARMOUT, m_cbo_alarmout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AlarmOut, CDialog)
	//{{AFX_MSG_MAP(AlarmOut)
	ON_BN_CLICKED(IDC_BTN_ALARMOUT_START, OnBtnAlarmoutStart)
	ON_BN_CLICKED(IDC_BTN_ALARMOUT_STOP, OnBtnAlarmoutStop)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AlarmOut message handlers
extern USER_HANDLE g_server_id ;

BOOL AlarmOut::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	for(int i = 0; i < 10; i++)
	{
		str.Format("%d",i + 1);
		m_cbo_alarmout.AddString(str);
	}
	m_cbo_alarmout.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AlarmOut::OnBtnAlarmoutStart() 
{
	// TODO: Add your control notification handler code here
	HW_NET_SET_SetAlarmOutState(g_server_id,m_cbo_alarmout.GetCurSel(),1);
}

void AlarmOut::OnBtnAlarmoutStop() 
{
	// TODO: Add your control notification handler code here
	HW_NET_SET_SetAlarmOutState(g_server_id,m_cbo_alarmout.GetCurSel(),0);	
}

BOOL AlarmOut::commit()
{
	return TRUE;
}

BOOL AlarmOut::refresh()
{
	return TRUE;
}

void AlarmOut::close()
{

}


