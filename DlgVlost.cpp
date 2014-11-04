// DlgVlost.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgVlost.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVlost dialog
extern USER_HANDLE g_server_id ;
extern int g_window_count;

CDlgVlost::CDlgVlost(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVlost::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVlost)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgVlost::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVlost)
	DDX_Control(pDX, IDC_COMBO1, m_cbo_channel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVlost, CDialog)
	//{{AFX_MSG_MAP(CDlgVlost)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVlost message handlers

BOOL CDlgVlost::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int id_out[]={IDC_VLOST_OUT1,IDC_VLOST_OUT2,IDC_VLOST_OUT3,IDC_VLOST_OUT4,
		IDC_VLOST_OUT5,IDC_VLOST_OUT6,IDC_VLOST_OUT7,IDC_VLOST_OUT8,
		IDC_VLOST_OUT9,IDC_VLOST_OUT10,IDC_VLOST_OUT11,IDC_VLOST_OUT12,
					IDC_VLOST_OUT13,IDC_VLOST_OUT14,IDC_VLOST_OUT15,IDC_VLOST_OUT16};

	int i;
	for(i = 0; i < 16; i++)
	{	
		m_chk_out[i] = (CButton*)GetDlgItem(id_out[i]);
	}	

	CString str;	
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d",i + 1);
		m_cbo_channel.AddString(str);
	}

	struct tDevConfig dev_config;
	memset(&dev_config,0,sizeof(dev_config));
	HW_NET_SET_GetDevConfig(g_server_id,&dev_config);
	m_alarm_out_count = dev_config.alarm_out_count;
	for(i = m_alarm_out_count; i < 16; i++)
	{
		m_chk_out[i]->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVlost::close()
{
	
}

BOOL CDlgVlost::refresh()
{
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0)
	{
		return FALSE;
	}

	struct tVideoLostCfg vlost_cfg;
	vlost_cfg.slot = slot;
	if(HW_NET_SET_GetVideoLostConfig(g_server_id,&vlost_cfg) == FALSE)
	{
		MessageBox("获取视频报警丢失失败!");
		return FALSE;
	}
	
	//当前只支持联动到输出
	int i;
	for(i = 0; i < m_alarm_out_count; i++)
	{
		if(vlost_cfg.handle.byRelAlarmOut[i])
		{
			m_chk_out[i]->SetCheck(1);
		}
		else
		{
			m_chk_out[i]->SetCheck(0);
		}
	}

	return TRUE;
}

BOOL CDlgVlost::commit()
{
	UpdateData(TRUE);
	
	int slot = m_cbo_channel.GetCurSel();
	if(slot < 0)
	{
		return FALSE;
	}

	struct tVideoLostCfg vlost_cfg;
	memset(&vlost_cfg,0,sizeof(vlost_cfg));
	vlost_cfg.slot = slot;
	vlost_cfg.handle.type = ALARM_TO_OUT;//当前只支持联动到输出

	int i;
	for(i = 0; i < m_alarm_out_count; i++)
	{
		vlost_cfg.handle.byRelAlarmOut[i] = m_chk_out[i]->GetCheck();	
	}

	if(HW_NET_SET_SetVideoLostConfig(g_server_id,&vlost_cfg) == FALSE)
	{
		MessageBox("设置视频报警丢失失败!");
		return FALSE;
	}
	
	MessageBox("设置成功!");
	return TRUE;
}


void CDlgVlost::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

void CDlgVlost::OnButton2() 
{
	// TODO: Add your control notification handler code here
	HW_NET_SET_SetAlarmOutState(g_server_id,0xff,0);
}
