// DlgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgAlarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm dialog


CDlgAlarm::CDlgAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarm)
	DDX_Control(pDX, IDC_CBO_DEFEND, m_cbo_defend);
	DDX_Control(pDX, IDC_COMBO5, m_cbo_copy_date);
	DDX_Control(pDX, IDC_COMBO3, m_cbo_type);
	DDX_Control(pDX, IDC_COMBO2, m_cbo_date);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_alarm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarm)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm message handlers
extern USER_HANDLE g_server_id ;
extern int g_window_count;
void CDlgAlarm::close()
{

}

BOOL CDlgAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spin_beg_h[0] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN1);
	m_spin_beg_h[0]->SetBuddy(GetDlgItem(IDC_EDIT_TIME1));
	m_spin_beg_h[1] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN3);
	m_spin_beg_h[1]->SetBuddy(GetDlgItem(IDC_EDIT_TIME3));
	m_spin_beg_h[2] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN5);
	m_spin_beg_h[2]->SetBuddy(GetDlgItem(IDC_EDIT_TIME5));
	m_spin_beg_m[0] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN7);
	m_spin_beg_m[0]->SetBuddy(GetDlgItem(IDC_EDIT_TIME7));
	m_spin_beg_m[1] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN9);
	m_spin_beg_m[1]->SetBuddy(GetDlgItem(IDC_EDIT_TIME9));
	m_spin_beg_m[2] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN11);
	m_spin_beg_m[2]->SetBuddy(GetDlgItem(IDC_EDIT_TIME11));
	m_spin_end_h[0] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN2);
	m_spin_end_h[0]->SetBuddy(GetDlgItem(IDC_EDIT_TIME2));
	m_spin_end_h[1] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN4);
	m_spin_end_h[1]->SetBuddy(GetDlgItem(IDC_EDIT_TIME4));
	m_spin_end_h[2] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN6);
	m_spin_end_h[2]->SetBuddy(GetDlgItem(IDC_EDIT_TIME6));
	m_spin_end_m[0] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN8);
	m_spin_end_m[0]->SetBuddy(GetDlgItem(IDC_EDIT_TIME8));
	m_spin_end_m[1] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN10);
	m_spin_end_m[1]->SetBuddy(GetDlgItem(IDC_EDIT_TIME10));
	m_spin_end_m[2] = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN12);
	m_spin_end_m[2]->SetBuddy(GetDlgItem(IDC_EDIT_TIME12));

	int i;
	for(i = 0; i < 3; i++)
	{
		m_spin_beg_h[i]->SetRange(0,23);
		m_spin_beg_m[i]->SetRange(0,59);
		m_spin_end_h[i]->SetRange(0,23);
		m_spin_end_m[i]->SetRange(0,59);
	}

	int id_rec[]={IDC_CHECK_REC1,IDC_CHECK_REC2,IDC_CHECK_REC3,IDC_CHECK_REC4,
					IDC_CHECK_REC5,IDC_CHECK_REC6,IDC_CHECK_REC7,IDC_CHECK_REC8,
					IDC_CHECK_REC9,IDC_CHECK_REC10,IDC_CHECK_REC11,IDC_CHECK_REC12,
					IDC_CHECK_REC13,IDC_CHECK_REC14,IDC_CHECK_REC15,IDC_CHECK_REC16};
	int id_out[]={IDC_CHECK_OUT1,IDC_CHECK_OUT2,IDC_CHECK_OUT3,IDC_CHECK_OUT4,
		IDC_CHECK_OUT5,IDC_CHECK_OUT6,IDC_CHECK_OUT7,IDC_CHECK_OUT8,
		IDC_CHECK_OUT9,IDC_CHECK_OUT10,IDC_CHECK_OUT11,IDC_CHECK_OUT12,
					IDC_CHECK_OUT13,IDC_CHECK_OUT14,IDC_CHECK_OUT15,IDC_CHECK_OUT16};
	for(i = 0; i < 16; i++)
	{
		m_chk_rec[i] = (CButton*)GetDlgItem(id_rec[i]);
		m_chk_out[i] = (CButton*)GetDlgItem(id_out[i]);
	}	
	
	for(i = g_window_count ; i < 16; i++)
	{
		m_chk_rec[i]->ShowWindow(SW_HIDE);
	}
	struct tDevConfig dev_config;
	memset(&dev_config,0,sizeof(dev_config));
	HW_NET_SET_GetDevConfig(g_server_id,&dev_config);
	m_alarm_in_count = dev_config.alarm_in_count;
	m_alarm_out_count = dev_config.alarm_out_count;
	for(i = m_alarm_out_count; i < 16; i++)
	{
		m_chk_out[i]->ShowWindow(SW_HIDE);
	}

	CString str;
	for(i = 0; i < m_alarm_in_count; i++)
	{
		str.Format("报警器%d",i+1);
		m_cbo_alarm.AddString(str);
	}
	m_cbo_date.AddString("星期天");
	m_cbo_date.AddString("星期一");
	m_cbo_date.AddString("星期二");
	m_cbo_date.AddString("星期三");
	m_cbo_date.AddString("星期四");
	m_cbo_date.AddString("星期五");
	m_cbo_date.AddString("星期六");

	m_cbo_copy_date.AddString("星期天");
	m_cbo_copy_date.AddString("星期一");
	m_cbo_copy_date.AddString("星期二");
	m_cbo_copy_date.AddString("星期三");
	m_cbo_copy_date.AddString("星期四");
	m_cbo_copy_date.AddString("星期五");
	m_cbo_copy_date.AddString("星期六");
	m_cbo_copy_date.AddString("全部");

	m_cbo_type.AddString("常开");
	m_cbo_type.AddString("常闭");

	m_cbo_defend.AddString("定时");
	m_cbo_defend.AddString("手动布防");
	m_cbo_defend.AddString("手动撤防");

	memset(&m_alarm_in_cfg,0,sizeof(m_alarm_in_cfg));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgAlarm::refresh()
{
	int alarm_no = m_cbo_alarm.GetCurSel();
	if(alarm_no < 0 || alarm_no >= m_alarm_in_count)
	{
		return FALSE;
	}

	memset(&m_alarm_in_cfg,0,sizeof(m_alarm_in_cfg));
	m_alarm_in_cfg.alarmin_no = alarm_no;
	if(HW_NET_SET_GetAlarmInConfig(g_server_id,&m_alarm_in_cfg) == FALSE)
	{
		MessageBox("获取报警设置失败!");
		return FALSE;
	}

	LONG defend_type;
	if(HW_NET_SET_GetAlarmDefend(g_server_id,alarm_no,&defend_type) == FALSE)
	{
		MessageBox("获取手动不撤防失败!");
		return FALSE;
	}
	m_cbo_defend.SetCurSel(defend_type);
	
	int i;
	for(i = 0; i < g_window_count; i++)
	{
		if(m_alarm_in_cfg.handle.byRelRecord[i])
		{
			m_chk_rec[i]->SetCheck(1);
		}else{
			m_chk_rec[i]->SetCheck(0);
		}
	}
	for(i = 0; i < m_alarm_out_count; i++)
	{
		if(m_alarm_in_cfg.handle.byRelAlarmOut[i])
		{
			m_chk_out[i]->SetCheck(1);
		}else{
			m_chk_out[i]->SetCheck(0);
		}
	}

	m_cbo_type.SetCurSel(m_alarm_in_cfg.type);
	m_cbo_date.SetCurSel(1);

	refresh_date(1);

	return TRUE;
}

