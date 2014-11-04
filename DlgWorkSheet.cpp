// DlgWorkSheet.cpp : implementation file
//

#include "stdafx.h"
#include "net_demo.h"
#include "DlgWorkSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkSheet dialog


CDlgWorkSheet::CDlgWorkSheet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWorkSheet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWorkSheet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgWorkSheet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWorkSheet)
	DDX_Control(pDX, IDC_COMBO5, m_cbo_copy_date);
	DDX_Control(pDX, IDC_COMBO2, m_cbo_date);
	DDX_Control(pDX, IDC_COMBO1, m_cbo_slot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWorkSheet, CDialog)
	//{{AFX_MSG_MAP(CDlgWorkSheet)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkSheet message handlers
extern USER_HANDLE g_server_id;
extern int g_window_count;
void CDlgWorkSheet::close()
{

}

BOOL CDlgWorkSheet::refresh_date(int day)
{
	int i = 0; 
	for(i = 0; i < 3; i++)
	{
		m_spin_beg_h[i]->SetPos(m_work_sheet.slotSheet.eachDaySheet[day].segment[i].beg.wHour);
		m_spin_beg_m[i]->SetPos(m_work_sheet.slotSheet.eachDaySheet[day].segment[i].beg.wMinute);
		m_spin_end_h[i]->SetPos(m_work_sheet.slotSheet.eachDaySheet[day].segment[i].end.wHour);
		m_spin_end_m[i]->SetPos(m_work_sheet.slotSheet.eachDaySheet[day].segment[i].end.wMinute);
		m_cbo_rec_type[i]->SetCurSel(m_work_sheet.slotSheet.eachDaySheet[day].segment[i].recType);
	}

	UpdateData(FALSE);
	return TRUE;
}

BOOL CDlgWorkSheet::refresh()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}
	
	m_work_sheet.slotNo =slot;
	memset(&m_work_sheet.slotSheet,0,sizeof(m_work_sheet.slotSheet));
	if(HW_NET_SET_GetHowellWorkSheet(g_server_id,&m_work_sheet) == FALSE)
	{
		MessageBox("获取工作表失败!");
		return FALSE;
	}

	m_cbo_date.SetCurSel(1);
	
	refresh_date(1);
	return TRUE;
}

BOOL CDlgWorkSheet::commit()
{
	int slot = m_cbo_slot.GetCurSel();
	if(slot < 0 || slot >= g_window_count)
	{
		return FALSE;
	}

	UpdateData(TRUE);

	if(HW_NET_SET_SetHowellWorkSheet(g_server_id,&m_work_sheet) == FALSE)
	{
		MessageBox("设置工作表失败!");
		return FALSE;
	}

	MessageBox("设置成功");
	return TRUE;
}

BOOL CDlgWorkSheet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;
	CString str;
	for(i = 0; i < g_window_count; i++)
	{
		str.Format("通道%d", i + 1);
		m_cbo_slot.AddString(str);
	}

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

	m_cbo_rec_type[0] = (CComboBox*)GetDlgItem(IDC_COMBO3);
	m_cbo_rec_type[1] = (CComboBox*)GetDlgItem(IDC_COMBO9);
	m_cbo_rec_type[2] = (CComboBox*)GetDlgItem(IDC_COMBO10);

	for(i = 0; i < 3; i++)
	{
		m_spin_beg_h[i]->SetRange(0,23);
		m_spin_beg_m[i]->SetRange(0,59);
		m_spin_end_h[i]->SetRange(0,23);
		m_spin_end_m[i]->SetRange(0,59);

		m_cbo_rec_type[i]->AddString("定时");
		m_cbo_rec_type[i]->AddString("移动侦测");	
		m_cbo_rec_type[i]->AddString("报警");
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

	memset(&m_work_sheet,0,sizeof(m_work_sheet));
	m_work_sheet.sheetNO = 2;//3511必须为2
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWorkSheet::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	
	refresh();
}

void CDlgWorkSheet::OnSelchangeCombo2() 
{
	// TODO: Add your control notification handler code here
	
	refresh_date(m_cbo_date.GetCurSel());
}

void CDlgWorkSheet::OnButton2() 
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
		m_work_sheet.slotSheet.eachDaySheet[day].segment[i].beg.wHour= m_spin_beg_h[i]->GetPos();
		m_work_sheet.slotSheet.eachDaySheet[day].segment[i].beg.wMinute = m_spin_beg_m[i]->GetPos();
		m_work_sheet.slotSheet.eachDaySheet[day].segment[i].end.wHour = m_spin_end_h[i]->GetPos();
		m_work_sheet.slotSheet.eachDaySheet[day].segment[i].end.wMinute = m_spin_end_m[i]->GetPos();
		m_work_sheet.slotSheet.eachDaySheet[day].segment[i].recType = m_cbo_rec_type[i]->GetCurSel();
	}
}

void CDlgWorkSheet::OnButton5() 
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
		m_work_sheet.slotSheet.eachDaySheet[copy_day] = m_work_sheet.slotSheet.eachDaySheet[src_day]; 		
	}else{
		//复制整个星期
		int i;
		for(i = 0; i < 7; i++)
		{
			if(i != src_day)
			{
				m_work_sheet.slotSheet.eachDaySheet[i] = m_work_sheet.slotSheet.eachDaySheet[src_day];				
			}
		}
	}
}
