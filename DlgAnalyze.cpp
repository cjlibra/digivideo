// DlgAnalyze.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgAnalyze.h"
#include "howellnetsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalyze dialog

extern USER_HANDLE g_server_id ;
extern int g_window_count;
CDlgAnalyze::CDlgAnalyze(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyze::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnalyze)
	m_analyze_enable = FALSE;
	m_analyze_focus_changed = 0;
	m_analyze_scene_changed = 0;
	m_analyze_scene_occluded = 0;
	//}}AFX_DATA_INIT
}


void CDlgAnalyze::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalyze)
	DDX_Control(pDX, IDC_CBO_ANALYZE_SLOT, m_cbo_slot);
	DDX_Check(pDX, IDC_CHK_ANALYZE_ENABLE, m_analyze_enable);
	DDX_Text(pDX, IDC_EDT_ANALYZE_FOCUS_CHANGE, m_analyze_focus_changed);
	DDV_MinMaxInt(pDX, m_analyze_focus_changed, 1, 100);
	DDX_Text(pDX, IDC_EDT_ANALYZE_SCENE_CHANGED, m_analyze_scene_changed);
	DDV_MinMaxInt(pDX, m_analyze_scene_changed, 1, 100);
	DDX_Text(pDX, IDC_EDT_ANALYZE_SCENE_OCCLUDED, m_analyze_scene_occluded);
	DDV_MinMaxInt(pDX, m_analyze_scene_occluded, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalyze, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalyze)
	ON_CBN_SELCHANGE(IDC_CBO_ANALYZE_SLOT, OnSelchangeCboAnalyzeSlot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalyze message handlers

BOOL CDlgAnalyze::OnInitDialog() 
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

BOOL CDlgAnalyze::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	net_analyze_t analyze_cfg;
	analyze_cfg.slot = slot;

	if(!HW_NET_SET_GetAnalyze(g_server_id,&analyze_cfg))
	{
		MessageBox("获取视频分析设置失败!");
		return FALSE;
	}

	m_analyze_enable = analyze_cfg.enable;
	m_analyze_scene_changed = analyze_cfg.scene_changed_threso;
	m_analyze_scene_occluded = analyze_cfg.scene_occluded_threso;
	m_analyze_focus_changed = analyze_cfg.focus_changed_threso;	

	UpdateData(FALSE);
	return TRUE;
}

void CDlgAnalyze::close()
{

}

BOOL CDlgAnalyze::commit()
{
	UpdateData(TRUE);

	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	net_analyze_t analyze_cfg;
	analyze_cfg.slot = slot;
	analyze_cfg.enable = m_analyze_enable;
	analyze_cfg.scene_changed_threso = m_analyze_scene_changed;
	analyze_cfg.scene_occluded_threso = m_analyze_scene_occluded;
	analyze_cfg.focus_changed_threso = m_analyze_focus_changed;
	
	if(!HW_NET_SET_SetAnalyze(g_server_id,&analyze_cfg))
	{
		MessageBox("设置视频分析设置失败!");
		return FALSE;
	}

	MessageBox("视频分析设置成功");

	return TRUE;
}

void CDlgAnalyze::OnSelchangeCboAnalyzeSlot() 
{
	// TODO: Add your control notification handler code here
	refresh();
}