BOOL CDlgAlarm::refresh_date(int day)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		m_spin_beg_h[i]->SetPos(m_alarm_in_cfg.schedule[day][i].byStartHour);
		m_spin_beg_m[i]->SetPos(m_alarm_in_cfg.schedule[day][i].byStartMin);
		m_spin_end_h[i]->SetPos(m_alarm_in_cfg.schedule[day][i].byStopHour);
		m_spin_end_m[i]->SetPos(m_alarm_in_cfg.schedule[day][i].byStopMin);
	}

	UpdateData(FALSE);
	return TRUE;
}

void CDlgAlarm::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

void CDlgAlarm::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here

	refresh_date(m_cbo_date.GetCurSel());
}

void CDlgAlarm::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int day = m_cbo_date.GetCurSel();
	if(day < 0)
	{
		return ;
	}
	int i;
	for(i = 0; i < 3; i++)
	{
		m_alarm_in_cfg.schedule[day][i].byStartHour = m_spin_beg_h[i]->GetPos();
		m_alarm_in_cfg.schedule[day][i].byStartMin = m_spin_beg_m[i]->GetPos();
		m_alarm_in_cfg.schedule[day][i].byStopHour = m_spin_end_h[i]->GetPos();
		m_alarm_in_cfg.schedule[day][i].byStopMin = m_spin_end_m[i]->GetPos();
	}

}

void CDlgAlarm::OnButton5() 
{
	// TODO: Add your control notification handler code here
	int src_day = m_cbo_date.GetCurSel();
	if(src_day < 0)
	{
		return;
	}
	int copy_day = m_cbo_copy_date.GetCurSel();
	if(copy_day < 7)
	{
		//复制1天
		memcpy(m_alarm_in_cfg.schedule[copy_day],m_alarm_in_cfg.schedule[src_day],MAX_SEGMENT * sizeof(HW_NET_SCHEDTIME));
	}else{
		//复制整个星期
		int i;
		for(i = 0; i < 7; i++)
		{
			if(i != src_day)
			{
				memcpy(m_alarm_in_cfg.schedule[i],m_alarm_in_cfg.schedule[src_day],MAX_SEGMENT * sizeof(HW_NET_SCHEDTIME));
			}
		}
	}
}

BOOL CDlgAlarm::commit()
{
	int alarm_no = m_cbo_alarm.GetCurSel();
	if(alarm_no < 0 || alarm_no >= m_alarm_in_count)
	{
		return FALSE;
	}
	
	int i;
	m_alarm_in_cfg.alarmin_no = alarm_no;
	for(i = 0; i < g_window_count; i++)
	{		
		m_alarm_in_cfg.handle.byRelRecord[i] = m_chk_rec[i]->GetCheck();	
	}
	for(i = 0; i < m_alarm_out_count; i ++)
	{
		m_alarm_in_cfg.handle.byRelAlarmOut[i] = m_chk_out[i]->GetCheck();
	}

	m_alarm_in_cfg.type = m_cbo_type.GetCurSel();

	if(HW_NET_SET_SetAlarmInConfig(g_server_id,&m_alarm_in_cfg) == FALSE)
	{
		MessageBox("设置报警输入失败!");
		return FALSE;
	}

	LONG defend_type = m_cbo_defend.GetCurSel();
	if(HW_NET_SET_SetAlarmDefend(g_server_id,alarm_no,defend_type) == FALSE)
	{
		MessageBox("设置布撤防失败!");
		return FALSE;
	}

	MessageBox("设置成功!");
	return TRUE;
}
